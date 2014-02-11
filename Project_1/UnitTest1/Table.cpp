#include "stdafx.h"


int Table::findColumnIndex(string cname){

	int index = -2;

	for( int i = 0; i < tableColumn.size(); i++){

		if( tableColumn[i].getColumnName() == cname ){

			index = i;
			return index;
		}

	}

	return index;
}
int Table::insertInto(vector<Datum> data){

	//Check that the vector of datum equals the number of columns

	if( data.size() != tableColumn.size() ){

		return CANT_FIT_DATA;
	}

	for(int i = 0; i < tableColumn.size(); i++ ){

		tableColumn[i].insertInto(data[i]);

	}

	return 0;
}
int Table::renameColumn(string oldname, string newname){

	int cIndex = findColumnIndex(oldname);

	if( cIndex == ATTRIBUTE_NOT_FOUND ){

		return ATTRIBUTE_NOT_FOUND;
	}

	tableColumn[cIndex].setColumnName(newname);
	
	return 0;
}
int Table::selectColumn(string cname, Column& rcolumn){

	int cIndex = findColumnIndex(cname);

	if( cIndex == ATTRIBUTE_NOT_FOUND ){

		return ATTRIBUTE_NOT_FOUND;
	}

	rcolumn = tableColumn[cIndex];

	return 0;

}