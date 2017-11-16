#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <fstream>
#include <vector> 

constexpr int size = 32;

using namespace std;
using namespace cv;

int main(void)
{
	

		
	Size small_size(size,size);


	for(int i = 1; i <= 4; i++)
	{
		

		for(int j = 1; j <= 13; j++)
		{
			Mat image;
			
			stringstream ss;
			cerr << i << '\n';
			ss << "class_"<< i << "/1." << i << "." << setfill('0') << setw(2) << j << ".tiff";
			std::string path;

			ss >> path;
				

			image = imread(path,CV_LOAD_IMAGE_COLOR);
			
			if(image.empty())
			{
				cout << "Could not open or find the image\n";
				return -1;
			}

			int image_counter = 0;
		
			for(int y = 0; y < image.rows; y += small_size.height)
			{
				for(int x = 0; x < image.cols; x += small_size.width)
				{
					Rect rect = Rect(x,y,small_size.width,small_size.height);

					Mat temp(image,rect);	
					cerr << temp.size() << '\n';
				 	imwrite("class_"+to_string(i)+"/output"+to_string(j)+"_"+to_string(image_counter++)+".tiff",temp); 
					
				}
					


			}


		}	
			
			
	}

	

}
