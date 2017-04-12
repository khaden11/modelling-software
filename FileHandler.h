#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <QFileDialog>
#include "Object.h"

class FileHandler : public QObject
{
Q_OBJECT
public:
    FileHandler();
    //FileHandler();

    QFileDialog *fileDialog;
    QFileDialog *saveDialog;

    void load();
    void loadScene(QString fileName);

    void saveAs(std::vector<Object> objectList);
    void saveScene(QString fileName, std::vector<Object> objectList);

signals:

    void loadObjectsToScreen(std::vector<Object> objects);
};

#endif // FILEHANDLER_H
