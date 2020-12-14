#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <functional>
#include <algorithm>
#include <execution>

namespace utils
{
	using pixel_t = cv::Vec3b;
	using tr_pix_func_t = std::function<void(pixel_t const& src, pixel_t& dst)>;

	inline void transform_pixels(cv::Mat const& src, cv::Mat& dst, tr_pix_func_t const& func)
	{
		assert(src.rows == dst.rows);
		assert(src.cols == dst.cols);
		assert(src.type() == dst.type());

		for (auto y = 0; y < src.rows; ++y)
		{
			auto src_ptr = src.ptr<pixel_t>(y);
			auto dst_ptr = dst.ptr<pixel_t>(y);
			for (auto x = 0; x < src.cols; ++x)
			{
				func(src_ptr[x], dst_ptr[x]);
			}
		}
	}


	inline void transform_pixels_par(cv::Mat const& src, cv::Mat& dst, tr_pix_func_t const& func)
	{
		assert(src.rows == dst.rows);
		assert(src.cols == dst.cols);
		assert(src.type() == dst.type());

		std::vector<int> rows(src.rows);
		std::iota(rows.begin(), rows.end(), 0);

		auto const row_func = [&](int y)
		{
			auto src_ptr = src.ptr<pixel_t>(y);
			auto dst_ptr = dst.ptr<pixel_t>(y);
			for (auto x = 0; x < src.cols; ++x)
			{
				func(src_ptr[x], dst_ptr[x]);
			}
		};

		std::for_each(std::execution::par, rows.begin(), rows.end(), row_func);
	}
}