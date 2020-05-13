#include </usr/include/opencv4/opencv2/highgui/highgui.hpp>
#include <iostream>
#include "mpi.h"

using namespace std;
using namespace cv;

void imprimir(Mat m);
void salypimienta(Mat im, int n);

int main(int argc, char *argv[]){
    

    Mat imagen = imread(argv[1], IMREAD_GRAYSCALE);
    cout << "La imagen mide " << imagen.rows << " x " << imagen.cols << " pixeles" << endl;
    
    // Processing
    double startTime = MPI_Wtime();
    salypimienta(imagen,50000);
    double totalTime = MPI_Wtime() - startTime;
    
    //show image
    imshow("Imagen transformada", imagen);
    imwrite(argv[2], imagen);
    
    cout << "Time: " << totalTime << "segundos" << endl;
    //imprimir(imagen);
    waitKey(0);
    return 1;
}
void imprimir(Mat m) {
    int colores[256];
    for (int i = 0; i < 256; i++){
        colores[i] = 0;
    }
    for (int j = 0; j < m.rows; j++){
        for (int i = 0; i < m.cols; i++){
            int pixel = m.at<uchar>(j, i);
            /*cout << (int)m.at<uchar>(j, i) << "\t";*/
            colores[pixel]++;
        }

    }
    //Impresion de cantidad de colores
    cout << "color=Cantidad" << endl;
    for (int i = 0; i < 256; i++){
        if (colores[i]>0){
            cout << i << " = " << colores[i] << endl;
         }
    }
}
void salypimienta(Mat im, int n) {
    int i, j;
    for (int k = 0; k < n; k++){
        i = rand() % im.cols;
        j = rand() % im.rows;
        int a = rand() % 2;
        //cout << a << endl;
        if (a==1){
            im.at<uchar>(j, i) = 255;
        }
        else{
            im.at<uchar>(j, i) = 0;
        }
    }
}