#include "Camera.h"
#include "math.h"
#include <QDebug>
#include "stdio.h"
#include "Matrix.h"

const float pArray[4] = {45.0, 10.0/6.5, 0.1, 100.0};
const float threeArray[9] = {15.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0};
const float topArray[9] = {0.0, 40.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0};

Camera::Camera()
{
    memcpy(&pvals, &pArray, sizeof(pArray));

    p1 = Point(0,0,0);
    p2 = Point(0,5,-1);
    p3 = Point(0,-2,5);

    rotMatrix = Matrix(Vector(1,0,0,0),Vector(0,1,0,0),
                       Vector(0,0,1,0),Vector(0,0,0,1));
}

void Camera::setMode(std::string mode) {
    cameraMode = mode;
    zoom = 1;

    if (mode == "3D") {
        memcpy(&lvals, &threeArray, sizeof(threeArray));
    }
    if (mode == "TOP") {
        memcpy(&lvals, &topArray, sizeof(topArray));
    }
}

void Camera::setRotationMatrix(Point axis) {
    /*float quat[4] = {0.1*-axis[0]*sin(angle*0.7/2.0),
                          0.1*-axis[1]*sin(angle*0.7/2.0),
                          0.1*-axis[2]*sin(angle*0.7/2.0),
                          cos(angle*0.7/2.0)};
    //memcpy(&quat, &quatArray, sizeof(quatArray));

    float quaternionMatrix[16] =
                       {1 - (2.0*pow(quat[1],2)) - (2.0*pow(quat[2],2)),
                        2.0*quat[0]*quat[1] - (2.0*quat[2]*quat[3]),
                        2.0*quat[0]*quat[2] + (2.0*quat[1]*quat[3]),
                        0,
                        2.0*quat[0]*quat[1] + (2.0*quat[2]*quat[3]),
                        1 - (2.0*pow(quat[0],2)) - (2.0*pow(quat[2],2)),
                        2.0*quat[1]*quat[2] - (2.0*quat[0]*quat[3]),
                        0,
                        2.0*quat[0]*quat[2] - (2.0*quat[1]*quat[3]),
                        2.0*quat[1]*quat[2] + (2.0*quat[0]*quat[3]),
                        1 - (2.0*pow(quat[0],2)) - (2.0*pow(quat[1],2)),
                        0,
                        0, 0, 0, 1};*/

    float ang = angle*(M_PI/180);

    rotMatrix = Matrix(
                Vector(cos(ang),0,-sin(ang),0),
                Vector(0,1,0,0),
                Vector(sin(ang),0,cos(ang),0),
                Vector(0,0,0,1));

    //memcpy(&matrixQuat, &quaternionMatrix, sizeof(quaternionMatrix));
}

void Camera::setPan(Point intersect) {
    pan = pan + Vector((panBegin[0] - intersect[0]),
                       (panBegin[1] - intersect[1]),
                       (panBegin[2] - intersect[2]));
    updateDrawPlane();
}

void Camera::updateDrawPlane() {

    /*Matrix panMatrix(Vector(1,0,0,0),
                     Vector(0,1,0,0),
                     Vector(0,0,1,0),
                     Vector(pan[0]*0.1,pan[1]*0.1,pan[2]*0.1,1));*/
    Matrix panMatrix(Vector(1,0,0,pan[0]*0.1),
                     Vector(0,1,0,pan[1]*0.1),
                     Vector(0,0,1,pan[2]*0.1),
                     Vector(0,0,0,1));

    p1 = Point(0,0,0);
    p2 = Point(0,5,-1);
    p3 = Point(0,-2,5);

    p1 = rotMatrix * p1;
    p2 = rotMatrix * p2;
    p3 = rotMatrix * p3;

    p1 = panMatrix * p1;
    p2 = panMatrix * p2;
    p3 = panMatrix * p3;
}

Vector Camera::getArcballVector(float x, float y) {
    Vector vec(x, y, 0);
    // Work out if the mouse clicked point is outside the virtual sphere
    // Use pythagoras
    float distance = pow(vec[0],2) + pow(vec[1],2);
    vec.z = sqrt((1.5*1.5) - distance);
    return vec;
}

void Camera::setRotation() {
    float dir = dot(cross(rotCurr, rotLast), Vector(0,0,1));
    if (dir < 0) {
        rotDir = 1;
    }
    if (dir >= 0) {
        rotDir = -1;
    }
    if (rotDir == 1) {
        angle = fmod((angle + dot(rotCurr, rotLast)*0.7),360);
    }
    if (rotDir == -1) {
        angle = fmod((angle - dot(rotCurr, rotLast)*0.7),360);
    }
    rotLast = rotCurr;

    setRotationMatrix(Point(0,1,0));
    updateDrawPlane();
}

