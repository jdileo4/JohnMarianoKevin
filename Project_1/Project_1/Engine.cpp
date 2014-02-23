#include "stdafx.h"
#include "Engine.h"
#include <vector>

#define TYPE_ERROR			-1
#define CANT_FIND_TABLE		-21

using namespace std;
//Use this one instead of the while(...)i++
int Engine::FindTable(string tname){

	int index = -21;

	for(int i = 0; i < entityTables.size(); i++ ){

		if( entityTables[i].getName() == tname ){

			index = i;
			return index;
		}
	}
	return index;
}


void Engine::createEntityTable(string name, vector<string> keyCol){
	Table entityTable = Table(name, keyCol);
	entityTables.push_back(entityTable);
}

//type should be either "string" or "number"
void Engine::addAttribute(string tableName, string colName, string type){
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables[i].addColumn(colName, type);
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

void Engine::removeTable(string tableName){
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables.erase(entityTables.begin()+i);
}

int Engine::insertInto(string tableName, vector<Datum> rowData){
	int errorCode;
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	errorCode = entityTables[i].addRow(rowData);
	return errorCode;
}

int Engine::updateEntity(string tableName, string att, void* newVal, string cond, void* condVal){
	//TODO fix this, either update to void* handling or maybe should overload function
	int i = 0, j = 0;
	//Look for the table in the engine
	i = FindTable(tableName);
	if( i == -21 ){
		return CANT_FIND_TABLE;
	}
	//look for the column in the table
	j = entityTables[i].findColumn(att);

	//get type for Datum constructor
	string type = entityTables[i].getColumns()[j].getType();

	int dataSize = entityTables[i].getColumns()[j].getData().size();
	vector<Datum> tempData = entityTables[i].getColumns()[j].getData();

	if (type == "number"){
		int* temp = (int*)condVal;
		int intCondVal = *temp;
		if(cond == ">"){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() > intCondVal)
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);		
			}
		} else if(cond == "<"){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() < intCondVal)
					//entityTables[i].getColumns()[j].updateData(Datum(newVal), k);
						entityTables[i].columns[j].updateData(Datum(newVal, type), k);
			}
		} else if(cond == "=="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() == intCondVal)
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);		
			}
		} else if(cond == ">="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() >= intCondVal)
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);
			}
		} else if(cond == "<="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() <= intCondVal)
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);
			}
		}
	}
	else if (type == "string"){
		string* temp = (string*)condVal;
		string stringCondVal = *temp;
		if(cond == "=="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getName() == stringCondVal)
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);		
			}
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

Table Engine::selection(Table initTable, string seleTable, string att, string cond, int condVal){
	int i = 0;
	while(initTable.getColumns()[i].getName() != att) i++;

	int dataSize = initTable.getColumns()[i].getData().size();
	int keySize = initTable.getKeys().size();

	vector<Datum> tempData = initTable.getColumns()[i].getData();
	Table* sTable = new Table(seleTable, initTable.getKeys());

	if(cond == ">"){
		for(int j = 0; j < dataSize; j++){
			if(tempData[j].getValue() > condVal){
				vector<Datum> tempRow;
				for(int k = 0; k < keySize; k++){
					tempRow.push_back(initTable.getColumns()[k].getData()[j]);				
				}
				sTable->addRow(tempRow);
			}
		}
	} else if(cond == "<"){
		for(int j = 0; j < dataSize; j++){
			if(tempData[j].getValue() > condVal){
				vector<Datum> tempRow;
				for(int k = 0; k < keySize; k++){
					tempRow.push_back(initTable.getColumns()[k].getData()[j]);				
				}
				sTable->addRow(tempRow);
			}
		}
	} else if(cond == "=="){
		for(int j = 0; j < dataSize; j++){
			if(tempData[j].getValue() > condVal){
				vector<Datum> tempRow;
				for(int k = 0; k < keySize; k++){
					tempRow.push_back(initTable.getColumns()[k].getData()[j]);				
				}
				sTable->addRow(tempRow);
			}
		}
	} else if(cond == ">="){
		for(int j = 0; j < dataSize; j++){
			if(tempData[j].getValue() > condVal){
				vector<Datum> tempRow;
				for(int k = 0; k < keySize; k++){
					tempRow.push_back(initTable.getColumns()[k].getData()[j]);				
				}
				sTable->addRow(tempRow);
			}
		}
	} else if(cond == "<="){
		for(int j = 0; j < dataSize; j++){
			if(tempData[j].getValue() > condVal){
				vector<Datum> tempRow;
				for(int k = 0; k < keySize; k++){
					tempRow.push_back(initTable.getColumns()[k].getData()[j]);				
				}
				sTable->addRow(tempRow);
			}
		}
	}

	return *sTable;
}

Table Engine::projection(Table initTable, string projTable, string att){
	int i = 0;
	while(initTable.getColumns()[i].getName() != att) i++;

	int colSize = initTable.getColumns()[i].getData().size();

	vector<string> projKeys;
	projKeys.push_back(att);

	Table* tPtr = new Table(projTable, projKeys);
	Table pTable = *tPtr;
	vector<Datum> tempData = initTable.getColumns()[i].getData();

	string colName = initTable.getColumns()[i].getName();
	string colType = initTable.getColumns()[i].getType();

	pTable.addColumn(colName, colType);
	for(int j = 0; j < colSize; j++){
		pTable.getColumns()[0].addData(tempData[j]);
	}

	return pTable;
}

Table Engine::rename(Table initTable, string reTable, vector<string> newAtt){
	Table* tPtr = new Table(reTable, newAtt);
	Table rTable = *tPtr;

	rTable = initTable;
	rTable.updateKeys(newAtt);

	for(int i = 0; i < newAtt.size(); i++){
		rTable.getColumns()[i].setName(newAtt[i]);
	}

	return rTable;
}

/*Table Engine::setUnion(string unionName, Table table1, Table table2){
if(table1.getKeys() != table2.getKeys())
cerr << "Keys do not match" << endl;

Table* tPtr = new Table(unionName, table1.getKeys());
Table uTable = *tPtr;

uTable.updateColumns(table1.getColumns());

int colSize = table2.getColumns().size();
int rowSize = table2.getKeys().size();

for(int i = 0; i < rowSize; i++){
vector<Datum> tempRow;
for(int j = 0; j < colSize; j++){
Datum tempData = table2.getColumns()[j].getData()[i];
if(find(tempRow.begin(), tempRow.end(), tempData) == tempRow.end())
tempRow.push_back(tempData);
}
uTable.addRow(tempRow);
}

return uTable;
}

Table Engine::setDifference(string diffName, Table table1, Table table2){
if(table1.getKeys() != table2.getKeys())
cerr << "Keys do not match" << endl;

Table* tPtr = new Table(diffName,table1.getKeys());
Table dTable = *tPtr;

int colSize = table2.getColumns().size();
int rowSize = table2.getKeys().size();

for(int i = 0; i < rowSize; i++){
vector<Datum> tempRow;
for(int j = 0; j < colSize; j++){
Datum tempData1 = table1.getColumns()[j].getData()[i];
vector<Datum> tempData2 = table2.getColumns()[j].getData();
if(find(tempData2.begin(), tempData2.end(), tempData1) == tempData2.end())
tempRow.push_back(tempData1);
}
dTable.addRow(tempRow);
}

return dTable;
}*/

Table Engine::crossProduct(string crossName, Table table1, Table table2){
	vector<string> newKeys = table1.getKeys();
	vector<string> keys2 = table2.getKeys();
	newKeys.insert(newKeys.end(), keys2.begin(), keys2.end());

	Table* tPtr = new Table(crossName, newKeys);
	Table cTable = *tPtr;

	int dataSize = table1.getColumns()[0].getData().size();
	int keySize1 = table1.getKeys().size();
	int keySize2 = keys2.size();

	for(int i = 0; i < keys2.size(); i++){
		vector<Datum> tempData;
		for(int j = 0; j < dataSize; j++){
			for(int k = 0; k < keySize1; k++){
				tempData.push_back(table1.getColumns()[k].getData()[j]);
			}

			for(int k = 0; k < keySize2; k++){
				tempData.push_back(table2.getColumns()[k].getData()[i]);
			}
		}
		cTable.addRow(tempData);
	}

	return cTable;
}

Table Engine::naturalJoin(string joinName, Table table1, Table table2){
	vector<string> newKeys = table1.getKeys();
	vector<string> keys2 = table2.getKeys();
	newKeys.insert(newKeys.end(),keys2.begin(), keys2.end());

	vector<Column> newCols = table1.getColumns();
	vector<Column> cols2 = table2.getColumns();
	newCols.insert(newCols.end(), cols2.begin(), cols2.end());

	Table* tPtr = new Table(joinName, newKeys);
	Table njTable = *tPtr;

	njTable.updateColumns(newCols);

	return njTable;
}

bool Engine::isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

	char * p ;
	strtol(s.c_str(), &p, 10) ;

	return (*p == 0) ;
}