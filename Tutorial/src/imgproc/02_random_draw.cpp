#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


constexpr int WINDOW_HEIGHT = 500;
constexpr int WINDOW_WIDTH = 500;
constexpr int x_1 = 1;
constexpr int x_2 = WINDOW_WIDTH - 2;
constexpr int y_1 = 1;
constexpr int y_2 = WINDOW_HEIGHT - 2;
constexpr auto WINDOW_NAME = "Window";
constexpr int NUMBER = 50;

void Drawing_Random_Lines(cv::Mat& image, cv::RNG& rng);
void Displaying_Random_Text(cv::Mat& image, cv::RNG& rng);


void random_draw()
{
	cv::RNG rng(0xFFFFFFFF);
	cv::Mat image = cv::Mat::zeros(WINDOW_HEIGHT, WINDOW_WIDTH, CV_8UC3);

	cv::imshow(WINDOW_NAME, image);
	cv::waitKey();

	Drawing_Random_Lines(image, rng);
	cv::imshow(WINDOW_NAME, image);
	cv::waitKey();

	Displaying_Random_Text(image, rng);
	cv::imshow(WINDOW_NAME, image);
	cv::waitKey();
}


static cv::Scalar randomColor(cv::RNG& rng)
{
	int icolor = (unsigned)rng;
	return cv::Scalar(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}


int random_x(cv::RNG& rng)
{
	return rng.uniform(x_1, x_2);
}


int random_y(cv::RNG& rng)
{
	return rng.uniform(y_1, y_2);
}


cv::Point random_point(cv::RNG& rng)
{
	cv::Point pt;

	pt.x = rng.uniform(x_1, x_2);
	pt.y = rng.uniform(y_1, y_2);

	return pt;
}


void Drawing_Random_Lines(cv::Mat& image, cv::RNG& rng)
{
	int lineType = 8;
	cv::Point pt1 = random_point(rng);
	cv::Point pt2 = random_point(rng);
	for (int i = 0; i < NUMBER; i++)
	{
		pt1.x = random_x(rng);
		pt1.y = random_y(rng);
		pt2.x = random_x(rng);
		pt2.y = random_y(rng);

		line(image, pt1, pt2, randomColor(rng), rng.uniform(1, 10), 8);
	}
}


void Displaying_Random_Text(cv::Mat& image, cv::RNG& rng)
{
	int lineType = 8;
	for (int i = 1; i < NUMBER; i++)
	{
		cv::Point pt = random_point(rng);

		putText(image, "Testing text rendering", pt, rng.uniform(0, 8),
			rng.uniform(0, 100) * 0.05 + 0.1, randomColor(rng), rng.uniform(1, 10), lineType);
	}

}