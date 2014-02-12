#include "stdafx.h"
#include "CppUnitTest.h"
#include "Engine.h"
#include <vector>
#include "CppUnitTestAssert.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(createEntityTable){
			Engine engine = Engine();
			
			vector<string> twoKeyColumns;
			twoKeyColumns.push_back("one");
			twoKeyColumns.push_back("two");

			engine.createEntityTable("table1", twoKeyColumns);
			Assert::AreEqual(engine.entityTables[0].getKeys().at(0).c_str(), "one");
			Assert::AreEqual(engine.entityTables[0].getKeys().at(1).c_str(), "two");
			Assert::AreEqual(engine.entityTables[0].getName().c_str(), "table1");

			Assert::AreEqual((int)engine.entityTables[0].getKeys().size(), 2);
		}

		//depends on success of createEntityTable test
		TEST_METHOD(addAttribute){
			Engine engine = Engine();
			//make table to test
			vector<string> twoKeyColumns;
			twoKeyColumns.push_back("one");
			twoKeyColumns.push_back("two");

			engine.createEntityTable("table1", twoKeyColumns);
			
			engine.addAttribute("table1", "one", "string");
			engine.addAttribute("table1", "two", "string");
			engine.addAttribute("table1", "threeValue", "number");


			Assert::AreEqual(engine.entityTables.at(0).getColumns().at(0).getName().c_str(), "one");
			Assert::AreEqual(engine.entityTables.at(0).getColumns().at(1).getName().c_str(), "two");
			Assert::AreEqual(engine.entityTables.at(0).getColumns().at(2).getName().c_str(), "threeValue");

			Assert::AreEqual(engine.entityTables.at(0).getColumns().at(0).getType().c_str(), "string");
			Assert::AreEqual(engine.entityTables.at(0).getColumns().at(1).getType().c_str(), "string");
			Assert::AreEqual(engine.entityTables.at(0).getColumns().at(2).getType().c_str(), "number");
		}

		//depends on success of above 2 tests
		TEST_METHOD(createRelationTable){
			Engine engine = Engine();
			//make table1 to test
			vector<string> twoKeyColumns;
			twoKeyColumns.push_back("one");
			twoKeyColumns.push_back("two");

			engine.createEntityTable("table1", twoKeyColumns);
			
			engine.addAttribute("table1", "one", "string");
			engine.addAttribute("table1", "two", "string");
			engine.addAttribute("table1", "threeValue", "number");


			//make table2 to test
			vector<string> twoKeyColumns2;
			twoKeyColumns2.push_back("ten");
			twoKeyColumns2.push_back("thirty");

			engine.createEntityTable("table2", twoKeyColumns2);
			
			engine.addAttribute("table2", "ten", "string");
			engine.addAttribute("table2", "twenty", "string");
			engine.addAttribute("table2", "thirtyValue", "number");

			//create relation table
			engine.createRelationTable("Relation Table 1", "table1", "table2");
			
			//test table keys (should be same as table attributes)
			vector<string> keyColumnsConcatenated(twoKeyColumns);
			keyColumnsConcatenated.push_back(twoKeyColumns2.at(0));
			keyColumnsConcatenated.push_back(twoKeyColumns2.at(1));
			Assert::AreEqual(4, (int)keyColumnsConcatenated.size()); //make sure keyc.c. is correct
			Assert::AreEqual(engine.relationTables.at(0).getKeys().size(), 
				keyColumnsConcatenated.size());
			Assert::AreEqual(engine.relationTables.at(0).getKeys().at(2), 
				keyColumnsConcatenated.at(2));

			//test table column names / attributes
			//columns should be {one, two, ten, twenty}
			Assert::AreEqual(engine.relationTables.at(0).
				getColumns().at(0).
				getName().c_str(), 
				"one");
			Assert::AreEqual(engine.relationTables.at(0).
				getColumns().at(1).
				getName().c_str(), 
				"two");
			Assert::AreEqual(engine.relationTables.at(0).
				getColumns().at(2).
				getName().c_str(), 
				"ten");
			Assert::AreEqual(engine.relationTables.at(0).
				getColumns().at(3).
				getName().c_str(), 
				"thirty");
		}

		TEST_METHOD(insertInto){
			Engine engine = Engine();
			//make table to test
			vector<string> twoKeyColumns;
			twoKeyColumns.push_back("one");
			twoKeyColumns.push_back("two");

			engine.createEntityTable("table1", twoKeyColumns);
			
			engine.addAttribute("table1", "one", "string");
			engine.addAttribute("table1", "two", "string");
			engine.addAttribute("table1", "threeValue", "number");


			//make rows to insert into table (string, string, number)
			vector<Datum> rowData1;
			rowData1.push_back(Datum("column 0, row 0"));
			rowData1.push_back(Datum("column 1, row 0"));
			rowData1.push_back(Datum(20));

			vector<Datum> rowData2;
			rowData2.push_back(Datum("column 0, row 1"));
			rowData2.push_back(Datum("column 1, row 1"));
			rowData2.push_back(Datum(21));

			engine.insertInto("table1", rowData1);
			engine.insertInto("table1", rowData2);
			
			Assert::AreEqual(engine.entityTables.at(0).
				getColumns().at(0).
				getData().at(0).
				toString().c_str(), 
				"column 0, row 0");
			//depends on working Datum::toString()
			Assert::AreEqual(engine.entityTables.at(0).
				getColumns().at(2).
				getData().at(0).
				toString().c_str(), 
				"20");
			Assert::AreEqual(engine.entityTables.at(0).
				getColumns().at(1).
				getData().at(1).
				toString().c_str(), 
				"column 1, row 1");

		}





		//TEST_METHOD(CONSTRUCTORS)
		//{
		//	//TEST VARIABLES----
		//	vector<string>keys;
		//	keys.push_back("Name");
		//	keys.push_back("ID");

		//	//DATUM-------
		//	Datum datum1(100.00);
		//	Datum datum2("Fred");
		//	Datum datum3(100);

		//	//COLUMN------
		//	Column column1("Name","STRING");

		//	//TABLE-------
		//	Table table1("Patients",keys);
		//}

		//TEST_METHOD(FUNCTIONS)
		//{
		//	vector<string>keys;

		//	vector<Datum> data;
		//	data.push_back(Datum("Mariano"));

		//	vector<Datum> data2;
		//	data2.push_back(Datum("Mariano"));
		//	data2.push_back(Datum("John"));
		//	data2.push_back(Datum("Kevin"));

		//	Column column1("Name","STRING");
		//	////column1.insertInto(Datum("Mariano"));
		//	////column1.insertInto(Datum("John"));
		//	////column1.insertInto(Datum("Kevin"));

		//	Table table1("Patients",keys);
		//	table1.addColumn(column1);
		//	////Assert::AreEqual(0,table1.insertInto(data));
		//	////Assert::AreEqual(CANT_FIT_DATA,table1.insertInto(data2));
		//	//Assert::AreEqual(0,table1.insertInto(data2));

		//	//Renaming Unit test
		//	////Assert::AreEqual(0,table1.renameColumn("Name","Names"));
		//	////Assert::AreEqual(ATTRIBUTE_NOT_FOUND,table1.renameColumn("Name","Doctors"));
		//	////Assert::AreNotEqual(0,table1.renameColumn("Name","Doctors"));
		//	////Assert::AreNotEqual(ATTRIBUTE_NOT_FOUND,table1.renameColumn("Names","Name"));

		//	/*Selection Unit test, this first part is obsolete, just testing whether 
		//	//Contents for column were being copied
		//	
		//	string name = "Name";
		//	string type = "STRING";
		//	Column dummy("dummy","dummy");

		//	Column testColumn = table1.selectColumn("Name",dummy);
		//	vector<Datum> test(testColumn.getColumnDatum());

		//	int size = testColumn.getColumnDatum().size();

		//	Assert::AreEqual(name,testColumn.getColumnName());
		//	Assert::AreEqual(type,testColumn.getColumnType());
		//	Assert::AreEqual(name,test[1].getName());
		//	Assert::AreEqual(4,size);
		//	*/
		//	//Still Selection Unit Test

		//	string name = "Name";
		//	string type = "STRING";
		//	Column dummy("dummy","dummy");
		//	////Assert::AreEqual(0,table1.selectColumn("Name",dummy));
		//	////Assert::AreEqual(ATTRIBUTE_NOT_FOUND,table1.selectColumn("Names",dummy));
		//	Assert::AreEqual(name,dummy.getColumnName());
		//	Assert::AreEqual(type,dummy.getColumnType());

		//}

	};
}