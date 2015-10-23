#pragma once

#include "Common.h"
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

namespace Raven {

     // A wrapper class around drawable assets to allow for sorting.
     // Sorting is based on layer first, priority second.
     // A low priority means it will be drawn first, i.e. below other objects
    struct Renderable {

        Renderable(const float offsetX = 0.f, const float offsetY = 0.f, 
            const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : offsetX(offsetX), offsetY(offsetY), renderLayer(renderLayer), renderPriority(renderPriority), drawPtr(nullptr) {}

        // A Drawable pointer used SOLELY for generic drawing (errors occurred otherwise)
        sf::Drawable* drawPtr;

        // The rendering layer for macro-sorting of render content
        cmn::ERenderingLayer renderLayer;

        // The drawing-order priority within the rendering layer. Top-most = high priority
        int renderPriority;

        // X-axis displacement from the Renderer's owner's Transform.transform.x
        float offsetX;

        // Y-axis displacement from the Renderer's owner's Transform.transform.y
        float offsetY;

        // Assuming usage of priority queue with fixed max-heap functionality
        // Need to have a 'less-than' operator that places higher priorities at minimum values
        // Therefore, it simply references the 'greater-than' operator to achieve the desired inversion
        bool operator<(const Renderable &other) const {
            return *this > other;
        }
        
        // Helps to sort Renderables based on layer and priority.
        // Higher priorities are given minimal values b/c priority queues pop off items max-first.
        bool operator>(const Renderable &other) const {
            if (renderLayer > other.renderLayer) {
                return true;
            }
            else if (renderLayer == other.renderLayer) {
                if (renderPriority > other.renderPriority) {
                    return true;
                }
            // else if (renderPriority == other.renderPriority)
            //     Behavior undefined. No guarantees for which
            //     object will be drawn on top
            // }
            //
            }
            return false;
        }
    };

    // A sortable Text for rendering
    struct RenderableText : public Renderable {

        RenderableText(const std::string &textContent = "", const sf::Vector2f &position = sf::Vector2f(),
            const std::string &fontFilePath = "", const sf::Color &color = sf::Color::White, const float offsetX = 0.f,
            const float offsetY = 0.f, cmn::ERenderingLayer renderLayer = cmn::ERenderingLayer::NO_LAYER, int renderPriority = 0)
            : Renderable(offsetX, offsetY, renderLayer, renderPriority), fontFilePath(fontFilePath) {

            drawPtr = &text;

            if (!font.loadFromFile(fontFilePath)) {
                cerr << "Error: RenderableText failed to load font file <" + fontFilePath + ">" << endl;
                throw 1;
            }

            text.setString(textContent);
            text.setColor(color);
            text.setPosition(position);
            text.setFont(font);
        }

        // A Font used to format text
        sf::Font font;
        
        // The Font file path
        std::string fontFilePath;

        // A Text used to draw text to the window        
        sf::Text text;
    };

    // A base class for sortable Shapes for rendering
    struct RenderableShape : public Renderable {
        RenderableShape(const float offsetX = 0.f, const float offsetY = 0.f, 
            const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : Renderable(offsetX, offsetY, renderLayer, renderPriority) {}
    };

    // A base class for sortable Circles for rendering
    struct RenderableCircle : public RenderableShape {
        RenderableCircle(const float offsetX = 0.f, const float offsetY = 0.f, 
            const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : RenderableShape(offsetX, offsetY, renderLayer, renderPriority) {
        
            drawPtr = &circle;
        }

        sf::CircleShape circle;
    };

    // A base class for sortable Rectangles for rendering
    struct RenderableRectangle : public RenderableShape {
        RenderableRectangle(const float offsetX = 0.f, const float offsetY = 0.f, 
            const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : RenderableShape(offsetX, offsetY, renderLayer, renderPriority) {
        
            drawPtr = &rectangle;
        }

        sf::RectangleShape rectangle;
    };

    // A base class for sortable Sprites for rendering & animation
    struct RenderableSprite : public Renderable {
        RenderableSprite(const std::string &textureFileName = "",
            const std::string &animName = "", const int frameId = 0, const float offsetX = 0.f, const float offsetY = 0.f,
            const cmn::ERenderingLayer &renderLayer = cmn::ERenderingLayer::NO_LAYER, const int renderPriority = 0)
            : Renderable(offsetX, offsetY, renderLayer, renderPriority), textureFileName(textureFileName), animName(animName), 
            frameId(frameId), sprite() {
        
            drawPtr = &sprite;
        }

        // The source texture file for the current sprite(sheet)        
        std::string textureFileName;

        // The name of the animation in use. Empty string ("") indicates no animation necessary
        std::string animName;

        // The index of the frame of the animation currently being displayed. Only significant if animation is necessary
        int frameId;

        // The sprite to be sorted
        sf::Sprite sprite;
    };

     // A helper class to contain information regarding a given Animation.
     // A given texture (as a spritesheet) may contain several animation frames.
    struct Animation {

         // Initializes a new instance of the Animation struct.
         // Assume any given texture with multiple sub-images holds a single animation
         // with only horizontal translation between frames and no padding at all.
        Animation(std::string textureFileName = "", int size = 0, bool isLooping = false, float animationSpeed = 1.0f,
                int frameWidth = cmn::STD_UNITX, int frameHeight = cmn::STD_UNITY)
                : textureFileName(textureFileName), size(size), isLooping(isLooping), animationSpeed(animationSpeed),
                animationProgress(0.0f), frameWidth(frameWidth), frameHeight(frameHeight) {

            // Ensure that we have one viewing rectangle (sf::IntRect) into the texture for each sprite frame
            frames.resize(size);

            // Ensure that each successive window is placed over the next frame of the spritesheet animation in turn
            for (int i = 0; i < frames.size(); ++i) {
                frames[i].width = frameWidth;
                frames[i].height = frameHeight;
                frames[i].left += i*frameWidth;
            }
        }

        // The sections of the texture the animation draws from for each sprite        
        std::vector<sf::IntRect> frames;
        
        // Whether the sprite animation should stop at the end or loop back to the start        
        bool isLooping;

        // The width of each frame in the imported spritesheet texture        
        int frameWidth;

        // The height of each frame in the imported spritesheet texture        
        int frameHeight;

        // The number of frames in the spritesheet animation        
        int size;

        // The speed of the animation (how many ticks per 1 iteration of the animation loop?)        
        double animationSpeed;

        // The current progress towards reaching the animationSpeed threshold
        double animationProgress;

        // The name of the texture file referenced by the animation (the spritesheet, single line)        
        std::string textureFileName;
    };

    // A wrapper for an sf::Clock and ex::TimeDelta that allows a higher level of control over the clock
    struct Timer {
        
        // Initializes a new instance of the <see cref="Timer"/> struct.
        Timer(sf::Clock clock = sf::Clock(), ex::TimeDelta deltaTime = 0.0, bool isPlaying = true)
            : elapsedTime(deltaTime), isPlaying(isPlaying) {}
        
        // Gets the elapsed time.
        ex::TimeDelta getElapsedTime() {
            return elapsedTime + clock.getElapsedTime().asSeconds();
        }

        // Rewind or advance the timer by a given amount        
        void scan(ex::TimeDelta scanDistancetance) {
            elapsedTime += scanDistancetance;
        }

        // Pause timer only if timer is currently playing        
        void pause() {
            if (isPlaying) {
                elapsedTime += clock.restart().asSeconds();
                isPlaying = false;
            }
            else {
                cerr << "Timer already paused." << endl;
            }
        }

        // Play timer only if timer is currently paused        
        void play() {
            if (!isPlaying) {
                clock.restart();
                isPlaying = true;
            }
            else {
                cerr << "Timer is already playing." << endl;
            }
        }
                
        // Restarts the timer, returning the time up to that point
        ex::TimeDelta restart() {
            ex::TimeDelta time = getElapsedTime();
            elapsedTime = 0.0;
            clock.restart();
            return time;
        }

    private:        
        // An sf::Clock to assist in recording time
        sf::Clock clock;
        
        // Record of elapsed time
        ex::TimeDelta elapsedTime;
        
        // Current state of timer
        bool isPlaying;
    };
}