#ifndef MANTA_STATS_WINDOW_HPP
#define MANTA_STATS_WINDOW_HPP

#include <vector>

class MEngine;

class StatsWindow {
public:
    bool open = false;

    int maxDeltaHistory = 10;

    std::vector<float> pastDeltaTimes;

    void DrawGUI(MEngine *engine);
};

#endif
