#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <list>
#include <string>
#include <vector>
#include "Point.h"
#include "Vector.h"
#include "Matrix.h"

#include <QPoint>
#include <iostream>

class Object
{
public:
    // MAIN OBJECT STORAGE VARIABLES
    std::string objectName;
    std::string objectType;

    // can be used to hold normals for a particular object
    std::vector<Point> normals;
    // tex coordinates for object stored here
    std::vector<Point> tex2dCoords;
    // Used to store three values for colour of the object
    Point objectColour;

    // variables to store vertex, face number and a title
    int vertexNumber;
    int faceNumber;
    std::string objectTitle;

    // variables to store information about the size of an object
    float minX, maxX, minY, maxY, minZ, maxZ;
    float objectScale;

    // variable to store the .m file header
    std::vector<std::string> header; 
    
    // object should have a list of points for the vertices
    std::vector<Point> vertices; 

    // store a list of unsigned ints for the object's faces
    std::vector<unsigned int> triangles; 
    


    QPoint dragBegin;
    float xBegin, yBegin, zBegin;
    float dragX, dragY, dragZ;
    float scale;

    Object();

    // initialiser using information from a .m file
    Object(std::vector<std::string> Header,
	   std::vector<std::string> Vertices,
	   std::vector<std::string> Faces);
    
    // set the objects information using the .m file information
    void setFileHeader(std::vector<std::string> Header);
    void setVertices(std::vector<std::string> Vertices);
    void setFaces(std::vector<std::string> Faces);
    // write the file to a specific path
    void writeToFile(std::string filePath);

    void setVariables(std::string scale, std::string type,
                      std::string colour, std::string origin,
                      std::string translate);

    Vector getArcballVector(float x, float y);
    Vector rotLast, rotCurr, rotAxis;
    float angle;
    Vector quat;
    Matrix quaternionMatrix;
    void setRotation();

    // method to scale object to fit on screen
    void scaleObject();
    // method to generate a list of colours for each triangle
    void generateColours();   

    // overload ostream operator to print point coordinates to screen
    friend std::ostream &operator<<(std::ostream &output, const Object &o);

    float panUserYBegin, panUserYDiff, panUserYTotal;
};



#endif
