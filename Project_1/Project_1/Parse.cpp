#include "stdafx.h"
#include "Engine.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

//TODO: 
#define TYPE_ERROR			-1
#define CANT_FIND_TABLE		-21

Engine engine;

bool isInteger(const string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

	char * p ;
	strtol(s.c_str(), &p, 10) ;

	return (*p == 0) ;
}

int createTable(vector<string> operations){

	string tableName;
	vector<string> data;
	vector<string> keys;

	//Take the name out of the vector
	tableName = operations[2];

	//Insert parameters in data vector
	for(int i = 3; i < operations.size(); i++ ){

		if( (i%2 == 0) && ( (operations[i] != "STRING") && (operations[i] != "INTERGER") ) ){

			return TYPE_ERROR;
		}

		//If we find primary, the first key should be positioned at i+2
		else if( operations[i] == "PRIMARY" ){

			//Get keys from i+2 until end of vector
			for(int j = i+2; j < operations.size(); j++){

				keys.push_back(operations[j]);

			}
			//No more things, break;
			break;
		}

		else{

			data.push_back(operations[i]);
		}
	}

	//Engine.
	return 0;
}

int insertInto(vector<string> operations){

	string tableName = operations[2];
	vector<Datum> data;

	for(int i = 5; i < operations.size(); i++){

		if( isInteger(operations[i]) == true ){

			int number = atoi(operations[i].c_str());
			data.push_back(Datum(number));
		}

		else{

			data.push_back(Datum(operations[i]));
		}
	}

	engine.insertInto(tableName,data);

	return 0;
}

int openTable(vector<string> operations){

	char symbol = NULL;
	string input;
	string type;

	//When symbolcount == 0, save first line in keys; when symbolcount == 1, add columns to table; 
	//When symbolcount > 1 save in data;
	int symbolcount = 0;				
	string tableName = operations[1];

	//We will store values here
	vector<Datum> data;
	vector<string> keys;

	ifstream dbFile;
	string filename = tableName + ".db";

	//Open the file
	dbFile.open(filename);

	//First one gets the table name
	dbFile >> tableName;

	while(!dbFile.eof()){

		dbFile >> input;

		//Check for consecutive semicolons
		if( symbol == input[0] ){

			//end of file, do nothing
		}

		else if( input == ";" ){

			symbol = ';';

			//We are reusing the vector data, insert to table depending on symbolcount
			//and clear its content once we reach end of row
			if( symbolcount == 0 ){

				engine.createEntityTable(tableName,keys);
			}

			if( symbolcount > 1 ){

				engine.insertInto(tableName,data);
				data.clear();
			}

			symbolcount = symbolcount + 1;
		}

		else{
			//No consecutive ';', clear the symbol
			symbol = NULL;

			if( symbolcount == 0 ){

				keys.push_back(input);
			}

			else if( symbolcount == 1){

				dbFile >> type;

				engine.addAttribute(tableName,input,type);
			}

			else{
				data.push_back(Datum(input));
			}
		}

	}

	return 0;
}

int createFile(vector<string> operations){

	ofstream dbFile;
	string filename = operations[1]+ ".db";

	cout << filename << endl;

	dbFile.open(filename);

	dbFile.close();

	return 0;
}

int saveToTable(vector<string> operations){

	int tIndex;
	string tableName = operations[1];

	ofstream dbFile;
	string filename = tableName + ".db";

	dbFile.open(filename);

	//Look for the table in the engine
	tIndex = engine.FindTable(tableName);

	if( tIndex == -21 ){

		return CANT_FIND_TABLE;
	}

	//Declare variables for clarification, reuse tableName
	tableName = engine.entityTables[tIndex].getName();
	int nOfColumns = engine.entityTables[tIndex].getColumns().size();
	int nOfRows = engine.entityTables[tIndex].getColumns()[0].getData().size();
	int nOfPrimaryKeys = engine.entityTables[tIndex].getKeys().size();

	dbFile << tableName;
	dbFile << " ";

	//Get the primary keys of the table
	for( int i = 0; i < nOfPrimaryKeys; i++ ){

		dbFile << engine.entityTables[tIndex].getKeys()[i] << " ";
	}

	dbFile << ";\n";

	//Now the Column Names;
	for( int i = 0; i < nOfColumns; i++ ){

		dbFile << engine.entityTables[tIndex].getColumns()[i].getName() << " ";
		dbFile << engine.entityTables[tIndex].getColumns()[i].getType() << " ";
	}
	//Dont forget, End the line with ; and \n
	dbFile << ";\n";

	//Now save the data in rows to file
	for( int i = 0; i < nOfRows; i++){

		for( int j = 0; j < nOfColumns; j++ ){

			dbFile << engine.entityTables[tIndex].getColumns()[j].getData()[i].getName() << " ";
		}		
		dbFile << ";\n";
	}

	dbFile << ";\n";
	dbFile << ";";

	dbFile.close();

	return 0;

}

//UPDATE Random SET (Color = 20) WHERE (Color < 5) 
int updateTable(vector<string> operations){

	string tableName = operations[1];

	string attributeName = operations[3];

	string condition = operations[8];
	void* newValuePtr;
	void* condValuePtr;

	if( isInteger(operations[4]) ){

		int newValue = atoi(operations[5].c_str());
		newValuePtr = &newValue;


		if( isInteger(operations[9]) ){

			int condValue = atoi(operations[9].c_str());
			condValuePtr = &condValue;
		}
	}

	else{

		string newValue = operations[5];
		newValuePtr = &newValue;
		string condValue = operations[9];
		condValuePtr = &condValue;
	}

	engine.updateEntity(tableName,attributeName,newValuePtr,condition,condValuePtr);

	return 0;

}

//DELETE FROM relation-Name Where (Color < 5)
int deleteFrom(vector<string> operations){

	string tableName = operations[2];

	string attributeName = operations[4];

	string condition = operations[5];

	void* condValuePtr;

	if( isInteger(operations[6]) ){

		int condValue = atoi(operations[6].c_str());
		condValuePtr = &condValue;
	}

	else{

		string condValue = operations[6];
		condValuePtr = &condValue;

	}
	engine.deleteFrom(tableName,attributeName,condition,condValuePtr);

	return 0;

}

string queryType(int index,vector<string> operations){

	string tableName = operations[0];
	string instruction = operations[index];
	string attribute;
	string attributeName;
	string operation;
	string othertablename;

	// select (kind == "dog") select (kind == "food ) animals;
	// dogs + (select (kind == "cat") animals);
	// common_names <- project (name) (select (aname == name && akind != kind) (a * animals));

	//Check if instruction is an operation or an attributeName
	if( instruction == "select" ){

		attribute = operations[index+1];
		operation = operations[index+2];
		attributeName = operations[index+3];
		othertablename = queryType(index+4,operations);

		int tIndex = engine.FindTable(othertablename);

		void* attributeNamePtr;
		if( isInteger(attributeName) ){

			int condValue = atoi(attributeName.c_str());
			attributeNamePtr = &condValue;
		}

		else{

			string condValue = attributeName;
			attributeNamePtr = &condValue;

		}

		engine.entityTables.push_back(engine.selection(engine.entityTables[tIndex].getName(),tableName,attribute,operation,attributeNamePtr));

		return tableName;

	}

	// <- project (attributename) | atomic-exp
	else if( instruction == "project" ){

		attributeName = operations[index+1];

		othertablename = queryType(index+2,operations);

		int tIndex = engine.FindTable(othertablename);

		//TODO: HACK...add attribute name to a vector for projection...should be able to handle
		//more than one attributeName
		vector<string> attributeList;
		attributeList.push_back(attributeName);

		engine.entityTables.push_back(engine.projection(engine.entityTables[tIndex].getName(),tableName,attributeList));

		return tableName;

	}

	else if( instruction == "rename" ){

		vector<string> data;

		attributeName = operations[index+1];

		othertablename = queryType(index+2,operations);

		int tIndex = engine.FindTable(othertablename);

		engine.entityTables.push_back(engine.rename(engine.entityTables[tIndex].getName(),tableName,data));

		return tableName;


	}

	//Check if its an operation like +, *
	else if( index+2 < operations.size() ){

		//Atomic expression + Atomic expression
		if( operations[index+1] == "+" ){

			int t1Index = engine.FindTable(instruction);

			if( t1Index == -21 ){

				return "Error";
			}

			othertablename = queryType(index+1,operations);

			int t2Index = engine.FindTable(othertablename);

			if( t2Index == -21) {

				return "Error";
			}

			//engine.setUnion(tableName,engine.entityTables[t1Index],engine.entityTables[t2Index]);

			return tableName;
		}

		else if( instruction == "-"){

			int t1Index = engine.FindTable(instruction);

			if( t1Index == -21 ){

				return "Error";
			}

			othertablename = queryType(index+1,operations);

			int t2Index = engine.FindTable(othertablename);

			if( t2Index == -21) {

				return "Error";
			}

			//engine.setDifference(tableName,engine.entityTables[t1Index],engine.entityTables[t2Index]);

			return tableName;

		}

		else if( instruction == "*"){

			int t1Index = engine.FindTable(instruction);

			if( t1Index == -21 ){

				return "Error";
			}

			othertablename = queryType(index+1,operations);

			int t2Index = engine.FindTable(othertablename);

			if( t2Index == -21) {

				return "Error";
			}

			//engine.crossProduct(tableName,engine.entityTables[t1Index],engine.entityTables[t2Index]);

			return tableName;
		}

		else if( instruction == "JOIN"){

			int t1Index = engine.FindTable(instruction);

			if( t1Index == -21 ){

				return "Error";
			}

			othertablename = queryType(index+1,operations);

			int t2Index = engine.FindTable(othertablename);

			if( t2Index == -21) {

				return "Error";
			}

			//engine.naturalJoin(tableName,engine.entityTables[t1Index],engine.entityTables[t2Index]);

			return tableName;
		}

	}

	//else its an attribute name
	else{

		return instruction;
	}

}

//compare stuff
Table comparisons(vector<string> operations, int index){

	vector<string> key;
	string identifier = operations[index];

	if( index + 1 == operations.size() ){

		return Table("Table1",key);
	}

	string operation = operations[index+1];

	if( operation == ">" ){


	}

	else if( operation == "<" ){

	}

	else if( operation == "!=" ){

	}
	else if( operation == ">=" ){


	}
	else if( operation == "<=" ){


	}

	else if( operation == "==" ){


	}

	else{

	}

	return Table("TABLE1",key);
}

string getString(vector<string> operations, int index){


	if( index == operations.size() - 1 ){

		return operations[index];
	}

}

int showTable(vector<string> operations){

	//Find the table using second index in operation since
	//SHOW tableName, tableName always second string
	int tIndex = engine.FindTable(operations[1]);

	if ( tIndex == -21 ){

		return CANT_FIND_TABLE;
	}

	cout << engine.entityTables[tIndex].getName() << " ";

	for( int i = 0; i < engine.entityTables[tIndex].getKeys().size(); i++){

		cout << engine.entityTables[tIndex].getKeys()[i] << " ";
	}

	cout << endl;

	for(int i = 0; i < engine.entityTables[tIndex].getColumns().size(); i++ ){

		cout << engine.entityTables[tIndex].getColumns()[i].getName() << " "; 
		cout << engine.entityTables[tIndex].getColumns()[i].getType() << " ";
	}

	cout << endl;

	for(int i = 0; i < engine.entityTables[tIndex].getColumns()[0].getData().size(); i++){

		for( int j = 0; j < engine.entityTables[tIndex].getColumns().size(); j++ ){

			cout << engine.entityTables[tIndex].getColumns()[j].getData()[i].getName() << " ";

		}
		cout << endl;
	}

	return 0;
}

int Operations(vector<string> operations){

	if( operations[0] == "CREATE" ) {

		if( createTable(operations) == TYPE_ERROR ){

			return TYPE_ERROR;	
		}

	}

	else if( operations[0] == "INSERT"){

		insertInto(operations);
	}

	else if( operations[0] == "OPEN" ){

		openTable(operations);
	}

	else if( operations[0] == "WRITE" ){

		createFile(operations);
	}

	else if( operations[0] == "CLOSE" ){

		saveToTable(operations);
	}

	else if( operations[0] == "SHOW"){

		showTable(operations);
	}

	else if( operations[0] == "UPDATE"){

		updateTable(operations);
	}

	else if( operations[0] == "EXIT"){

		return 1;
	}

	else if( operations[1] == "<-" ){

		//Three is how this function should start for query
		queryType(2,operations);
	}

	return 0;

}

int Parse(string message, vector<string>& command){

	string b;
	size_t found;
	int size = 0;
	bool exit = false;
	stringstream input;

	input << message;

	while(!exit){

		//take strings out of the stream
		input >> b;

		//check the string doesnt start with a (, if it does, erase it
		if( (b[0] == '(' || b[0] == '"') ) { b.erase(b.begin()); }

		//Check if there string is in this format: (name,kind) 
		//Locate comma...
		found = b.find(',');


		//if you find the comma and its not at the end i.e. STRING, ... then
		if( found != string::npos && b[found] != b[b.size()-1] ){

			//Separate the strings and copy the string to the left of comma into a char
			char* s1 = new char[found];
			b.copy(s1,found,0);

			//copy doesnt null terminate the copy, so we have to manually do it
			s1[found] = '\0';

			//copy the buffer into a string
			string c(s1);

			//Erase it from the original string
			b.erase(b.begin(),b.begin()+found+1);

			//and push back the remaining string.
			command.push_back(s1);
		}

		//Remove special characters, if ;, exit
		if( b[b.size()-1] == ';') { b.pop_back(); exit = true;}
		if( b[b.size()-1] == ',') { b.pop_back();}
		if( b[b.size()-1] == '"') { b.pop_back();}
		if( b[b.size()-1] == ')') { b.pop_back();}
		if( b[b.size()-1] == ')') { b.pop_back();}
		command.push_back(b);
	}

	return 0;
}

int main(){

	int b;
	string message = "OPEN Random2;";
	vector<string> data;

	//Parse takes the message, parses it and stores the
	//individual strings in a vector passed as reference
	Parse(message,data);

	//This function processes the data in vector and does an 
	//action based on the operation, i.e. OPEN, SHOW
	Operations(data);

	message = "OPEN Random;";
	//Parse takes the message, parses it and stores the
	//individual strings in a vector passed as reference
	data.clear();
	Parse(message,data);

	//This function processes the data in vector and does an 
	//action based on the operation, i.e. OPEN, SHOW
	Operations(data);

	message = "dogs <- Random + Random2;";
	data.clear();

	Parse(message,data);
	Operations(data);

	cin >> b;

}
