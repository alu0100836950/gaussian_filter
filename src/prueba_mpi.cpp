#include <iostream>
#include <opencv2/opencv.hpp>
#include "mpi.h"
#include <math.h>



using namespace std;
using namespace cv;



void applyFilter(Mat *imagen, Mat *result){

    
    Mat kernel;
    kernel.create(5, 5,cv::DataType<double>::type);
    double sigma= 5.0;
    double sum=0.0;

    int filterHeight = kernel.rows;
    int filterWidth = kernel.cols;
    

    for (int i=0 ; i<filterHeight ; i++) {
        for (int j=0 ; j<filterWidth ; j++) {
            kernel.at<double>(i,j) = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);    
            sum += kernel.at<double>(i,j);
        }
    }

    for (int i=0 ; i<filterHeight ; i++) {
        for (int j=0 ; j<filterWidth ; j++) {
            kernel.at<double>(i,j) /= sum;
        }
    }    

    int height = imagen->rows;
    int width = imagen->cols;
    int i,j,h,w;

    cout << "height "<< height << endl;
    cout <<"width  "<< width << endl;


    cout << "filterHeight "<< filterHeight << endl;
    cout <<" filterWidth "<< filterWidth << endl;


    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            for (h=i ; h<i+filterHeight ; h++) {
                for (w=j ; w<j+filterWidth ; w++) {
                    //cout <<  "posicion del kernel-> " << kernel.at<double>(h-i, j) << endl;
                    //cout <<  "posicion de la matriz-> " << imagen->at<uchar>(h,w) << endl;

                    result->at<uchar>(i,j) += kernel.at<double>(h-i, w-j) * imagen->at<uchar>(h,w);
                }
            }
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

        imagen = new Mat(imread(argv[1], cv::IMREAD_GRAYSCALE)) ;
        cout << "La imagen mide " << imagen->rows << " x " << imagen->cols << " pixeles" << endl;
    

        result = new Mat(imagen->rows, imagen->cols, CV_8UC1);
        elementos_por_proc = imagen->total() / size;
        
        
        
        for(int i = 1; i < size; i++){

            MPI_Send(&elementos_por_proc, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            
        }
    }else{

        MPI_Recv(&elementos_por_proc, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);    
        
    }
 

    vector<uchar> *sub_imagen_vector = new vector<uchar>(elementos_por_proc);
    vector<uchar> *sub_result = new vector<uchar>(elementos_por_proc);
  
    
    double startTime = MPI_Wtime();


    if(rank == 0){
        MPI_Scatter(imagen->data, elementos_por_proc, MPI_BYTE, sub_imagen_vector->data(), elementos_por_proc, MPI_BYTE, 0, MPI_COMM_WORLD);
        
    }else{
        MPI_Scatter(NULL, 0, NULL, sub_imagen_vector->data(), elementos_por_proc, MPI_BYTE, 0, MPI_COMM_WORLD);
    }
 
    Mat *aux_imagen = new Mat(*sub_imagen_vector);
    Mat *aux_result = new Mat(*sub_result);

   
    applyFilter(aux_imagen, aux_result);


    MPI_Barrier(MPI_COMM_WORLD);
        
    if (rank == 0){
        
        MPI_Gather(aux_result->data, elementos_por_proc, MPI_BYTE, result->data, elementos_por_proc, MPI_BYTE, 0, MPI_COMM_WORLD);
    }else{
        MPI_Gather(aux_result->data, elementos_por_proc, MPI_BYTE, NULL, 0, NULL, 0, MPI_COMM_WORLD);
    }


    double totalTime = MPI_Wtime() - startTime;

    if(rank == 0){

        imwrite(argv[2], *result);
        cout << "Time: " << totalTime << "segundos" << endl;  

    }
    
    MPI_Finalize();
    return 0;
}