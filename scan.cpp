#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;
//Se usa OpenCV para diferentes funciones de procesamiento de señales y actúa como GUI

enum escalaGrisesMethods{
	Gray_AVG=1,
	GRAY_WEIGHT=2
};

enum ThresholdMethods {
	THRESH_MOD = 1
};

Mat src,gray, gradiente,sobel,thresh,trillar,cerrado;

Mat ReadImage(string imgName) {
	Mat img = imread(imgName, IMREAD_COLOR);
	if (img.empty()) {
		cout << "No se puede leer la imagen: " << imgName << std::endl;
		system("EXIT");
	}
	return img;
}
// Se lee la imagen mediante imgName y sino no se puede leer
//------------------------------------------------------

Mat escalaGrises(Mat src, int method){
	Mat output(scr.rows,src.cols,CV_8UC1);
	if(method==GRAY_AVG)
	{
		int i,j;
		for(i=0;i<src.rows;i++)
		{
			for(j=0;j<src.cols;j++)
			{
				int Blue = src.at<Vec3b>(i, j)[0]; 
				int Green = src.at<Vec3b>(i, j)[1]; 
				int Red = src.at<Vec3b>(i, j)[2]; 
				int escalaGrises = (Blue + Green + Red) / 3;
				output.at<uchar>(i, j) = escalaGrises;
			}
		}
	}
	else if (method==GRAY_WEIGHT){
		for(int i=0;i<scr.rows;i++)
		{
			for(int j=0;j<src.cols;j++)
			{
				int Blue = src.at<Vec3b>(i, j)[0]; 
				int Green = src.at<Vec3b>(i, j)[1];
				int Red = src.at<Vec3b>(i, j)[2]; 
				int escalaGrises = (0.3 * Red) + (0.59 * Green) + (0.11 * Blue);
				output.at<uchar>(i, j) = escalaGrises;
			}
		}
	}
	return output;
}



Mat thresholding(Mat sobel, int method) {
	Mat output = Mat(sobel.rows, sobel.cols, CV_8U);
	if (method == THRESH_MOD) {
		if (sobel.channels() != 1) {
			cout << "The input image must be single-channeled!" << endl;
			system("EXIT");
		}
		
	double minVal = 0, maxVal = 0;
		for (int i = 0; i < sobel.rows; i++) {
			for (int j = 0; j < sobel.cols; j++) {
				//Mat kernel = Mat::zeros(9, 9, output.type());
				// Implementar la lógica para llenar el núcleo de 9x9 con
				// valores de la Mat gris, respetando los límites.

				//Scalar avg_intensity = mean(kernel);
				//minMaxLoc(kernel, &minVal, &maxVal);

				if (sobel.at<uchar>(i, j) <= 255 && sobel.at<uchar>(i, j) > threshold_value) {
					output.at<uchar>(i, j) = 255;
				}
				else {
					output.at<uchar>(i, j) = 0;
				}
			}
		}
		
	}
	return output;
}

void thresholding_call(int, void*) {
	threshold_value = (int)threshold_value;
	thresh = Mat(blurred.rows, blurred.cols, CV_8U);
	thresh = thresholding(blurred, THRESH_MOD);
	imshow("Thresholded Image", thresh);
}

// Función de detección de bordes Sobel
Mat SobelDetect(Mat gray) {
	int dx[3][3] = { {1, 0, -1},{2, 0, -2},{1, 0, -1 } };
	int dy[3][3] = { {1, 2, 1},{0, 0, 0},{-1, -2, -1} };

	Mat output = Mat(gray.rows, gray.cols, CV_8U);
	Mat kernel = Mat(3, 3, CV_8U);

	int max = -200, min = 2000;

	for (int i = 1; i < gray.rows - 2; i++) {
		for (int j = 1; j < gray.cols - 2; j++) {
			// aplicar el núcleo en las direcciones X e Y
			int sumX = 0;
			int sumY = 0;
			uchar ker;
			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					// obtener el valor del píxel (i,j)
					kernel.at<uchar>(m + 1, n + 1) = gray.at<uchar>(i + m, j + n);
					sumX += kernel.at<uchar>(m + 1, n + 1) * dx[m + 1][n + 1];
					sumY += kernel.at<uchar>(m + 1, n + 1) * dy[m + 1][n + 1];
				}
			}
			int sum = abs(sumX) + abs(sumY);
			//cout << sum << endl;
			output.at<uchar>(i, j) = (sum > 255) ? 255 : sum;
			//output2.at<uchar>(i, j) = kernel.at<uchar>(i, j);
		}
	}
	return output;
}

	// Detección de bordes Sobel
	sobel = Mat(src.rows, src.cols, CV_32F);
	sobel = SobelDetect(gray);
	namedWindow("Sobel Edge Detection", WINDOW_NORMAL);
	imshow("Sobel Edge Detection", sobel);
	waitKey(0);
	destroyWindow("Sobel Edge Detection");


// Función para difuminar la imagen
Mat Difuminar(Mat gradiente) {
	Mat output = Mat(gradiente.rows, gradiente.cols, CV_8U);
	int total = 0;
	// Obtener el promedio de los valores de los píxeles de la imagen
	for (int i = 0; i < gradiente.rows - 2; i++) {
		for (int j = 0; j < gradiente.cols - 2; j++) {
			int sum = 9;
			total = 0; 			// Reiniciar el total
			for (int m = -sum / 2; m <= sum / 2; m++) {
				for (int n = -sum/ 2; n <= sum/ 2; n++) {
					int tx = i + m;			// Obtener la posición del píxel en la imagen
					int ty = j + n;		
					if(tx > 0 && tx < gradiente.rows && ty > 0 && ty < gradiente.cols) {
						total += gradiente.at<uchar>(tx, ty); // Sumar el valor del píxel
					}
				}
			}
			output.at<uchar>(i, j) = total / sum / sum;
		}
	}
	return output;
}

// Función Marco Rectangular
Mat cerrarContornos(Mat trillar, int separar_pixel) {
	Mat output(trillar.rows, trillar.cols, CV_8U); 
	for (int i = 0; i < trillar.rows; i++) {
		for (int j = 0; j < trillar.cols; j++) {
			if (trillar.at<uchar>(i, j) == 255) {	
				for (int a = 0; a < separar_pixel + 1; a++) {
					if (trillar.at<uchar>(i, j + a) == 255) {
						for (int b = 0; b < a; b++) {
							trillar.at<uchar>(i, j + b) = 255;
						}
					}
				}
			}
		}
	} 
	for (int i = 0; i < trillar.rows; i++) {
		for (int j = 0; j < trillar.cols; j++) {
			output.at<uchar>(i, j) = trillar.at<uchar>(i, j);
		}
	}
	return output; 
}

// Funcion main
int main() {
	String imgName;
	cout << "Introduzca el nombre del archivo de imagen (ex. barcode.jpg): " << endl;
	cin >> imgName;
	// Se lee la imagen
	src = ReadImage(imgName);
	// Se muestra la imagen original
	namedWindow("Original Image", WINDOW_NORMAL);
	imshow("Original Image", src);
	waitKey(0);
	destroyWindow("Original Image");

	// Se muestra la imagen difuminada
	Mat difuminada = Difuminar(gradiente);
	namedWindow("Difuminada", WINDOW_NORMAL);
	imshow("Difuminada", difuminada);
	waitKey(0);
	destroyWindow("Difuminada");

	cerrado = cerrarContornos(trillar, 100);
	namedWindow("Imagen contorneada cerrada", WINDOW_NORMAL);
	imshow("Imagen contorneada cerrada", cerrado);
	waitKey(0);
	destroyWindow("Imagen contorneada cerrada");
	
	// Se obtiene la gradiente
	convertScaleAbs(sobel, gradient);
	namedWindow("Gradiente", WINDOW_NORMAL);
	imshow("Gradiente", gradient);
	waitKey(0);
	destroyWindow("Gradiente");
	
	
	// Se crea una barra de seguimiento
	const char* trackbar_value = "Valor";
	createTrackbar(trackbar_value, "Thresholded Image", &threshold_value, 254, thresholding_call);
	thresholding_call(0, 0);
	thresh = Mat(blurred.rows, blurred.cols, CV_8U);
	
	

	return 0;
}
