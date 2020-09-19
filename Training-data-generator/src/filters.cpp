#include <iostream>
#include <vector>
#include <string>

#include "../bin/CImg.h"
#include <jpeglib.h>
#include <png.h>

#include "filters.hpp"

using namespace std;
using namespace cimg_library;

//creates filters with different colors.
//takes in num_of_filters(int), which dictates how many filters to generate
vector<CImg<unsigned char> > create_filters(int num_of_filters)
{
	vector<CImg<unsigned char> >filters_collection;
	for (int i = 0; i < num_of_filters; i++)
	{
		cout << "Creating filter " << i + 1 << endl;
		CImg<unsigned char> filter(480, 480, 1, 4);
		int red = rand() % 256;
		int green = rand() % 256;
		int blue = rand() % 256;
		int alpha = rand() % 50;
		cimg_forXY(filter, x, y) 
		{
			filter(x, y, 0, 0) = red;
			filter(x, y, 0, 1) = green; 
			filter(x, y, 0, 2) = blue;
			filter(x, y, 0, 3) = alpha;
		}
		filters_collection.push_back(filter);
	}
	return filters_collection;
}

//creates a merged filter from 3 random filters
//takes in filters_vector (vector<CImg<unsigned char> >), which contains the vector of filters to choose from.
CImg<unsigned char> merge_filters(vector<CImg<unsigned char> > filters_vector) 
{	
	CImg<unsigned char> filter1 = filters_vector[int(rand()%filters_vector.size())];
	CImg<unsigned char> filter2 = filters_vector[int(rand()%filters_vector.size())];
	CImg<unsigned char> filter3 = filters_vector[int(rand()%filters_vector.size())];

	CImg<unsigned char> base(480, 480, 1, 4);
	cimg_forXY(base, x, y) 
	{
		base(x, y, 0, 0) = (filter1(x, y, 0, 0) + filter2(x, y, 0, 0) + filter3(x, y, 0, 0))/3;	
		base(x, y, 0, 1) = (filter1(x, y, 0, 1) + filter2(x, y, 0, 1) + filter3(x, y, 0, 1))/3;
		base(x, y, 0, 2) = (filter1(x, y, 0, 2) + filter2(x, y, 0, 2) + filter3(x, y, 0, 2))/3;
		base(x, y, 0, 3) = (filter1(x, y, 0, 3) + filter2(x, y, 0, 3) + filter3(x, y, 0, 3))/3;
	}
	return base;
	
}
