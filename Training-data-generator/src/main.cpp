/////////////////////////////////////////////
// Program: Gezi Training Data Generator 2 //
// File: main.cpp			   //
/////////////////////////////////////////////



#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <dirent.h>
#include <fstream>

#define cimg_use_jpeg 1
#define cimg_display 1
#include "../bin/CImg.h"
#include "png.h"
#include "jpeglib.h"


#include "filters.hpp"
#include "images.hpp"
#include "counting.hpp"


using namespace std;
using namespace cimg_library;

// vector<vector<double> > generate_images();
// merges images and saves them. 
// runs one instance per thread
// start and end marks the starting image and the ending image this function will process.
// returns one chunk of the annotations vector specific to the section this function was assigned to
vector<vector<double> > generate_images(int start, int end, vector<CImg<unsigned char> > filters_vec, int num_of_bg, int num_of_markers, int num_of_images)
{
	vector<vector<double> > annotation_chunk = {};
	for (int i = start; i <= end; i++)
	{	
		Image image;
		CImg<unsigned char> merged_filter = merge_filters(filters_vec);

		image.final_image = image.merge_images(num_of_bg, num_of_markers, merged_filter);
		
		if (double(i) / double(num_of_images) < 0.9)
		{
			string save_path = "Images/Processed Images/training/" + to_string(i) + ".jpg";
			image.final_image.save(save_path.c_str());	
		} 
		else if (double(i) / double(num_of_images) < 0.99)	
		{
			string save_path = "Images/Processed Images/validating/" + to_string(i) + ".jpg";	
			image.final_image.save(save_path.c_str());	
		} 
		else
		{
			string save_path = "Images/Processed Images/testing/" + to_string(i) + ".jpg";	
			image.final_image.save(save_path.c_str());	
		}
				
		annotation_chunk.push_back(image.annotations);

	}

	return annotation_chunk;
}

void create_annotations (vector<vector<double> > annotations)
{
	ofstream outfile;
	string annotations_path;
	for (int i = 0; i < annotations.size(); i++)
	{
		string folder;
		if (double(i)/double(annotations.size()) < 0.9)
		{
			folder = "training/";
		}
		else if (double(i)/double(annotations.size()) < 0.99)
		{
			folder = "validating/";
		}
		else
		{
			folder = "testing/";
		}

		annotations_path = "Annotations/" + folder + to_string(i) + ".txt";
		outfile.open(annotations_path);

		for (int j = 0; j < annotations[i].size(); j++)
		{
			outfile << annotations[i][j] << " ";
			if (j%5 == 4)
			{
				outfile << "\n";
			}
		}
		outfile.close();
	}


	ofstream trainfile;
	ofstream valfile;
	ofstream testfile;
	string trainfile_path = "Annotations/datafile/train.txt";
	string valfile_path = "Annotations/datafile/val.txt";
	string testfile_path = "Annotations/datafile/test.txt";
	trainfile.open(trainfile_path);
	valfile.open(valfile_path);
	testfile.open(testfile_path);
	

	for (int j = 0; j < annotations.size(); j++)
	{
		if (double(j)/double(annotations.size()) < 0.9)
		{
			trainfile << "../gezi_training_data/images/training/" + to_string(j) + ".jpg" << "\n";	
		}
		else if (double(j)/double(annotations.size()) < 0.99)
		{
			valfile << "../gezi_training_data/images/validating/" + to_string(j) + ".jpg" << "\n";
		}
		else
		{
			testfile << "../gezi_training_data/images/testing/" + to_string(j) + ".jpg" << "\n";
		}

	}
	trainfile.close();
	valfile.close();
	testfile.close();

	ofstream classes;
	classes.open("Annotations/datafile/classes.names");
	classes << "black \nbrown \nred \norange \nyellow \ngreen \nblue \nviolet \ngray \nwhite";
	classes.close();
}




int main()
{
	
	int num_of_bg = 10;
	int num_of_markers = 9;
	int num_of_images = 100000;
	int threads = 16;

	vector<int> boundaries = {0};
	for (int i = 1; i < num_of_images + 1; i++)
	{
		if (i % (num_of_images/threads) == 0) 
		{	
			boundaries.push_back(i);
			boundaries.push_back(i+1);
		}
	}
	boundaries.pop_back();

	vector<CImg<unsigned char> > filters_vec = create_filters(100);

	system("rm -r 'Images/Processed Images/testing'");
	system("rm -r 'Images/Processed Images/training'");
	system("rm -r 'Images/Processed Images/validating'");
	system("rm -r 'Annotations/'");
	system("mkdir 'Images/Processed Images/testing'");
	system("mkdir 'Images/Processed Images/training'");
	system("mkdir 'Images/Processed Images/validating'");
	system("mkdir 'Annotations'");
	system("mkdir 'Annotations/datafile'");
	system("mkdir 'Annotations/training'");
	system("mkdir 'Annotations/testing'");
	system("mkdir 'Annotations/validating'");


	vector<vector<double> > t0_annotations_vec;
	thread t0 ([&t0_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t0_annotations_vec = generate_images(boundaries[0], boundaries[1], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t1_annotations_vec;
	thread t1 ([&t1_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t1_annotations_vec = generate_images(boundaries[2], boundaries[3], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t2_annotations_vec;
	thread t2 ([&t2_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t2_annotations_vec = generate_images(boundaries[4], boundaries[5], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t3_annotations_vec;
	thread t3 ([&t3_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t3_annotations_vec = generate_images(boundaries[6], boundaries[7], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});
		
	vector<vector<double> > t4_annotations_vec;
	thread t4 ([&t4_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t4_annotations_vec = generate_images(boundaries[8], boundaries[9], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});


	vector<vector<double> > t5_annotations_vec;
	thread t5 ([&t5_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t5_annotations_vec = generate_images(boundaries[10], boundaries[11], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t6_annotations_vec;
	thread t6 ([&t6_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t6_annotations_vec = generate_images(boundaries[12], boundaries[13], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t7_annotations_vec;
	thread t7 ([&t7_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t7_annotations_vec = generate_images(boundaries[14], boundaries[15], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t8_annotations_vec;
	thread t8 ([&t8_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t8_annotations_vec = generate_images(boundaries[16], boundaries[17], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t9_annotations_vec;
	thread t9 ([&t9_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t9_annotations_vec = generate_images(boundaries[18], boundaries[19], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t10_annotations_vec;
	thread t10 ([&t10_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t10_annotations_vec = generate_images(boundaries[20], boundaries[21], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t11_annotations_vec;
	thread t11 ([&t11_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t11_annotations_vec = generate_images(boundaries[22], boundaries[23], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});
		
	vector<vector<double> > t12_annotations_vec;
	thread t12 ([&t12_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t12_annotations_vec = generate_images(boundaries[24], boundaries[25], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});


	vector<vector<double> > t13_annotations_vec;
	thread t13 ([&t13_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t13_annotations_vec = generate_images(boundaries[26], boundaries[27], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t14_annotations_vec;
	thread t14 ([&t14_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t14_annotations_vec = generate_images(boundaries[28], boundaries[29], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});

	vector<vector<double> > t15_annotations_vec;
	thread t15 ([&t15_annotations_vec, boundaries, filters_vec, num_of_bg, num_of_markers, num_of_images](){
		t15_annotations_vec = generate_images(boundaries[30], boundaries[31], filters_vec, num_of_bg, num_of_markers, num_of_images);
	});




	thread tx (cout_num_of_files, num_of_images);


	t0.join();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	t10.join();
	t11.join();
	t12.join();
	t13.join();
	t14.join();
	t15.join();
		
	tx.join();

	
	vector<vector<double> > ann_vec = {};
	ann_vec.insert(ann_vec.end(), t0_annotations_vec.begin(), t0_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t1_annotations_vec.begin(), t1_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t2_annotations_vec.begin(), t2_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t3_annotations_vec.begin(), t3_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t4_annotations_vec.begin(), t4_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t5_annotations_vec.begin(), t5_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t6_annotations_vec.begin(), t6_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t7_annotations_vec.begin(), t7_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t8_annotations_vec.begin(), t8_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t9_annotations_vec.begin(), t9_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t10_annotations_vec.begin(), t10_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t11_annotations_vec.begin(), t11_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t12_annotations_vec.begin(), t12_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t13_annotations_vec.begin(), t13_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t14_annotations_vec.begin(), t14_annotations_vec.end());
	ann_vec.insert(ann_vec.end(), t15_annotations_vec.begin(), t15_annotations_vec.end());


	cout << ann_vec.size() << ", " << ann_vec[0].size() << endl;
	

	create_annotations(ann_vec);








	return 0;
}


