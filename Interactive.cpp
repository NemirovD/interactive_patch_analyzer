#include "Interactive.hpp"



_Image::_Image(){
	this->time = NULL;
	this->image = cv::Mat();
}


_Image::_Image(cv::Mat image, double time){
	this->time = time;
	this->image = image;
}

_Image::~_Image(){
	this->time =NULL;
	this->image.release();
}

void _Image::setImage(cv::Mat img){

	this->image = img;
}

bool _Image::operator<(const _Image img){
	return (this->time < img.time);
}

bool _Image::operator>(const _Image img){
	return (this->time > img.time);
}

bool _Image::operator==(const _Image img){
	return (this->time == img.time);
}


_Patch::_Patch(){
	this->rect = cv::Rect();
	this->value = NULL;
}

_Patch::_Patch(cv::Rect rect, double value){
	this->rect = rect;
	this->value = value;
}

_Patch::~_Patch(){
	this->rect = cv::Rect();
	this->value = NULL;
}



_UserPatchedImage::_UserPatchedImage(){
	this->time = NULL;
	this->image = cv::Mat();
	this->patches.~vector();
}

_UserPatchedImage::_UserPatchedImage(_Image img, std::vector<cv::Rect> rects){
	this->time = img.time;
	this->image = img.image;
	this->patches = std::vector<Patch>();
	
	cv::Mat tROI, intROI;
	Patch tPatch;
	
	for(int i = 0; i < rects.size(); ++i){
		
		tROI = cv::Mat(this->image, rects[i]);
		cv::integral(tROI, intROI);
		
		int* p = intROI.ptr<int>(intROI.rows-1);
		p+=intROI.cols-1;
		
		tPatch = Patch(rects[i], *p);
		this->patches.push_back(tPatch);
	}
}

_UserPatchedImage::~_UserPatchedImage(){
	this->time = NULL;
	this->image.release();
	this->patches.~vector();
}


Patch _UserPatchedImage::operator[](int i){
	return this->patches[i];
}


cv::Mat formatMat(cv::Mat mat, TCHAR* format){

	if(_tcscmp(format, _T("B"))==0){
		cv::Mat BGR[3];
		cv::split(mat, BGR);
		return BGR[0];
	}else if(_tcscmp(format, _T("G"))==0){
		cv::Mat BGR[3];
		cv::split(mat, BGR);
		return BGR[1];
	}else if(_tcscmp(format, _T("R"))==0){
		cv::Mat BGR[3];
		cv::split(mat, BGR);
		return BGR[2];
	}else if(_tcscmp(format, _T("H"))==0){
		cv::Mat HLS[3];
		cv::split(mat, HLS);
		return HLS[0];
	}else if(_tcscmp(format, _T("L"))==0){
		cv::Mat HLS[3];
		cv::split(mat, HLS);
		return HLS[1];
	}else if(_tcscmp(format, _T("S"))==0){
		cv::Mat HLS[3];
		cv::split(mat, HLS);
		return HLS[2];
	}else{
		//defaults to grey in case of incorrect argument
		cv::cvtColor(mat,mat,CV_BGR2GRAY);
		return mat;
	}


}

std::string makeString(TCHAR * str){
	
	std::string toReturn;
	#ifdef _UNICODE
	toReturn = narrow(std::wstring(str));
	#else
	toReturn = std::string(str);
	#endif
	
	return toReturn;
}



std::vector<Image> loadImgs(TCHAR * directory, TCHAR* format){
	//This method will load images from
	// a directory to a vector for storage
	//so that it'll be ready for usage.
	int seconds, milliseconds;
	std::vector<Image> images;
	TCHAR dir[MAX_PATH];
	WIN32_FIND_DATA fd;
	HANDLE fh = INVALID_HANDLE_VALUE;
	cv::Mat tMat;

	_tcscpy(dir,directory);
	_tcscat(dir,_T("\\*"));

	fh = FindFirstFile(dir,&fd);

	if(fh == INVALID_HANDLE_VALUE){
		std::cout << "could not find dir" << std::endl;
		return std::vector<Image>();
	}


	do{
		if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
		}else{
			_tcscpy(dir, directory);
			_tcscat(dir, "\\");
			_tcscat(dir, fd.cFileName);
			sscanf(fd.cFileName, "%d%*c%*c%d", &seconds,&milliseconds);
			tMat = cv::imread(std::string(dir));
			
			tMat = formatMat(tMat, format);
			
			images.push_back(Image(tMat, (double)(seconds + (milliseconds/1000.0f))));
			
			if(cv::imread(makeString(dir)).empty()){
				std::cout << "not working" << std::endl;
			
			}

			std::sort(images.begin(),images.end());
		}
	}while(FindNextFile(fh,&fd) != 0);

	return images;
}

cv::Rect box;
bool drawing = false;
std::vector<cv::Rect> user_patches;
cv::Mat mouse_callback_image;

void rect_draw_mouse_callback(int event, int x, int y, int flags, void* param){
	
	cv::Mat img = mouse_callback_image;
	cv::Rect rect;
	
	switch(event){
		case CV_EVENT_MOUSEMOVE:
			if(drawing){
				box.width = x - box.x;
				box.height = y - box.y;
			} break;
			
		case CV_EVENT_LBUTTONDOWN:
			drawing = true;
			box = cv::Rect(x,y,0,0);
			break;
			
		case CV_EVENT_LBUTTONUP:
			drawing = false;
			
			if(box.width < 0){
				box.x += box.width;
				box.width *= -1;
			}
			
			if(box.height < 0){
				box.y += box.height;
				box.height *=-1;
			}
			
			cv::rectangle(img,box,cv::Scalar(255));
			rect = cv::Rect(box);
			user_patches.push_back(rect);
			break;
	
	}
	return;
}


std::vector<cv::Rect> drawPatches(cv::Mat img){
	std::string name = "Image";
	cv::Mat temp_image;
	cv::namedWindow(name);
	
	mouse_callback_image = img;
	
	cv::setMouseCallback(name, rect_draw_mouse_callback, 0);
	
	while(user_patches.size() < 7){
		temp_image = img.clone();
		if(drawing)
			cv::rectangle(temp_image,box,cv::Scalar(255));
			
		cv::imshow(name, temp_image);
		
		if(cv::waitKey(33) == 13){
			break;
		}
	}
	
	temp_image.release();
	cv::destroyWindow(name);
	
	return user_patches;
}

std::vector<UserPatchedImage> getPatches(std::vector<Image> Images, std::vector<cv::Rect> rects){
	std::vector<UserPatchedImage> toReturn;
	
	for(int i = 0; i < Images.size(); ++i){
		toReturn.push_back(UserPatchedImage(Images[i], rects));
	}
	
	return toReturn;
}




void writeRects(std::vector<cv::Rect> rectangles, cv::Mat imgr){
cv::Scalar colors[7];
colors[0] = cv::Scalar(255,0,0);
colors[1] = cv::Scalar(0,255,0);
colors[2] = cv::Scalar(0,0,255);
colors[3] = cv::Scalar(255,176,0);
colors[4] = cv::Scalar(255,0,255);
colors[5] = cv::Scalar(0,255,255);
colors[6] = cv::Scalar(0,0,0);

	cv::Mat img = imgr.clone();
	
	
	cv::cvtColor(img,img,CV_GRAY2BGR);
	for(int i = 0; i < rectangles.size(); ++i){
		
		cv::rectangle(img, rectangles[i], colors[i]);
	}
	
	cv::imshow("dataImage.jpg", img);
	cv::waitKey(0);
	return;
}

