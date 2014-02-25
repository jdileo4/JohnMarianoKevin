#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <sstream>
#include <iostream>
#include "Engine.h"

class Parser{
public:

	int showTable(vector<string> operations);
	int openTable(vector<string> operations);
	int insertInto(vector<string> operations);
	int createFile(vector<string> operations);
	int Operations(vector<string> operations);
	int deleteFrom(vector<string> operations);
	int createTable(vector<string> operations);
	int saveToTable(vector<string> operations);
	int updateTable(vector<string> operations);
	int Parse(string message, vector<string>& command);

	string queryType(int index,vector<string> operations);
	string getString(vector<string> operations, int index);

	Table comparisons(vector<string> operations, int index);

	Engine engine;

};

#endif
