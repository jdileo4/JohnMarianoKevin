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
	void deleteFrom(string tableName, string att, string cond, int condVal);

	Table selection(Table initTable, string seleTable, string att, string cond, int condVal);
	Table projection(Table initTable, string projTable, string att);
	Table rename(Table initTable, string reTable, vector<string> newAtt);

	Table setUnion(string unionName, Table table1, Table table2);
	Table setDifference(string diffName, Table table1, Table table2);
	Table crossProduct(string crossName, Table table1, Table table2);
	Table naturalJoin(string joinName, Table table1, Table table2);

	void removeTable(string tableName);
	
//private:
	vector<Table> entityTables;
	vector<Table> relationTables;
private:
	//helper, credit to stackoverflow user
	bool isInteger(const string & s);
};

#endif