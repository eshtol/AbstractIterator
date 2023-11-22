#pragma once

#include <iterator>
#include <memory>


template <typename T> struct abstract_iterator
{
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;

	struct Concept
	{
		virtual Concept& operator ++() = 0;
		virtual value_type operator *() const = 0;
		virtual bool operator !=(const Concept&) const = 0;
		virtual std::unique_ptr<Concept> copy() const = 0;

		virtual ~Concept() = 0 {}
	};


	template <typename IterType> struct Model : Concept
	{
		using value_type = IterType::value_type;

		IterType m_iter;

		Model(const IterType& iter) : m_iter(iter) {}
		Model(IterType&& iter) : m_iter(std::move(iter)) {}

		Model& operator ++() override
		{
			++m_iter;
			return *this;
		}

		value_type operator *() const override
		{
			return *m_iter;
		}

		bool operator !=(const Concept& other) const override
		{
			return m_iter != static_cast<const Model&>(other).m_iter;
		}

		std::unique_ptr<Concept> copy() const override { return std::make_unique<Model>(m_iter); }

	};

	std::unique_ptr<Concept> m_ptr;

	abstract_iterator(const abstract_iterator& other) { m_ptr = other.m_ptr->copy(); }
	abstract_iterator(abstract_iterator&& other) = default;

	template <typename Iter> abstract_iterator(Iter&& iter) :
		m_ptr(std::make_unique<Model<Iter>>(std::forward<Iter>(iter)))
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

	value_type operator *() const
	{
		return **m_ptr.get();
	}

	bool operator !=(const abstract_iterator& other) const
	{
		return *m_ptr.get() != *other.m_ptr.get();
	}

};

template <typename Iter> abstract_iterator(Iter) -> abstract_iterator<typename Iter::value_type>;
