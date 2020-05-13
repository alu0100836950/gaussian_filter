#include </usr/include/opencv4/opencv2/highgui/highgui.hpp>
#include <iostream>
#include "mpi.h"
#include <vector>

using namespace std;
using namespace cv;

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

void salypimienta(Mat *im, int n) {
    int i, j;
    for (int k = 0; k < n; k++){
        i = rand() % im->cols;
        j = rand() % im->rows;
        int a = rand() % 2;
        //cout << a << endl;
        if (a==1){
            im->at<uchar>(j, i) = 255;
        }
        else{
            im->at<uchar>(j, i) = 0;
        }
    }
     
}

int main(int argc, char *argv[]){
    
    int elementos_por_proc;
    int tag, rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    tag = 100;

    Mat *imagen = new Mat();
    Mat *result;

    if(rank == 0){
        
        imagen = new Mat(imread(argv[1], IMREAD_GRAYSCALE));
        cout << "La imagen mide " << imagen->rows << " x " << imagen->cols << " pixeles" << endl;

        result = new Mat(imagen->rows, imagen->cols, CV_8UC1);

        elementos_por_proc = imagen->total() / size;

        for(int i = 1; i < size; i++){

            MPI_Send(&elementos_por_proc, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        }
    }else{

        MPI_Recv(&elementos_por_proc, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    };
    
    
    vector<uchar> *sub_imagen = new vector<uchar>(elementos_por_proc);
    vector<uchar> *sub_result = new vector<uchar>(elementos_por_proc);

    // Processing
    double startTime = MPI_Wtime();

    if(rank == 0){
        MPI_Scatter(imagen->data, elementos_por_proc, MPI_BYTE, sub_imagen->data(), elementos_por_proc, MPI_BYTE, 0, MPI_COMM_WORLD);
    }else{
        MPI_Scatter(NULL, 0, NULL, sub_imagen->data(), elementos_por_proc, MPI_BYTE, 0, MPI_COMM_WORLD);
    };

    Mat *aux_imagen = new Mat(*sub_imagen);
    Mat *aux_result = new Mat(*sub_result);

    salypimienta(aux_imagen, 30000);
    MPI_Barrier(MPI_COMM_WORLD);


    if (rank == 0){
        
        MPI_Gather(aux_imagen->data, elementos_por_proc, MPI_BYTE, result->data, elementos_por_proc, MPI_BYTE, 0, MPI_COMM_WORLD);
    }else{
        MPI_Gather(aux_imagen->data, elementos_por_proc, MPI_BYTE, NULL, 0, NULL, 0, MPI_COMM_WORLD);
    }

    double totalTime = MPI_Wtime() - startTime;
    
    if(rank == 0){
    
        //show image
        imshow("Imagen transformada", *result);
        imwrite(argv[2], *result);
        
        cout << "Time: " << totalTime << "segundos" << endl;
        
        //imprimir(imagen);
        waitKey(0);
          
    }

    MPI_Finalize();
    return 0;  
}

