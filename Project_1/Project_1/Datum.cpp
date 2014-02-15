#include "stdafx.h"
#include "Datum.h"

string Datum::toString(){
	if (name != ""){
		return name;
	}
	else if (value != UNINITIALIZED){
		return std::to_string(value);
	}
	else{
		return "NO VALUE";
	}
}