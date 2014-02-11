#ifndef ENGINE_H
#define ENGINE_H

#include "Table.h"
#include <vector>

using namespace std;

class Engine{

public:

	//only create an empty table with name and keyCol.  Parser
	//will create table then add addributes.
	void createEntityTable(string name, vector<string> keyCol);

	//type should be either "string" or "number"
	void addAttribute(string tableName, string colName, string type);

	//creates table and adds attributes automatically, unlike createEntityTable
	void createRelationTable(string name, string table1, string table2);

	//add a row to a particular table.  Parser assembles rowData to pass.
	void addEntity(string tableName, vector<Datum> rowData);

	//TODO make condVal and newVal void pointers and dereference them where
	//necessary in the implementation
	void updateEntity(string tableName, string att, int newVal, string cond, int condVal);
	void deleteFrom(string tableName, string att, string cond, int condVal);

	Column selection();

	void removeTable(string tableName);
	
//private:
	vector<Table> entityTables;
	vector<Table> relationTables;
private:
	bool isInteger(const std::string & s);
};

#endif