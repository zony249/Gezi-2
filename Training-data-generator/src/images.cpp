/////////////////////////////////////////////
// Program: Gezi Training Data Generator 2 //
// File: images.cpp			   //
/////////////////////////////////////////////


#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#define cimg_use_png 1
#include "../bin/CImg.h"
#include <jpeglib.h>
#include <png.h>



#include "images.hpp"

using namespace std;
using namespace cimg_library;

CImg<unsigned char> Image::merge_images(int num_of_bg, int num_of_markers, CImg<unsigned char> filter)
{
	CImg<unsigned char> base_image(416, 416, 1, 3);
	string path_bg = "Images/Backgrounds/" + to_string(rand()%num_of_bg) + ".png";
	int shift = -(rand()%160);
	CImg<unsigned char> background(path_bg.c_str());
	background.resize(background.width() + rand() % 30, background.height() + rand() % 30, 1, 3);
	base_image.draw_image(shift, 0, background, 1);


	int how_many_markers = rand()%6; // random number between 0 and 5 inclusive.
	vector<int> pos_x_vec;
	vector<int> pos_y_vec;

	// Attaches markers to the image
	for (int i = 0; i < how_many_markers; i++)
	{
		//combine images
		int marker_index_rand = rand()%num_of_markers;
		string path_marker = "Images/Markers/" + to_string(marker_index_rand) + ".png";
		CImg<unsigned char> marker(path_marker.c_str());
		double color;
		switch(marker_index_rand)
		{
			case 0:
				color = 2;
				break;
			case 1:
				color = 4;
				break;
			case 2: 
				color = 4;
				break;
			case 3:
				color = 4;
				break;
			case 4:
				color = 8;
				break;
			case 5:
				color = 4;
				break;
			case 6: 
				color = 0;
				break;
			case 7:
				color = 4;
				break;
			case 8:
				color = 2;
				break;
			default:
				color = 0;
				break;

		}


		//resize the marker by a random amount between 0.4 and 1
		float resize_scale = float(rand())/float(RAND_MAX) * 0.6 + 0.4;
		marker.resize(marker.width()*resize_scale, marker.height()*resize_scale, 1, 4);
		double marker_width = marker.width();
		double marker_height = marker.height();
		double pos_x;
		double pos_y;

		//makes sure that markers do not overlap
		if (pos_x_vec.size() > 0)
		{
			pos_x = rand()%416;
			pos_y = rand()%416;
			pos_x_vec.push_back(pos_x);
			pos_y_vec.push_back(pos_y);
		}
		else
		{
			pos_x = rand()%416;
			pos_y = rand()%416;
			pos_x_vec.push_back(pos_x);
			pos_y_vec.push_back(pos_y);
		}
		double angle = M_PI * 2 * (double(rand())/double(RAND_MAX));		
		marker = rotate(marker, angle);
			
		CImg<unsigned char> marker_rgb = marker.get_shared_channels(0, 2);
		CImg<unsigned char> marker_a = marker.get_shared_channel(3);

		base_image.draw_image(pos_x - marker_width, pos_y - marker_height, marker_rgb, marker_a, 1, 255);
		
		vector<double> marker_attr = {color, pos_x/416, pos_y/416, marker_width/416, marker_height/416};

		for (int j = 0; j < marker_attr.size(); j++)
		{
			annotations.push_back(marker_attr[j]);
		}
		
	}
	
	CImg<unsigned char> filter_rgb = filter.get_shared_channels(0, 2);
	CImg<unsigned char> filter_a = filter.get_shared_channel(3);


	base_image.draw_image(0, 0, filter_rgb, filter_a, 1, 255);

	return base_image;
}


CImg<unsigned char> rotate(CImg<unsigned char> image, double a)
{
	short width = image.width();
	short height = image.height();
	CImg<unsigned char> base_image(width*2, height*2, 1, 4);
	cimg_forXY(base_image, x, y) 
	{
		base_image(x, y, 0, 3) = 0;
	}

	vector<double> rot_mat = {
		cos(a), -sin(a),
		sin(a), cos(a)
	};
	
	short center_x = width/2;
	short center_y = height/2;
	cimg_forXY(image, x, y) 
	{
		short pix_vec_x = x - center_x;
		short pix_vec_y = y - center_y;

		float transformed_x = rot_mat[0] * pix_vec_x + rot_mat[1] * pix_vec_y;	
		float transformed_y = rot_mat[2] * pix_vec_x + rot_mat[3] * pix_vec_y;
		
		base_image(transformed_x + center_x * 2 + 1, transformed_y + center_y * 2, 0, 0) = image(x, y, 0, 0);
		base_image(transformed_x + center_x * 2 + 0, transformed_y + center_y * 2, 0, 0) = image(x, y, 0, 0);

		base_image(transformed_x + center_x * 2 + 1, transformed_y + center_y * 2, 0, 1) = image(x, y, 0, 1);
		base_image(transformed_x + center_x * 2 + 0, transformed_y + center_y * 2, 0, 1) = image(x, y, 0, 1);

		base_image(transformed_x + center_x * 2 + 1, transformed_y + center_y * 2, 0, 2) = image(x, y, 0, 2);	
		base_image(transformed_x + center_x * 2 + 0, transformed_y + center_y * 2, 0, 2) = image(x, y, 0, 2);

		base_image(transformed_x + center_x * 2 + 1, transformed_y + center_y * 2, 0, 3) = image(x, y, 0, 3);
		base_image(transformed_x + center_x * 2 + 0, transformed_y + center_y * 2, 0, 3) = image(x, y, 0, 3);


	}
	return base_image;
}

double dist(int x1, int y1, int x2, int y2)
{	
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
