#include "pch.h"
#include "CppUnitTest.h"
#include "../2_lab/RBT.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:

		RBTree test;
		
		TEST_METHOD(find_and_insert_test)
		{
			test.insert('a');
			bool expected = true;
			bool inreal = false;
			if (test.find('a') != nullptr)
				inreal = true;
			Assert::AreEqual(inreal, expected);
		}

		TEST_METHOD(remove_test)
		{
			test.insert('k');
			test.insert('l');
			test.insert('m');
			test.remove('m');
			bool expected = false, inreal = true;
			if (test.find('m')->symb != 'm')
				inreal = false;
			Assert::AreEqual(inreal, expected);
		}

		TEST_METHOD(print_exception)
		{
			try {
				test.Print();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("The tree is empty", msg);
			}
		}

		TEST_METHOD(clear_exception)
		{
			try {
				test.clear();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("The tree is empty", msg);
			}
		}

		TEST_METHOD(remove_exception)
		{
			test.insert('t');
			test.insert('y');
			test.insert('a');

			try {
				test.remove('f');
			}
			catch (const char* msg)
			{
				Assert::AreEqual("There's no key like that", msg);
			}
		}

		TEST_METHOD(encoding_test)
		{
			string quote = "Hello";
			int i = 0;
			while (i < quote.length())
			{
				test.insert(quote[i]);
				i++;
			}

			test.encoding_tree();

			bool expected = true, inreal = false;

			if (test.find('H')->code.get_front()->get_data() == false && test.find('H')->code.get_front()->get_next()->get_data() == true)
				if (test.find('e')->code.get_front()->get_data() == true && test.find('e')->code.get_front()->get_next()->get_data() == false)
					if (test.find('l')->code.get_front()->get_data() == true && test.find('l')->code.get_front()->get_next()->get_data() == true)
						if (test.find('o')->code.get_front()->get_data() == false && test.find('o')->code.get_front()->get_next()->get_data() == false)
							inreal = true;
			Assert::AreEqual(inreal, expected);

		}

		TEST_METHOD(encoding_exception)
		{
			try {
				test.encoding_tree();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("The tree is empty", msg);
			}
		}


		TEST_METHOD(decoding_test)
		{
			string quote = "Hello";
			int i = 0;
			while (i < quote.length())
			{
				test.insert(quote[i]);
				i++;
			}

			test.encoding_tree();

			string todecode = "0110111100";;
			string decoded;
			test.decoding(todecode, decoded);
			Assert::AreEqual(decoded, quote);
		}
	};
}
