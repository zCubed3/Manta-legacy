#ifndef MANTA_COMPONENT_HPP
#define MANTA_COMPONENT_HPP

class AActor;
class Renderer;
class Resources;
class World;

// Also UE4-esque terminology here, components are prefixed with C, therefore CComponent!
// Components are behavior that can be attached to an actor and will be updated and drawn alongside it
class CComponent {
public:
    AActor* owner;

    bool isVisible, isEnabled;

    virtual void Update() = 0;
    virtual void Draw(Renderer *renderer, Resources *resources) = 0;
    virtual void DrawImGuiWindowSub(World *world, Resources* resources, int index) = 0;
};

#endif
