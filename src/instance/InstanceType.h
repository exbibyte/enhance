#ifndef INSTANCETYPE_H
#define INSTANCETYPE_H

class InstanceType {
public:
    class ObjectGeneric{};
    class Player{};
    class Npc{};
    class Duel{};
    class Spectator{};

    class PolyVerts{};
    class Displacement{};
    class Rotation{};
};

enum class eInstanceType {
    ObjectGeneric,
    Player,
    Npc,
    Duel,
    Spectator,
    PolyVerts,
    Displacement,
    Rotation,
};

#endif
