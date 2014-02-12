#include "stdafx.h"
#include "Engine.h"
#include <vector>

using namespace std;

void Engine::createEntityTable(string name, vector<string> keyCol){
	Table* entityTable = new Table(name, keyCol);
	entityTables.push_back(*entityTable);
}

void Engine::createRelationTable(string name, string table1, string table2){
	vector<string> keyColumns;

	for (int i = 0; i < entityTables.size(); i++){
		if ( entityTables.at(i).getName() == table1){
			for (int j = 0; j < entityTables.at(i).getKeys().size(); j++){
				keyColumns.push_back(entityTables.at(i).getKeys().at(j));
			}
		}
	}
	for (int i = 0; i < entityTables.size(); i++){
		if ( entityTables.at(i).getName() == table2){
			for (int j = 0; j < entityTables.at(i).getKeys().size(); j++){
				keyColumns.push_back(entityTables.at(i).getKeys().at(j));
			}
		}
	}

	Table result = Table(name, keyColumns);
	for (int i = 0; i < keyColumns.size(); i++){
		if (isInteger(keyColumns.at(i))){
			result.addColumn(keyColumns.at(i), "number");
		}
		else{
			result.addColumn(keyColumns.at(i), "string");
		}
	}

	relationTables.push_back(result);

}

//type should be either "string" or "number"
void Engine::addAttribute(string tableName, string colName, string type){
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables[i].addColumn(colName, type);
}

void Engine::addEntity(string tableName, vector<Datum> rowData){
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables[i].addRow(rowData);
}

void Engine::updateEntity(string tableName, string att, int newVal, string cond, int condVal){
	int i = 0, j = 0;
	while(entityTables[i].getName() != tableName) i++;
	while(entityTables[i].getColumns()[j].getName() != att) j++;

	int dataSize = entityTables[i].getColumns()[j].getData().size();
	vector<Datum> tempData = entityTables[i].getColumns()[j].getData();

	if(cond == ">"){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() > condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(newVal);
		}
	} else if(cond == "<"){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() < condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(newVal);
		}
	} else if(cond == "=="){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() == condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(newVal);
		}
	} else if(cond == ">="){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() >= condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(newVal);
		}
	} else if(cond == "<="){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() <= condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(newVal);
		}
	}
}

void Engine::deleteFrom(string tableName, string att, string cond, int condVal){
	int i = 0, j = 0;
	while(entityTables[i].getName() != tableName) i++;
	while(entityTables[i].getColumns()[j].getName() != att) j++;

	int dataSize = entityTables[i].getColumns()[j].getData().size();
	vector<Datum> tempData = entityTables[i].getColumns()[j].getData();

	if(cond == ">"){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() > condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(NULL);
		}
	} else if(cond == "<"){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() > condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(NULL);
		}
	} else if(cond == "=="){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() > condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(NULL);
		}
	} else if(cond == ">="){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() > condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(NULL);
		}
	} else if(cond == "<="){
		for(int k = 0; k < dataSize; k++){
			if(tempData[k].getValue() > condVal)
				entityTables[i].getColumns()[j].getData()[k].setValue(NULL);
		}
	}
}

void Engine::removeTable(string tableName){
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables.erase(entityTables.begin()+i);
}

bool Engine::isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}