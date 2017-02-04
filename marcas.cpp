#include <iostream> 
#include <cstdlib>
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <highgui.h>
#include "opencv2/opencv.hpp"
#include "time.h"
#include <opencv2/highgui/highgui.hpp>
#include <Tools.h>
#include <Tools2.h>

#define HD 0
#define FULL_HD 1
#define IM_WIDTH_VGA 640
#define IM_HEIGHT_VGA 480
#define IM_WIDTH_HD 1280
#define IM_HEIGHT_HD 720

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{


//****************Cámara****
  Mat frame;
  VideoCapture capture;
  char key; 
  int nP=12;//número de puntos
    //*************Cámara**********
    cvNamedWindow("frame",WINDOW_NORMAL );//WINDOW_NORMAL);//WINDOW_AUTOSIZE );
    //VideoCapture capture;
    //Mat frame;
    if (!capture.open(1))
    {
      cout<<"no pudo abrir la camara, se usara la webcam integrada"<<endl; //si no se puede acceder a la cámara USB usa la webcam de la laptop
      capture.open(0);
    }
    if(!capture.isOpened())
    {
      cout<<"No abre"<<endl;
      return -1;
    }
        cout<<"Se abrió la cámara"<<endl;
    #if FULL_HD
    capture.set(CV_CAP_PROP_FRAME_WIDTH,1920);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    #elif HD
    capture.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,720);
    #else
    capture.set(CV_CAP_PROP_FRAME_WIDTH,640);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);
    #endif

    Mat K, scnPts, imgPts, RT, dummy, testPoints;
    vector<Point2d>P;
    //P.resize(4);//12 puntos
    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));

    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));

    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));
    P.push_back(Point2d(0, 0));

    getMatPointsMouse(frame, capture, P);

    /*               imgPts=(Mat_<double>(3, 4) <<P[0].x, P[1].x, P[2].x, P[3].x,
                                              P[0].y, P[1].y, P[2].y, P[3].y,
                                               1, 1, 1, 1);*/
imgPts=(Mat_<double>(3, nP) <<P[0].x, P[1].x, P[2].x, P[3].x, P[4].x, P[5].x, P[6].x, P[7].x, P[8].x, P[9].x, P[10].x,  P[11].x,
                              P[0].y, P[1].y, P[2].y, P[3].y, P[4].y, P[5].y, P[6].y, P[7].y, P[8].y, P[9].y, P[10].y,  P[11].y,
                              1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

    ParamsK pK(616.164, 616.82, 0, 325.528, 228.66);
     K=(Mat_<double>(3, 3)<<pK.fx, pK.gamma, pK.cx, 0, pK.fy,  pK.cy, 0, 0, 1);
    dummy=(Mat_<double>(3, 4)<<1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);

    Point3f N;
    double D;
    //Definimos 4 puntos que yacen en un plano en el espacio.
    /*scnPts = (Mat_<double>(4, 4) << 0., 0, .4, .4, 
                                    0., .4, .4, 0., 
                                    0., 0., 0., 0., 
                                    1., 1., 1., 1.);*/

    //CLIR 
    /*scnPts = (Mat_<double>(4, 4) << 0., 0, .8, .8, 
                                    0., .8, .8, 0., 
                                    0., 0., 0., 0., 
                                    1., 1., 1., 1.);*/
    //CLIR2
    /*scnPts = (Mat_<double>(4,nP) << 0., 0, .0, .4, .4, .4, .8, .8, .8, 1.2, 1.2, 1.2,  
                                    0., .4, .8, 0., .4, .8, 0., .4, .8, 0., .4, .8,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.);*/

 /*   scnPts = (Mat_<double>(4,9) << 0., 0, .0, .4, .4, .4, .8, .8, .8, 
                                   0., .4, .8, 0., .4, .8, 0., .4, .8,  
                                   0., 0., 0., 0., 0., 0., 0., 0., 0., 
                                   1., 1., 1., 1., 1., 1., 1., 1., 1.);*/
    //Americas
    /*scnPts = (Mat_<double>(4, 4) << 0., 0, .665, .665, 
                                    0., .665, .665, 0., 
                                    0., 0., 0., 0., 
                                    1., 1., 1., 1.);*/

    //Americas 2
    scnPts = (Mat_<double>(4,nP) << 0., 0, .0, .33, .33, .33, .66, .66, .66, .99, .99, .99,
                                   0., .33, .66, 0., .33, .66, 0., .33, .66,  0., .33, .66,
                                   0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 
                                   //0., .33, .66, 0., .33, .66, 0., .33, .66,  0., .33, .66,
                                   1., 1., 1., 1., 1., 1., 1., 1., 1. , 1., 1., 1.);


    //Se pasan a metros
    imgPts=K.inv()*imgPts;
    //Normalizamos las coordenadas obtenidas.
    for (int i=0;i<imgPts.cols;++i)
        imgPts(Rect(i, 0, 1, 3)) /= imgPts.at<double>(2, i);

    //Invocamos funciones que calcula la ecuación del plano en el marco
    //de referencia de la cámara.
    Mat R, T;
    getPlaneOrientation2(scnPts, imgPts, N, D, R, T);
    //Construimos la matriz G
    Size_<int> sizeG(4,4);
    Mat G(sizeG, R.type());
    buildG(G, R, T);
    cout<<endl<<"R: "<<endl<<R<<endl<<"T: "<<endl<<T<<endl<<"G= "<<G<<endl;
    //Creación de vector de vector de matrices
    vector<Mat> vMat;
    int nMat=24;
    for(int i=0; i<nMat; i++)
      vMat.push_back((Mat_<double>(4,1)<<0,0,0, 0));//I=icialización
    //return 0;
    //Se obtiene la linea de fuga
    Point3f vanishingLine;
    getVanishingLine (K, N, vanishingLine);//La linea de fuga es devuelta en m
    Scalar_ <uchar> colors[4]; 
    colors[0] = Scalar_<uchar> (0,0,255);
    colors[1] = Scalar_<uchar> (255,0,0);
    colors[2] = Scalar_<uchar> (0,255,0);
    colors[3] = Scalar_<uchar> (255,255,255);

    cout << "N = " << N << endl;
    cout << "D = " << D << endl;

   //PosP posP(0.0, 0.3, .0, .3);//En x y z, desde donde y el tamaño de los saltos en pix
   //PosP posP(0.0, 0.332, 0, .332);
   /*PosP posP(-.27, 0.4, .55, .4);
    int cols=4;//4;
    int rows=18;//18;*/
   PosP posP(.0, 0.33, 0.0 , .33);
    int cols=4;//4;
    int rows=3;//18;
   
  float y=posP.startZ;
  float z=0;
  float xTemp, yTemp, zTemp;
  Mat marcas3d, colTemp, marcas2d, center3d, center2d; 
  xTemp=0;
  zTemp=1;
//  yTemp=(D-N.x*xTemp-N.z*zTemp)/N.y;

  center3d =(Mat_<double>(4,1)<<xTemp,yTemp,zTemp,1);

  for(int i=0; i<rows; i++, y+=posP.skipZ)
  {
    float x=posP.startX;
    for(int j=0; j<cols; j++, x+=posP.skipX )
    {
      //y=(D-N.x*x-N.z*z)/N.y;
      //z=(D-N.x*x-N.y*y)/N.z;
    z=0;
      if(i==0 and j==0)
      {
        marcas3d=(Mat_<double>(4, 1)<<x, y, z, 1);
      }
      else
      {
        colTemp=(Mat_<double>(4, 1)<<x, y, z, 1);
        hconcat( marcas3d, colTemp,marcas3d);
      }
    }
  }
  //cout<<"Matriz marcas3d: "<<endl<<marcas3d<<endl;
  marcas3d=G*scnPts;
  marcas2d=K*dummy*marcas3d;
  center2d=K*dummy*center3d;
//Deshomoge...
    center2d(Rect(0, 0, 1, 3)) /= center2d.at<double>(2, 0);
    Point centerP(center2d.at<double>(0,0), center2d.at<double>(1,0));
    cout<<"Punto central en la imagen: "<<centerP<<endl;
    cout<<"Punto central en 3d: "<<center3d<<endl;
    for (int i=0;i<marcas2d.cols;++i)
        marcas2d(Rect(i, 0, 1, 3)) /= marcas2d.at<double>(2, i);
  //cout<<"Marcas 2d rows: "<<marcas2d.rows<<endl<<"cols: "<<marcas2d.cols<<endl;

    Point3f cornerTL, cornerTR, cornerBL, cornerBR;
    vector<Point3f*>vCorners;
    vCorners.push_back(&cornerTL);
    vCorners.push_back(&cornerBL);
    vCorners.push_back(&cornerBR);
    vCorners.push_back(&cornerTR);

    vector<Point2f>vCorners2D;
    vCorners2D.push_back(Point2f(0,0));
    vCorners2D.push_back(Point2f(0,0));
    vCorners2D.push_back(Point2f(0,0));
    vCorners2D.push_back(Point2f(0,0));

    getIntersec(frame, vCorners, vanishingLine, K, N, D);//Devuelve las intersecciones al plano
    //getIntersec2(frame, vCorners, vanishingLine, K, N, D, P);
    points3dTo2d(vCorners, K, dummy, vCorners2D);//Convierte el vector de las intersecciones de 3d a 2d para visualización
    vector<Point2f>vps2d;
    for(int i=0; i<nP; i++)
    {
      vps2d.push_back(Point2f(0,0));
    }
    pointsSelect3dTo2D(P, vps2d, K, N, D, dummy);//Con los puntos seleccionados con el mouse se halla la intersección en el plano y se proyectan en 2D

        //Point tempPoint(P2DCorner.at<double>(0, 0), P2DCorner.at<double>(1, 0));
        //circle(frame, vCorners2D[0],5, Scalar(200, 0, 0) ,10);
        //cout<<"Punto en la imagen: "<<vCorners2D[0]<<endl;
        //cout<<"Matriz 3d en la imagen"<<endl<<marcas2d<<endl;
//Pinta en el frame
cout<<endl<<endl<<"Marcas verdes"<<endl;
    for(int i=0; i<8; i++)
    {
        Point3f tempPoint(marcas3d.at<double>(0, i), marcas3d.at<double>(1, i), marcas3d.at<double>(2, i));
        //Point2f tempPoint(marcas2d.at<double>(0, i), marcas2d.at<double>(1, i));
        cout<<tempPoint<<", "<<endl;
    }
    cout<<endl<<endl;
    cout<<"Número de marcas verdes: "<<marcas2d.cols<<endl;
    for(;;)
    {
      capture>>frame;
      for(int j=0; j<marcas2d.cols; j++)
      {
        drawLine(frame, vanishingLine, colors[0]);
        for(int i=0; i<vCorners2D.size(); i++)
          circle(frame, vCorners2D[i],5, Scalar(200, 0, 0) ,10);
        for(int i=0; i<vps2d.size(); i++)
          circle(frame, vps2d[i],5, Scalar(0, 0, 200) ,5);

        Point tempPoint(marcas2d.at<double>(0, j), marcas2d.at<double>(1, j));
        circle(frame, tempPoint,5, Scalar(0, 200, 0) ,10);
        circle(frame, centerP,5, Scalar(200, 200, 200) ,10);//Punto central de la cámara
      }
    
      imshow("frame", frame);
      key=waitKey(1);
    }

    /*
    for(int i=0; i<marcas2d.rows(); i++)
    {
      for(int j=0; j<marcas2d.cols(); j++)
      {
        
      }
    }*/

  return 0;
}
