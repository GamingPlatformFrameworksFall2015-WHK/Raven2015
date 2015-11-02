/*
 *
 */
#include "Common.h"
#include "RenderingSystem.h"
#include "SFML/Graphics.hpp"
#include <algorithm>
#include "Game.h"

using namespace Raven;

    void RenderingSystem::receive(const GUIRegisterTextureEvent& e) {
        if (e.textureFilePath != "") {
            textureMap.insert(std::make_pair(e.textureFilePath, sf::Texture()));
        }
        else {
            cerr << "Warning: Failed attempt to load texture with empty string name" << endl;
        }
        if (!textureMap[e.textureFilePath].loadFromFile(e.textureFilePath.c_str())) {
            cerr << "Warning: Failed to load texture at path: " + e.textureFilePath << endl;
        }
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

        // If we are currently in editMode, don't bother updating the animation frames.
        // Just draw everything as-is.
        // DEV WARNING: This resulted in sprites being drawn as full textures w/o animation, even after EditMode was toggled off
        //if (cmn::game->editMode) {
        //   return;
        //}

        // Acquire each std::string-RenderableSprite pair in the renderer
        for (auto name_renderable : renderer.sprites) {

            try {
                if (assets->animations->find(name_renderable.second->animName) == assets->animations->end()) {
                    cerr << "Warning: Sprite attempting to use non-existent Animation" << endl;
                    continue;
                }
            }
            catch (std::exception e) {
                cerr << endl << e.what() << endl;
            }
    
            // Save the current Animation
            std::shared_ptr<Animation> anim = assets->animations->at(name_renderable.second->animName);

            // How much progress have we made towards iterating frames?
            anim->animationProgress += dt * anim->animationSpeed;

            // Move the current frame appropriately and reset the progress
            if (anim->animationSpeed < 0 && anim->animationProgress < -1) {
                name_renderable.second->frameId += (int)anim->animationProgress;
                anim->animationProgress += (int)anim->animationProgress;
            }
            else if (anim->animationSpeed > 0 && anim->animationProgress > 1) {
                name_renderable.second->frameId += (int)anim->animationProgress;
                anim->animationProgress -= (int)anim->animationProgress;
            }

            // If looping, then modulate the result within the available frames
            if (anim->isLooping) {
                name_renderable.second->frameId %= anim->frames.size();
            }
            else { //else, clamp the result between the two extreme ends of the frame sequence
                cmn::clamp<int>(name_renderable.second->frameId, 0, (int)anim->frames.size() - 1);
            }

            // Update the map value with the altered animation data
            assets->animations->at(name_renderable.second->animName) = anim;

            // Set the renderer's sprite to the IntRect in the animation's frames vector using the frame ID
            name_renderable.second->sprite.setTextureRect(anim->frames[name_renderable.second->frameId]);
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
                    transform->transform.x - name_renderable.second->sprite.getTextureRect().width*0.75f + name_renderable.second->offsetX,
                    transform->transform.y - name_renderable.second->sprite.getTextureRect().width*1.5f + name_renderable.second->offsetY);
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

