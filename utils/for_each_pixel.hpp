#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <functional>
#include <execution>
#include <algorithm>
#include <vector>

namespace utils
{
	inline void for_each_pixel(cv::Mat const& img, std::function<void(cv::Vec3b const&)> const& func)
	{
		for (auto y = 0; y < img.rows; ++y)
		{
			auto ptr = img.ptr<cv::Vec3b>(y);
			for (auto x = 0; x < img.cols; ++x)
			{
				func(ptr[x]);
				//func(img.at<cv::Vec3b>(y, x));
				//func(img.at<cv::Vec3b>(cv::Point(x, y)));
			}
		}
	}


	inline void for_each_pixel(cv::Mat const& img, std::function<void(size_t row, size_t col)> const& func)
	{
		for (auto y = 0; y < img.rows; ++y)
		{			
			for (auto x = 0; x < img.cols; ++x)
			{
				func(y, x);
			}
		}
	}


	inline void for_each_pixel_par_row(cv::Mat const& img, std::function<void(cv::Vec3b const&)> const& func)
	{
		std::vector<int> rows(img.rows);
		std::iota(rows.begin(), rows.end(), 0);

		auto const row_func = [&](int y)
		{
			for (auto x = 0; x < img.cols; ++x)
			{
				func(img.at<cv::Vec3b>(cv::Point(x, y)));
			}
		};

		std::for_each(std::execution::par, rows.begin(), rows.end(), row_func);
	}


	inline void for_each_pixel_par_col(cv::Mat const& img, std::function<void(cv::Vec3b const&)> const& func)
	{
		std::vector<int> cols(img.cols);
		std::iota(cols.begin(), cols.end(), 0);

		auto const col_func = [&](int x)
		{
			for (auto y = 0; y < img.rows; ++y)
			{
				func(img.at<cv::Vec3b>(cv::Point(x, y)));
			}
		};

		std::for_each(std::execution::par, cols.begin(), cols.end(), col_func);
	}


	inline void LUT_c(cv::Mat const& src, std::array<uint8_t, 256> table, cv::Mat& dst)
	{
		for (auto y = 0; y < src.rows; ++y)
		{
			auto src_ptr = src.ptr<uchar>(y);
			auto dst_ptr = dst.ptr<uchar>(y);
			for (auto x = 0; x < src.cols; ++x)
			{
				//dst.at<uchar>(y, x) = table[dst.at<uchar>(y, x)];
				dst_ptr[x] = table[src_ptr[x]];
			}
		}
	}


	inline void LUT_stl(cv::Mat const& src, std::array<uint8_t, 256> table, cv::Mat& dst)
	{
		std::vector<int> rows(src.rows);
		std::iota(rows.begin(), rows.end(), 0);

		auto const row_func = [&](int y)
		{
			auto src_ptr = src.ptr<uchar>(y);
			auto dst_ptr = dst.ptr<uchar>(y);
			for (auto x = 0; x < src.cols; ++x)
			{
				//dst.at<uchar>(y, x) = table[dst.at<uchar>(y, x)];
				dst_ptr[x] = table[src_ptr[x]];
			}
		};

		std::for_each(std::execution::par, rows.begin(), rows.end(), row_func);

		/*auto const func = [&](auto p) { return table[p]; };
		std::transform(std::execution::par, src.begin<uchar>(), src.end<uchar>(), dst.begin<uchar>(), func);*/
		
	}
}

