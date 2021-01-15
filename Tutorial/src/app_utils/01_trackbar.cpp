#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

constexpr auto IMAGE_PATH_1 = "D:/repos/OpenCV/Tutorial/in_files/mclaren.jpg";
constexpr auto IMAGE_PATH_2 = "D:/repos/OpenCV/Tutorial/in_files/corvette.jpg";


namespace trackbar01
{
	const int alpha_slider_max = 100;
	int alpha_slider;
	double alpha;
	double beta;
	cv::Mat src1;
	cv::Mat src2;
	cv::Mat dst;


	static void on_trackbar(int, void*)
	{
		alpha = (double)alpha_slider / alpha_slider_max;
		beta = (1.0 - alpha);
		cv::addWeighted(src1, alpha, src2, beta, 0.0, dst);
		cv::imshow("Linear Blend", dst);
	}


	void run()
	{
		src1 = cv::imread(IMAGE_PATH_1, cv::IMREAD_COLOR);
		src2 = cv::imread(IMAGE_PATH_2, cv::IMREAD_COLOR);
		if (src1.empty() || src2.empty())
		{
			std::cout << "Could not read the images\n";
			return;
		}


		alpha_slider = 0;
		cv::namedWindow("Linear Blend", cv::WINDOW_AUTOSIZE); // Create Window

		char TrackbarName[50];
		sprintf_s(TrackbarName, "Alpha x %d", alpha_slider_max);
		cv::createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);

		on_trackbar(alpha_slider, 0);
		cv::waitKey(0);
	}
}


void trackbar()
{
	trackbar01::run();
}