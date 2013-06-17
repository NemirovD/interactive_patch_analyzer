#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <tchar.h>
#include <strsafe.h>
#include "Interactive.hpp"

int main(int argc, TCHAR * args[]){
	std::vector<Image> images;
	std::vector<cv::Rect> rects;
	std::vector<UserPatchedImage> upi;
	
	std::ofstream file_out;
	
	if(argc!=3){
	
		std::cout << "Usage: test.exe <image directory> <color space>" << std::endl;
		return -1;
	}
	
	images = loadImgs(args[1], args[2]);
	
	if(images.empty()){
		std::cerr << "Something went wrong and the images vector is empty" << std::endl;
		std::cerr << "Maybe you put the wrong directory name in" << std::endl;
		return -1;
	}
	
	
	rects = drawPatches(images[0].image);
	
	upi = getPatches(images, rects);
	
	file_out.open("datadump.txt");
	std::vector<double> averages;
	double temp_average;
	for(int i = 0; i < rects.size(); ++i){
		temp_average = 0;
		for(int j = 0; j < upi.size(); ++j){
		
			temp_average += (upi[j][i].getValue()/upi[j][i].getRect().area());
		}
		
		temp_average /= upi.size();
		averages.push_back(temp_average);
	}
	
	for(int i = 0; i < rects.size(); ++i){
		
		for(int j = 0; j < upi.size(); ++j){
		
			file_out << upi[j].time << " " << ((double)upi[j][i].getValue()/upi[j][i].getRect().area())/averages[i] << std::endl;
		}
		file_out << "END" << std::endl;
	}
	file_out.flush();
	file_out.close();
	
	file_out.open("datadump2.txt");
	for(int i = 0; i < rects.size(); ++i){
		
		for(int j = 0; j < upi.size(); ++j){
		
			file_out << upi[j].time << " " << (upi[j][i].getValue()/upi[j][i].getRect().area()) << std::endl;
		}
		file_out << "END" << std::endl;
	}
	file_out.flush();
	file_out.close();
	
	system("cmd /c start matlab -nodesktop -nosplash -r \"GraphScript\"");
	
	writeRects(rects,images[0].image);
	
	return 0;
}