#include "stdafx.h"
#include "Datum.h"

string Datum::toString(){
	if (name == ""){
		return std::to_string(value);
	}
	else if (value == UNINITIALIZED){
		return name;
	}
	else{
		return "Error: Datum never initialized";
	}
}