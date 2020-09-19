/////////////////////////////////////////////
// Project: Gezi Training Data Generator 2 //
// File: images.hpp 			   //
/////////////////////////////////////////////


#ifndef _IMAGES_HPP
#define _IMAGES_HPP

#include <iostream>
#include <vector>
#include <string>

#include "../bin/CImg.h"
#include <jpeglib.h>
#include <png.h>

using namespace std;
using namespace cimg_library;

class Image 
{
	public:


		CImg<unsigned char> final_image;
		vector<double> annotations;
		CImg<unsigned char> merge_images(int num_of_bg, int num_of_markers, CImg<unsigned char> filter);
		
};

CImg<unsigned char> rotate(CImg<unsigned char> image, double a);
double dist(int x1, int y1, int x2, int y2);
#endif
