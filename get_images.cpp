//============================================================================
// Name        : get_images.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Get Images from camera using OpenCV
//============================================================================

/**

C:/local/opencv-4.2.0/build/bin must be in path or
	all DLLs copied to the executable directory
	or ecexutable copied an run in this directory


INCLUDEPATH += \
	C:/local/opencv-4.2.0/build \
	C:/local/opencv-4.2.0/modules/core/include \
	C:/local/opencv-4.2.0/modules/highgui/include \
	C:/local/opencv-4.2.0/modules/modules/include \
	C:/local/opencv-4.2.0/modules/imgcodecs/include \
	C:/local/opencv-4.2.0/modules/imgproc/include \
	C:/local/opencv-4.2.0/modules/videoio/include \
	C:/local/libzip-1.10.1/build/bin/libzip\include \

LIBS += \
	-LC:/local/opencv-4.2.0/build/bin \
	-lopencv_core420 \
	-lopencv_highgui420 \
	-lopencv_imgcodecs420 \
	#-lopencv_imgproc420 \
	#-lopencv_videoio420 \
	-LC:/local/libzip-1.10.1/build/bin/libzip/bin \
	-lzip \

*/
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

#define ENABLE_TEXT_OUTPUT	false
#define ENABLE_IMAGE_VIEW	true

#define AeroLog_Track(...)	do { printf(__FUNCTION__, "\t", __LINE__); printf("\n"); fflush(stdout); } while(false)
#define AeroLog_Info(format, ...)  do { printf(__FUNCTION__, "\t", __LINE__, "\t"); printf( format, __VA_ARGS__); printf("\n"); fflush(stdout); } while(false)
#define AeroLog_Info_(...)  do { printf(__FUNCTION__, "\t", __LINE__, "\t"); printf( __VA_ARGS__); printf("\n"); fflush(stdout); } while(false)

#include <fstream>
//#include <filesystem>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdint.h>
#include <vector>
#include <chrono>

#include <opencv2/core.hpp>
//#include <opencv2/core/ocl.hpp>
//#include <opencv2/core/utility.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


bool captureAndSaveImage(int cameraIndex, const std::string &folderPath, int imageIndex) {
    // Open the camera
    cv::VideoCapture cap(cameraIndex);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return false;
    }

    // Capture an image
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "Error: Could not capture image." << std::endl;
        return false;
    }

    // Generate the output file name
    std::ostringstream oss;
    oss << folderPath << "/image_" << std::setw(4) << std::setfill('0') << imageIndex << ".png";
    std::string outputFileName = oss.str();

    // Save the image to a PNG file
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(3);
    if (!cv::imwrite(outputFileName, frame, compression_params)) {
        std::cerr << "Error: Could not save image to " << outputFileName << std::endl;
        return false;
    }

	/******** start of extra code for visualization and file saving *********/
#if ENABLE_IMAGE_VIEW
	auto imageWidth = frame.cols;
	auto imageHeight = frame.rows;
	cv::namedWindow("view");
	cv::imshow("view", frame);
	cv::resizeWindow("view", imageWidth/2, imageHeight/2);
	cv::waitKey();
	cv::destroyWindow("view");
#endif /* ENABLE_IMAGE_VIEW */

    std::cout << "Image saved to " << outputFileName << std::endl;
    return true;
}

int main(int argc, char *argv[]) {

    int cameraIndex = 6; // Default camera index  ( index 2 gray scale, low res camera ; index 4 is depth camera; index 6 is high resolution color camera)
	int numberOfFrames = 20; 		// Number of frames to capture
    std::string folderPath = "/home/francisco/workspace/get_images/captured_images";

	// Parse command line arguments
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "-c" && i + 1 < argc) {
			cameraIndex = std::stoi(argv[++i]);
		} else if (std::string(argv[i]) == "-f" && i + 1 < argc) {
			numberOfFrames = std::stoi(argv[++i]);
		}
	}
	
    int imageIndex = 1; // Example image index

	for (int i = 0; i < numberOfFrames; i++) {
		if (!captureAndSaveImage(cameraIndex, folderPath, imageIndex++)) {
			return 1;
		}
	}

	return 0;
}
