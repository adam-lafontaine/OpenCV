#pragma once
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <functional>


namespace video_test
{
	inline void process_capture(std::function<void(cv::Mat&, cv::Mat&)> const& func)
	{
		auto const quit = [](auto c) { return c == 'q' || c == 27; };

		cv::VideoCapture cap(0);

		cv::Mat src, dst;

		for (;;)
		{
			cap >> src;
			if (src.empty())
			{
				break;
			}

			func(src, dst);

			cv::imshow("Source", src);
			cv::imshow("Result", dst);

			if (quit(cv::waitKey(15)))
			{
				break;
			}
		}

	}
}


