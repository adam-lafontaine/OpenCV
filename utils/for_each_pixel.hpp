#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <functional>
#include <execution>
#include <algorithm>
#include <vector>

namespace utils
{
	inline void for_each_pixel_seq(cv::Mat const& img, std::function<void(size_t x, size_t y)> const& func)
	{
		for (auto y = 0; y < img.rows; ++y)
		{			
			for (auto x = 0; x < img.cols; ++x)
			{
				func(x, y);
			}
		}
	}


	inline void for_each_pixel_seq(cv::Mat const& img, std::function<void(cv::Vec3b const&)> const& func)
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


	inline void for_each_pixel_seq(cv::Mat const& img, std::function<void(uchar)> const& func)
	{
		for (auto y = 0; y < img.rows; ++y)
		{
			auto ptr = img.ptr<uchar>(y);
			for (auto x = 0; x < img.cols; ++x)
			{
				func(ptr[x]);
			}
		}
	}


	inline void for_each_pixel_par(cv::Mat const& img, std::function<void(size_t x, size_t y)> const& func)
	{
		auto const get_y = [&](int range_val) { return range_val / img.cols; };
		auto const get_x = [&](int range_val) { return range_val % img.cols; };

		auto const range_func = [&](cv::Range const& range)
		{
			for (int r = range.start; r < range.end; r++)
			{
				func(get_x(r), get_y(r));
			}
		};

		cv::Range range(0, img.rows * img.cols);
		cv::parallel_for_(range, range_func);
	}


	inline void for_each_pixel_par(cv::Mat const& img, std::function<void(cv::Vec3b const&)> const& func)
	{
		auto const get_y = [&](int range_val) { return range_val / img.cols; };
		auto const get_x = [&](int range_val) { return range_val % img.cols; };

		auto const range_func = [&](cv::Range const& range)
		{
			for (int r = range.start; r < range.end; r++)
			{
				auto p = img.ptr<cv::Vec3b>(get_y(r))[get_x(r)];
				func(p);
			}
		};

		cv::Range range(0, img.rows * img.cols);
		cv::parallel_for_(range, range_func);
	}


	inline void for_each_pixel_par(cv::Mat const& img, std::function<void(uchar)> const& func)
	{
		auto const get_y = [&](int range_val) { return range_val / img.cols; };
		auto const get_x = [&](int range_val) { return range_val % img.cols; };

		auto const range_func = [&](cv::Range const& range)
		{
			for (int r = range.start; r < range.end; r++)
			{
				auto p = img.ptr<uchar>(get_y(r))[get_x(r)];
				func(p);
			}
		};

		cv::Range range(0, img.rows * img.cols);
		cv::parallel_for_(range, range_func);
	}


	inline void for_each_pixel_par_stl(cv::Mat const& img, std::function<void(size_t x, size_t y)> const& func)
	{
		size_t id_size = static_cast<size_t>(img.rows) * static_cast<size_t>(img.cols);

		std::vector<int> ids(id_size);
		std::iota(ids.begin(), ids.end(), 0);

		auto const get_y = [&](int id) { return id / img.cols; };
		auto const get_x = [&](int id) { return id % img.cols; };

		auto const id_func = [&](int id)
		{
			func(get_x(id), get_y(id));
		};

		std::for_each(std::execution::par, ids.begin(), ids.end(), id_func);
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

