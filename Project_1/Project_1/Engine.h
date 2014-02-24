#ifndef ENGINE_H
#define ENGINE_H

#include "Table.h"
#include <vector>

using namespace std;

class Engine{

public:
	int FindTable(string tname);

	//only create an empty table with name and keyCol.  Parser
	//will create table then add addributes.
	void createEntityTable(string name, vector<string> keyCol);

	//type should be either "string" or "number"
	void addAttribute(string tableName, string colName, string type);

	//creates table and adds attributes automatically, unlike createEntityTable
	void createRelationTable(string name, string table1, string table2);

	//add a row to a particular table.  Parser assembles rowData to pass.
	int insertInto(string tableName, vector<Datum> rowData);


	int updateEntity(string tableName, string att, void* newVal, string cond, void* condVal);
	int deleteFrom(string tableName, string att, string cond, void* condVal);

	Table selection(string initTable, string seleTable, string att, string cond, void* condVal);
	Table projection(string initTable, string projTable, vector<string> att);
	Table rename(string initTable, string reTable, vector<string> att);

	Table setUnion(string unionName, Table table1, Table table2);
	Table setDifference(string diffName, Table table1, Table table2);
	Table crossProduct(string crossName, string table1, string table2);
	Table naturalJoin(string joinName, string table1, string table2);

	void removeTable(string tableName);
	
//private:
	vector<Table> entityTables;
	vector<Table> relationTables;
private:
	//helper, credit to stackoverflow user
	bool isInteger(const string & s);
};

#endif