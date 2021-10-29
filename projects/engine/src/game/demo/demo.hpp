#ifndef MANTA_DEMO_HPP
#define MANTA_DEMO_HPP

#define MANTA_GAME_TYPE MDemoGame

#include <game/game.hpp>

class MDemoGame : public MGame {
    virtual void Start(MEngine* engine) override;

    virtual void Update(MEngine* engine) override;
    virtual void Draw(MEngine* engine) override;
};


#endif
