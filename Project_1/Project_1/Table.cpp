#include "stdafx.h"
#include "Table.h"
#include <vector>

using namespace std;

void Table::addRow(vector<Datum> data){
	for(int i = 0; i < columns.size(); i++){
		columns[i].addData(data[i]);
	}
}

//type should be either "string" or "number"
void Table::addColumn(string name, string type){
	Column* newCol = new Column(name, type);

	columns.push_back(*newCol);
	keys.push_back(name);
}