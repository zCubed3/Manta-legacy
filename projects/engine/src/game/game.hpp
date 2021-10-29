#ifndef MANTA_GAME_HPP
#define MANTA_GAME_HPP

// MGame, the fancy way of wrapping the behavior of the engine up so I don't have to include a bunch of nonsensical code in the engine loop
//
// Template for getting your game to be recognized is as follows
//
// In your header:
// #define MANTA_GAME_TYPE YourGameClass
//
// In /manta.hpp:
// #include <your_header_file.hpp>

class MEngine;

class MGame {
public:
    virtual void Start(MEngine* engine) = 0;

    virtual void Update(MEngine* engine) = 0;
    virtual void Draw(MEngine* engine) = 0;
};


#endif //MANTA_GAME_HPP
