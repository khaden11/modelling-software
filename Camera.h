#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include "Point.h"
#include "Matrix.h"

class Camera
{
public:
    Camera();

    void setMode(std::string mode);

    void setRotationMatrix(Point axis);

    void updateDrawPlane();

    void setPan(Point intersect);

    Vector getArcballVector(float x, float y);
    void setRotation();

    // variables storing camera's translation information

    float quat[4];
    Matrix rotMatrix;
    float matrixQuat[16];

    // Values to store the perspective and lookat variables
    float lvals[9];
    float pvals[4];

    // Store the camera zoom variable
    float zoom;
    // Store the cameras pan variables
    Point panBegin, pan;
    // Store the cameras rotation variables
    float angle, rotDir;
    Vector rotLast, rotCurr;

    // Point objects to store points on the drawplane
    Point p1, p2, p3;

    std::string cameraMode;
};

#endif // CAMERA_H
