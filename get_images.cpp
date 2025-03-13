//============================================================================
// Name        : get_images.cpp
// Author      : francisco.ares@bairesdev.com
// Version     : 0.0.1
// Copyright   : Copyright (c) 2025 Francisco J. A. Ares - MIT License
// Description : Get images from camera using OpenCV
//============================================================================
// Compile: g++ -o get_images get_images.cpp `pkg-config --cflags --libs opencv4`
// Run: ./get_images -c 6 -f 20
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
//#include <filesystem>
#include <stdint.h>
#include <vector>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
//#include <opencv2/core/ocl.hpp>
//#include <opencv2/core/utility.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/videoio.hpp>

#define ENABLE_TEXT_OUTPUT	false
#define ENABLE_IMAGE_VIEW	true

#define Log_Track(...)	do { printf(__FUNCTION__, "\t", __LINE__); printf("\n"); fflush(stdout); } while(false)
#define Log_Info(format, ...)  do { printf(__FUNCTION__, "\t", __LINE__, "\t"); printf( format, __VA_ARGS__); printf("\n"); fflush(stdout); } while(false)
#define Log_Info_(...)  do { printf(__FUNCTION__, "\t", __LINE__, "\t"); printf( __VA_ARGS__); printf("\n"); fflush(stdout); } while(false)


//! /brief Capture an image from the camera and save it to a PNG file
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

//! /brief Main function
//
//! The main function parses command line arguments, captures images from a camera, and saves them to PNG files.
//! The camera index and number of frames to capture can be specified as command line arguments.
//! The images are saved to a folder named "captured_images" in the current working directory as PNG files with names "image_0001.png", "image_0002.png", etc.
//! The images are saved with a compression level of 3.  During the process, the images are displayed in a window and the window is closed after a key is pressed.
//! The parameter are:
//! -c <camera_index> : Camera index (default: 4)
//! -f <number_of_frames> : Number of frames to capture (default: 20)
//! /param argc Number of command line arguments
//! /param argv Command line arguments
//! /return 0 on success, 1 on failure
int main(int argc, char *argv[]) {

    int cameraIndex = 4; // Default camera index  ( index 2 gray scale, low res camera ; index 4 is depth camera; index 6 is high resolution color camera)
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
