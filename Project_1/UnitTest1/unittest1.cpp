#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
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

			int arg1 = 5;
			int arg2 = 21;
			int* arg1ptr = &arg1;
			int* arg2ptr = &arg2;
			engine.updateEntity("table1", "threeValue", (void*)arg1ptr, "<", (void*)arg2ptr);
			//should make threevalue {5, 21}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 0).c_str(), "5");
			
			arg1 = 1;
			arg2 = 5;
			arg1ptr = &arg1;
			arg2ptr = &arg2;
			engine.updateEntity("table1", "threeValue", (void*)arg1ptr, "<=", (void*)arg2ptr);
			//should make {1, 21}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 0).c_str(), "1");
			
			arg1 = 10;
			arg2 = 21;
			arg1ptr = &arg1;
			arg2ptr = &arg2;
			engine.updateEntity("table1", "threeValue", (void*)arg1ptr, "==", (void*)arg2ptr);
			//should make {1, 10}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 1).c_str(), "10");
			
			arg1 = 5;
			arg2 = 10;
			arg1ptr = &arg1;
			arg2ptr = &arg2;
			engine.updateEntity("table1", "threeValue", (void*)arg1ptr, ">=", (void*)arg2ptr);
			//should make {1, 5}
			Assert::AreEqual(engine.entityTables.at(0).getDatum(2, 1).c_str(), "5");
			
			arg1 = 30;
			arg2 = 0;
			arg1ptr = &arg1;
			arg2ptr = &arg2;
			engine.updateEntity("table1", "threeValue", (void*)arg1ptr, ">", (void*)arg2ptr);
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

			string arg1 = "column 1, row 1";
			string* arg1ptr = &arg1;

			engine.deleteFrom("table1", "two", "==", (void*)arg1ptr);

			Assert::AreEqual(engine.entityTables.at(0).
				getColumns().at(1).
				getData().at(1).
				toString().c_str(), 
				"NO VALUE");
		}

		TEST_METHOD(selection){
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

			  vector<Datum> rowData3;
			  rowData3.push_back(Datum("column 0, row 2"));
			  rowData3.push_back(Datum("column 1, row 2"));
			  rowData3.push_back(Datum(22));

			  vector<Datum> rowData4;
			  rowData4.push_back(Datum("column 0, row 3"));
			  rowData4.push_back(Datum("column 1, row 3"));
			  rowData4.push_back(Datum(23));

			  engine.insertInto("table1", rowData1);
			  engine.insertInto("table1", rowData2);
			  engine.insertInto("table1", rowData3);
			  engine.insertInto("table1", rowData4);
      
			  int arg1 = 22;
			  int* arg1Ptr = &arg1;

			  engine.entityTables.push_back(engine.selection("table1", "table2", "threeValue", "<=", (void*)arg1Ptr));

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(0).
				getData().at(0).
				toString().c_str(), 
				"column 0, row 0");

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(1).
				getData().at(0).
				toString().c_str(), 
				"column 1, row 0");

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(2).
				getData().at(0).
				toString().c_str(), 
				"20");

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(0).
				getData().at(1).
				toString().c_str(), 
				"column 0, row 1");

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(1).
				getData().at(1).
				toString().c_str(), 
				"column 1, row 1");

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(2).
				getData().at(1).
				toString().c_str(), 
				"21");
      	}

		TEST_METHOD(projection){
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

			  vector<Datum> rowData3;
			  rowData3.push_back(Datum("column 0, row 2"));
			  rowData3.push_back(Datum("column 1, row 2"));
			  rowData3.push_back(Datum(22));

			  vector<Datum> rowData4;
			  rowData4.push_back(Datum("column 0, row 3"));
			  rowData4.push_back(Datum("column 1, row 3"));
			  rowData4.push_back(Datum(23));

			  engine.insertInto("table1", rowData1);
			  engine.insertInto("table1", rowData2);
			  engine.insertInto("table1", rowData3);
			  engine.insertInto("table1", rowData4);

			  vector<string> attributes;
			  attributes.push_back("one");
			  attributes.push_back("threeValue");

			  engine.entityTables.push_back(engine.projection("table1", "table2", attributes));

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(0).
				getData().at(0).
				toString().c_str(), 
				"column 0, row 0");		

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(0).
				getData().at(1).
				toString().c_str(), 
				"column 0, row 1");				 

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(0).
				getData().at(2).
				toString().c_str(), 
				"column 0, row 2");	
			  			  
			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(0).
				getData().at(3).
				toString().c_str(), 
				"column 0, row 3");	

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(1).
				getData().at(0).
				toString().c_str(), 
				"20");		

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(1).
				getData().at(1).
				toString().c_str(), 
				"21");				 

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(1).
				getData().at(2).
				toString().c_str(), 
				"22");	
			  			  
			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(1).
				getData().at(3).
				toString().c_str(), 
				"23");	
		}

		TEST_METHOD(rename){
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

			  vector<string> attributes;
			  attributes.push_back("one2");
			  attributes.push_back("two2");
			  attributes.push_back("threeValue2");

			  engine.entityTables.push_back(engine.rename("table1", "table2", attributes));

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(0).
				getName().c_str(), 
				"one2");	

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(1).
				getName().c_str(), 
				"two2");

			  Assert::AreEqual(engine.entityTables.at(1).
				getColumns().at(2).
				getName().c_str(), 
				"threeValue2");
		}

		TEST_METHOD(setUnion){
			  Engine engine = Engine();
			  //make table to test
			  vector<string> twoKeyColumns;
			  twoKeyColumns.push_back("one");
			  twoKeyColumns.push_back("two");

			  engine.createEntityTable("table1", twoKeyColumns);
			  engine.createEntityTable("table2", twoKeyColumns);
      
			  engine.addAttribute("table1", "one", "string");
			  engine.addAttribute("table1", "twoValue", "number");

			  engine.addAttribute("table2", "one", "string");
			  engine.addAttribute("table2", "twoValue", "number");
    
			  //make rows to insert into table (string, string, number)
			  vector<Datum> rowData1;
			  rowData1.push_back(Datum("A"));
			  rowData1.push_back(Datum(0));

			  vector<Datum> rowData2;
			  rowData2.push_back(Datum("B"));
			  rowData2.push_back(Datum(1));

			  vector<Datum> tempRowData;
			  tempRowData.push_back(Datum("B"));
			  tempRowData.push_back(Datum(0));

			  vector<Datum> rowData3;
			  rowData3.push_back(Datum("C"));
			  rowData3.push_back(Datum(2));

			  vector<Datum> rowData4;
			  rowData4.push_back(Datum("D"));
			  rowData4.push_back(Datum(3));

			  engine.insertInto("table1", rowData1);
			  engine.insertInto("table1", rowData2);
			  engine.insertInto("table1", rowData3);

			  engine.insertInto("table2", tempRowData);
			  engine.insertInto("table2", rowData3);
			  engine.insertInto("table2", rowData4);

			  engine.entityTables.push_back(engine.setUnion("table3", "table1", "table2"));

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(0).getData().at(0).toString().c_str(),"A");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(1).getData().at(0).toString().c_str(),"0");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(0).getData().at(1).toString().c_str(),"B");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(1).getData().at(1).toString().c_str(),"1");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(0).getData().at(2).toString().c_str(),"C");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(1).getData().at(2).toString().c_str(),"2");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(0).getData().at(3).toString().c_str(),"B");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(1).getData().at(3).toString().c_str(),"0");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(0).getData().at(4).toString().c_str(),"D");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(1).getData().at(4).toString().c_str(),"3");

		}

		TEST_METHOD(setDifference){
			  Engine engine = Engine();
			  //make table to test
			  vector<string> twoKeyColumns;
			  twoKeyColumns.push_back("one");
			  twoKeyColumns.push_back("two");

			  engine.createEntityTable("table1", twoKeyColumns);
			  engine.createEntityTable("table2", twoKeyColumns);
      
			  engine.addAttribute("table1", "one", "string");
			  engine.addAttribute("table1", "twoValue", "number");

			  engine.addAttribute("table2", "one", "string");
			  engine.addAttribute("table2", "twoValue", "number");
    
			  //make rows to insert into table (string, string, number)
			  vector<Datum> rowData1;
			  rowData1.push_back(Datum("A"));
			  rowData1.push_back(Datum(0));

			  vector<Datum> rowData2;
			  rowData2.push_back(Datum("B"));
			  rowData2.push_back(Datum(1));

			  vector<Datum> tempRowData;
			  tempRowData.push_back(Datum("B"));
			  tempRowData.push_back(Datum(0));

			  vector<Datum> rowData3;
			  rowData3.push_back(Datum("C"));
			  rowData3.push_back(Datum(2));

			  vector<Datum> rowData4;
			  rowData4.push_back(Datum("D"));
			  rowData4.push_back(Datum(3));

			  engine.insertInto("table1", rowData1);
			  engine.insertInto("table1", rowData2);
			  engine.insertInto("table1", rowData3);

			  engine.insertInto("table2", tempRowData);
			  engine.insertInto("table2", rowData3);
			  engine.insertInto("table2", rowData4);

			  engine.entityTables.push_back(engine.setDifference("table3", "table1", "table2"));

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(0).getData().at(0).toString().c_str(),"A");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(1).getData().at(0).toString().c_str(),"0");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(0).getData().at(1).toString().c_str(),"B");

			  Assert::AreEqual(engine.entityTables.at(2).
								getColumns().at(1).getData().at(1).toString().c_str(),"1");

		}

		TEST_METHOD(crossProduct){
			  Engine engine = Engine();
			  //make table to test
			  vector<string> twoKeyColumns;
			  twoKeyColumns.push_back("one");
			  twoKeyColumns.push_back("two");

			  engine.createEntityTable("table1", twoKeyColumns);
			  engine.createEntityTable("table2", twoKeyColumns);
      
			  engine.addAttribute("table1", "one", "string");
			  engine.addAttribute("table1", "two", "string");
			  engine.addAttribute("table1", "threeValue", "number");

			  engine.addAttribute("table2", "one", "string");
			  engine.addAttribute("table2", "two", "string");
			  engine.addAttribute("table2", "threeValue", "number");
    
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

			  engine.insertInto("table2", rowData1);
			  engine.insertInto("table2", rowData2);

			  engine.entityTables.push_back(engine.crossProduct("table3", "table1", "table2"));

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(0).getData().at(0).
								toString().c_str(), "column 0, row 0");

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(3).getData().at(0).
								toString().c_str(), "column 0, row 0");

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(2).getData().at(1).
								toString().c_str(), "20");

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(5).getData().at(1).
								toString().c_str(), "21");

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(0).getData().at(2).
								toString().c_str(), "column 0, row 1");

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(3).getData().at(2).
								toString().c_str(), "column 0, row 0");

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(2).getData().at(3).
								toString().c_str(), "21");

			  Assert::AreEqual(engine.entityTables.at(2). getColumns().at(5).getData().at(3).
								toString().c_str(), "21");

		}

		TEST_METHOD(naturalJoin){

		}

/*		TEST_METHOD(OPEN_AND_WRITE)
		{
			Parser parser;

			vector<string> data;
			string message = "OPEN Random;";

			parser.Parse(message,data);

			//At the beginning, the file will not be there
			//If test doesn't pass, file is already there,so comment line
			parser.Operations(data);

			//Assert::AreEqual(1,parser.engine.getEntitySize());

			//Create the file and test for 0 now
			string message2 = "WRITE Random;";
			
			//Clear the vector
			data.clear();

			parser.Parse(message2,data);
			Assert::AreEqual(0,parser.Operations(data));

			//Now try to OPEN the file
			data.clear();

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));
		}
*/

		TEST_METHOD(CREATING_INSERTING_CLOSING)
		{
			Parser parser;

			//Create the table First, check if returning correctly
			vector<string> data;
			string message = "CREATE TABLE Food (FOOD STRING, PRICE INTERGER, TYPE STRING) PRIMARY KEY (FOOD,PRICE);";
			parser.Parse(message,data);
			Assert::AreEqual(0, parser.Operations(data));

			//Insert rows to the newly created table Food
			data.clear();
			message = "INSERT INTO Food VALUES FROM (SPAGUETTI, 20, Italian);";
			parser.Parse(message,data);
			Assert::AreEqual(0, parser.Operations(data));

			data.clear();
			message = "INSERT INTO Food VALUES FROM (Fish, 15, SeaFood);";
			parser.Parse(message,data);
			Assert::AreEqual(0, parser.Operations(data));
		
			//Assert::AreEqual(1,parser.engine.getEntitySize());
			data.clear();
			message = "WRITE Food;";
			parser.Parse(message,data);
			parser.Operations(data);

			//Finally, save table to a file
			data.clear();
			message = "CLOSE Food;";
			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

		}

/*		TEST_METHOD(QUERY_TYPE){

			Parser parser;

			//We first open a File containing a Table
			vector<string> data;
			string message = "OPEN Random;";

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			data.clear();
			message = "Favorite <- select (Food == SPAGUETTI) Random;";
			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			data.clear();
			message = "Favorite <- rename Food FOOD;";
			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			data.clear();
			message = "Favorite <- Random + Food;";
			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			data.clear();
			message = "Favorite <- Random - Food;";
			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			data.clear();
			message = "Favorite <- Random * Food;";
			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

		}

		TEST_METHOD(SHOW){

			//Get a table
			Parser parser;

			vector<string> data;
			string message = "OPEN Random;";

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			//Now show that table
			message = "SHOW Random;";

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

		}

		TEST_METHOD(UPDATE){

			//Get a table
			Parser parser;

			vector<string> data;
			string message = "OPEN Random;";

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			//Now show that table
			message = "UPDATE Random SET (Food = Bread) WHERE (Food = SPAGUETTI);";

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));


		}
		TEST_METHOD(DELETE){
			
			//Get a table
			Parser parser;

			vector<string> data;
			string message = "OPEN Random;";

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

			//Now show that table
			message = "DELETE FROM Random WHERE (FOOD == Spaguetti);";

			parser.Parse(message,data);
			Assert::AreEqual(0,parser.Operations(data));

		}
*/		
	};
}

