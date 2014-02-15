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

		TEST_METHOD(removeTable){
			//
			Engine engine = Engine();
			
			vector<string> twoKeyColumns;
			twoKeyColumns.push_back("one");
			twoKeyColumns.push_back("two");

			engine.createEntityTable("table1", twoKeyColumns);
			Assert::AreEqual(1, (int)engine.entityTables.size());

			engine.removeTable("table1");
			Assert::AreEqual(0, (int)engine.entityTables.size());
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

		TEST_METHOD(updateEntity){
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

			//TODO
			//engine.updateEntity("table1", "one", "newValue", "==", "column 0, row 0");
			//Assert::AreEqual
			//engine.updateEntity("table1", "two", "newValue", "==", "inequivalent");
			//Assert::Arenotequal

			engine.updateEntity("table1", "threeValue", 5, "<", 21);
			//should make threevalue {5, 21}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 0).c_str(), "5");

			engine.updateEntity("table1", "threeValue", 1, "<=", 5);
			//should make {1, 21}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 0).c_str(), "1");

			engine.updateEntity("table1", "threeValue", 10, "==", 21);
			//should make {1, 10}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 1).c_str(), "10");

			engine.updateEntity("table1", "threeValue", 5, ">=", 10);
			//should make {1, 5}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 1).c_str(), "5");

			engine.updateEntity("table1", "threeValue", 30, ">", 0);
			//should make {30, 30}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 0).c_str(), "30");
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 1).c_str(), "30");
		}

		TEST_METHOD(deleteFrom){
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
				getColumns().at(1).
				getData().at(1).
				toString().c_str(), 
				"column 1, row 1");
			
			engine.deleteFrom("table1", "two", "==", "column 1, row 1");
			//assert "column 1, row 1" doesn't exist


		}
	};
}

