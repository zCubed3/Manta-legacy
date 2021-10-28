//
// Created by liam on 10/26/21.
//

#ifndef MANTA_RENDERER_HPP
#define MANTA_RENDERER_HPP

#include <assets/model.hpp>
#include <unordered_map>

#include "../component.hpp"

class CRenderer : public CComponent {
public:
    std::vector<std::pair<Model **, Material **>> models;

    void Update() override;

    void Draw(Renderer *renderer, Resources *resources) override;

    void AddModel(Model **pModel, Material **pMaterial);

    void DrawImGuiWindowSub(World *world, Resources *resources, int index) override;
};


#endif //MANTA_RENDERER_HPP
