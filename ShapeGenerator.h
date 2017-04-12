#ifndef SHAPEGENERATOR_H
#define SHAPEGENERATOR_H
#include "Object.h"


class ShapeGenerator
{
public:
    ShapeGenerator();

    Object generateSphere();
    Object generateCube();
    Object generateCylinder();
    Object generateCone();
};

#endif // SHAPEGENERATOR_H
