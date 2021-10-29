#ifndef MANTA_ENGINE_HPP
#define MANTA_ENGINE_HPP

#include <rendering/renderer.hpp>

#include <console/console.hpp>

#include <assets/resources.hpp>

#include <actors/world.hpp>

class MGame;

// The engine can only exist once!
// This is a way to move a bunch of nonsense code out of main.cpp into a more unified place
// Prefixed with M, M for Master (or Manta), which denotes this is a core datatype
class MEngine {
public:
    MEngine();

    Renderer* renderer;

    Console console; // We always have a console :)
    Resources resources; // and resources so we can load engine content

    World world; // Can't have a game world without something to represent it

    MGame* game; // The engine will terminate if it can't find a game

    bool alive = true; // Change this to false at any time to abort the update loop and quit abruptly

    // Calling this hands off control from the main file to the engine instance until the engine is no longer running

    //
    // Loop is structured as so
    //
    // EditorUpdate() (Only if engine is started with -editor, used for very important tasks)
    // GameUpdate() // Calls the currently loaded MGame class and tells it to update
    //
    // BeginDraw() - Readies OpenGL for a new frame
    // Draw() - Causes everything in the world to draw
    // EditorDraw() (Only if engine is started with -editor, used for drawing editor UIs and other things)
    // EndDraw() - Tells OpenGL to swap the framebuffer, we're done with this frame

    void EngineLoop();
};


#endif
