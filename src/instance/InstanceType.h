#ifndef INSTANCETYPE_H
#define INSTANCETYPE_H

//template test section starts
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
//template test section ends

//iterative test section starts
enum class eInstanceType {
    //testing section starts
    ObjectGeneric,
    Player,
    Npc,
    Duel,
    Spectator,
    PolyVerts,
    Displacement,
    Rotation,
    //testing section ends

    PolyVertices,
    LightAmbient,
    LightSpectral,
    LightDiffuse,
    CameraProjection,
    MaterialAmbient,
    MaterialSpectral,
    MaterialDiffuse,
    MaterialShininess,
    OrientOffset,
    OrientRotation,
    LightComposition,
    MaterialComposition,
    EntityOrientation,
    EntityPackage,
};
//iterative test section ends
#endif
