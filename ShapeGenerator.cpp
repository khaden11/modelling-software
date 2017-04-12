#include "ShapeGenerator.h"
#include "math.h"



ShapeGenerator::ShapeGenerator()
{
}

// function to generate a sphere .m file and render it to screen
Object ShapeGenerator::generateCube()
{
    std::vector<Point> vertices;
    std::vector<unsigned int> faces;
    std::vector<Point> coords;

    Point vertexList[8] = {Point(-1,-1,-1), Point(-1,-1,1),
                           Point(-1,1,-1), Point(-1,1,1),
                           Point(1,-1,-1), Point(1,-1,1),
                           Point(1,1,-1), Point(1,1,1)};
    for (int i = 0; i < 8; i++) {
        vertices.push_back(vertexList[i]);
    }

    unsigned int faceList[36] = {0,1,3, 0,3,2,
                                 0,4,5, 0,5,1,
                                 1,5,7, 1,7,3,
                                 2,3,7, 2,7,6,
                                 2,0,4, 4,2,6,
                                 4,6,7, 4,7,5};
    for (int i = 0; i < 36; i++) {
        faces.push_back(faceList[i]);
    }

    // create an object with the information calculated
    Object cube;
    cube.vertices = vertices;
    cube.normals = vertices;
    cube.triangles = faces;
    cube.tex2dCoords = coords;
    // make sure the face number is the faces.size/3!
    cube.faceNumber = faces.size()/3;
    cube.vertexNumber = vertices.size();
    // default initial colour red
    cube.objectColour = Point(0.5, 0.5, 0.5);
    cube.objectType = "cube";
    cube.objectTitle = "cube";

    return cube;
}

// function to generate a sphere .m file and render it to screen
Object ShapeGenerator::generateSphere()
{
    int nRings = 20;
    int nTrianglesPerRing = 20;
    float radius = 0.75;
    // initialise vectors to hold the sphere's information
    std::vector<Point> vertices;
    std::vector<unsigned int> faces;
    std::vector<Point> coords;

    // index to keep track of the face indices
    float pointIndex = 0;

    // convert number of rings and triangles per rings to floats
    float ringFloat = (float)nRings;
    float triangleFloat = (float)nTrianglesPerRing;

    // x is an indexing variable that keeps track of the x texture coords
    float x = 0;
    // dX and dY are incremental counters to x and y indices
    float dX = 1/ringFloat;
    float dY = 1/triangleFloat;

    // loop over the rings in the sphere and calculate phis and nextPhis
    for (int ring = 0; ring < nRings; ring++, x+=dX) {

        float phi = (-M_PI / 2.0) + (ring * (M_PI/nRings));
        float cosPhi = cos(phi);
        float sinPhi = sin(phi);
        float nextPhi = (-M_PI / 2.0) + ((ring+1) * (M_PI/nRings));
        float cosNextPhi = cos(nextPhi);
        float sinNextPhi = sin(nextPhi);

        float y = 1;

        // loop over the triangles in the ring and calculate thetas and
        // nextthetas
        for (int triangle = 0; triangle < nTrianglesPerRing; triangle++, y-=dY) {
            float theta = triangle*((2*M_PI)/nTrianglesPerRing);
            float nextTheta = (triangle+1)*((2*M_PI)/nTrianglesPerRing);
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);
            float cosNextTheta = cos(nextTheta);
            float sinNextTheta = sin(nextTheta);

            // calculate the points for each vertex
            Point one(radius*cosPhi*cosTheta,
                      radius * cosPhi * sinTheta,
                      radius * sinPhi);
            Point two(radius*cosPhi*cosNextTheta,
                      radius * cosPhi * sinNextTheta,
                      radius * sinPhi);
            Point three(radius*cosNextPhi*cosNextTheta,
                      radius*cosNextPhi*sinNextTheta,
                      radius*sinNextPhi);
            Point four(radius*cosNextPhi*cosTheta,
                       radius*cosNextPhi*sinTheta,
                       radius*sinNextPhi);

            // put them into the vertex list
            vertices.push_back(one);
            vertices.push_back(two);
            vertices.push_back(three);
            vertices.push_back(four);

            // put in the texture coordinates for these points based on the
            // counter indexes
            coords.push_back(Point(y,x,0));
            coords.push_back(Point(y-dY,x,0));
            coords.push_back(Point(y-dY,x+dX,0));
            coords.push_back(Point(y,x+dX,0));

        // add the two triangles that form a quad for the face indexes
            faces.push_back(pointIndex);
            faces.push_back(pointIndex+1);
        faces.push_back(pointIndex+2);
            faces.push_back(pointIndex);
        faces.push_back(pointIndex+2);
        faces.push_back(pointIndex+3);

            // add to the point index
            pointIndex = pointIndex + 4;

        }
    }
    // create an object with the information calculated
    Object sphere;
    sphere.vertices = vertices;
    sphere.normals = vertices;
    sphere.triangles = faces;
    sphere.tex2dCoords = coords;
    // make sure the face number is the faces.size/3!
    sphere.faceNumber = faces.size()/3;
    sphere.vertexNumber = vertices.size();
    // default initial colour red
    sphere.objectColour = Point(0.5, 0.5, 0.5);
    sphere.objectType = "sphere";
    sphere.objectTitle = "sphere";

    return sphere;
}

// function to generate a sphere .m file and render it to screen
Object ShapeGenerator::generateCylinder()
{
    // initialise vectors to hold the sphere's information
    std::vector<Point> vertices;
    std::vector<unsigned int> faces;
    std::vector<Point> coords;

    float nSegments = 20.0;
    int index = 0;

    for (float i = 0.0; i < nSegments; i += 1.0)
      { /* a loop around circumference of a tube */
      float angle = M_PI * i * 2.0 / nSegments ;
      float nextAngle = M_PI * (i + 1.0) * 2.0 / nSegments;
      /* compute sin & cosine */
      float x1 = sin(angle), y1 = cos(angle);
      float x2 = sin(nextAngle), y2 = cos(nextAngle);

      vertices.push_back(Point(x1,y1,0.0));
      vertices.push_back(Point(x2,y2,1.0));
      vertices.push_back(Point(x1,y1,1.0));

      vertices.push_back(Point(x2,y2,0.0));
      vertices.push_back(Point(x2,y2,1.0));

      faces.push_back(index);
      faces.push_back(index+1);
      faces.push_back(index+2);

      faces.push_back(index);
      faces.push_back(index+3);
      faces.push_back(index+4);

      index = index + 5;
      }


    for (int vertex = 0; vertex < 20; vertex++)
      { // vertex loop
      float theta = vertex * (2 * M_PI) / nSegments; // in radians
      float x = sin(theta);
      float y = cos(theta);
      float z = 0.0;
      vertices.push_back(Point(x,y,z));
     } // vertex loop

    for (int triangle = 0; triangle < 18; triangle++) {
        faces.push_back(index);
        faces.push_back(index+1+triangle);
        faces.push_back(index+2+triangle);
    }

    for (int vertex = 0; vertex < 20; vertex++)
      { // vertex loop
      float theta = vertex * (2 * M_PI) / nSegments; // in radians
      float x = sin(theta);
      float y = cos(theta);
      float z = 1.0;
      vertices.push_back(Point(x,y,z));
     } // vertex loop

    index = index + 20;

    for (int triangle = 0; triangle < 18; triangle++) {
        faces.push_back(index);
        faces.push_back(index+1+triangle);
        faces.push_back(index+2+triangle);
    }

    Object cylinder;
    cylinder.vertices = vertices;
    cylinder.normals = vertices;
    cylinder.triangles = faces;
    // make sure the face number is the faces.size/3!
    cylinder.faceNumber = faces.size()/3;
    cylinder.vertexNumber = vertices.size();
    // default initial colour red
    cylinder.objectColour = Point(0.5, 0.5, 0.5);
    cylinder.objectType = "cylinder";
    cylinder.objectTitle = "cylinder";

    return cylinder;
}

// function to generate a sphere .m file and render it to screen
Object ShapeGenerator::generateCone()
{
    // initialise vectors to hold the sphere's information
    std::vector<Point> vertices;
    std::vector<unsigned int> faces;
    std::vector<Point> coords;

    float nSegments = 20.0;
    int index = 0;

    for (float i = 0.0; i < nSegments; i += 1.0)
      { /* a loop around circumference of a tube */
      float angle = M_PI * i * 2.0 / nSegments ;
      float nextAngle = M_PI * (i + 1.0) * 2.0 / nSegments;
      /* compute sin & cosine */
      float x1 = sin(angle), y1 = cos(angle);
      float x2 = sin(nextAngle), y2 = cos(nextAngle);

      vertices.push_back(Point(x1,y1,0.0));
      vertices.push_back(Point(x2,y2,0.0));
      vertices.push_back(Point(0.0,0.0,1.0));

      faces.push_back(index);
      faces.push_back(index+1);
      faces.push_back(index+2);

      index = index + 3;
      }


    for (int vertex = 0; vertex < 20; vertex++)
      { // vertex loop
      float theta = vertex * (2 * M_PI) / nSegments; // in radians
      float x = sin(theta);
      float y = cos(theta);
      float z = 0.0;
      vertices.push_back(Point(x,y,z));
     } // vertex loop

    for (int triangle = 0; triangle < 18; triangle++) {
        faces.push_back(index);
        faces.push_back(index+1+triangle);
        faces.push_back(index+2+triangle);
    }

    Object cylinder;
    cylinder.vertices = vertices;
    cylinder.normals = vertices;
    cylinder.triangles = faces;
    // make sure the face number is the faces.size/3!
    cylinder.faceNumber = faces.size()/3;
    cylinder.vertexNumber = vertices.size();
    // default initial colour red
    cylinder.objectColour = Point(0.5, 0.5, 0.5);
    cylinder.objectType = "cone";
    cylinder.objectTitle = "cone";

    return cylinder;
}

