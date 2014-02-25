//// Application.cpp : Defines the entry point for the console application.
////
//#include "Engine.h"
//using namespace std;
//
//const int QUIT = 0;
//
//int choice = -1;
//Bank bank = Bank();
//Statistics stat = Statistics();
//
////returns true if choice was successfully executed
//bool executeChoice(int choice);
//
////make a new customer and cout their account num and pin
//void newCustomer(string name);
//
////prints main menu
//void printMenu();
//
////verifies acctNum and pin and returns customer
////returns null if invalid information
//Customer* accessCustomer();
//
////truncate to 2 decimal places
//double roundTo2Dec(double num);
//
////update avgBalance in statistics
//void updateStats();
//
////get accounts from file
//void readFile();
//
//int main()
//{
//	readFile();
//	cout << "This is your Piggy Bank. Welcome valued customer." << endl;
//	while(choice != QUIT){
//		printMenu();
//		while(!executeChoice(choice = 
//			TUI::getChoiceWithPrompt(7, "Enter your choice: "))){
//				printMenu();
//		}
//	}
//	cout << "Quitting." << endl;
//	//TODO delete bank??? does it stay even after executable is terminated?
//	return 0;
//}
//
////returns true if choice was successfully executed
//bool executeChoice(int choice){ 
//	bool success = false;
//	try{
//		//switch statement for choices to execute
//		switch(choice){
//			//open account
//		case(1):{
//			cout << "Enter your first and last name: " << endl;
//			char name[50];
//			cin.getline(name, 256, '\n');
//			newCustomer(name);
//			stat.totalCustomers++;
//			stat.numDeposits++;
//			success = true;
//			break;
//				}
//				//close account
//		case (2):{
//			char answer[50];
//			Customer* customer;
//			cout << "Are you sure you want to close your account? (yes/no)"
//				<< endl;
//			cin.getline(answer, 256, '\n');
//			string yesno = answer;
//			if (yesno == "yes"){
//				customer = accessCustomer();
//				if (customer->getBalance() > 0){
//					cout << "You had $" << roundTo2Dec(customer->getBalance())
//						<< " in your account." << endl;
//					if(customer->getBalance() > 500){
//						cout << "Here's $500." << endl;
//						cout << "See cashier to complete transaction." << endl
//							<< endl;
//					}
//					else{
//						cout << "Here's " << roundTo2Dec(customer->getBalance())
//							<< endl << endl;
//					}
//				}
//				//remove customer
//				bank.removeCust(customer);
//				stat.totalCustomers--;
//			}
//			success = true;
//			break;
//				 }
//				 //withdraw money
//		case (3):{
//			Customer* customer = accessCustomer();
//			double withdraw;
//			withdraw = TUI::getIntWithPrompt(
//				"Enter the amount to withdraw (must be at most $500): ");
//			//Verify positive amount
//			if (withdraw < 0 || withdraw > 500 ||
//				withdraw > customer->getBalance()) throw BAD_INPUT;
//			customer->withdraw(withdraw);
//			if (customer->getBalance() < 50){ 
//				bank.removeCust(customer);
//				cout << "Balance less than $50.  Removed customer account." << endl;
//			}
//			success = true;
//			break;
//				 }
//				 //deposit money
//		case (4):{
//			Customer* customer = accessCustomer();
//			double deposit;
//			deposit = TUI::getIntWithPrompt(
//				"Enter the amount to deposit: ");
//			//verify positive amount
//			if (deposit < 0) throw BAD_INPUT;
//			customer->deposit(deposit);
//			stat.numDeposits++;
//			success = true;
//			break;
//				 }
//				 //print account balance
//		case (5):{
//			Customer* customer = accessCustomer();
//			cout << "Balance: " << customer->getBalance()
//				<< endl << endl;
//			success = true;
//			break;
//				 }
//				 //list last 10 transactions
//		case (6):{
//			Customer* customer = accessCustomer();
//			cout << "Last 10 transactions: " << endl;
//
//			stringstream ss;
//			ss << "transactions {";
//			for(int x = 0; x < customer->transactions.size(); x++)
//			{
//				ss << " (" << customer->transactions[x].fullDate 
//					<< " " << customer->transactions[x].balanceChange << ")";
//			}
//			ss << " }";
//			string fileTran = ss.str();
//			cout << fileTran << endl << endl << endl;
//			success = true;
//			break;
//				 }
//				 //
//		case (7):{
//			int choice2;
//			cout << "Choose one: " << endl;
//			cout << "1. Total number of customers" << endl
//				<< "2. Total amount of deposits" << endl
//				<< "3. Average current balance" << endl;
//			choice2 = TUI::getChoiceWithPrompt(3, "Enter your choice: ");
//			switch(choice2){
//			case(1):{
//				stat.update(bank);
//				cout << "Total number of customers = " << stat.totalCustomers
//					<< endl << endl;
//				break;
//					}
//			case(2):{
//				cout << "Total amount of deposits = " << stat.numDeposits
//					<< endl << endl;
//				break;
//					}
//			case(3):{
//				stat.update(bank);
//				cout << "Average current balance = " << stat.avgBalance
//					<< endl << endl;
//				break;
//					}
//			default:{
//				throw BAD_INPUT;
//					}
//			}
//			success = true;
//			break;
//				 }
//
//				 //quit
//		case (0):{
//			success = true;
//			break;
//				 }
//		default: success = false; throw BAD_INPUT;
//		}
//	} catch (string s){
//		cout << endl << s << endl << endl;
//		success = false;
//	}
//	//default invalid choice, return success = false
//	return success;
//}
//
//
////make a new customer and cout their account num and pin
//void newCustomer(string name){ //TODO
//	int acctNum = 0;
//	int pin = 0;
//	//generate acct num
//	while ( !acctNum || (std::find(bank.acctNums.begin(), bank.acctNums.end(), acctNum)
//		!= bank.acctNums.end())){  //while acctNum is 0 or it's already in acctNums
//			acctNum = rand() % 900000 + 100000 ;
//	}
//	//^loop random until num is not in bank.acctNums
//	//generate pin
//	pin = rand() %  9000 + 1000;
//	//bank.addCustomer with acct num, pin and name
//	bank.addCustomer(name, acctNum, pin);
//	//cout acct num and pin num
//	cout << "Customer added with account number " << acctNum
//		<< " and PIN " << pin << endl << endl;
//}
//
////prints main menu
//void printMenu(){
//	cout << "1. Open a new account" << endl
//		<< "2. Close the existing account" << endl
//		<< "3. Withdraw money (up to $500)" << endl
//		<< "4. Deposit money" << endl
//		<< "5. Account balance" << endl
//		<< "6. List the last 10 transactions" << endl
//		<< "7. Statistical information" << endl 
//		<< "0. Quit" << endl << endl;
//}
//
////verifies acctNum and pin and returns customer
//Customer* accessCustomer(){
//	bool success = false;
//	Customer* customer;
//	int acctNum;
//	int pin;
//	acctNum = TUI::getIntWithPrompt("Enter account number: ");
//	pin = TUI::getIntWithPrompt("Enter pin number: ");
//	customer = bank.getCustomer(acctNum, pin);
//	success = true;
//	return customer;
//}
//
////truncate to 2 decimal places
//double roundTo2Dec(double num){
//	double result = (int)(num*100.0)/100.0;
//	return result;
//}
//
//void readFile()
//{
//	vector<string> file;
//	string temp;
//	ifstream infile("customerAccountFile.txt");
//
//	while(!infile.eof())
//	{
//		getline(infile, temp);
//		file.push_back(temp);
//	}
//	infile.close();
//
//	for(int x = 0; x < file.size(); x=x+6)
//	{
//		string nameL = file[x];
//		string name = nameL.substr(10);
//
//		string accL = file[(x+1)];
//		int acc;
//		istringstream(accL.substr(8)) >> acc;
//		bank.acctNums.push_back(acc);
//
//		string pinL = file[(x+2)];
//		int pin;
//		istringstream (pinL.substr(4)) >> pin;
//
//		string balL = file[(x+3)];
//		double bal;
//		istringstream (balL.substr(8)) >> bal;
//
//		Customer c = Customer(name, acc, pin, bal);
//		bank.customerList.push_back(c);
//		stat.totalCustomers++;
//	}
//}