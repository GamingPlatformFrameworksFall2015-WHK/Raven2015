#include "RenderingSystem.h"
#include "SFML/Graphics.hpp"
#include <algorithm>

using namespace Raven;

// Initializes the loading of all textures defined by Renderers
void RenderingSystem::initialize(entityx::EntityManager &es) {

    es.each<Renderer>([&](ex::Entity &entity, Renderer &renderer) {
        for (auto item : renderer.sprites) {
            registerTexture(item.second->textureFileName);
        }

    });
}

// Registers the texture, permitting access from Animations and Renderers
void RenderingSystem::registerTexture(std::string textureFileName) {
    if (textureMap.find(textureFileName) == textureMap.end()) {
        cout << "Note: textureMap look up failed. Inserting new texture." << endl;

        // Store the referenced texture if it has not already been added
        sf::Texture tex;
        if (!tex.loadFromFile(textureFileName)) {
            cerr << "Error: texture file \"" + textureFileName + "\" could not be loaded." << endl;
        }
        else {
            textureMap[textureFileName] = tex;
        }
    }
}

// Registers the animation.
// There are two valid methods of passing parameters to the animation variable: 
// 1. Pass in a std::shared_ptr for Animation. (variable will be preserved in external pointer) 
// 2. Pass in a new Animation(...). (variable will be deleted when registerAnimation leaves scope) 
void RenderingSystem::registerAnimation(const std::string &animationName, Animation* const animation) {

    // Shared pointer to catch either another shared pointer or a new pointer
    std::shared_ptr<Animation> ptr(animation);

    // Null pointer check
    if (!animation) {
        cerr << "Error: Attempt to register null animation." << endl;
        throw 1;
    }

    // Error checking for window validity
    if (!canvas) {
        cerr << "Error: Attempt to register animation prior to initialization of render window." << endl;
        throw 1;
    }

    // Notify user if overwriting data instead of creating new animation storage
    if (animationMap.find(animationName) != animationMap.cend()) {
        cout << "Note: Animation name found during animation registration. Overwriting animation data." << endl;
    }

    // Store the animation under the given name
    animationMap[animationName] = *ptr;

    registerTexture(ptr->textureFileName);
}

// Updates all rendered assets by following the sequence below. 
// 1. Increments any and all animations by 1 frame. 
// 2. Sorts all current renderable assets based on their draw layer and priority. 
// 3. Iterates through each asset from back to front, drawing them. 
void RenderingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {

    // Error checking for window validity
    if (!canvas) {
        cerr << "Error: RenderingSystem::canvas invalid during attempt to draw on RenderingSystem::update" << endl;
        throw 1;
    }
    
    // Determine the next image to be drawn to the screen for each sprite
    es.each<Renderer>([&](ex::Entity &entity, Renderer &renderer) {

        // Acquire each std::string-RenderableSprite pair in the renderer
        for (auto name_renderable : renderer.sprites) {

            // Save the current Animation
            Animation anim = animationMap[name_renderable.second->animName];

            // How much progress have we made towards iterating frames?
            anim.animationProgress += dt * anim.animationSpeed;

            // Move the current frame appropriately and reset the progress
            if (anim.animationSpeed < 0 && anim.animationProgress < -1) {
                name_renderable.second->frameId += (int)anim.animationProgress;
                anim.animationProgress += (int)anim.animationProgress;
            }
            else if (anim.animationSpeed > 0 && anim.animationProgress > 1) {
                name_renderable.second->frameId += (int)anim.animationProgress;
                anim.animationProgress -= (int)anim.animationProgress;
            }

            // If looping, then modulate the result within the available frames
            if (anim.isLooping) {
                name_renderable.second->frameId %= anim.frames.size();
            }
            else { //else, clamp the result between the two extreme ends of the frame sequence
                cmn::clamp<int>(name_renderable.second->frameId, 0, (int)anim.frames.size() - 1);
            }

            // Update the map value with the altered animation data
            animationMap[name_renderable.second->animName] = anim;

            // Set the renderer's sprite to the IntRect in the animation's frames vector using the frame ID
            name_renderable.second->sprite.setTextureRect(anim.frames[name_renderable.second->frameId]);
        }
    });
    
    // Generate the sorted heap
    es.each<Renderer>([this](ex::Entity &entity, Renderer &renderer) {

        for (std::pair<std::string, std::shared_ptr<RenderableSprite>> name_renderable : renderer.sprites) {
            renderableHeap.push(*name_renderable.second);

            // Acquire the transform of the entity
            ex::ComponentHandle<Transform> transform = entity.component<Transform>();

            // Ensure that the asset is positioned properly
            if (transform) {
                name_renderable.second->sprite.setPosition(
                    transform->transform.x - cmn::STD_UNITX*.5f + name_renderable.second->offsetX,
                    transform->transform.y - cmn::STD_UNITY*.5f + name_renderable.second->offsetY);
            }

            // If the exact address of this texture is not the same as the one on record, reacquire it
            if (name_renderable.second->sprite.getTexture() != &textureMap[name_renderable.second->textureFileName]) {
                //cout << "Note: Re-applying texture to sprite" << endl;
                //This section is always entered for some reason...
                name_renderable.second->sprite.setTexture(textureMap[name_renderable.second->textureFileName]);
            }
        }

        for (std::pair<std::string, std::shared_ptr<RenderableRectangle>> name_renderable : renderer.rectangles) {
            renderableHeap.push(*name_renderable.second);

            // Acquire the transform of the entity
            ex::ComponentHandle<Transform> transform = entity.component<Transform>();

            // Ensure that the asset is positioned properly
            if (transform) {
                name_renderable.second->rectangle.setPosition(
                    transform->transform.x - cmn::STD_UNITX*.5f + name_renderable.second->offsetX,
                    transform->transform.y - cmn::STD_UNITY*.5f + name_renderable.second->offsetY);
            }
        }

        for (std::pair<std::string, std::shared_ptr<RenderableCircle>> name_renderable : renderer.circles) {
            renderableHeap.push(*name_renderable.second);

            // Acquire the transform of the entity
            ex::ComponentHandle<Transform> transform = entity.component<Transform>();

            // Ensure that the asset is positioned properly
            if (transform) {
                name_renderable.second->circle.setPosition(
                    transform->transform.x - cmn::STD_UNITX*.5f + name_renderable.second->offsetX,
                    transform->transform.y - cmn::STD_UNITY*.5f + name_renderable.second->offsetY);
            }
        }

        for (std::pair<std::string, std::shared_ptr<RenderableText>> name_renderable : renderer.texts) {
            renderableHeap.push(*name_renderable.second);

            // Acquire the transform of the entity
            ex::ComponentHandle<Transform> transform = entity.component<Transform>();

            // Ensure that the asset is positioned properly
            if (transform) {
                name_renderable.second->text.setPosition(
                    transform->transform.x - cmn::STD_UNITX*.5f + name_renderable.second->offsetX,
                    transform->transform.y - cmn::STD_UNITY*.5f + name_renderable.second->offsetY);
            }
        }
    });


    // Pop every sprite off the heap, drawing them as you go
    std::shared_ptr<Renderable> renderable = nullptr;
    canvas->Bind();
    canvas->Clear(sf::Color::Black);
    while (!renderableHeap.empty()) {
        canvas->Draw(*renderableHeap.top().drawPtr);
        renderableHeap.pop();
    }
    canvas->Display();
    canvas->Unbind();
}

