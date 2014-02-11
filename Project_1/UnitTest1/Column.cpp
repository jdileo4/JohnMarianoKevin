#include "stdafx.h"

Column& Column::operator=( Column column){

	columnName = column.getColumnName();
	columnType = column.getColumnType();
	
	Data = column.getColumnDatum();

	return *this;
}