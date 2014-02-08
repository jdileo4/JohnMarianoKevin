#include "stdafx.h"

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