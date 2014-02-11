#ifndef TABLE_H
#define TABLE_H

#include "Column.h"

using namespace std;

class Table{
private:
	int id;
	string name;

	vector<string> keys;
	vector<Column> columns;

public:
	Table(string tableName, vector<string> tableKeys)
		: name(tableName), keys(tableKeys){}

	string getName() { return name; }
	int getID() { return id; }
	vector<Column> getColumns() { return columns; }

	void addRow(vector<Datum> rowData);
	void addColumn(string name, string type);
};

#endif
