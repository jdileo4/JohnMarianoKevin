#include "stdafx.h"
#include "CppUnitTest.h"

#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(CONSTRUCTORS)
		{
			vector<string>keys;
			keys.push_back("Name");
			keys.push_back("ID");

			Datum datum1(100.00);
			Datum datum2("Fred");
			Datum datum3(100);

			Column column1("Name","STRING");

			Table table1("Patients",keys);
		}

		TEST_METHOD(FUNCTIONS)
		{
			vector<string>keys;

			vector<Datum> data;
			data.push_back(Datum("Mariano"));

			vector<Datum> data2;
			data2.push_back(Datum("Mariano"));
			data2.push_back(Datum("21"));

			Column column1("Name","STRING");

			Table table1("Patients",keys);
			table1.addColumn(column1);
			Assert::AreEqual(0,table1.insertInto(data));
			Assert::AreEqual(CANT_FIT_DATA,table1.insertInto(data2));
			//Assert::AreEqual(0,table1.insertInto(data2));

		}

	};
}