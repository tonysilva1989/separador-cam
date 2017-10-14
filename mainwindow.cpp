#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <QLabel>
#include <QTextEdit>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QtDebug>
#include <QCoreApplication>

/*
 * O extrator precisa selecionar a pasta na qual
 * estao disponiveis todas as pastas de captura ja existentes
*/

using namespace cv;
using namespace std;


void listFile(string strCam, string pathPosOut, string pathNegOut);
int redirectImg(string path, string endIP, string pathNegOut);


QString pathName;
QString output;



//Handle do botão de selecionar path contendo as pastas das cameras
void MainWindow::handleButton()
{
    pathName =  QFileDialog::getExistingDirectory(0, ("Select Cameras path"), QDir::currentPath());

    if(!pathName.isNull())
    {
      qDebug() << "selected cameras path : " << pathName.toUtf8();
    }
    mostrarPath(pathName);

}


//Handle do botao de selecionar path de saida dos recortes separados
void MainWindow::handleOutput()
{
    output =  QFileDialog::getExistingDirectory(0, ("Select Output Folder"), QDir::currentPath());

    if( !output.isNull() )
    {
      qDebug() << "selected output path : " << output.toUtf8();
    }
    mostrarPathOut(output);
}


void MainWindow::mostrarPath(QString variavel){

    ui->lineEdit->setText(variavel);
    ui->lineEdit->show();
}

void MainWindow::mostrarPathOut(QString variavel){

    ui->lineOut->setText(variavel);
    ui->lineOut->show();
}



void MainWindow::handleButtonGenerate()
{
    string strCam = pathName.toStdString();
    string pathPosOut = output.toStdString();
    string pathNegOut = output.toStdString();

    std::replace( strCam.begin(), strCam.end(), '/', '\\'); // replace all 'x' to 'y'
    std::replace( pathPosOut.begin(), pathPosOut.end(), '/', '\\'); // replace all 'x' to 'y'
    std::replace( pathNegOut.begin(), pathNegOut.end(), '/', '\\'); // replace all 'x' to 'y'

    strCam = strCam + '\\';
    pathPosOut = pathPosOut + "\\positivos\\";
    pathNegOut = pathNegOut + "\\negativos\\";

    listFile(strCam,pathPosOut,pathNegOut);

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
    }

MainWindow::~MainWindow()
{
    delete ui;
}



void listFile(string strCam, string pathPosOut, string pathNegOut){

        DIR *pCAMS;
        const char * campathPos;
        struct dirent *cams;
        string pathNeg,pathPos;

        campathPos = strCam.c_str();

        if (pCAMS = opendir(campathPos)){ //opening all cameras folder
             while(cams = readdir(pCAMS)){ //selecting one camera folder for iteration

                    if( strcmp(cams->d_name, ".") != 0 && strcmp(cams->d_name, "..") != 0 ){

                            string endIP = cams->d_name;

                            pathPos = strCam + endIP +  "\\InfoCamera\\Day\\Cars";
                            pathNeg = strCam + endIP + "\\InfoCamera\\Day\\Others";
                            const char * var = strCam.c_str();
                            const char * var2 = endIP.c_str();

                            redirectImg(pathNeg,endIP,pathNegOut);
                            redirectImg(pathPos,endIP,pathPosOut);

                            //Definindo as pastas para imagens a noite:
                            pathPos = strCam + endIP +  "\\InfoCamera\\Night\\Cars";
                            pathNeg = strCam + endIP + "\\InfoCamera\\Night\\Others";

                            redirectImg(pathNeg,endIP,pathNegOut);
                            redirectImg(pathPos,endIP,pathPosOut);

                        }
                    }
        }else{
            qDebug() << "Cam Folder not found";
        }
        closedir(pCAMS);
}

//Function to save images in a new destiny
int redirectImg(string path, string endIP, string pathNegOut){

    int size;
    DIR *pDIR;
    Mat image;
    const char * imgPath;
    struct dirent *nameImg;
    string newName,currName;

    imgPath = path.c_str();

    //if output folder doesn't exist
    if(!(pDIR=opendir(pathNegOut.c_str()))){
        mkdir(pathNegOut.c_str());
    }

            if( pDIR=opendir(imgPath)){ //Opening camera path
                while(nameImg = readdir(pDIR)){
                        if( strcmp(nameImg->d_name, ".") != 0 && strcmp(nameImg->d_name, "..") != 0 )
                        {
                            newName = nameImg->d_name;
                            currName = path + "\\" + nameImg->d_name;
                            size = newName.length();
                            image = imread(currName);
                             qDebug() << "Carregando imagem";
                             qDebug() <<  newName.c_str() << endl;

                            if(image.empty())
                            {
                                qDebug() <<"Imagem nao encontrada";
                                return 1;
                            }
                            if (image.empty()){
                                qDebug() << "Nao Carregou Imagem";
                                return 1;
                            }

                            newName.erase(size-4,4);
                            newName = pathNegOut + newName + "_"+ endIP + ".jpg";

                             try {
                                 imwrite(newName, image);
                                }
                             catch (runtime_error& ex) {
                                    fprintf(stderr, "Exception when saving image %s\n", ex.what());
                                    return 1;
                             }
                        }                    
                }
            }else{
                qDebug() <<  "Output Folder not found" << endl;
            }closedir(pDIR);
}
