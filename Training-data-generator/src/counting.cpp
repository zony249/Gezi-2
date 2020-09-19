/////////////////////////////////////////////
// Program: Gezi Training Data Generator 2 //
// File: counting.cpp			   //
/////////////////////////////////////////////





#include <iostream>
#include <dirent.h>
#include <thread>
#include <chrono>

#include "counting.hpp"

using namespace std;

void cout_num_of_files(int num_of_images)
{
	int num_of_training;
	int num_of_testing;
	int num_of_validating;
	int total = 0;
	while (total < num_of_images)
	{	//system("clear");
		num_of_training = count_from_dir("Images/Processed Images/training");
		num_of_validating = count_from_dir("Images/Processed Images/validating");
		num_of_testing = count_from_dir("Images/Processed Images/testing");
		total = num_of_training + num_of_validating + num_of_testing;
		cout << "Processing Images: "  << total << endl;
		this_thread::sleep_for(chrono::milliseconds(33));
	}

}

int count_from_dir(const char *path)
{
	struct dirent *dp;
   	DIR *dir = opendir(path);

    	// Unable to open directory stream
    	if (!dir) 
        	return -1; 
	int counter = 0;
    	while ((dp = readdir(dir)) != NULL)
    	{
        	counter ++;
    	}

    	// Close directory stream
    	closedir(dir);
	return counter;
}		
		
