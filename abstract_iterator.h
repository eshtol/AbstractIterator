#pragma once

#include <iterator>
#include <memory>
#include <stdexcept>


template <typename T> class abstract_iterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;

private:
	struct erasure_concept
	{
		virtual erasure_concept& operator ++() = 0;

		virtual reference operator *() const = 0;
		
		virtual bool operator !=(const erasure_concept&) const = 0;
		virtual bool operator ==(const erasure_concept&) const = 0;

		virtual std::unique_ptr<erasure_concept> copy() const = 0;

		virtual bool is_default_sentinel() const = 0;

		virtual ~erasure_concept() = 0 {}
	};

	template <typename iter_type> class erasure_model : public erasure_concept
	{
		//using reference = iter_type::reference;	// not presented in std::ranges::elements_view
		using reference = decltype(*std::declval<iter_type>());

		static_assert(std::is_const_v<std::remove_reference_t<reference>> ? std::is_const_v<T> : true,
					  "Don't forget a const-qualifier");

		iter_type m_iter;

	public:
		erasure_model(const iter_type& iter) : m_iter(iter) {}
		erasure_model(iter_type&& iter) : m_iter(std::move(iter)) {}

		erasure_model& operator ++() override
		{
			++m_iter;
			return *this;
		}

		reference operator *() const override
		{
			return *m_iter;
		}

		bool operator !=(const erasure_concept& other) const override
		{
			return m_iter != static_cast<const erasure_model&>(other).m_iter;
		}

		bool operator ==(const erasure_concept& other) const override
		{
			return m_iter == static_cast<const erasure_model&>(other).m_iter;
		}

		std::unique_ptr<erasure_concept> copy() const override { return std::make_unique<erasure_model>(m_iter); }

		bool is_default_sentinel() const override { return false; }
	};
	
	template <typename underlying_iter_type> class erasure_model<std::counted_iterator<underlying_iter_type>> : public erasure_concept
	{
		using iter_type = std::counted_iterator<underlying_iter_type>;
		//using reference = iter_type::reference;	// not presented in std::ranges::elements_view
		using reference = decltype(*std::declval<iter_type>());

		static_assert(std::is_const_v<std::remove_reference_t<reference>> ? std::is_const_v<T> : true,
					  "Don't forget a const-qualifier");

		iter_type m_iter;

	public:
		erasure_model(const iter_type& iter) : m_iter(iter) {}
		erasure_model(iter_type&& iter) : m_iter(std::move(iter)) {}

		erasure_model& operator ++() override
		{
			++m_iter;
			return *this;
		}

		reference operator *() const override
		{
			return *m_iter;
		}

		bool operator !=(const erasure_concept& other) const override
		{
			if (other.is_default_sentinel()) return m_iter != std::default_sentinel;
			return m_iter != static_cast<const erasure_model&>(other).m_iter;
		}

		bool operator ==(const erasure_concept& other) const override
		{
			if (other.is_default_sentinel()) return m_iter == std::default_sentinel;
			return m_iter == static_cast<const erasure_model&>(other).m_iter;
		}

		std::unique_ptr<erasure_concept> copy() const override { return std::make_unique<erasure_model>(m_iter); }

		bool is_default_sentinel() const override { return false; }
	};
	
	template <> class erasure_model<std::default_sentinel_t> : public erasure_concept
	{
	public:
		erasure_model(const std::default_sentinel_t&) {}
		erasure_model(std::default_sentinel_t&&) {}

		erasure_model& operator ++() override
		{
			throw std::out_of_range("Can't increment default sentinel");
		}

		reference operator *() const override
		{
			throw std::out_of_range("Can't dereference default sentinel");
		}

		bool operator !=(const erasure_concept& other) const override
		{
			return !other.is_default_sentinel();
		}

		bool operator ==(const erasure_concept& other) const override
		{
			return other.is_default_sentinel();
		}

		std::unique_ptr<erasure_concept> copy() const override { return std::make_unique<erasure_model>(std::default_sentinel); }

		bool is_default_sentinel() const override { return true; }
	};

	std::unique_ptr<erasure_concept> m_ptr;

public:
	abstract_iterator() = default;
	abstract_iterator(const abstract_iterator& other) { m_ptr = other.m_ptr->copy(); }
	abstract_iterator(abstract_iterator&& other) = default;

	template <typename iter_type> abstract_iterator(iter_type&& iter)
		requires !std::same_as<abstract_iterator, std::remove_cvref_t<iter_type>> :
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
		return *m_ptr.get() == *other.m_ptr.get();
	}

};

template <typename iter_type> abstract_iterator(iter_type) -> abstract_iterator<typename iter_type::value_type>;
