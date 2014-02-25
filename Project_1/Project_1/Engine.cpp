#include "stdafx.h"
#include "Engine.h"
#include <vector>

#define TYPE_ERROR			-1
#define CANT_FIND_TABLE		-21

using namespace std;


//Checks if a string is an integer 
bool Engine::isInteger(const std::string & s)
{
	if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

	char * p ;
	strtol(s.c_str(), &p, 10) ;

	return (*p == 0) ;
}

//Finds the table in entityTables given the name of the table
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

//Creates an entity table and initializes its name and keys
void Engine::createEntityTable(string name, vector<string> keyCol){
	Table entityTable = Table(name, keyCol);
	entityTables.push_back(entityTable);
}

//Adds a new column in a table and initializes its column name and type
//The "type" arguement should be either "string" or "number" otherwise the function will fail
void Engine::addAttribute(string tableName, string colName, string type){
	if(type != "string" || type != "number")
		cerr << "Wrong type arguement passed into function" << endl;

	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables[i].addColumn(colName, type);
}

//Creates a relation table from two tables with the names (table1) and (table2)
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

//Removes a table from entityTables
void Engine::removeTable(string tableName){
	int i = 0;
	while(entityTables[i].getName() != tableName) i++;

	entityTables.erase(entityTables.begin()+i);
}

//Inserts a row into a table with the specific name (tableName)
int Engine::insertInto(string tableName, vector<Datum> rowData){
	int errorCode;
	int i = 0;
	i = FindTable(tableName);

	errorCode = entityTables[i].addRow(rowData);
	return errorCode;
}

//Updates/Changes the values of a table that meet the conditional statement that is provided to a specificed value
int Engine::updateEntity(string tableName, string att, void* newVal, string cond, void* condVal){
	int errorCode = -1;
	int i = 0, j = 0;
	//Look for the table in the engine
	i = FindTable(tableName);
	if( i == -21 ) return CANT_FIND_TABLE;

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
				if(tempData[k].getValue() > intCondVal){
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);	
					errorCode = 0;
				}
			}
		} else if(cond == "<"){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() < intCondVal){
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);
					errorCode = 0;
				}
			}
		} else if(cond == "=="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() == intCondVal){
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);
					errorCode = 0;
				}
			}
		} else if(cond == ">="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() >= intCondVal){
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);
					errorCode = 0;
				}
			}
		} else if(cond == "<="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() <= intCondVal){
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);
					errorCode = 0;
				}
			}
		}
	} 
	else if (type == "string"){
		string* temp = (string*)condVal;
		string stringCondVal = *temp;
		if(cond == "=="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getName() == stringCondVal){
					entityTables[i].columns[j].updateData(Datum(newVal, type), k);
					errorCode = 0;
				}
			}
		}
	} 
	else {
		errorCode = TYPE_ERROR;
	}

	return errorCode;
}

//Deletes the values of a table that meet the conditional statement that is provided
int Engine::deleteFrom(string tableName, string att, string cond, void* condVal){
	int errorCode = -1;
	int i = 0, j = 0;
	//Look for the table in the engine
	i = FindTable(tableName);
	if( i == -21 ) return CANT_FIND_TABLE;

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
				if(tempData[k].getValue() > intCondVal){
					entityTables[i].columns[j].updateData(Datum(), k);	
					errorCode = 0;
				}
			}
		} else if(cond == "<"){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() < intCondVal){
					entityTables[i].columns[j].updateData(Datum(), k);
					errorCode = 0;
				}
			}
		} else if(cond == "=="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() == intCondVal){
					entityTables[i].columns[j].updateData(Datum(), k);
					errorCode = 0;
				}
			}
		} else if(cond == ">="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() >= intCondVal){
					entityTables[i].columns[j].updateData(Datum(), k);
					errorCode = 0;
				}
			}
		} else if(cond == "<="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getValue() <= intCondVal){
					entityTables[i].columns[j].updateData(Datum(), k);
					errorCode = 0;
				}
			}
		}
	} 
	else if (type == "string"){
		string* temp = (string*)condVal;
		string stringCondVal = *temp;
		if(cond == "=="){
			for(int k = 0; k < dataSize; k++){
				if(tempData[k].getName() == stringCondVal){
					entityTables[i].columns[j].updateData(Datum(), k);
					errorCode = 0;
				}
			}
		}
	} 
	else {
		errorCode = TYPE_ERROR;
	}

	return errorCode;
}

//Generates a subtable from a table of name (initTable)
//The subtable consists of only the ROWS that meet the conditional statement that is provided
Table Engine::selection(string initTable, string seleTable, string att, string cond, void* condVal){
	int i = 0, tID = 0;
	tID = FindTable(initTable);
	i = entityTables[tID].findColumn(att);

	Column tempCol = entityTables[tID].getColumns()[i];

	int dataSize = entityTables[tID].getColumns()[i].getData().size();
	int colSize = entityTables[tID].getColumns().size();

	vector<Datum> tempData = entityTables[tID].getColumns()[i].getData();
	Table* tPtr = new Table(seleTable, entityTables[tID].getKeys());
	Table sTable = *tPtr;

	for(int j = 0; j < colSize; j++){
		sTable.addColumn(entityTables[tID].getColumns()[j].getName(), 
						entityTables[tID].getColumns()[j].getType());
	}

	string type = entityTables[tID].getColumns()[i].getType();

	if (type == "number"){
		int* temp = (int*)condVal;
		int intCondVal = *temp;
		if(cond == ">"){
			for(int j = 0; j < dataSize; j++){
				if(tempData[j].getValue() > intCondVal){
					vector<Datum> tempRow;
					for(int k = 0; k < colSize; k++){
						tempRow.push_back(entityTables[tID].columns[k].getData()[j]);				
					}
					sTable.addRow(tempRow);
				}
			}
		} else if(cond == "<"){
			for(int j = 0; j < dataSize; j++){
				if(tempData[j].getValue() < intCondVal){
					vector<Datum> tempRow;
					for(int k = 0; k < colSize; k++){
						tempRow.push_back(entityTables[tID].columns[k].getData()[j]);				
					}
					sTable.addRow(tempRow);
				}
			}
		} else if(cond == "=="){
			for(int j = 0; j < dataSize; j++){
				if(tempData[j].getValue() == intCondVal){
					vector<Datum> tempRow;
					for(int k = 0; k < colSize; k++){
						tempRow.push_back(entityTables[tID].columns[k].getData()[j]);				
					}
					sTable.addRow(tempRow);
				}
			}
		} else if(cond == ">="){
			for(int j = 0; j < dataSize; j++){
				if(tempData[j].getValue() >= intCondVal){
					vector<Datum> tempRow;
					for(int k = 0; k < colSize; k++){
						tempRow.push_back(entityTables[tID].columns[k].getData()[j]);				
					}
					sTable.addRow(tempRow);
				}
			}
		} else if(cond == "<="){
			for(int j = 0; j < dataSize; j++){
				if(tempData[j].getValue() <= intCondVal){
					vector<Datum> tempRow;
					for(int k = 0; k < colSize; k++){
						tempRow.push_back(entityTables[tID].columns[k].getData()[j]);					
					}
					sTable.addRow(tempRow);
				}
			}
		}
	}
	else if (type == "string"){
		string* temp = (string*)condVal;
		string stringCondVal = *temp;
		if(cond == "=="){
			for(int j = 0; j < dataSize; j++){
				if(tempData[j].getName() == stringCondVal){
					vector<Datum> tempRow;
					for(int k = 0; k < colSize; k++){
						tempRow.push_back(entityTables[tID].columns[k].getData()[j]);					
					}
					sTable.addRow(tempRow);
				}
			}
		}
	} 

	return sTable;
}

//Generates a subtable from a table of name (initTable)
//The subtable consists of only the COLUMNS that are specificed
Table Engine::projection(string initTable, string projTable, vector<string> att){
	int tID = 0, cID = 0;
	tID = FindTable(initTable);

	int colSize = entityTables[tID].columns[0].getData().size();

	Table* tPtr = new Table(projTable, att);
	Table pTable = *tPtr;

	for(int i = 0; i < att.size(); i++){
		cID = entityTables[tID].findColumn(att[i]);
		vector<Datum> tempData = entityTables[tID].columns[cID].getData();

		string colName = entityTables[tID].columns[cID].getName();
		string colType = entityTables[tID].columns[cID].getType();

		pTable.addColumn(colName, colType);
		for(int j = 0; j < colSize; j++){
			pTable.columns[i].addData(tempData[j]);
		}
	}

	return pTable;
}

//Generates a new table that contains the columns and rows of an exisiting table of 
//name (initTable) and renames the column names of that table (initTable).
Table Engine::rename(string initTable, string reTable, vector<string> att){
	int tID = 0;
	tID = FindTable(initTable);

	Table* tPtr = new Table(reTable, att);
	Table rTable = *tPtr;

	rTable = entityTables[tID];
	rTable.updateKeys(att);

	for(int i = 0; i < att.size(); i++){
		rTable.columns[i].setName(att[i]);
	}

	return rTable;
}

//Generates a new table that is a union of the tables (table1) and (table2)
//The union of the tables combines both tables together and removes any duplicate rows
Table Engine::setUnion(string unionName, string table1, string table2){
	int t1ID = 0, t2ID = 0;
	bool notDupe;
	int counter = 0;

	t1ID = FindTable(table1);
	t2ID = FindTable(table2);

	if(entityTables[t1ID].getKeys() != entityTables[t2ID].getKeys())
		cerr << "Keys do not match" << endl;

	Table* tPtr = new Table(unionName, entityTables[t1ID].getKeys());
	Table uTable = *tPtr;

	uTable.updateColumns(entityTables[t1ID].getColumns());

	int colSize = entityTables[t2ID].getColumns().size();

	int rowSize1 = entityTables[t1ID].columns[0].getData().size();
	int rowSize2 = entityTables[t2ID].columns[0].getData().size();

	for(int i = 0; i < rowSize2; i++){
		for(int j = 0; j < rowSize1; j++){
			notDupe = true;
			counter = 0;
			for(int k = 0; k < colSize; k++){
				vector<Datum> row;
				Datum t1Data = entityTables[t1ID].getColumns()[k].getData()[j];
				Datum t2Data = entityTables[t2ID].getColumns()[k].getData()[i];
				
				if(entityTables[t2ID].getColumns()[k].getType() == "string"){
					if(t1Data.getName() == t2Data.getName()){
						counter++;
						if(counter == colSize) notDupe = false;
					} else {
						if((notDupe == true) && (k == colSize-1)){
							for(int l = 0; l < colSize; l++){
								row.push_back(entityTables[t2ID].getColumns()[l].getData()[i]);
							}
							uTable.addRow(row);
						} else if((notDupe == true) && (i == rowSize2-1)){
							for(int l = 0; l < colSize; l++){
								row.push_back(entityTables[t2ID].getColumns()[l].getData()[i]);
							}
							uTable.addRow(row);
						}

						break;
					}
				} else if(entityTables[t2ID].getColumns()[k].getType() == "number"){
					if(t1Data.getValue() == t2Data.getValue()){
						counter++;
						if(counter == colSize) notDupe = false;
					} else {
						if((notDupe == true) && (k == colSize-1)){
							for(int l = 0; l < colSize; l++){
								row.push_back(entityTables[t2ID].getColumns()[l].getData()[i]);
							}
							uTable.addRow(row);
						} else if((notDupe == true) && (i == rowSize2-1)){
							for(int l = 0; l < colSize; l++){
								row.push_back(entityTables[t2ID].getColumns()[l].getData()[i]);
							}
							uTable.addRow(row);
						}

						break;
					}
				}

				if((notDupe == true) && (k == colSize-1)){
					for(int l = 0; l < rowSize2; l++){
						row.push_back(entityTables[t1ID].getColumns()[k].getData()[l]);
					}
					uTable.addRow(row);
				}
			}
		}
	}

	return uTable;
}

//Generates a new table that is the difference of the tables (table1) and (table2)
//In this case, the table would generate ((table1) DIFF (table2)) where the new table
// would have the rows of (table1) without any shared rows that it might have with (table2)
Table Engine::setDifference(string diffName, string table1, string table2){
	int t1ID = 0, t2ID = 0;
	bool notDupe;
	int counter = 0;

	t1ID = FindTable(table1);
	t2ID = FindTable(table2);

	if(entityTables[t1ID].getKeys() != entityTables[t2ID].getKeys())
		cerr << "Keys do not match" << endl;

	Table* tPtr = new Table(diffName, entityTables[t1ID].getKeys());
	Table dTable = *tPtr;

	int colSize = entityTables[t2ID].getColumns().size();

	int rowSize1 = entityTables[t1ID].columns[0].getData().size();
	int rowSize2 = entityTables[t2ID].columns[0].getData().size();

	for(int i = 0; i < colSize; i++){
		dTable.addColumn(entityTables[t1ID].getColumns()[i].getName(), entityTables[t1ID].getColumns()[i].getType());
	}

	vector<int> colID;
	vector<int> rowID;

	for(int i = 0; i < rowSize2; i++){
		for(int j = 0; j < rowSize1; j++){
			notDupe = true;
			counter = 0;
			for(int k = 0; k < colSize; k++){
				vector<Datum> row;
				Datum t1Data = entityTables[t1ID].getColumns()[k].getData()[j];
				Datum t2Data = entityTables[t2ID].getColumns()[k].getData()[i];
				
				if(entityTables[t2ID].getColumns()[k].getType() == "string"){
					if(t1Data.getName() == t2Data.getName()){
						counter++;
						if(counter == colSize){
							notDupe = false;
							for(int l = 0; l < colSize; l++){
								rowID.push_back(j);
							}
						}
					} else break;
				} else if(entityTables[t2ID].getColumns()[k].getType() == "number"){
					if(t1Data.getValue() == t2Data.getValue()){
						counter++;
						if(counter == colSize){
							notDupe = false;
							for(int l = 0; l < colSize; l++){
								rowID.push_back(j);
							}
						}
					} else break;
				}
			}
		}
	}

	for(int i = 0; i < rowSize1; i++){
		vector<Datum> row;
		if(find(rowID.begin(), rowID.end(), i) == rowID.end()){
			for(int j = 0; j < colSize; j++){
				row.push_back(entityTables[t1ID].getColumns()[j].getData()[i]);
			}
			dTable.addRow(row);
		}
	}

	return dTable;
}

//Generates a new table that is the Cartesian product of the two tables (table1) and (table2)
Table Engine::crossProduct(string crossName, string table1, string table2){
	int t1ID = 0, t2ID = 0;
	t1ID = FindTable(table1);
	t2ID = FindTable(table2);

	vector<string> newKeys = entityTables[t1ID].getKeys();
	vector<string> keys2 = entityTables[t2ID].getKeys();
	newKeys.insert(newKeys.end(), keys2.begin(), keys2.end());

	Table* tPtr = new Table(crossName, newKeys);
	Table cTable = *tPtr;

	int dataSize1 = entityTables[t1ID].columns[0].getData().size();
	int dataSize2 = entityTables[t2ID].columns[0].getData().size();

	int colSize1 = entityTables[t1ID].columns.size();
	int colSize2 = entityTables[t2ID].columns.size();

	for(int i = 0; i < colSize1; i++){
		cTable.addColumn(entityTables[t1ID].getColumns()[i].getName(), 
						entityTables[t1ID].getColumns()[i].getType());
	}
	for(int i = 0; i < colSize2; i++){
		cTable.addColumn(entityTables[t2ID].getColumns()[i].getName(), 
						entityTables[t2ID].getColumns()[i].getType());
	}


	for(int i = 0; i < dataSize1; i++){
		for(int j = 0; j < dataSize2; j++){
			vector<Datum> tempData;
			for(int k = 0; k < colSize1; k++){
				tempData.push_back(entityTables[t1ID].getColumns()[k].getData()[i]);
			}

			for(int k = 0; k < colSize2; k++){
				tempData.push_back(entityTables[t2ID].getColumns()[k].getData()[j]);
			}
			cTable.addRow(tempData);
		}
	}

	return cTable;
}

//This function does not have a test and was therefore commented out entirely since 
//there is no way to justify that our naturalJoin function works without a test.
//Our main problem with the naturalJoin came with the challenge of uniqueness
//Table Engine::naturalJoin(string joinName, string table1, string table2){
	/*int t1ID = 0, t2ID = 0;
	t1ID = FindTable(table1);
	t2ID = FindTable(table2);

	for(int i = 0; i < entityTables[t1ID].getKeys().size(); i++){
		for(int j = 0; j < entityTables[t2ID].getKeys().size(); j++){
			if(entityTables[t1ID].getKeys()[i] == entityTables[t2ID].getKeys()[j]){
				
			}
		}
	}

	vector<string> newKeys = entityTables[t1ID].getKeys();
	vector<string> keys2 = entityTables[t2ID].getKeys();
	newKeys.insert(newKeys.end(), keys2.begin(), keys2.end());

	vector<Column> newCols = entityTables[t1ID].getColumns();
	vector<Column> cols2 = entityTables[t2ID].getColumns();
	newCols.insert(newCols.end(), cols2.begin(), cols2.end());

	Table* tPtr = new Table(joinName, newKeys);
	Table njTable = *tPtr;

	njTable.updateColumns(newCols);

	return njTable;*/
//}
