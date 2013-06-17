OBJECTFILES = driver.obj Interactive.obj

STANDALONEOPTIONS = /EHsc /nologo /c

OWNINCLUDES = C:\Android Project\opencv\build\include;

OPENCV_LIB = C:\Android Project\opencv\build\x86\vc10\lib

LINK =  "$(OPENCV_LIB)\opencv_core240.lib" \
		"$(OPENCV_LIB)\opencv_highgui240.lib" \
		"$(OPENCV_LIB)\opencv_imgproc240.lib" \
		"$(OPENCV_LIB)\opencv_video240.lib" \
		"$(OPENCV_LIB)\opencv_ml240.lib" \
		"$(OPENCV_LIB)\opencv_features2d240.lib" \
		"$(OPENCV_LIB)\opencv_calib3d240.lib"

INCLUDE = $(INCLUDE)$(OWNINCLUDES)

prog: $(OBJECTFILES)
	link /OUT:interactive_patch_analyzer.exe /nologo $(OBJECTFILES) $(LINK)
	DEL $(OBJECTFILES)
	
driver.obj:
	cl $(STANDALONEOPTIONS) driver.cc
	
Interactive.obj:
	cl $(STANDALONEOPTIONS) Interactive.cpp
	
run:
	interactive_patch_analyzer "C:\Users\100423722\Desktop\vid3" L