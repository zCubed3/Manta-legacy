#ifndef MANTA_DEMO_HPP
#define MANTA_DEMO_HPP

#define MANTA_GAME_TYPE MDemoGame

#include <game/game.hpp>

#define PROBE_TESTER

#ifdef PROBE_TESTER
class ALight;
#endif

class MDemoGame : public MGame {
    virtual void Start(MEngine* engine) override;

    virtual void Update(MEngine* engine) override;
    virtual void Draw(MEngine* engine) override;

    double mouseX, mouseY;
    bool lockCursor, hasLocked;

#ifdef PROBE_TESTER
    ALight *rLight, *bLight, *gLight;
#endif
};


#endif
