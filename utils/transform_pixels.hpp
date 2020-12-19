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

		auto const width = static_cast<size_t>(src.cols);
		size_t id_size = static_cast<size_t>(src.rows) * width;

		// C++20 ranges
		std::vector<int> ids(id_size);
		std::iota(ids.begin(), ids.end(), 0);

		auto const get_y = [&](int id) { return id / width; };
		auto const get_x = [&](int id) { return id % width; };

		auto const id_func = [&](int id)
		{
			auto const y = get_y(id);
			auto const x = get_x(id);
			auto src_ptr = src.ptr<pixel_t>(y);
			auto dst_ptr = dst.ptr<pixel_t>(y);

			func(src_ptr[x], dst_ptr[x]);
		};

		std::for_each(std::execution::par, ids.begin(), ids.end(), id_func);
	}
}