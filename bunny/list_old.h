#ifndef LIST_H
#define LIST_H

#include <sstream>             // for stringstream


template <typename T>
class list
{
	// element of a list
	struct list_element
	{
		T data{};                          // element data
		list_element* next{ nullptr };     // pointer to next element
		list_element* prev{ nullptr };     // pointer to the previous element

		list_element() = default;
	};

	list_element* head;					   // pointer to first element
	size_t cnt;							   // element counter

public:
	// forward iterator
	struct iterator
	{
		list_element* m_current;

		// goes to the next list element (prefix ++)
		iterator& operator++()
		{
			this->m_current = m_current->next;
			return *this;
		}

		// postfix ++
		iterator operator++(int)
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		// returns true if the pointers are equal
		bool operator==(const iterator& it) const
		{
			return this->m_current == it.m_current;
		}

		// returns true if the pointers are unequal
		bool operator!=(const iterator& it) const
		{
			return this->m_current != it.m_current;
		}

		// ...
		iterator operator+(size_t i) const
		{
			iterator it = *this;
			for (size_t idx = 0; idx < i && it.m_current != nullptr; idx++)
				++it;
			return it;
		}

		// de-referencing
		T& operator*()
		{
			return this->m_current->data;
		}
	};

	// Get an iterator to the first element of the list
	iterator begin()
	{
		iterator it;
		it.m_current = this->head;
		return it;
	}

	// Get an iterator to the elment after the last element of the list.
	iterator end()
	{
		iterator it;
		it.m_current = nullptr;
		return it;
	}

	// erases the item at the location of the iterator
	void erase(iterator& it)
	{
		delete it.m_current;
		it.m_current = nullptr;
	}

	// adds an item at the location before the iterator
	void insert(iterator& it, const T& value)
	{
		// General Case
		list_element* elemA = it.m_current->prev;
		list_element* elemB = it.m_current;
		list_element* elemNew = new list_element;
		elemNew->data = value;

		// insert "elemNew" between "elemA" and "elemB"
		elemA->next = elemNew;
		elemNew->prev = elemA;
		elemB->prev = elemNew;
		elemNew->next = elemB;
		this->cnt++;
	}

	// Create an empty list.
	list()
	{
		this->cnt = 0;
		this->head = nullptr;
	}
	
	// destructor
	~list()
	{
		while (this->head)
		{
			auto tmp = this->head->next;
			delete this->head;
			this->head = tmp;
		}
		this->cnt = 0;
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
			delete this->head;
			this->head = new list_element;
			auto newit = this->head;
			auto it = li.head;
			for (size_t i = 0; i < li.cnt; i++)
			{
				newit->data = it->data;
				if (it->next != nullptr)
				{
					newit->next = new list_element;
					newit = newit->next;
					it = it->next;
				}
				else
					newit->next = nullptr;
			}
			this->cnt = li.cnt;
		}
		return *this;
	}

	// Get the number of elements in the list
	size_t size() const
	{
		return this->cnt;
	}

	// Returns a string representation of the list.
	std::string toString()
	{
		std::stringstream ss;
		if (this->cnt == 0)
			ss << "[Empty]";
		else
		{
			auto it = this->head;
			while (it != nullptr)
			{
				ss << "[" << it->data << "]";
				it = it->next;
			}
		}
		return ss.str();
	}

	// adds an item at the front of the list
	void push_front(const T& value)
	{
		list_element* obj = new list_element;
		obj->data = value;
		if (this->head != nullptr)
			// list is not empty, add before the first element
			this->head->prev = obj;
		obj->next = this->head;
		obj->prev = nullptr;
		this->head = obj;
		++this->cnt;
	}

	// Add an element at the end of the list
	void push_back(const T& value)
	{
		list_element* obj = new list_element;
		obj->data = value;
		obj->next = nullptr;
		if (this->head != nullptr)
		{
			list<T>::iterator it = this->begin();
			for (size_t i = 1; i < (*this).size(); i++)
				++it;
			it.m_current->next = obj;
			this->cnt++;
		}
		else
		{
			this->head = obj;
			this->cnt++;
		}
	}

	// removes all elements with certain value from list
	void remove(const T value)
	{
		if (this->cnt == 0)
			return;
		if (this->cnt == 1)
		{
			if (this->head->data == value)
			{
				delete this->head;
				this->head = nullptr;
				--this->cnt;
			}
			return;
		}
		if (this->head->data == value)
		{
			auto temp = this->head->next;
			delete this->head;
			this->head = temp;
			--this->cnt;
		}
		else
		{
			auto it = this->head;
			while (it->next != nullptr)
			{
				if (it->next->data == value)
				{
					auto peek = it->next;
					it->next = peek->next;
					delete peek;
					--this->cnt;
					break;
				}
				else
					it = it->next;
			}
		}
	}

	// returns true if the value is in the list
	bool search(const T& value) const
	{
		list_element* temp = this->head;
		for (size_t i = 0; i < this->cnt; i++)
		{
			if (temp->data == value)
				return true;
			else
				temp = temp->next;
		}
		return false;
	}

	// adds an item at the given position
	void insert(const T& value, size_t idx)
	{
		if (this->begin != nullptr)
		{
			if (this->cnt < idx)
				return;

			list_element* obj = new list_element;
			obj->data = value;
			auto it = this->head;
			for (size_t i = 0; i < idx - 1; i++)
				it = it->next;
			obj->next = it->next;
			it->next = obj;
			this->cnt++;
		}
	}

	// returns the element at a certain position (starting at 1) TODO: Change to start at 0; 1 is unintuitive
	T& at(size_t idx) const
	{
		if (this->cnt < idx || idx < 0)
			throw std::invalid_argument("INVALID INDEX");
		auto it = this->head;
		for (size_t i = 0; i < idx; i++)
			it = it->next;
		return it->data;
	}
};

#endif