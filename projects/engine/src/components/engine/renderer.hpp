//
// Created by liam on 10/26/21.
//

#ifndef MANTA_RENDERER_HPP
#define MANTA_RENDERER_HPP

#include <assets/model.hpp>

#include "../component.hpp"

class CRenderer : public CComponent {
public:
    std::vector<Model**> models;

    void Update() override;

    void Draw(Renderer *renderer, Resources *resources) override;

    void DrawImGuiWindowSub(World *world, Resources* resources, int index) override;
};


#endif //MANTA_RENDERER_HPP
