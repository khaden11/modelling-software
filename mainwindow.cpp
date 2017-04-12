#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
#include <iostream>
#include <sstream>
#include <QLabel>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up appropriate connections with signals and slots in the ui
    connect(ui->viewComboBox, SIGNAL(currentIndexChanged(int)), ui->modelCoords,
            SLOT(changeCameraMode(int)));

    connect(ui->wireframe, SIGNAL(toggled(bool)), ui->modelCoords,
            SLOT(gridViewMode(bool)));
    connect(ui->drawPlane, SIGNAL(toggled(bool)), ui->modelCoords,
            SLOT(drawPlaneMode(bool)));

    this->showMaximized();
    connect(ui->sphereButton, SIGNAL(clicked()), ui->modelCoords,
            SLOT(generateSphere()));
    connect(ui->cubeButton, SIGNAL(clicked()), ui->modelCoords,
            SLOT(generateCube()));
    connect(ui->cylinderButton, SIGNAL(clicked()),
            ui->modelCoords, SLOT(generateCylinder()));
    connect(ui->coneButton, SIGNAL(clicked()),
            ui->modelCoords, SLOT(generateCone()));

    connect(ui->actionUndo, SIGNAL(triggered()), ui->modelCoords,
            SLOT(undoAction()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->modelCoords, SIGNAL(updateObjectSelectedInfo(Object&)),
            this, SLOT(updateObjectInfo(Object&)));

    connect(ui->eraseAll, SIGNAL(clicked()), ui->modelCoords, SLOT(eraseAll()));
    connect(ui->actionLoad, SIGNAL(triggered()), ui->modelCoords, SLOT(loadScene()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), ui->modelCoords, SLOT(saveSceneAs()));
    connect(ui->modelCoords, SIGNAL(clearObjectSelectedInfo()), this, SLOT(clearObjectInfo()));

    connect(this, SIGNAL(updateObject(Object&)), ui->modelCoords,
            SLOT(updateSelectedObject(Object&)));

    infoSet = false;

    colorDialog = new QColorDialog();
    colorDialog->setWindowFlags(Qt::WindowStaysOnTopHint|
                                Qt::BypassWindowManagerHint);

}

MainWindow::~MainWindow()
{

    delete ui;

}

// slot to receive a selected object by the user and display object info
void MainWindow::updateObjectInfo(Object &object)
{
    currentObject = object;

    ui->objectLabel->setText(object.objectTitle.c_str());
    char scaleText[20];
    sprintf(scaleText, "%.2f", object.scale);
    ui->xScale->setText(scaleText);
    ui->yScale->setText(scaleText);
    ui->zScale->setText(scaleText);
    char xMoveText[10];
    sprintf(xMoveText, "%.2f", object.xBegin+object.dragX);
    ui->xMove->setText(xMoveText);
    char yMoveText[10];
    sprintf(yMoveText, "%.2f", object.yBegin+object.dragY);
    ui->yMove->setText(yMoveText);
    char zMoveText[10];
    sprintf(zMoveText, "%.2f", object.zBegin+object.dragZ);
    ui->zMove->setText(zMoveText);
    char rColourText[10];
    sprintf(rColourText, "%.2f", object.objectColour[0]*255.0);
    ui->rColour->setText(rColourText);
    char gColourText[10];
    sprintf(gColourText, "%.2f", object.objectColour[1]*255.0);
    ui->gColour->setText(gColourText);
    char bColourText[10];
    sprintf(bColourText, "%.2f", object.objectColour[2]*255.0);
    ui->bColour->setText(bColourText);


    // open a colour dialog to choose object colour
    connect(ui->chooseColourBtn, SIGNAL(clicked()), colorDialog, SLOT(show()));
    connect(colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(updateColour(QColor)));
}

// update object colour and send back to polygon widget
void MainWindow::updateColour(QColor colour)
{
    currentObject.objectColour = Point(colour.red(), colour.green(), colour.blue());
    updateObject(currentObject);
}

void MainWindow::clearObjectInfo() {
    ui->objectLabel->setText("");
    ui->xScale->setText("");
    ui->yScale->setText("");
    ui->zScale->setText("");
    ui->xMove->setText("");
    ui->yMove->setText("");
    ui->zMove->setText("");
    ui->rColour->setText("");
    ui->gColour->setText("");
    ui->bColour->setText("");
}

