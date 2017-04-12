#include "PolygonWidget.h"
#include "Object.h"
#include "Camera.h"
#include <QtGui>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>

#include <cstring>
#include <QPushButton>
#include "FileHandler.h"

#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

// constructor
PolygonWidget::PolygonWidget(QWidget *parent)
: QGLWidget(parent)
{
    selectedSomething = false;
    selectedPixels = Point(0,0,0);
    selectedIndex = 1000;

    panObject = false;
    userTranslateY = false;
    objectSelected = false;

    dragShape = false;

    // set up the initial bools and camera variables
    changeCamera = true;
    cameraMode = "3D";
    gridviewBool = true;
    drawPlaneBool = false;
    // zero pan, zoom and rotate values.
    //cameraPan = Point(0,0,0);
    cameraRotateAngle = 0.0;
    cameraRotateAxis = Vector(0,0,0);

    shapeGenerator = new ShapeGenerator();
    fileHandler = new FileHandler();

    QObject::connect(fileHandler, SIGNAL(loadObjectsToScreen(std::vector<Object>)),
            this, SLOT(receiveObjects(std::vector<Object>)));

    cam = Camera();
    cam.setMode("3D");
} 

// constructor
// called when OpenGL context is set up
void PolygonWidget::initializeGL()
{ 
    // initializeGL()
    // set the widget background colour
    glClearColor(0.3, 0.3, 0.3, 0.0);
} 

// initializeGL()
// called every time the widget is resized
void PolygonWidget::resizeGL(int w,  int h)
{ 
    // resizeGL()
    // set the viewport to the entire widget
    glViewport(0, 0, w, h);
} 

// called every time the widget needs painting
void PolygonWidget::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective( cam.pvals[0], cam.pvals[1],
                    cam.pvals[2], cam.pvals[3] );

    // clear the widget - use depth buffer for rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_FUNC);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    // clear previously specified colours
    glClearColor (0.3, 0.3, 0.3, 0.0);
    glLineWidth(1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam.lvals[0], cam.lvals[1], cam.lvals[2],
              cam.lvals[3], cam.lvals[4], cam.lvals[5],
              cam.lvals[6], cam.lvals[7], cam.lvals[8]);

    // Update the view with the cameras stored scale, translation and rotation values
    glScalef(cam.zoom, cam.zoom, cam.zoom);
    glRotatef(cam.angle, 0, 1, 0);

    glTranslatef(-cam.pan[0]*0.1, -cam.pan[1]*0.1, -cam.pan[2]*0.1);


    // BEFORE ANYTHING IS RENDERED TO SCREEN, CHECK IF USER IS SELECTING OBJECT
    // TO RENDER OBJECTS IN DIFFERENT COLOURS INITIALLY
    if (selectedSomething) {

        // for all objects that have been initialised on screen
        for (int i = 0; i < loadedObjects.size(); i++) {

            Object currObject = loadedObjects[i];
            // Assign an easy i to colour identifier
            Point colour(i/255.0f,0.5,0.5);
            // push the values to the model matrix so you can draw each object
            // with their specified transforms
            glPushMatrix();

            // call the opengl functions scale, translate and rotate
            glTranslatef(currObject.xBegin + currObject.dragX,
                         currObject.yBegin + currObject.dragY,
                         currObject.zBegin + currObject.dragZ);

            if (currObject.objectType == "sphere") {
                glScalef(currObject.scale, currObject.scale, currObject.scale);
            }
            if (currObject.objectType == "cube") {
                glScalef(currObject.scale*0.5, currObject.scale*0.5, currObject.scale*0.5);
            }
            if (currObject.objectType == "cylinder") {
                glScalef(currObject.scale, currObject.scale, currObject.scale);
            }
            if (currObject.objectType == "cone") {
                glScalef(currObject.scale, currObject.scale, currObject.scale);
            }

            Matrix quat = currObject.quaternionMatrix;
            float quaternionMatrix[16];
            int index = 0;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    quaternionMatrix[index] = quat[i][j];
                    index++;
                }
            }
            glMultMatrixf(quaternionMatrix);

            glTranslatef(0,currObject.panUserYTotal + currObject.panUserYDiff,0);

            // Render the object with the generated ID colour
            renderObject(currObject, colour);
            // Pop matrix after drawing object
            glPopMatrix();
        }

        glFlush();
        glFinish();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        unsigned char data[4];

        glReadPixels(selectedPixels[0], selectedPixels[1],1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        int pickedID = (int)data[0];
        if (pickedID == 76) {
            selectedIndex = 1000;
            objectSelected = false;
            selectedSomething = false;
            changeCamera = true;
            clearObjectSelectedInfo();
            updateGL();
        }
        else {
            selectedIndex = pickedID;
            objectSelected = true;
            selectedSomething = false;
            changeCamera = false;
            updateGL();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_FUNC);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glClearColor (0.3, 0.3, 0.3, 0.0);

    }

    if (panObject) {
        Object currObject = loadedObjects[selectedIndex];

        // push the values to the model matrix so you can draw each object
        // with their specified transforms
        glPushMatrix();

        // call the opengl functions scale, translate and rotate
        glTranslatef(currObject.xBegin + currObject.dragX,
                     currObject.yBegin + currObject.dragY,
                     currObject.zBegin + currObject.dragZ);

        if (currObject.objectType == "sphere") {
            glScalef(currObject.scale, currObject.scale, currObject.scale);
        }
        if (currObject.objectType == "cube") {
            glScalef(currObject.scale*0.5, currObject.scale*0.5, currObject.scale*0.5);
        }
        if (currObject.objectType == "cylinder") {
            glScalef(currObject.scale, currObject.scale, currObject.scale);
        }
        if (currObject.objectType == "cone") {
            glScalef(currObject.scale, currObject.scale, currObject.scale);
        }

        Matrix quat = currObject.quaternionMatrix;
        float quaternionMatrix[16];
        int index = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                quaternionMatrix[index] = quat[i][j];
                index++;
            }
        }
        glMultMatrixf(quaternionMatrix);

        glTranslatef(0,currObject.panUserYTotal + currObject.panUserYDiff,0);

        drawAxes();

        glPopMatrix();

        glFlush();
        glFinish();

        unsigned char data[4];
        glReadPixels(selectedPanPixels[0], selectedPanPixels[1],1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        int pickedID = (int)data[0];
        if (pickedID == 76) {
            panObject = false;
            updateGL();
        }
        if (pickedID == 255) {
            panObject = false;
            userTranslateY = true;
            currObject.panUserYBegin = selectedPanPixels[1];
            loadedObjects[selectedIndex] = currObject;
            updateGL();
        }
        else {
            panObject = false;
            updateGL();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_FUNC);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glClearColor (0.3, 0.3, 0.3, 0.0);
    }


    if (drawPlaneBool) {
        float ang = -cam.angle*(M_PI/180);
        float zoominv = 1 + (1 - cam.zoom);
        float rotMatrix[16] =
                    {cos(ang),0,-sin(ang),0,0,1,0,0,
                    sin(ang),0,cos(ang),0,0,0,0,1};
        float panMatrix[16] = {1,0,0,0,
                               0,1,0,0,
                               0,0,1,0,
                               cam.pan[0]*0.1,cam.pan[1]*0.1,cam.pan[2]*0.1,1};
        float zoomMatrix[16] = {zoominv,0,0,0,
                                0,zoominv,0,0,
                                0,0,zoominv,0,
                                0,0,0,1};

        glColor3f(0.0,1.0,0.0);

        glPushMatrix();
        glMultMatrixf(panMatrix);
        glMultMatrixf(rotMatrix);
        glMultMatrixf(zoomMatrix);
        glBegin(GL_LINES);
        glVertex3f(0,-50,0);
        glVertex3f(0,50,0);

        glVertex3f(0,0,-50);
        glVertex3f(0,0,50);

        glEnd();
        glPopMatrix();
    }

    // if the grid view checkbox is selected, draw a 10 x 10 grid to help view the
    // camera's current view and translation
    if (gridviewBool)
    {
        glColor3f(1.0,1.0,1.0);
        // loop from 1 to 10 and draw a grid on the x/z axes
        glBegin(GL_LINES);
        for (int i=-300;i<=300;i=i+2)
        {
            glVertex3f(i,0,-300);
            glVertex3f(i,0,300);

            glVertex3f(300,0,i);
            glVertex3f(-300,0,i);
        }
        glEnd();
    }
    GLfloat LightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat LightShine[]= { 50.0f };
    GLfloat LightPosition[]= { 1.0f, 1.0f, 1.0f, 1.0f };



     //Add lighting
    /*glLightfv(GL_LIGHT0 ,GL_SHININESS, LightShine);
    glLightfv(GL_LIGHT0 ,GL_SPECULAR, LightSpecular);
    glLightfv(GL_LIGHT0 ,GL_POSITION, LightPosition);
    // enable the appropriate lighting methods
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);*/
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel (GL_SMOOTH);

    // for all objects that have been initialised on screen
    for (int i = 0; i < loadedObjects.size(); i++) {

        Object currObject = loadedObjects[i];

        // push the values to the model matrix so you can draw the earth with the
        // transformed values from the sliders
        glPushMatrix();


        // call the opengl functions scale, translate and rotate
        glTranslatef(currObject.xBegin + currObject.dragX,
                     currObject.yBegin + currObject.dragY,
                     currObject.zBegin + currObject.dragZ);



        Matrix quat = currObject.quaternionMatrix;
        float quaternionMatrix[16];
        int index = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                quaternionMatrix[index] = quat[i][j];
                index++;
            }
        }


        glMultMatrixf(quaternionMatrix);

        glTranslatef(0,currObject.panUserYTotal + currObject.panUserYDiff,0);

        if (currObject.objectType == "sphere") {
            glScalef(currObject.scale, currObject.scale, currObject.scale);
        }
        if (currObject.objectType == "cube") {
            glScalef(currObject.scale*0.5, currObject.scale*0.5, currObject.scale*0.5);
        }
        if (currObject.objectType == "cylinder") {
            glScalef(currObject.scale, currObject.scale, currObject.scale);
        }
        if (currObject.objectType == "cone") {
            glScalef(currObject.scale, currObject.scale, currObject.scale);
        }

        renderObject(currObject, currObject.objectColour);

        if (i == selectedIndex) {
            glDisable(GL_COLOR_MATERIAL);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_DEPTH_FUNC);
            drawAxes();
            glEnable(GL_DEPTH_FUNC);
            glDepthFunc(GL_LESS);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_COLOR_MATERIAL);

            stringstream ss;
            ss << i;
            string str = ss.str();
            currObject.objectTitle = currObject.objectTitle + "_" + str;
            updateObjectSelectedInfo(currObject);
        }

        // go back to the normal non-transformed matrix
        glPopMatrix();

    }

    // flush to screen
    glFlush();

    // disable lighting methods and texture2d after drawing
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);

}

// If the user selects a different camera view, update the variables
// by resetting the current camera transforms and modelview transformations
void PolygonWidget::changeCameraMode(int selectedMode)
{
    if (selectedMode == 0) {
        cam.setMode("3D"); cam.pan = Point(0,0,0), cam.zoom = 1;
        glLoadIdentity();
        updateGL();}
    if (selectedMode == 1) {
        cam.setMode("TOP"); cameraPan = Point(0,0,0);
        glLoadIdentity();
        updateGL();}
}

// when the checkbox view grid is selected, update the bool and updateGL
void PolygonWidget::gridViewMode(bool selected)
{
    gridviewBool = selected; updateGL();
}

// when the checkbox view grid is selected, update the bool and updateGL
void PolygonWidget::drawPlaneMode(bool selected)
{
    drawPlaneBool = selected; updateGL();
}

// function to set the loaded object of the widget with a given widget object
void PolygonWidget::setLoadedObject(Object &object)
{
    object.scale = 0;
    qDebug() << "vertexnum = " << object.vertexNumber << ", facenum = " << object.faceNumber;

    loadedObjects.push_back(object);

    // set the boolean object loaded to true (program knows to draw or not)
    drawing = true;
    changeCamera = false;
    updateGL();
}

// function to render an object requiring a texture
void PolygonWidget::renderObject(Object &object, Point &colour)
{
    // get the object's vertex/face/normal/tex values
    objectVertices = object.vertices;
    objectTriangles = object.triangles;
    std::vector<Point> normals = object.normals;
    // Get the specified object colour
    float R = colour[0];
    float G = colour[1];
    float B = colour[2];

    // loop over all the vertices, drawing a triangle with the three vertices
    // (specified in faces) along with their normals and texCoords
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < object.faceNumber; i++) {
        for (int j = 0; j < 3; j++) {
            // should eventually allow colour specification for each object
            glColor3f(R, G, B);
            /*glNormal3f(normals[objectTriangles[i*3+j]][0],
               normals[objectTriangles[i*3+j]][1],
               normals[objectTriangles[i*3+j]][2]);*/
            glVertex3f(objectVertices[objectTriangles[i*3+j]][0],
               objectVertices[objectTriangles[i*3+j]][1],
               objectVertices[objectTriangles[i*3+j]][2]);

        }
    }
    glEnd();
}

// Using the current projection and view matrices, get the mouse clicked
// position in three coordinates
Point PolygonWidget::getMouseModelCoords(QMouseEvent *event)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
    // Get the current information
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    // get the window coordinates
    winX = (float)event->x();
    winY = (float)viewport[3] - (float)event->y();
    glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT,
                                        GL_FLOAT, &winZ );

    // convert to view coordinates using the projection and viewport
    gluUnProject( winX, winY, winZ, modelview, projection, viewport,
                  &posX, &posY, &posZ);

    return Point(posX, posY, posZ);
}

// Using the current projection and view matrices, get the mouse clicked
// position in three coordinates
Point PolygonWidget::getMouseModelCoords(QMouseEvent *event, float depth)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
    // Get the current information
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    // get the window coordinates
    winX = (float)event->x();
    winY = (float)viewport[3] - (float)event->y();

    // convert to view coordinates using the projection and viewport and
    // a specified camera depth z value
    gluUnProject( winX, winY, depth, modelview, projection, viewport,
                  &posX, &posY, &posZ);

    return Point(posX, posY, posZ);
}

// Method to get point of intersection between a ray and a plane
Point PolygonWidget::getRayPlaneIntersection(Point camPos, Point endPos,
                              Point pointOnPlane, Point point1,
                              Point point2)
{
    // Get the vector the ray is casting
    Vector rayDirection = normalize(camPos - endPos);

    // Calculate two vectors on that plane
    Vector planeVec1 = (point1 - pointOnPlane);
    Vector planeVec2 = (point2 - pointOnPlane);
    // Get the plane normal
    Vector planeNormal = cross(planeVec1, planeVec2);
    // Calculate the t value used in the ray equation S = P0 + t*vector
    float t = dot(planeNormal,(pointOnPlane - camPos))/
                 (dot(planeNormal, rayDirection));
    // Calculate the point of intersection on the XZ plane
    Point intersection = camPos + (t*rayDirection);

    return intersection;
}

// Get if the mouse wheel scrolls and change the zoom accordingly
void PolygonWidget::wheelEvent(QWheelEvent *event)
{
    // If the camera is selected, update the camera's zoom variable
    if (changeCamera) {
        zoomCamera = true;
        // if negative, zoom out, if positive zoom in
        if (event->delta() < 0) {
            cam.zoom *= 0.9;
        }
        if (event->delta() > 0) {
            cam.zoom *= 1.1;
        }
        updateGL();
        zoomCamera = false;
    }
}

// Overload on mouse press depending on what the user wants to do
void PolygonWidget::mousePressEvent(QMouseEvent *event)
{
    printBools();

    // Get the current mouse coordinates
    Point worldCoords = getMouseModelCoords(event);

    // Get the point where the mouse is from the cameras near and far field
    // used to get intersection of a plane when drawing objects later
    Point camPos = getMouseModelCoords(event, 0.0);
    Point endPos = getMouseModelCoords(event, 1.0);

    Point drawClick;
    Point panClick;
    // Get the intersection point of the ray cast from the mouse and the
    // plane the grid is drawn on. Give the argument of the mouse points
    // and three points on the correct plane
    // If 3D or top view, intersect the XZ plane
    if (cameraMode == "3D" || cameraMode == "TOP") {
        drawClick = getRayPlaneIntersection(camPos, endPos,
                                               cam.p1,
                                               cam.p2,
                                               cam.p3);
        panClick = getRayPlaneIntersection(camPos, endPos,
                                                Point(0,0,0),
                                                Point(1,0,1),
                                                Point(3,0,4));
    }

    // If the user is using the left button and an object is not selected
    if ((event->buttons() & Qt::LeftButton) && changeCamera) {
        // start to translate the camera using the initial coordinate
        cam.panBegin = Point(panClick[0], panClick[1], panClick[2]);
        updateGL();
    }

    // If using the right button, start the rotation of the camera getting
    // the position of the mouse on the trackball
    if (event->buttons() & Qt::RightButton && changeCamera) {
        float x =  (event->x()/(float)(this->width())*2.0) - 1.0;
        float y = -((event->y()/(float)(this->height())*2.0) - 1.0);
        // OLD ARCBALL CODE
        cam.rotLast = cam.getArcballVector(x, y);
    }

    // If the user is using the left button and an object is not selected
    if ((event->buttons() & Qt::RightButton) && objectSelected) {
        // start to translate the camera using the initial coordinate
        float x =  (event->x()/(float)(this->width())*2.0) - 1.0;
        float y = -((event->y()/(float)(this->height())*2.0) - 1.0);
        Object selectedObj = loadedObjects[selectedIndex];
        selectedObj.rotLast = selectedObj.getArcballVector(x, y);
        loadedObjects[selectedIndex] = selectedObj;
        updateGL();
    }

    if ((event->buttons() & Qt::LeftButton) && objectSelected) {
        panObject = true;

        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY;
        // Get the current information
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetIntegerv( GL_VIEWPORT, viewport );

        // get the window coordinates
        winX = (float)event->x();
        winY = (float)viewport[3] - (float)event->y();
        selectedPanPixels[0] = winX;
        selectedPanPixels[1] = winY;

        updateGL();
    }


    // If drawing using the left button, drag a shape onto the screen,
    // using the mouse coordinates as to where to start drawing the shape
    if (event->button() == Qt::LeftButton && drawing) {
        dragShape = true;

        Object object = loadedObjects.back();
        object.xBegin = drawClick[0];
        object.yBegin = drawClick[1];
        object.zBegin = drawClick[2];
        object.scale = 0;

        // update the object with the dragging coordinates
        loadedObjects.back() = object;

        updateGL();
    }
}

// Overload the mouse move while clicked event
void PolygonWidget::mouseMoveEvent(QMouseEvent *event)
{
    printBools();

    Point worldCoords = getMouseModelCoords(event);

    // Get the point where the mouse is from the cameras near and far field
    // used to get intersection of a plane when drawing objects later
    Point camPos = getMouseModelCoords(event, 0.0);
    Point endPos = getMouseModelCoords(event, 1.0);

    Point drawClick;
    Point panClick;
    // Get the intersection point of the ray cast from the mouse and the
    // plane the grid is drawn on. Give the argument of the mouse points
    // and three points on the correct plane
    // If 3D or top view, intersect the XZ plane
    if (cameraMode == "3D" || cameraMode == "TOP") {
        drawClick = getRayPlaneIntersection(camPos, endPos,
                                               cam.p1,
                                               cam.p2,
                                               cam.p3);
        panClick = getRayPlaneIntersection(camPos, endPos,
                                                Point(0,0,0),
                                                Point(1,0,1),
                                                Point(3,0,4));
    }

    // If the user is dragging with the LMB, calculate the new panning coords
    if ((event->buttons() & Qt::LeftButton) && changeCamera) {
        cam.setPan(panClick);
        updateGL();
    }

    // If the user is dragging with the LMB, calculate the new panning coords
    if ((event->buttons() & Qt::LeftButton) && userTranslateY) {
        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winY;
        // Get the current information
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetIntegerv( GL_VIEWPORT, viewport );

        winY = (float)viewport[3] - (float)event->y();
        Object currObject = loadedObjects[selectedIndex];
        currObject.panUserYDiff = winY - currObject.panUserYBegin;
        loadedObjects[selectedIndex] = currObject;

        updateGL();
    }

    // If the user is dragging with the RMB, calculate the rotation coords
    if (event->buttons() & Qt::RightButton && changeCamera) {
        float x =  (event->x()/(float)(this->width())*2.0) - 1.0;
        float y = -((event->y()/(float)(this->height())*2.0) - 1.0);
        //cameraRotateCurr = Vector(x, y, 0);
        cam.rotCurr = cam.getArcballVector(x, y);
        cam.setRotation();
        updateGL();
    }

    // If the user is using the left button and an object is not selected
    if ((event->buttons() & Qt::RightButton) && objectSelected) {
        // start to translate the camera using the initial coordinate
        float x =  (event->x()/(float)(this->width())*2.0) - 1.0;
        float y = -((event->y()/(float)(this->height())*2.0) - 1.0);
        Object selectedObj = loadedObjects[selectedIndex];
        selectedObj.rotCurr = selectedObj.getArcballVector(x, y);
        selectedObj.setRotation();
        loadedObjects[selectedIndex] = selectedObj;
        updateGL();
    }

    // If the user is dragging/drawing a shape
    if ((event->buttons() & Qt::LeftButton) && dragShape && drawing) {
        Object object = loadedObjects.back();

        object.dragX = (drawClick[0] - object.xBegin)/2.0;
        object.dragY = (drawClick[1] - object.yBegin)/2.0;
        object.dragZ = (drawClick[2] - object.zBegin)/2.0;
        object.scale = 0.85*sqrt(pow(drawClick[0] - object.xBegin,2)+
                pow(object.zBegin - drawClick[2],2));
        loadedObjects.back() = object;

        updateGL();
    }
}

// Overload the mouse release event - mostly calculating final mouse movements
// and telling the program that the event is finished
void PolygonWidget::mouseReleaseEvent(QMouseEvent *event)
{
    printBools();
    Point worldCoords = getMouseModelCoords(event);

    // Get the point where the mouse is from the cameras near and far field
    // used to get intersection of a plane when drawing objects later
    Point camPos = getMouseModelCoords(event, 0.0);
    Point endPos = getMouseModelCoords(event, 1.0);

    Point drawClick;
    Point panClick;
    // Get the intersection point of the ray cast from the mouse and the
    // plane the grid is drawn on. Give the argument of the mouse points
    // and three points on the correct plane
    // If 3D or top view, intersect the XZ plane
    if (cameraMode == "3D" || cameraMode == "TOP") {
        drawClick = getRayPlaneIntersection(camPos, endPos,
                                               cam.p1,
                                               cam.p2,
                                               cam.p3);
        panClick = getRayPlaneIntersection(camPos, endPos,
                                                Point(0,0,0),
                                                Point(1,0,1),
                                                Point(3,0,4));
    }

    // If the right button is released, rotation is finished
    if (event->button() & Qt::RightButton && changeCamera) {
        updateGL();
    }

    // If LMB released, translate to the end point and translation is finished
    if ((event->button() & Qt::LeftButton) && changeCamera) {
        cam.setPan(panClick);
        updateGL();
    }

    // If the user is dragging with the LMB, calculate the new panning coords
    if ((event->button() & Qt::LeftButton) && userTranslateY) {
        userTranslateY = false;
        objectSelected = true;

        Object currObject = loadedObjects[selectedIndex];
        currObject.panUserYTotal += currObject.panUserYDiff;
        currObject.panUserYDiff = 0;
        loadedObjects[selectedIndex] = currObject;
        updateGL();
    }

    if ((event->button() & Qt::LeftButton) && (userTranslateY == false)
            && (objectSelected || changeCamera)) {
        // If released a click, tell the program the user has clicked and get
        // those clicked coordinates (used in first render of objects to get
        // object selection
        selectedSomething = true;

        GLint viewport[4];
        GLdouble modelview[16];
        GLdouble projection[16];
        GLfloat winX, winY;
        // Get the current information
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetIntegerv( GL_VIEWPORT, viewport );

        // get the window coordinates
        winX = (float)event->x();
        winY = (float)viewport[3] - (float)event->y();
        selectedPixels[0] = winX;
        selectedPixels[1] = winY;

        updateGL();
    }

    // If LMB released, translate and scale the object to the end point and
    // translation is finished
    if (event->button() == Qt::LeftButton && dragShape && drawing) {
        Object object = loadedObjects.back();

        object.dragX = (drawClick[0] - object.xBegin)/2.0;
        object.dragY = (drawClick[1] - object.yBegin)/2.0;
        object.dragZ = (drawClick[2] - object.zBegin)/2.0;

        object.scale = 0.85*sqrt(pow(drawClick[0] - object.xBegin,2)+
                           pow(object.zBegin - drawClick[2],2));
        loadedObjects.back() = object;

        updateGL();
        // Stop drawing the object and allow the user to change the camera
        // as default again
        dragShape = false;
        drawing = false;
        changeCamera = true;

        // unactivate the button so it can be used to draw more spheres
        pressedButton->setEnabled(false);
        pressedButton->setEnabled(true);
    }
}

// function to generate a sphere .m file and render it to screen
void PolygonWidget::generateSphere()
{
    pressedButton = qobject_cast<QPushButton *>(sender());

    Object sphere = shapeGenerator->generateSphere();
    // set the loaded object to the window so it can render
    setLoadedObject(sphere);
}

// function to generate a sphere .m file and render it to screen
void PolygonWidget::generateCube()
{
    pressedButton = qobject_cast<QPushButton *>(sender());

    Object cube = shapeGenerator->generateCube();
    // set the loaded object to the window so it can render
    setLoadedObject(cube);
}

// function to generate a sphere .m file and render it to screen
void PolygonWidget::generateCylinder()
{
    pressedButton = qobject_cast<QPushButton *>(sender());

    Object cylinder = shapeGenerator->generateCylinder();
    // set the loaded object to the window so it can render
    setLoadedObject(cylinder);
}

void PolygonWidget::generateCone()
{
    pressedButton = qobject_cast<QPushButton *>(sender());

    Object cone = shapeGenerator->generateCone();
    // set the loaded object to the window so it can render
    setLoadedObject(cone);
}


void PolygonWidget::drawAxes() {
    glLineWidth(4.0);
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0,0,0);
    glVertex3f(1,0,0);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0,0,0);
    glVertex3f(0,0,1);
    glEnd();
    glLineWidth(1.0);
    Object cone = shapeGenerator->generateCone();
    glPushMatrix();
    glTranslatef(1.0,0.0,0.0);
    glRotatef(90.0,0.0,1.0,0.0);
    glScalef(0.05,0.05,0.1);
    Point green(0.0,1.0,0.0);
    renderObject(cone, green);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0,1.0,0.0);
    glRotatef(270.0,1.0,0.0,0.0);
    glScalef(0.05,0.05,0.1);
    Point red(1.0,0.0,0.0);
    renderObject(cone, red);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0,0.0,1.0);
    glRotatef(270.0,0.0,0.0,1.0);
    glScalef(0.05,0.05,0.1);
    Point blue(0.0,0.0,1.0);
    renderObject(cone, blue);
    glPopMatrix();
}

// Erase the last object drawn to screen
void PolygonWidget::undoAction()
{
    if (loadedObjects.size() > 0) {
        loadedObjects.erase(loadedObjects.end()-1);
        updateGL();
    }
}

void PolygonWidget::eraseAll() {
    loadedObjects.clear();
    selectedIndex = 1000;
    objectSelected = false;
    updateGL();
}

void PolygonWidget::loadScene() {
    fileHandler->load();
}

void PolygonWidget::receiveObjects(vector<Object> objects)
{
    loadedObjects.clear();
    for (int i = 0; i < objects.size(); i++) {
        loadedObjects.push_back(objects[i]);
    }
    //qDebug() << "Set new objects";
    updateGL();
}

void PolygonWidget::saveSceneAs() {
    fileHandler->saveAs(loadedObjects);
}

void PolygonWidget::printBools() {
    /*qDebug() << "panObject " << panObject << ", userTranslateY " << userTranslateY
             << ", objectSelected " << objectSelected << ", changeCamera " <<
                changeCamera << ", selectedSomething " << selectedSomething;*/
}

