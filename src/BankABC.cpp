// --------------------------------
// Final Project
//
// Authors: Samuel Daly,
//          Shize Li,
//          Ahmed Mohammed,
//          Ryan Matte
//
// CSI2372 Fall 2019
// --------------------------------

//************************************************************************
//
//   Project
//   Fall 2019
//
//   Mohamed TALEB
//
// This program reads information about clients and transactions in the following 2 files:
//          - clients.txt
//          - transact.txt
//
// It also allows certain operations on the read data:
//          - Display client bank accounts
//          - Updating client accounts
//
//************************************************************************
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <string>

#include "BankABC.h"

using namespace std;

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
    accountId = type = updateDate = 0;
    clientName = new char[1];
    balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
                                    char * name, long newDate, double newBalance):
                                    accountId(id), type(newType),
                                    updateDate(newDate), balance(newBalance)
{
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
     if (clientName)
     {
          delete [] clientName;
     }
}

inline void BankAccount::setAccountId(long account)
{
     accountId = account;
}

inline void BankAccount::setType(int newType)
{
     type = newType;
}

inline void BankAccount::setClientName(char * name)
{
     if (clientName)
     {
          delete [] clientName;
     }
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
     updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
     balance = newBalance;
}

void BankAccount::print()
{
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << accountId << "\t\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char * name,
                                    long newDate, double balanceArg, int nbyear) :
                                    BankAccount(id, newType, name,
                                    newDate, balanceArg), nbyears(nbyear)
{}

inline void DepositAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

void DepositAccount::print()
{
    Bonus();
    BankAccount::print();
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
  setBalance(getBalance() + (getBalance()* getRate() * (getNbYears())/36000.00));
}



//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char * name,
                                    long newDate, double newBalance, int nbyear,
                                    double newRate) : BankAccount(id, newType,
                                    name, newDate, newBalance), nbyears(nbyear), rate(newRate)
{ }

inline void LoanAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
     rate = newRate;
}

void LoanAccount::print()
{
    BankAccount::print();
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
                                    int codeTr = 01, double amountTr):
                                    accountId(idTr), type(typeTr),
                                    date(dateTr), code(codeTr),
                                    amount(amountTr)
{ }

inline void Transaction::setAccountId(long compteTr)
{
     accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
     type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
     date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
     code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
     amount = amountTr;
}




//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount ** list)
{

  BankAccount * pointer = nullptr;
    for (int i = 0; i < K_SizeMax; i++){
        for (int j = 0; j < K_SizeMax - 1; j++){
            //IF THE ACCOUNTID = 0, BREAK THE LOOP
            if (list[j+1]->getAccountId() == 0){
                break;
            }
            //Check if the account at J is bigger than the account at J+1
            else if (list[j]->getAccountId() > list[j+1]->getAccountId()){
                //DO THE ACCOUNT SWAP
                pointer = list[j];
                list[j] = list [j+1];
                list[j+1] = pointer;
            }
        }
    }

    //Temporary strings
    string accountName1;
    string accountName2;

    //Loop over account to evaluate by name now
    for (int i = 0; i < K_SizeMax; i++){
        for (int j = 0; j < K_SizeMax; j++){
            if (list[j+1]->getAccountId() == 0){
                break;
            }
            else if (list[j]->getType() > list[j+1]->getType()){
                accountName1 = list[j]->getClientName();
                accountName2 = list[j+1]->getClientName();
                if (accountName1 == accountName2){
                    pointer = list[j];
                    list [j] = list[j+1];
                    list [j+1] = pointer;
                }
            }
        }
    }






}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount ** readAccounts()
{
    ifstream inputFile("CLIENTS.TXT");	// Opening the input file
    if (!inputFile)            		// If the file is not found...
    {
        cout << "File not found !!!" << endl;
        exit(0);
    }

    BankAccount ** listAccounts = new BankAccount*[K_SizeMax];
    if (!listAccounts) {
        cout << "Full memory. !!!" << endl;
        return listAccounts;
    }

    BankAccount ** pAccount = listAccounts;

    long accountRead, dateRead;
    int TypeRead, nbyearRead, counter = 0;
    double balanceRead, RateRead;
    char nameRead[60];

    inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
    inputFile.getline(nameRead, 60);

    while (inputFile && (counter < K_SizeMax - 1)){
        // YOU HAVE TO DO SOMETHING FROM HERE !!!

          if (TypeRead == 3) {
               *pAccount = new DepositAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead);
          }
          else if (TypeRead == 4) {
               balanceRead += (balanceRead * nbyearRead * RateRead) / 36000;
               *pAccount = new LoanAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead);
          }
          else {
               *pAccount = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead);
          }

        // UNTIL THIS POINT.

          inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
          inputFile.getline(nameRead, 60);
          pAccount++;
          counter++;
    }
     *pAccount = new BankAccount();
     return listAccounts;
}





//*****************************************************************************************
// Purpose: This function validates whether the transaction code
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
    if ( ( (trans.getCode() == 02) && ( isDepositAccount() || isLoanAccount()) ) ||
         ( (trans.getCode() == 03) && ( isDepositAccount() || isLoanAccount() || isSavingsAccount() ) ) )
      {
       return FALSE;
      }
    else
      {
       return TRUE;
      }

}





//******************************************************************************
// Purpose: This function is used to execute the transaction already performed
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
       {
         if (trans.getCode() == 01)    // Deposit
           {
             setBalance(getBalance() + trans.getAmount());
           }
         else
           { if (trans.getCode() == 02)    // Withdrawal
                {
                  if (getBalance() >= trans.getAmount())
                     { setBalance(getBalance() - (trans.getAmount() + 0.50)); }
                  else {cout << " insufficient balance!! " << endl; }
                }
             else 			// Check
                {
                  if (getBalance() >= trans.getAmount())
                     {
                       setBalance(getBalance() - trans.getAmount());
                     }
                  else {cout << " insufficient balance!! " << endl; }
                }
           }

       }

}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
        if (trans.getCode() == 01)    // Deposit
        {
          setBalance(getBalance() - trans.getAmount());

        }
     }
}





//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt'
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************
void updateAccounts(BankAccount ** listAccounts) {
     ifstream inputFile("TRANSACT.TXT");	// Opening the input file

     // Check if the file opened correctly
     if (!inputFile) {
          cout << "Transaction file not found!";
          return;
     }

	long id, type;
	int date, code;
	double amount;

     // go through the file
	while (inputFile) {

          // read a line in the file
          inputFile >> id >> type >> date >> code >> amount;

          // for each account
		for (int i = 0; i < K_SizeMax; i++) {
               // if reached the end
			if (listAccounts[i]->getAccountId() == 0) {
				break;
			}
               // if account matches the one read from the file
			if (listAccounts[i]->getAccountId() == id && listAccounts[i]->getType() == type) {
                    // do the transaction
				Transaction trans(id, type, date, code, amount);
				listAccounts[i]->executeTransaction(trans);
				break;
			}
		}
	}
}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount ** listAccounts)
{
    cout << endl << endl << endl;

    Bool find[K_SizeMax];
    for(int k = 0; k < K_SizeMax; k++) {find[k] = FALSE;}

    cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
    cout << "                       ------------------------------------------" << endl << endl;

    for ( int i = 0; i < K_SizeMax; i++){
         //Break if last account
          if (listAccounts[i]->getAccountId() == 0){
                break;
            }

         string clName = (*listAccounts[i]).getClientName();
         bool printBalance = FALSE;

         //print headers per client name
         if (!find[i]){
               cout << "        Client Name: " << clName  << endl;
               cout << "Bank Account\t\tType\tUpdate Date\tBalance\t\tNb.Years\tRate" << endl;
               cout << "------------\t\t----\t-----------\t-------\t\t--------\t----" << endl;
               printBalance = TRUE;
         }


          double tb = 0;
          // loop over list members and compare to current memeber
          for (int j =i ; j< K_SizeMax; j++){

               if (listAccounts[j]->getAccountId() == 0){
                break;
               }

              if(!find[j]){
                   string clName2 = (*listAccounts[j]).getClientName();
                   if(clName2 == clName){

                        int type = (*listAccounts[j]).getType();

                         // define bank account pointer according to type
                        BankAccount *p = listAccounts[j];
                        if(type == 4){
                             LoanAccount *p = dynamic_cast <LoanAccount*> (listAccounts[j]);
                        } else if (type == 3) {
                             DepositAccount *p = dynamic_cast <DepositAccount*> (listAccounts[j]);
                        }



                        (*p).print();
                        cout << endl;
                        find[j] = TRUE; // skip this account in the next loop

                         // calculating the total balance
                        double bl = (*p).getBalance();
                        tb += bl;
                   }
              }

         }
         //printing the total balance
         if(printBalance){
               cout <<"          TOTAL ACCOUNTS: "<< tb << endl;
               cout << endl << endl;
         }
    }
}




int main()
{
    BankAccount ** list = readAccounts();
    sortAccounts(list);
    displayAccounts(list);
    updateAccounts(list);
    cout << endl << endl;
    cout << "               ************************************************" << endl;
    cout << "               *            REDISPLAYING AFTER UPDATE         *" << endl;
    cout << "               ************************************************" << endl;
    displayAccounts(list);
    cout << endl;

	system("PAUSE");
	return 0;
}
