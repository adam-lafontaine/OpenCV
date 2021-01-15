#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <array>
#include <vector>
#include <cstdint>
#include <limits>
#include <cassert>
#include <algorithm>

namespace utils
{
	template<typename In_t, typename Out_t>
	class TypeMap
	{
	private:

		using wide_t = long double;

		In_t m_in_min = std::numeric_limits<In_t>::min();
		In_t m_in_max = std::numeric_limits<In_t>::max();
		Out_t m_out_min = std::numeric_limits<Out_t>::min();
		Out_t m_out_max = std::numeric_limits<Out_t>::max();

	public:
		TypeMap() {}

		TypeMap(In_t const& in_min, In_t const& in_max, Out_t const& out_min, Out_t const& out_max)
		{
			m_in_min = in_min;
			m_in_max = in_max;
			m_out_min = out_min;
			m_out_max = out_max;
		}

		Out_t get(In_t const& val)
		{
			auto const min = static_cast<wide_t>(m_in_min);
			wide_t const ratio = (val - min) / (m_in_max - min);

			return m_out_min + static_cast<Out_t>((m_out_max - m_out_min) * ratio);
		}
	};


	//template<typename In_t, typename Out_t>
	//class TypeMapConst
	//{
	//private:
	//	constexpr In_t m_in_min = std::numeric_limits<In_t>::min();
	//	constexpr In_t m_in_max = std::numeric_limits<In_t>::max();
	//	constexpr Out_t m_out_min = std::numeric_limits<Out_t>::min();
	//	constexpr Out_t m_out_max = std::numeric_limits<Out_t>::max();
	//
	//public:
	//	TypeMapConst() {}
	//
	//	constexpr Out_t get(In_t const& val)
	//	{
	//		auto const ratio = static_cast<long double>(val - m_in_min) / (m_in_max - m_in_min);
	//
	//		return m_out_min + static_cast<Out_t>((m_out_max - m_out_min) * ratio);
	//	}
	//};


	double lookup_lerp(std::vector<double> const& from, std::vector<double> const& to, double from_val)
	{
		assert(from.size() == to.size());
		assert(std::is_sorted(from.begin(), from.end()));

		size_t high_id = 0;
		for (; high_id < from.size() && from[high_id] < from_val; ++high_id)
			;

		size_t low_id = high_id - 1;

		return to[low_id] + (to[high_id] - to[low_id]) * (from_val - from[low_id]) / (from[high_id] - from[low_id]);		
	}


	constexpr std::array<uint8_t, 256> make_reduced_lookup(uint8_t divisor)
	{
		std::array<uint8_t, 256> table = { 0 };

		for (size_t i = 0; i < 256; ++i)
			table[i] = divisor * (static_cast<uint8_t>(i) / divisor);

		return table;

	}


	cv::Mat make_reduced_lookup_mat(uint8_t divisor)
	{
		std::array<uint8_t, 256> table = { 0 };

		for (size_t i = 0; i < 256; ++i)
			table[i] = divisor * (static_cast<uint8_t>(i) / divisor);

		cv::Mat mat_lookup(1, 256, CV_8U);
		auto p = mat_lookup.ptr();
		for (int i = 0; i < 256; ++i)
			p[i] = table[i];

		return mat_lookup;
	}
}


