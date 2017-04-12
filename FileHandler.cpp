#include "FileHandler.h"
#include <QString>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <QDebug>

using namespace std;

FileHandler::FileHandler() {}

void FileHandler::load()
{
    fileDialog = new QFileDialog;

    QString fileName = fileDialog->getOpenFileName();
    if(!fileName.isEmpty())
        loadScene(fileName);
}

void FileHandler::loadScene(QString fileName)
{
    // get the filepath from the qstring filename
    string filePath = fileName.toUtf8().constData();
    // open the file with an ifstream
    ifstream myFile (filePath.c_str());
    string line;

    vector<Object> objectList;

    int objNumber;
    int objIndex = 0;

    vector<string> vertexList, faceList;
    string scale, type, colour, origin, translate;

    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            if (line.find("ObjectNumber")!=string::npos) {
                istringstream linestream(line);
                string word;
                int i = 0;
                while (linestream >> word) {
                    if (i == 1) {
                        objNumber = atoi(word.c_str());
                    }
                    i++;
                }
            }
            stringstream ss;
            ss << objIndex;
            string str = ss.str();

            string vStr = "Vertex"+str;
            string fStr = "Face"+str;
            string sStr = "Scale"+str;
            string cStr = "Colour"+str;
            string typeStr = "Type"+str;
            string oStr = "Origin"+str;
            string tStr = "Translate"+str;


            if (line.find(vStr)!=string::npos) {
                vertexList.push_back(line);
            }
            if (line.find(fStr)!=string::npos) {
                faceList.push_back(line);
            }
            if (line.find(sStr)!=string::npos) {
                scale = line;
            }
            if (line.find(typeStr)!=string::npos) {
                type = line;
            }
            if (line.find(cStr)!=string::npos) {
                colour = line;
            }
            if (line.find(oStr)!=string::npos) {
                origin = line;
            }
            if (line.find(tStr)!=string::npos) {
                translate = line;

                Object obj = Object();
                obj.setVertices(vertexList);
                obj.setFaces(faceList);
                obj.setVariables(scale, type, colour, origin, translate);
                obj.faceNumber = faceList.size();
                obj.vertexNumber = vertexList.size();

                objectList.push_back(obj);

                vertexList.clear();
                faceList.clear();
                scale = colour = origin = translate = type = "";

                objIndex++;
            }
        }
        myFile.close();
    }

    loadObjectsToScreen(objectList);
}

/*void FileHandler::loadObjectsToScreen(std::vector<Object> objects)
{

}*/

void FileHandler::saveAs(std::vector<Object> objectList)
{
    saveDialog = new QFileDialog;

    QString fileName = saveDialog->getSaveFileName();
    if(!fileName.isEmpty())
        saveScene(fileName, objectList);
}

void FileHandler::saveScene(QString fileName, std::vector<Object> objectList)
{
    // get the filepath from the qstring filename
    string filePath = fileName.toUtf8().constData();
    // open the file with an ifstream
    ofstream writeFile;
    writeFile.open(filePath.c_str());

    writeFile << "ObjectNumber  " << objectList.size() << "\n";
    for (int index = 0; index < objectList.size(); index++) {
        Object currObj = objectList[index];

        // write the vertex information in the right format
        for (int i = 0; i < currObj.vertices.size(); i++) {
            Point vertex = currObj.vertices[i];
            writeFile << "Vertex" << index << " " << i << " " <<
                         vertex[0] << " " << vertex[1]
                  << " " << vertex[2] << "\n";
        }
        // write the face information in the right format
        for (int i = 0; i < currObj.triangles.size(); i = i + 3) {
            writeFile << "Face" << index << " " << i/3 << "  " <<
                         currObj.triangles[i] << "  "
                  << currObj.triangles[i+1] << "  " <<
                     currObj.triangles[i+2] << "\n";
        }
        writeFile << "Scale" << index << "  " <<
                     currObj.scale << "\n";
        writeFile << "Type" << index << "  " <<
                     currObj.objectType << "\n";
        writeFile << "Colour" << index << "  " <<
                     currObj.objectColour[0] << "  " <<
                  currObj.objectColour[1] << "  " <<
                     currObj.objectColour[2] << "\n";
        writeFile << "Origin" << index << "  " <<
                     currObj.xBegin << "  " <<
                     currObj.yBegin << "  " <<
                     currObj.zBegin << "\n";
        writeFile << "Translate" << index << "  " <<
                     currObj.dragX << "  " <<
                     currObj.dragY << "  " <<
                     currObj.dragZ << "\n";

    }
    writeFile.close();
}

