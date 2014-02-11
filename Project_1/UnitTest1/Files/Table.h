#ifndef TABLE_H
#define TABLE_H

#include "Column.h"

#define CANT_FIT_DATA	-1
#define ATTRIBUTE_NOT_FOUND	-2

class Table{

public:

	Table(string tName, vector<string> tKeys)
		:tableName(tName){}
	
	void addRow(vector<Datum> rowData);
	void addColumn(Column column) { tableColumn.push_back(column); }
	
	int insertInto(vector<Datum> data);
	int renameColumn(string oldname, string newname);

private:

	int tableID;
	string tableName;

	vector<string> keyColumns;
	vector<Column> tableColumn;
};

#endif
