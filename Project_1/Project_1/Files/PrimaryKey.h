#ifndef PRIMARY_KEY_H
#define PRIMARY_KEY_H

#include <vector>
#include <string>
#include "Datum.h"

using namespace std;

class PrimaryKey{
	string table;
	vector<string> columnNames;
	vector<Datum> datumValues;
};

#endif
