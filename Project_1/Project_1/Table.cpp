#include "stdafx.h"
#include "Table.h"
#include <vector>

using namespace std;

int Table::addRow(vector<Datum> data){
	int errorCode = -1;
	if (data.size() == columns.size()){
		for(int i = 0; i < columns.size(); i++){
			columns[i].addData(data[i]);
		}
		errorCode = 0;
	}
	else{
		errorCode = 1;
	}
	return errorCode;
}

//type should be either "string" or "number"
void Table::addColumn(string name, string type){
	Column newCol = Column(name, type);

	columns.push_back(newCol);
}

string Table::getDatum(int column, int row){
	return columns.at(column).getData().at(row).toString();
}

void Table::updateValue(int columnIndex, int rowIndex, void* newValue){
	columns[columnIndex].updateData(
		Datum(newValue, columns[columnIndex].getType()), rowIndex);
	//TODO make newValue void pointer
}

void Table::deleteValue(int columnIndex, int rowIndex){
	columns[columnIndex].updateData(Datum(), rowIndex);
}

int Table::findColumn(string columnName){
	int index = -21;
	for(int i = 0; i < columns.size(); i++ ){
		if( columns[i].getName() == columnName ){
			index = i;
			return index;
		}
	}
	return index;
}