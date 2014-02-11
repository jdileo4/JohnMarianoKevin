#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(CONSTRUCTORS)
		{
			//TEST VARIABLES----
			vector<string>keys;
			keys.push_back("Name");
			keys.push_back("ID");

			//DATUM-------
			Datum datum1(100.00);
			Datum datum2("Fred");
			Datum datum3(100);

			//COLUMN------
			Column column1("Name","STRING");

			//TABLE-------
			Table table1("Patients",keys);
		}

		TEST_METHOD(FUNCTIONS)
		{
			vector<string>keys;

			vector<Datum> data;
			data.push_back(Datum("Mariano"));

			vector<Datum> data2;
			data2.push_back(Datum("Mariano"));
			data2.push_back(Datum("John"));
			data2.push_back(Datum("Kevin"));

			Column column1("Name","STRING");
			column1.insertInto(Datum("Mariano"));
			column1.insertInto(Datum("John"));
			column1.insertInto(Datum("Kevin"));

			Table table1("Patients",keys);
			table1.addColumn(column1);
			Assert::AreEqual(0,table1.insertInto(data));
			Assert::AreEqual(CANT_FIT_DATA,table1.insertInto(data2));
			//Assert::AreEqual(0,table1.insertInto(data2));

			//Renaming Unit test
			Assert::AreEqual(0,table1.renameColumn("Name","Names"));
			Assert::AreEqual(ATTRIBUTE_NOT_FOUND,table1.renameColumn("Name","Doctors"));
			Assert::AreNotEqual(0,table1.renameColumn("Name","Doctors"));
			Assert::AreNotEqual(ATTRIBUTE_NOT_FOUND,table1.renameColumn("Names","Name"));

			/*Selection Unit test, this first part is obsolete, just testing whether 
			//Contents for column were being copied
			
			string name = "Name";
			string type = "STRING";
			Column dummy("dummy","dummy");

			Column testColumn = table1.selectColumn("Name",dummy);
			vector<Datum> test(testColumn.getColumnDatum());

			int size = testColumn.getColumnDatum().size();

			Assert::AreEqual(name,testColumn.getColumnName());
			Assert::AreEqual(type,testColumn.getColumnType());
			Assert::AreEqual(name,test[1].getName());
			Assert::AreEqual(4,size);
			*/
			//Still Selection Unit Test

			string name = "Name";
			string type = "STRING";
			Column dummy("dummy","dummy");
			Assert::AreEqual(0,table1.selectColumn("Name",dummy));
			Assert::AreEqual(ATTRIBUTE_NOT_FOUND,table1.selectColumn("Names",dummy));
			Assert::AreEqual(name,dummy.getColumnName());
			Assert::AreEqual(type,dummy.getColumnType());

		}

	};
}