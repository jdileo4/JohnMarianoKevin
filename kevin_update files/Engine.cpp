#include "Engine.h"
#include <vector>

using namespace std;

void Engine::createEntityTable(string name, vector<string> keyCol){
	Table* entityTable = new Table(name, keyCol);
	entityTables.push_back(*entityTable);
}

void Engine::createRelationTable(string name, string table1, string table2){
	//Not sure whats going on here; need to talk about this
}

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

void Engine::removeTable(string tableName){
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables.erase(entityTables.begin()+i);
}