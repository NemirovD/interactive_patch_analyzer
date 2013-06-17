## Interactive Patch Analyser

A program that allows the user to view trend of values across a set of images with the time of the image taken encoded in the file name.

#### Usage

interactive_patch_analyser &lt;Image Folder&gt; &lt;Channel To Analyse&gt;
	
&lt;Channel To Analyse&gt; can be one of the following options:

* H &mdash; Hue channel of the HLS color space
* L &mdash; Light channel of the HLS color space
* S &mdash; Saturation channel of the HLS color space
* B &mdash; Blue channel of the BGR color space
* G &mdash; Green channel of the BGR color space
* R &mdash; Red channel of the BGR color space
* GREY &mdash; A greyscale version of the image

#### File Format

The program will expect a file format where each line has two decimal numbers. The first being the time that the data was observed, and the second being the value of the data.

##### The Makefile

The Makefile was designed to be used on windows systems and uses NMake to run it. Also the compiler uses cl.exe and link.exe to compile and link the objects respectively. These programs can be acquired by downloading installing Microsoft Visual Studio or the free version, Microsoft Visual Studio Express.

##### Additional Notes

The txt file, datadump.txt and datadump is a sample version of what all output files made by this program will look like.