#ifndef PRIMARYKEY_H
#define PRIMARYKEY_H

#include <string>
#include <vector>
#include "Datum.h"

using namespace std;


class PrimaryKey{

public:

private:

	string table;
	vector<string> columnNames;
	vector<Datum> datumValues;

};

#endif
