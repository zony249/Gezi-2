#ifndef _FILTERS_HPP
#define _FILTERS_HPP

#include <iostream>
#include <vector>
#include <string>

#include "../bin/CImg.h"
#include <png.h>
#include <jpeglib.h>

using namespace std;
using namespace cimg_library;

vector<CImg<unsigned char> > create_filters(int num_of_filters);
CImg<unsigned char> merge_filters(vector<CImg<unsigned char> >filters_vector);
	
	


#endif
