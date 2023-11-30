#pragma once

#include "abstract_iterator.h"


template <typename T> class abstract_range
{
	abstract_iterator<T> m_begin,
		                 m_end;

	std::size_t m_size;

public:
	template <typename container> abstract_range(container&& cont) :
		m_begin(std::begin(cont)),
		m_end(std::end(cont)),
		m_size(std::size(cont))
	{
		static_assert(std::is_const_v<std::remove_reference_t<container>> ? std::is_const_v<T> : true,
					  "Don't forget a const-qualifier");
	}

	const auto& begin() const { return m_begin; }
	const auto& end() const { return m_end; }

	auto& begin() { return m_begin; }
	auto& end() { return m_end; }

	auto size() const { return m_size; }
	auto empty() const { return size() == 0; }
};


template <typename container> abstract_range(container) -> abstract_range<typename container::value_type>;
