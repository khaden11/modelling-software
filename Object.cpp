#include "Object.h"
#include "Point.h"
#include "Matrix.h"
#include "math.h"


#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <QDebug>


using namespace std;

Object::Object() {
    quaternionMatrix = Matrix(Vector(1,0,0,0),
                              Vector(0,1,0,0),
                              Vector(0,0,1,0),
                              Vector(0,0,0,1));
    quat = normalize(Vector(0,0,0,1));

    panUserYDiff = 0;
    panUserYTotal = 0;
};

// create the object initialiser requiring three pieces of information taken
// from a .m file
Object::Object(std::vector<std::string> Header,
	       std::vector<std::string> Vertices,
	       std::vector<std::string> Faces)
{
    // initialise variables used
    vertexNumber = 0;
    faceNumber = 0;
 
    minX = 0; maxX = 0; minY = 0; maxY = 0; minZ = 0; maxZ = 0;

    xBegin = 0; yBegin = 0; zBegin = 0;
    dragX = 0; dragY = 0; dragZ = 0; scale = 0;

    setFileHeader(Header);
    setVertices(Vertices);
    setFaces(Faces);
    scaleObject();
}

// get the .m header information
void Object::setFileHeader(std::vector<std::string> Header)
{
    header = Header;
    for (int i = 0; i < Header.size(); i++) {
	// get the main information line of the header
	if (Header[i].find("vertices")!=string::npos) {
	    istringstream linestream(Header[i]);
            string word;
	    int j = 0;
	    while (linestream >> word) {
		if (j == 1) {
		    // get the object title
		    objectTitle = word;
		}
	        if (word.find("vertices")!=string::npos) {
		    // get and set the number of vertices for the object
		    vertexNumber = atoi(word.substr(9,word.size()).c_str());
	        }
		if (word.find("faces")!=string::npos) {
    		    // get and set the number of faces for the object
		    faceNumber = atoi(word.substr(6,word.size()).c_str()); 
		}
	        j++;
	    }
        }
    }
}

void Object::setVariables(std::string scale, std::string type,
                          std::string colour, std::string origin,
                          std::string translate)
{
    istringstream ls1(scale);
    string scaleStr;
    int j = 0;
    while (ls1 >> scaleStr) {
        if (j == 1) {
            this->scale = atof(scaleStr.c_str());
        }
        j++;
    }

    istringstream lstype(type);
    string typeStr;
    j = 0;
    while (lstype >> typeStr) {
        if (j == 1) {
            this->objectType = typeStr.c_str();
            this->objectTitle = typeStr.c_str();
        }
        j++;
    }

    istringstream linestream(colour);
    string word;
    j = 0;
    string x, y, z;
    Point p;
    // while reading over the line
    while (linestream >> word) {
        // get the x, y and z coordinate as a string
        if (j == 1) {
            x = word;
        }
        if (j == 2) {
            y = word;
        }
        if (j == 3) {
            // then at the end of the line input these values into a point
            // object
            z = word;
            istringstream strx(x);
            istringstream stry(y);
            istringstream strz(z);
            strx >> p[0];
            stry >> p[1];
            strz >> p[2];
        }
        j++;
    }
    this->objectColour = p;

    istringstream ls2(origin);
    string originStr;
    j = 0;
    string x1, y1, z1;
    Point p1;
    // while reading over the line
    while (ls2 >> originStr) {
        // get the x, y and z coordinate as a string
        if (j == 1) {
            x1 = originStr;
        }
        if (j == 2) {
            y1 = originStr;
        }
        if (j == 3) {
            // then at the end of the line input these values into a point
            // object
            z1 = originStr;
            istringstream strx(x1);
            istringstream stry(y1);
            istringstream strz(z1);
            strx >> p1[0];
            stry >> p1[1];
            strz >> p1[2];
        }
        j++;
    }
    this->xBegin = p1[0];
    this->yBegin = p1[1];
    this->zBegin = p1[2];

    istringstream ls3(translate);
    string translateStr;
    j = 0;
    string x2, y2, z2;
    Point p2;
    // while reading over the line
    while (ls3 >> translateStr) {
        // get the x, y and z coordinate as a string
        if (j == 1) {
            x2 = translateStr;
        }
        if (j == 2) {
            y2 = translateStr;
        }
        if (j == 3) {
            // then at the end of the line input these values into a point
            // object
            z2 = translateStr;
            istringstream strx(x2);
            istringstream stry(y2);
            istringstream strz(z2);
            strx >> p2[0];
            stry >> p2[1];
            strz >> p2[2];
        }
        j++;
    }
    this->dragX = p2[0];
    this->dragY = p2[1];
    this->dragZ = p2[2];
}

// set the vertices for the object
void Object::setVertices(std::vector<std::string> Vertices)
{
    // read over each string with vertex information
    for (int i = 0; i < Vertices.size(); i++) {
	istringstream linestream(Vertices[i]);
        string word;
	int j = 0;
        string x, y, z;
    	Point p;
	// while reading over the line
        while (linestream >> word) {
	    // get the x, y and z coordinate as a string
	    if (j == 2) {
		x = word;
	    }
	    if (j == 3) {
		y = word;
	    }
	    if (j == 4) {
		// then at the end of the line input these values into a point
		// object
		z = word;
		istringstream strx(x);
		istringstream stry(y);
		istringstream strz(z);
		strx >> p[0];
		stry >> p[1];
		strz >> p[2];
	    }
	    j++;
        }
	// add this point to a list of overall points
	vertices.push_back(p);
	// update the objects minimum and maximum x, y and z coords for use
	// in initially scaling the object
	if (p[0] < minX)
	    minX = p[0];
	if (p[0] > maxX)
	    maxX = p[0];
        if (p[1] < minY)
	    minY = p[1];
	if (p[1] > maxY)
	    maxY = p[1];
	if (p[2] < minZ)
	    minZ = p[2];
	if (p[2] > maxZ)
	    maxZ = p[2];
    }
}

// set the faces for the object from the information provided
void Object::setFaces(std::vector<std::string> Faces)
{
    // read in the faces information strings
    for (int i = 0; i < Faces.size(); i++) {
	istringstream linestream(Faces[i]);
        string word;
	int j = 0;
	Point p;
	// add each integer to a triangle list
        while (linestream >> word) {
	    if (j == 2 || j == 3 || j == 4) {
		unsigned int integer = atoi (word.c_str());
		triangles.push_back(integer);
	    }
	    j++;
        }
    }
}

// scale the object according to it's size to fit on screen
void Object::scaleObject()
{
    // get the difference between x and y
    float xDiff = maxX - minX;
    float yDiff = maxY - minY;
    // scale the object by x or y on screen to fit the ratio best
    if (xDiff >= yDiff)
        objectScale = 1/(xDiff*2);
    if (yDiff > xDiff)
        objectScale = 1/(yDiff*2);

    // create a scalar matrix with that value
    Matrix scalar (Vector(objectScale,0,0,0),
		   Vector(0,objectScale,0,0),
		   Vector(0,0,objectScale,0),
		   Vector(0,0,0,1));
    // transform all the vertices by the specified scalar
    for (int i = 0; i < vertexNumber; i++) {
	vertices[i] = scalar * vertices[i];
    }
}

// overload ostream operator to print point coordinates to screen
std::ostream &operator<<(std::ostream &output, const Object &o)
{
    output << o.dragX << ", " << o.dragY << ", " << o.scale << "\n";
}

// method to write object to a .m file
void Object::writeToFile(std::string fileName)
{
    // open a file stream with a modified file name
    ofstream writeFile;
    string writeFileName = fileName.substr(0,fileName.size()-2) + "write.m";
    writeFile.open(writeFileName.c_str());
    // write a header to show it is a copy of the .m file
    writeFile << "# " << objectTitle << "write.m (copied from " << objectTitle << ".m)\n";
    // write the header information in the right format
    for (int i = 0; i < header.size() ; i++) {
	writeFile << header[i] << "\n";
    }
    // write the vertex information in the right format  
    for (int i = 0; i < vertices.size(); i++) {
	Point vertex = vertices[i];
	writeFile << "Vertex " << i << " " << vertex[0] << " " << vertex[1] 
		  << " " << vertex[2] << "\n";
    }
    // write the face information in the right format
    for (int i = 0; i < triangles.size(); i = i + 3) {
	writeFile << "Face  " << i/3 << "  " << triangles[i] << "  "
		  << triangles[i+1] << "  " << triangles[i+2] << "\n";
    }
    writeFile.close();
}

Vector Object::getArcballVector(float x, float y) {
    Vector vec(x, y, 0);
    // Work out if the mouse clicked point is outside the virtual sphere
    // Use pythagoras
    float distance = pow(vec[0],2) + pow(vec[1],2);
    vec.z = sqrt((1.5*1.5) - distance);
    return normalize(vec);
}

void Object::setRotation()
{
    qDebug() << length(rotLast) << ", " << length(rotCurr);
    angle = acos(dot(rotLast, rotCurr));
    rotAxis = cross(rotLast, rotCurr);

    quat = normalize(Vector(cos(angle/2.0),
                 rotAxis[0]*sin(angle/2.0),
                 rotAxis[1]*sin(angle/2.0),
                 rotAxis[2]*sin(angle/2.0)));

    quaternionMatrix = Matrix(Vector(
                        1 - (2.0*pow(quat[2],2)) - (2.0*pow(quat[3],2)),
                        2.0*quat[1]*quat[2] - (2.0*quat[3]*quat[0]),
                        2.0*quat[1]*quat[3] + (2.0*quat[2]*quat[0]),
                        0), Vector(
                        2.0*quat[1]*quat[2] + (2.0*quat[3]*quat[0]),
                        1 - (2.0*pow(quat[1],2)) - (2.0*pow(quat[3],2)),
                        2.0*quat[2]*quat[3] - (2.0*quat[1]*quat[0]),
                        0), Vector(
                        2.0*quat[1]*quat[3] - (2.0*quat[2]*quat[0]),
                        2.0*quat[2]*quat[3] + (2.0*quat[1]*quat[0]),
                        1 - (2.0*pow(quat[1],2)) - (2.0*pow(quat[2],2)),
                        0), Vector(
                        0, 0, 0, 1)) * quaternionMatrix;
}
