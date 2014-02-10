
#include "Datum.h"

string Datum::toString(){
	if (name == ""){
		return to_string(value);
	}
	else if (value = UNINITIALIZED){
		return name;
	}
	else{
		return "ERROR: Datum has two types of values (string and number)";
	}
}
