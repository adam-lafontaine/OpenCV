#pragma once

#include <iterator>

namespace utils
{
	class IndexRange
	{
	public:

		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = size_t;
			using difference_type = std::ptrdiff_t;
			using pointer = const size_t*;
			using reference = size_t;

			value_type m_val;

			explicit iterator() : m_val(0) {}

			explicit iterator(value_type val) : m_val(val) {}
			iterator& operator++() { ++m_val; return *this; }
			iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
			bool operator==(iterator other) const { return m_val == other.m_val; }
			bool operator!=(iterator other) const { return !(*this == other); }
			reference operator*() const { return m_val; }
		};

		iterator::value_type m_min = 0;
		iterator::value_type m_max = 1;

		IndexRange(iterator::value_type max) { m_max = max < 1 ? 1 : max; }

		iterator begin() { return iterator(0); }
		iterator end() { return iterator(m_max + 1); }
	};
}