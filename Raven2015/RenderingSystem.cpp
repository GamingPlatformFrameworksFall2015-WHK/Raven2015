#include "RenderingSystem.h"
#include "SFML/Graphics.hpp"
#include <algorithm>

using namespace Raven;

/*
 * Automatically register all textures associated with SpriteRenderers thus far
 */
void RenderingSystem::initialize(entityx::EntityManager &es, sf::RenderWindow &window) {

    renderWindow = &window;

    es.each<SpriteRenderer>([&](ex::Entity &entity, SpriteRenderer &renderer) {
        registerTexture(renderer.textureFileName);
    });
}

/*
 * Register a texture asset to permit access from Animations and SpriteRenderers
 */
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

/*
 * Register an Animation asset to permit access from Animators and SpriteRenderers
 */
void RenderingSystem::registerAnimation(std::string animationName, std::shared_ptr<Animation> animation) {

    // Null pointer check
    if (!animation) {
        cerr << "Error: Attempt to register null animation." << endl;
        throw 1;
    }

    // Error checking for window validity
    if (!renderWindow) {
        cerr << "Error: Attempt to register animation prior to initialization of render window." << endl;
        throw 1;
    }

    // Notify user if overwriting data instead of creating new animation storage
    if (animationMap.find(animationName) != animationMap.cend()) {
        cout << "Note: Animation name found during animation registration. Overwriting animation data." << endl;
    }

    // Store the animation under the given name
    animationMap[animationName] = *animation;

    registerTexture(animation->textureFileName);
}
            
/*
 * 1. Increments any and all animations by 1 frame.
 * 2. Sorts all current sprites based on their draw layer and priority.
 * 3. Iterates through each sprite from back to front, drawing them.
 */
void RenderingSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {

    // Error checking for window validity
    if (!renderWindow) {
        cerr << "Error: RenderingSystem::window invalid during attempt to draw on RenderingSystem::update" << endl;
        throw 1;
    }
    
    // Determine the next image to be drawn to the screen for each sprite
    es.each<SpriteRenderer, Animator>([&](ex::Entity &entity, SpriteRenderer &renderer, Animator &animator) {
        // Save the current Animation
        Animation anim = animationMap[animator.animName];

        // How much progress have we made towards iterating frames?
        anim.animationProgress += dt * anim.animationSpeed;

        // Move the current frame appropriately and reset the progress
        if (anim.animationSpeed < 0 && anim.animationProgress < -1) {
            animator.frameId += (int)anim.animationProgress;
            anim.animationProgress += (int)anim.animationProgress;
        }
        else if (anim.animationSpeed > 0 && anim.animationProgress > 1) {
            animator.frameId += (int)anim.animationProgress;
            anim.animationProgress -= (int)anim.animationProgress;
        }

        // If looping, then modulate the result within the available frames
        if (anim.isLooping) {
            animator.frameId %= anim.frames.size();
        }
        else { //else, clamp the result between the two extreme ends of the frame sequence
            cmn::clamp<int>(animator.frameId, 0, (int)anim.frames.size()-1);
        }

        // Update the map value with the altered animation data
        animationMap[animator.animName] = anim;

        // Set the renderer's sprite to the IntRect in the animation's frames vector using the frame ID
        renderer.sprite.setTextureRect(anim.frames[animator.frameId]);
    });

    // Generate the sorted heap
    es.each<SpriteRenderer>([&](ex::Entity &entity, SpriteRenderer &renderer) {
        // Add the renderer to the spriteHeap
        spriteHeap.push_back(renderer);
        push_heap(spriteHeap.begin(), spriteHeap.end());
        ex::ComponentHandle<Transform> transform = entity.component<Transform>();
        renderer.sprite.setPosition(transform->transform.x - cmn::STD_UNITX*.5f, transform->transform.y - cmn::STD_UNITY*.5f);

        // If the exact address of this texture is not the same as the one on record, reacquire it
        if (renderer.sprite.getTexture() != &textureMap[renderer.textureFileName]) {
            cout << "Note: Re-applying texture to sprite" << endl;
            renderer.sprite.setTexture(textureMap[renderer.textureFileName]);
        }
    });

    // Pop every sprite off the heap, drawing them as you go
    while (!spriteHeap.empty()) {
        SpriteRenderer renderer;
        renderer = spriteHeap.front();
        renderer.sprite.setTexture(textureMap[renderer.textureFileName]);
        renderWindow->draw(renderer.sprite);
        pop_heap(spriteHeap.begin(), spriteHeap.end());
        spriteHeap.pop_back();
    }

    es.each<TextRenderer>([&](ex::Entity &entity, TextRenderer &renderer) {
        renderWindow->draw(renderer.text);
    });
}

