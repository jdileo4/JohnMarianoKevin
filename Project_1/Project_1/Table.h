#ifndef TABLE_H
#define TABLE_H

#include "Column.h"

using namespace std;

class Table{
private:
	int id;
	string name;
	vector<string> keys;

public:
	//TODO: make private
	vector<Column> columns;

	Table(string tableName, vector<string> tableKeys)
		: name(tableName), keys(tableKeys){}

	string getName() { return name; }
	int getID() { return id; }
	vector<Column> getColumns() { return columns; }
	vector<string> getKeys() { return keys; } 

	//returns index of named column from columns
	int findColumn(string columnName);
	void updateColumns(vector<Column> newColumns) { columns  = newColumns; }
	void updateKeys(vector<string> newKeys) { keys = newKeys; }
	void updateValue(int columnIndex, int rowIndex, void* newValue);
	void deleteValue(int columnIndex, int rowIndex);

	//returns 0 if success, 1 if rowData is too big to fit in table
	int addRow(vector<Datum> rowData);

	//type should be either "string" or "number"
	void addColumn(string name, string type);
	string getDatum(int column, int row);
};

#endif
