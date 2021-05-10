#include "pch.h"
#include "CppUnitTest.h"
#include "../bunny/list.h"
#include "../bunny/bunny.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace bunnyColonyUnitTester
{
	TEST_CLASS(iteratorUnitTester)
	{
		list<bunny> col;
	public:
		TEST_METHOD(iterator_increment)
		{
			bunny a;
			bunny b;
			col.push_front(a);
			col.push_front(b);
			auto it = col.begin();
			Assert::IsTrue(b == *it);
			++it;
			Assert::IsTrue(a == *it);
		}

		TEST_METHOD(iterator_increment_beyond_limits)
		{
			bunny a;
			col.push_front(a);
			auto it = col.begin();
			++it;
			++it;
			Assert::IsTrue(it == col.end());
		}

		TEST_METHOD(iterator_decrement)
		{
			bunny a;
			bunny b;
			col.push_front(a);
			col.push_front(b);
			auto it = col.begin();
			++it;
			--it;
			Assert::IsTrue(it == col.begin());
		}

		TEST_METHOD(iterator_decrement_beyond_limits)
		{
			bunny a;
			col.push_front(a);
			auto it = col.begin();
			--it;
			Assert::IsTrue(it == col.end()); // col.end() returns nullptr
		}

		TEST_METHOD(add_iterator)
		{
			bunny a, b, c, d;
			col.push_front(a);
			col.push_front(b);
			col.push_front(c);
			col.push_front(d);
			auto it = col.begin();
			it = it + 2;
			Assert::IsTrue(*it == col.at(2));
			it = it + 1;
			Assert::IsTrue(*it == col.at(3));
		}

		TEST_METHOD(add_iterator_beyond_limits)
		{
			bunny a, b;
			col.push_front(a);
			col.push_front(b);
			auto it = col.begin();
			it = it + 10;
			Assert::IsTrue(it == col.end());
		}
	};

	TEST_CLASS(listUnitTester)
	{
		list<bunny> col;
	public:
		TEST_METHOD(push_front)
		{
			bunny a, b, c;
			col.push_front(a);
			col.push_front(b);
			col.push_front(c);
			Assert::IsTrue(c == col[0]);
			Assert::IsTrue(b == col[1]);
			Assert::IsTrue(a == col[2]);
		}

		TEST_METHOD(push_back)
		{
			bunny a, b, c;
			col.push_back(a);
			col.push_back(b);
			col.push_back(c);
			Assert::IsTrue(a == col[0]);
			Assert::IsTrue(b == col[1]);
			Assert::IsTrue(c == col[2]);
		}

		TEST_METHOD(insert)
		{
			bunny a, b, c;
			col.insert(a, 0);
			col.insert(b, 1);
			col.insert(c, 1);
			Assert::IsTrue(a == col[0]);
			Assert::IsTrue(c == col[1]);
			Assert::IsTrue(b == col[2]);
		}

		TEST_METHOD(insert_past_limits)
		{
			bunny a;
			col.insert(a, 10);
			Assert::IsTrue(col.begin() == col.end());
		}

		TEST_METHOD(insert_before_limits)
		{
			bunny a;
			col.insert(a, -2);
			Assert::IsTrue(col.begin() == col.end());
		}

		TEST_METHOD(insert_interator)
		{
			bunny a, b, c;
			col.push_front(a);
			col.push_front(b);
			auto it = col.begin();
			++it;
			col.insert(it, c);
			Assert::IsTrue(b == col[0]);
			Assert::IsTrue(c == col[1]);
			Assert::IsTrue(a == col[2]);
		}

		TEST_METHOD(erase_iterator)
		{
			bunny a, b, c;
			col.push_front(a);
			col.push_front(b);
			col.push_front(c);
			auto it = col.begin();
			++it;
			col.erase(it);
			Assert::IsTrue(c == col[0]);
			Assert::IsTrue(a == col[1]);
		}

		TEST_METHOD(search)
		{
			bunny a, b, c;
			col.push_front(a);
			col.push_front(b);
			col.push_front(c);
			Assert::IsTrue(col.search(a));
			Assert::IsTrue(col.search(a));
			Assert::IsTrue(col.search(a));
			col.remove(a);
			Assert::IsFalse(col.search(a));
		}
	};
}
