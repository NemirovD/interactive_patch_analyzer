#include <iostream>
#include <cstdio>
#include <cstring>
#include <opencv2\opencv.hpp>
#include <tchar.h>
#include <strsafe.h>





typedef struct _Image{
	_Image();
	_Image(cv::Mat,double);
	~_Image();
	void setImage(cv::Mat img);
	double time;
	cv::Mat image;
	bool operator<(const _Image img);
	bool operator>(const _Image img);
	bool operator==(const _Image img);
} Image;

typedef struct _Patch{
	public:
	_Patch();
	_Patch(cv::Rect rect, double value);
	~_Patch();
	cv::Rect getRect(){return this->rect;}
	double getValue(){return this->value;}
private:
	cv::Rect rect;
	double value;
} Patch;


typedef struct _UserPatchedImage{
	_UserPatchedImage();
	_UserPatchedImage(_Image img, std::vector<cv::Rect> rects);
	~_UserPatchedImage();
	Patch operator[](int i);
	
	double time;
	cv::Mat image;
	std::vector<Patch> patches;

} UserPatchedImage;

std::vector<Image> loadImgs(TCHAR * directory, TCHAR* format);


std::vector<cv::Rect> drawPatches(cv::Mat img);

std::vector<UserPatchedImage> getPatches(std::vector<Image> Images, std::vector<cv::Rect> rects);

void writeRects(std::vector<cv::Rect>, cv::Mat);

