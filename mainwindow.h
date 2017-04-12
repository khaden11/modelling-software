#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QColorDialog>
#include <QLabel>
#include <QVBoxLayout>

#include "Object.h"
#include "PolygonWidget.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QPushButton *sphereButton;
    PolygonWidget *modelCoords;

    bool infoSet;
    QLabel *objectLabel;

    QColorDialog *colorDialog;
    QLabel *colourLabel;
    QPushButton *chooseColourBtn;

    Object currentObject;

signals:

    void updateObject(Object &object);

public slots:

    void updateObjectInfo(Object &object);
    void updateColour(QColor colour);

    void clearObjectInfo();


private:

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
