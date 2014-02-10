#ifndef ENGINE_H
#define ENGINE_H

#include "Table.h"
#include <vector>

using namespace std;

class Engine{
private:
	vector<Table> entityTables;
	vector<Table> relationTables;

public:

	void createEntityTable(string name, vector<string> keyCol);
	void createRelationTable(string name, string table1, string table2);

	void addAttribute(string tableName, string colName, string type);
	void addEntity(string tableName, vector<Datum> rowData);

	void removeTable(string tableName);
};

#endif