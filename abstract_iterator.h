#pragma once

#include <iterator>
#include <memory>


template <typename T> class abstract_iterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;

	struct erasure_concept
	{
		virtual erasure_concept& operator ++() = 0;
		virtual reference operator *() const = 0;
		virtual bool operator !=(const erasure_concept&) const = 0;
		virtual std::unique_ptr<erasure_concept> copy() const = 0;

		virtual ~erasure_concept() = 0 {}
	};

	template <typename iter_type> class erasure_model : public erasure_concept
	{
//		static_assert(std::is_same_v<typename iter_type::value_type, value_type>);

//		using reference = iter_type::reference;

		iter_type m_iter;

	public:
		erasure_model(const iter_type& iter) : m_iter(iter) {}
		erasure_model(std::remove_reference_t<iter_type>&& iter) : m_iter(std::move(iter)) {}

		erasure_model& operator ++() override
		{
			++m_iter;
			return *this;
		}

		reference operator *() const override
		{
			return const_cast<reference>(*m_iter);
		}

		bool operator !=(const erasure_concept& other) const override
		{
			return m_iter != static_cast<const erasure_model&>(other).m_iter;
		}

		std::unique_ptr<erasure_concept> copy() const override { return std::make_unique<erasure_model>(m_iter); }

	};

	std::unique_ptr<erasure_concept> m_ptr;

public:
	abstract_iterator() = default;
	abstract_iterator(const abstract_iterator& other) { m_ptr = other.m_ptr->copy(); }
	abstract_iterator(abstract_iterator&& other) = default;

	template <typename iter_type> abstract_iterator(iter_type&& iter) :
		m_ptr(std::make_unique<erasure_model<iter_type>>(std::forward<iter_type>(iter)))
	{}


	abstract_iterator& operator =(const abstract_iterator& other)
	{
		m_ptr = other.m_ptr->copy();
		return *this;
	}

	abstract_iterator& operator ++()
	{
		++(*m_ptr.get());
		return *this;
	}

	abstract_iterator operator ++(int)
	{
		const auto old = *this;
		++*this;
		return old;
	}

	reference operator *() const
	{
		return **m_ptr.get();
	}

	bool operator !=(const abstract_iterator& other) const
	{
		return *m_ptr.get() != *other.m_ptr.get();
	}

	bool operator ==(const abstract_iterator& other) const
	{
		return !(*this != other);
	}

};

template <typename iter_type> abstract_iterator(iter_type) -> abstract_iterator<typename iter_type::value_type>;
