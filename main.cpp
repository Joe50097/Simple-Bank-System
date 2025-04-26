#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <ctime>
#include <cmath>

using namespace std;

const int Max_Accounts = 100;
string name1[Max_Accounts]; //first name
string name2[Max_Accounts]; //last name
double balance[Max_Accounts]; //money in acc
int AccountNb[Max_Accounts]; //account nb in array
int AccountCounter = 0; 
int pins[Max_Accounts]; //pins for each acc
int search;

int GenerateAccount() { //random number generator

	int AccNum = rand() % 900000 + 100000;
	return AccNum;
} 
int AddAccount(string InputName1, string InputName2, double InputBalance, int pin, int AccNum) { //takes names and balance and pin and adds to array
	if (AccountCounter < Max_Accounts) {
		name1[AccountCounter] = InputName1;
		name2[AccountCounter] = InputName2;
		balance[AccountCounter] = InputBalance;
		AccountNb[AccountCounter] = AccNum;
		pins[AccountCounter] = pin;
		cout << "Welcome " << name1[AccountCounter] << " " << name2[AccountCounter] << ", your Initial Balance is: " << balance[AccountCounter] << endl;
		cout << "Your account number is: ";
		AccountCounter++;
		return AccNum;
	}
	else {
		cout << "Account limit reached.";
		return -1;
	}
	} 
int BinarySearch(int accountNumber) {
	int low = 0;
	int high = AccountCounter - 1;

	while (low <= high) {
		int mid = low + (high - low) / 2;

		if (AccountNb[mid] == accountNumber) { //to check if acc was the middle
			return mid; 
		}
		else if (AccountNb[mid] < accountNumber) {
			low = mid + 1;
		}
		else { // if [mid] > accountNumber
			high = mid - 1;
		}
	}

	return -1; // Account number not found
}
//saves acc
void SaveAccounts() {
	ofstream outFile("accounts.txt");
	if (outFile.is_open()) {
		for (int i = 0; i < AccountCounter; i++) {
			outFile << name1[i] << " " << name2[i] << " " << balance[i] << " " << AccountNb[i] << " " << pins[i] << endl;
		}
		outFile.close();
	}
} //saves arrays
//loads acc at the start
void LoadAccounts() {
	ifstream inFile("accounts.txt"); if (inFile.is_open()) {
		AccountCounter = 0;
		while (inFile >> name1[AccountCounter] >> name2[AccountCounter] >> balance[AccountCounter] >> AccountNb[AccountCounter] >> pins[AccountCounter]) 
		{ 
			AccountCounter++; 
			if (AccountCounter >= Max_Accounts) { 
				break; } 
		} 
		inFile.close(); 
		cout << "Account details loaded from accounts.txt" << endl; 
	} 
	else { 
		cout << "No saved account data found." << endl; 
	} 
} //loads arrays
// deposit
double Deposit(int accountNumber,int pin, double amount) {
	search = BinarySearch(accountNumber);
;	if (search != -1) {  
		if (pins[search] == pin) {
				balance[search] += amount;
				cout << "Deposited " << amount << " into account number " << accountNumber << endl;
				cout << "New balance: " << balance[search] << endl;
				return balance[search];
		}
		else {
				cout << "Incorrect Pin!";
				return -1;
		}
		
	} 
	cout << "Account number " << accountNumber << " not found." << endl; 
	return -1; 
}

// withdraw function 
double Withdraw(int accountNumber, int pin, double amount) {
	search = BinarySearch(accountNumber);
	if (search != -1) {	
		if (pins[search] == pin) {
			if (amount < 0) {
					cout << "Error: Withdraw amount can't be negative." << endl;
					return -1;
				}
			if (balance[search] >= amount) {
					balance[search] -= amount;
					cout << "Successfully withdrew " << amount << " from account number " << accountNumber << endl;
					cout << "Remaining balance: " << balance[search] << endl;
					return balance[search];
				}
			else {
					cout << "Error, insufficient balance for this withdraw." << endl;
					return -1;
				}
			}
		else {
				cout << "Error, incorrect PIN." << endl;
				return -2;
			}
		
	}
	cout << "Error, account number not found." << endl;
	return -3;
}

// transfer function 
void Transfer(int Sender_Account_Number, int Sender_Pin, int Receiver_Account_Number, double Transfer_Amount) {
	int SenderAcc = BinarySearch(Sender_Account_Number);
	int ReceiverAcc = BinarySearch(Receiver_Account_Number);
	// validation lal sender account
	if (SenderAcc == -1) {
		cout << "Error, sender's account number not found." << endl;
		return;
	}
	if (pins[SenderAcc] != Sender_Pin) {
		cout << "Error: Incorrect PIN for the sender's account." << endl;
		return;
	}

	// validation lal receiver account
	if (ReceiverAcc == -1) {
		cout << "Error, receiver's account number not found." << endl;
		return;
	}

	// validation lal transfer amount
	if (Transfer_Amount < 0) {
		cout << "Error, transfer amount cannot be negative." << endl;
		return;
	}
	if (balance[SenderAcc] < Transfer_Amount) {
		cout << "Error, insufficient balance in the sender's account." << endl;
		return;
	}

	// heda to process the transfer
	balance[SenderAcc] -= Transfer_Amount;
	balance[ReceiverAcc] += Transfer_Amount;

	cout << "Successfully transferred $" << Transfer_Amount << " from account "
		<< Sender_Account_Number << " to account " << Receiver_Account_Number << "." << endl;
	cout << "New balance for sender's account: $" << balance[SenderAcc] << endl;
	cout << "New balance for receiver's account: $" << balance[ReceiverAcc] << endl;
}

// function to view account + calculating interest over n years
double ViewAccount(int accountNumber, int pin) {
	search = BinarySearch(accountNumber);
	if (search != -1) {
		if (pins[search] == pin) { // to check if the pin houwe correct
				cout << "Account Details:" << endl;
				cout << "First Name: " << name1[search] << endl;
				cout << "Last Name: " << name2[search] << endl;
				cout << "Account Balance in $: " << balance [search];
				return balance[search];
			}
		else {
				cout << "Error, incorrect PIN." << endl;
				return -1;
		}
		
	}
	cout << "Error, account number not found." << endl;
	return -1;
}
double CompoundInterest(double balance, double AnnualRate, int timesCompounded, double years) {
	return balance * pow(1 + AnnualRate / timesCompounded, timesCompounded * years);
}

//view all accounts
void ViewAllAccounts() {
	if (AccountCounter == 0) {
		cout << "No accounts available to display." << endl;
		return;
	}
	// hon mna3moul display lal account numbers
	cout << "All Account Numbers:" << endl;
	for (int i = 0; i < AccountCounter; i++) {
		cout << AccountNb[i] << endl;
	}
}

// function to change pin
void ChangePin(int accountNumber, int pin, int New_Pin) {
	search = BinarySearch(accountNumber);
	if (search != -1) {
		if (pins[search] == pin) { // check and validate the old pin
				if (New_Pin >= 100 && New_Pin <= 999) { // validate the new pin to make sure it's 3 digits
					pins[search] = New_Pin; // to update the pin
					cout << "PIN successfully changed for account number " << accountNumber << endl;
				}
				else {
					cout << "Error, new PIN must be a 3-digit number." << endl;
				}
				return;
		}
		else {
				cout << "Error, incorrect old PIN." << endl;
				return;
		}
		
	}
	cout << "Error, account number not found." << endl;
}
void BubbleSort() {
	for (int i = 0; i < AccountCounter - 1; i++) {
		for (int j = 0; j < AccountCounter - 1; j++) {
			if (AccountNb[j] > AccountNb[j + 1]) {
				
				int tempAccNum = AccountNb[j];
				AccountNb[j] = AccountNb[j + 1];
				AccountNb[j + 1] = tempAccNum;

				
				string tempName1 = name1[j];
				name1[j] = name1[j + 1];
				name1[j + 1] = tempName1;

				
				string tempName2 = name2[j];
				name2[j] = name2[j + 1];
				name2[j + 1] = tempName2;

				
				double tempBalance = balance[j];
				balance[j] = balance[j + 1];
				balance[j + 1] = tempBalance;

				
				int tempPin = pins[j];
				pins[j] = pins[j + 1];
				pins[j + 1] = tempPin;
			}
		}
	}
}


int main() {
	int MenuNb;
	string name1, name2;
	double balance, d_w; //d_w is for deposits and withdrawal
	int pin;
	int AccNum;
	int option;
	srand(time(0));	
	LoadAccounts();
	do {	
		BubbleSort();
		cout << "\nPick an Option: " << endl;
		cout << "1- Add Account \n2- Deposit \n3- Withdraw \n4- Transfer \n5- View Account \n6- View All Accounts \n7- Change Pin \n8- Exit " << endl;
		cout << "Enter your option: ";
		cin >> MenuNb;

		while (MenuNb < 1 || MenuNb > 8) {
			cout << "Error, number should be between 1 and 8: ";
			cin >> MenuNb;
		} 

		switch (MenuNb) {

		// case 1
		case 1:
			cout << "Enter Full Name: ";
			cin >> name1;
			cin >> name2;
			cout << "Enter Initial Balance: ";
			cin >> balance;
			while (balance < 0) {
				cout << "Balance can't be negative, enter again: ";
				cin >> balance;
			}
			cout << "Enter 3 digit Account Pin: ";
			cin >> pin;
			while (pin > 999 || pin < 100) {
				cout << "Pin must be 3 digits: ";
				cin >> pin;
			}
			AccNum = GenerateAccount();
			cout << AddAccount(name1, name2, balance, pin, AccNum);
			if (AccNum != -1)
				SaveAccounts();
			break;

		// case 2
		case 2:
			int newBalance;
			cout << "Enter Account Number: "; 
			cin >> AccNum;
			cout << "Enter Pin: ";
			cin >> pin;
			cout << "Enter Deposit Amount: "; 
			cin >> d_w; 
			while (d_w < 0) {
				cout << "Deposit amount can't be negative, enter again: "; 
				cin >> d_w;
			} 
			
			newBalance = Deposit(AccNum, pin, d_w);
			if (newBalance != -1) 
				SaveAccounts();
			break;

		// case 3
		case 3:
			do {
				cout << "Enter Account Number: ";
				cin >> AccNum;
				cout << "Enter PIN: ";
				cin >> pin;
				cout << "Enter Withdrawal Amount: ";
				cin >> d_w;
				while (d_w < 0) {
					cout << "Withdrawal amount can't be negative, enter again: "; // error message if the user put negative balance
					cin >> d_w;
				}
				double result = Withdraw(AccNum, pin, d_w);
				if (result == -1) {
					// for errors
				}
				else if (result == -2 || result == -3) {
					// for errors
				}
				else {
					SaveAccounts();
					break; // withdraw succesfull that's why break so it cancel the loop
				}
			} while (true);
			break;

		// case 4
		case 4:
			int Sender_Account_Number, Receiver_Account_Number, Sender_Pin;
			double Transfer_Amount;

			do {
				// we ask the user (sender) to enter account number and pin
				cout << "Enter your Account Number (Sender): ";
				cin >> Sender_Account_Number;
				cout << "Enter your PIN: ";
				cin >> Sender_Pin;

				// we ask the user to give the account number of the receiver
				cout << "Enter the Account Number of the Receiver: ";
				cin >> Receiver_Account_Number;

				// we ask the amount to transfer
				do {
					cout << "Enter the amount to transfer: ";
					cin >> Transfer_Amount;
					if (Transfer_Amount < 0) {
						cout << "Error ,transfer amount cannot be negative." << endl;
					}
				} while (Transfer_Amount < 0); // for the amount to be positive

				// we call the transfer function
				Transfer(Sender_Account_Number, Sender_Pin, Receiver_Account_Number, Transfer_Amount);
				SaveAccounts();
				// we ask the user if he wants to do another transfer
				do {
					cout << "Do you want to try another transfer? (1 for Yes or 0 for No): ";
					cin >> option;
				} while (option != 1 && option != 0);
				if (option == 0) break;
			} while (true);
			break;

		// case 5
		case 5:
			double annualRate, years;
			cout << "Enter Account Number: ";
			cin >> AccNum;
			cout << "Enter PIN: ";
			cin >> pin;
			// we call the function to view the account
			balance = ViewAccount(AccNum, pin); //the function returns the value, if there was an error its changed to -1
			cout << "\nDo you wish to calculate compound interest? (1 for yes, 0 for no): ";
			cin >> option;
			while (option != 1 && option != 0) {
				cout << "Enter 1 or 0: ";
				cin >> option;
			}
			if (option == 1) {
				if (balance != -1) {
					cout << "\nEnter annual interest rate (as a percentage): ";
					cin >> annualRate;
					cout << "Enter the number of years: ";
					cin >> years;
					double interest = CompoundInterest(balance, annualRate / 100, 1, years);
					cout << "The balance after " << years << " years with an annual interest rate of " << annualRate << "% will be: " << interest << "$" << endl;
				}
			}
			else {
				break;
			}
			break;

		// case 6
		case 6:
			ViewAllAccounts();
			break;

		// case 7
		case 7:
			int New_Pin;
			cout << "Enter Account Number: ";
			cin >> AccNum;
			cout << "Enter your current PIN: ";
			cin >> pin;
			cout << "Enter your new PIN (3 digits): ";
			cin >> New_Pin;

			// calling the function
			ChangePin(AccNum, pin, New_Pin);
			SaveAccounts();
			break;

		}

	} while (MenuNb != 8);

	// exiting animation

	cout << "Exiting the program";
	cout << ".";
	this_thread::sleep_for(chrono::seconds(1));
	cout << ".";
	this_thread::sleep_for(chrono::seconds(1));
	cout << ".";
	this_thread::sleep_for(chrono::seconds(1));

	cout << endl;

	cout << "Have a great day/night <3" << endl;
	this_thread::sleep_for(chrono::seconds(1));

	cout << "Thank you for using our program :)" << endl;
	this_thread::sleep_for(chrono::seconds(1));

	cout << "Program created by the legendary coding duo, the two most amazing humans to ever code: Joe & Paul!" << endl;
	this_thread::sleep_for(chrono::seconds(1));

	return 0;
}
// thank you for reading this :)
// have a nice day/night <3