#include <QGLWidget>
#include "Matrix.h"
#include "Object.h"
#include "Camera.h"
#include "FileHandler.h"

#include "ShapeGenerator.h"

#include <vector>
#include <string>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>



#ifndef _GL_POLYGON_WIDGET_H_
#define _GL_POLYGON_WIDGET_H_


class PolygonWidget: public QGLWidget
{
    Q_OBJECT
    public slots:
    
    // methods for generating sphere and torus .m files
    void generateSphere();
    void generateCube();
    void generateCylinder();
    void generateCone();

    // Method to undo the last drawing action taken
    void undoAction();

    // Method to change perspective of camera
    void changeCameraMode(int selectedMode);

    // View camera grid in relation to drawn objects
    void gridViewMode(bool selected);

    void drawPlaneMode(bool selected);

    void eraseAll();

    void loadScene();
    void receiveObjects(std::vector<Object> objects);

    void saveSceneAs();

    signals:

    void updateObjectSelectedInfo(Object &object);
    void clearObjectSelectedInfo();


    public:
        
    bool drawPlaneBool;
    bool gridviewBool;

    bool panObject;
    bool userTranslateY;
    Point selectedPanPixels;

    void printBools();


    PolygonWidget(QWidget *parent);

    // Update the ui with object selected information
    void updateObjectInfo();

    // set the current loaded object to draw
    void setLoadedObject(Object &object);
    void drawObject();

    // function to draw a sphere
    void renderObject(Object &object, Point &colour);

    void drawAxes();

    FileHandler *fileHandler;


    // OBJECT SELECTION MANAGEMENT
    bool selectedSomething;
    Point selectedPixels;
    // bool to show if object is selected
    bool objectSelected;
    int selectedIndex;


    // VARIABLES TO STORE CAMERA INFORMATION

    // Variables to store chosen camera settings
    std::string cameraMode;
    bool changeCamera;

    Camera cam;
    // Variables to store the camera position variables
    Point cameraPan, cameraPanBegin;
    Vector cameraRotateLast, cameraRotateCurr, cameraRotateAxis;
    float cameraRotateAngle;
    bool rotateCamera, zoomCamera, translateCamera;
    int rotateDirection;

    // FUNCTIONS FOR USE IN MOUSE CLICKS
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    // function to get the mouse coords related to the viewpoint
    Point getMouseModelCoords(QMouseEvent *event);
    // function to get mouse position using a depth - used to cast rays from
    // camera
    Point getMouseModelCoords(QMouseEvent *event, float depth);

    // Get point of intersection using a ray and a plane giving the point
    // from the camera, the point at the far view of the camera, and three
    // points on the target plane
    Point getRayPlaneIntersection(Point camPos, Point endPos,
                                  Point pointOnPlane, Point point1,
                                  Point point2);


    // Button to control drawing sphere shapes on screen
    QPushButton *pressedButton;    

    protected:
    
    // define the standard opengl functions used in a QGLWidget
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();


    
    private:

    // variable to store object information
    ShapeGenerator *shapeGenerator;

    bool drawing;
    Object currObject;
    std::vector<Object> loadedObjects;
    bool dragShape;



    
    // matrix variable stores the current specified transformation matrix
    Matrix transformMatrix;
 
    // variables to store vertex/face/colour information from an object
    std::vector<Point> objectVertices;
    std::vector<unsigned int> objectTriangles;

};

#endif
