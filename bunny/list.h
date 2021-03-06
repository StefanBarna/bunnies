#ifndef LIST_H
#define LIST_H

#include <stdexcept>

template <typename T>
class list
{
	// list component [PRIVATE STRUCTURE]
	struct list_element
	{
		T m_data{};							// element data
		list_element* m_next{ nullptr };	// pointer to next element
		list_element* m_prev{ nullptr };	// pointer to previous element

		// base constructor
		list_element() = default;
	};

	list_element* m_head;					// pointer to the first element
	size_t m_cnt;							// keeps track of number of elements

public:
	// iterator [PUBLIC STRUCTURE]
	struct iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		list_element* m_current;			// current position of iterator

		// proceeds to the following list_element
		iterator& operator++()
		{
			if (this->m_current != nullptr)
				this->m_current = m_current->m_next;
			return *this;
		}

		// suffix incrementing operator
		iterator operator++(int)
		{
			iterator it = *this;
			++(*this);
			return it;
		}

		// regresses to the previous list_element
		iterator& operator--()
		{
			if (this->m_current->m_prev != nullptr)
				this->m_current = m_current->m_prev;
			else
				this->m_current = nullptr;
			return *this;
		}

		// suffex decrementing operator
		iterator operator--(int)
		{
			iterator it = *this;
			--(*this);
			return it;
		}

		// query checking if iterator pointers are equal
		bool operator==(const iterator& it) const
		{
			return this->m_current == it.m_current;
		}

		// query checking if iterator pointers are not equal
		bool operator!=(const iterator& it) const
		{
			return this->m_current != it.m_current;
		}

		// moves forward a parameter number of steps
		iterator operator+(size_t i) const
		{
			iterator it = *this;
			for (size_t idx = 0; idx < i && it.m_current != nullptr; idx++)
			{
				++it;
				if (it.m_current == nullptr)
					break;
			}
			return it;
		}

		// dereference
		T& operator*()
		{
			return this->m_current->m_data;
		}

		// -> operator
		T* operator->()
		{
			return &(this->m_current->m_data);
		}
	};

	// query returning an iterator to the first element of a list
	iterator begin() const
	{
		iterator it;
		it.m_current = this->m_head;
		return it;
	}

	// query returning an iterator to the last element of a list
	iterator end() const
	{
		iterator it;
		it.m_current = nullptr;
		return it;
	}

	// base constructor
	list()
	{
		this->m_cnt = 0;
		this->m_head = nullptr;
	}

	// destructor
	~list()
	{
		while (this->m_head)
		{
			auto next = this->m_head->m_next;
			delete this->m_head;
			this->m_head = next;
		}
		this->m_cnt = 0;
	}
	
	// copy constructor
	list(const list& li)
	{
		*this = li;
	}

	// copy assignment operator
	list& operator=(const list& li)
	{
		if (this != &li)
		{
			delete this;
			this->m_head = new list_element;
			auto thisIt = this->m_head;
			auto listIt = li.m_head;
			for (size_t i = 0; i < li.cnt; i++)
			{
				thisIt->m_data = listIt->m_data;
				if (listIt->m_next != nullptr)
				{
					thisIt->m_next = new list_element;
					thisIt->m_next->m_prev = thisIt;
					thisIt = thisIt->m_next;
					listIt = listIt->m_next;
				}
			}
			this->m_cnt = li.m_cnt;
		}
		return *this;
	}

	// query returning the size of the list
	size_t size() const
	{
		return this->m_cnt;
	}

	// adds an item to the front of the list
	void push_front(const T& val)
	{
		list_element* elem = new list_element;
		elem->m_data = val;
		if (this->m_head != nullptr)
			this->m_head->m_prev = elem;
		elem->m_next = this->m_head;
		this->m_head = elem;
		++this->m_cnt;
	}
	
	// adds an item at the end of the list
	void push_back(const T& val)
	{
		list_element* elem = new list_element;
		elem->m_data = val;
		if (this->m_head != nullptr)
		{
			list_element* it = this->m_head;
			while (it->m_next != nullptr)
				it = it->m_next;
			it->m_next = elem;
			elem->m_prev = it;
		}
		else
			this->m_head = elem;
		++this->m_cnt;
	}

	// adds an item at the given postiion [starts at 0]
	void insert(const T& val, size_t idx)
	{
		if (0 <= this->m_cnt && this->m_cnt >= idx)
		{
			list_element* elem = new list_element;
			elem->m_data = val;
			if (this->begin() != this->end())
			{
				auto it = this->begin();
				for (size_t i = 0; i < idx - 1; ++i, ++it);
				elem->m_next = it.m_current->m_next;
				elem->m_prev = it.m_current;
				if (idx != this->m_cnt)
					it.m_current->m_next->m_prev = elem;
				it.m_current->m_next = elem;
			}
			else
				this->m_head = elem;
			++this->m_cnt;
		}
	}

	// adds an item to the location before the iterator
	void insert(iterator& it, const T& val)
	{
		if (it.m_current == nullptr)
			return;
		if (it == this->begin())
			this->push_front(val);
		else
		{
			list_element* elem = new list_element;
			elem->m_data = val;
			it.m_current->m_prev->m_next = elem;
			elem->m_prev = it.m_current->m_prev;
			it.m_current->m_prev = elem;
			elem->m_next = it.m_current;
			this->m_cnt++;
		}
	}

	// erases the item at the location of the iterator
	void erase(iterator& it)
	{
		if (it == this->begin())								// if we delete the head of the list
		{
			if (this->m_cnt == 1)								// list contains only one element whilst deleting the head
				this->m_head = nullptr;
			else												// list contains more than one element whilst deleting the head
			{
				this->m_head = this->m_head->m_next;
				this->m_head->m_prev = nullptr;
			}
		}
		else if (it.m_current->m_next == nullptr)				// if we delete the tail of the list
			it.m_current->m_prev->m_next = nullptr;
		else													// general case; delete some element in the middle of the list
		{
			it.m_current->m_prev->m_next = it.m_current->m_next;
			it.m_current->m_next->m_prev = it.m_current->m_prev;
		}
		delete it.m_current;
		it.m_current = nullptr;
		--this->m_cnt;
	}

	// removes first element with a certain value from the list
	void remove(const T val)
	{
		for (auto it = this->begin(); it != this->end(); ++it)
		{
			if (*it == val)
			{
				this->erase(it);
				return;
			}
		}
	}

	// query returning if a value is in the list
	bool search(const T& val) const
	{
		for (auto it = this->begin(); it != this->end(); ++it)
		{
			if (*it == val)
				return true;
		}
		return false;
	}

	// query returning the element at a position [starts at 0]
	T& at(size_t idx) const
	{
		if (this->m_cnt < idx || idx < 0)
			throw std::invalid_argument("INVALID INDEX");
		auto it = this->begin();
		for (size_t i = 0; i < idx; ++it, ++i);
		return *it;
	}

	// [] operator overload
	T& operator[](size_t idx) const
	{
		return this->at(idx);
	}
};

#endif