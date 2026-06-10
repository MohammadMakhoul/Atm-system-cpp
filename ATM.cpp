#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "ClientFile.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enMainMenueOptions { eQucikWithdraw = 1, eNormalWithDraw = 2, eDeposit = 3, eCheckBalance = 4, eExit = 5 };

//Head
void ShowNormalWithDrawScreen();
void Login();
void ShowMainMenue();
sClient Client;
string AccountNumber;
void ShowQuickWithdrawScreen();
void CheckBalanceScreen();
void ShowDepositScreen();

//String Fun & Convert
vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;
}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

//Loading & Saveing From File
vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

//Read Fun
int ReadDepositAmount()
{
    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;
    return Amount;
}
int ReadWithDrawAmont()
{
    int Amount;
    cout << "\nEnter an amount multiple of 5's ? ";
    cin >> Amount;
    while (Amount % 5 != 0)
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    } return Amount;
}
short ReadATMMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
short ReadQuickWithdrawOption()
{
    short Choice = 0;
    while (Choice < 1 || Choice>9)
    {
        cout << "\nChoose what to do from [1] to [9] ? ";
        cin >> Choice;
    } return Choice;
}

//Go Back Screen
void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

//Find Fun
bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}
bool FindClientByAccountNumber(string AccountNumber, string Pin, sClient& Client)
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == Pin)
        {
            Client = C;
            return true;
        }

    }
    return false;

}
//Deposit Fun
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}
void PerfromDepositOption() 
{
    double DepositAmount = ReadDepositAmount();
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(Client.AccountNumber, DepositAmount, vClients); 
    Client.AccountBalance += DepositAmount;
}
short getQuickWithDrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1:
        return  20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

// Perfrom Fun
void PerfromNormalWithdrawOption()
{
    int WithDrawBalance = ReadWithDrawAmont();
    if (WithDrawBalance > Client.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowNormalWithDrawScreen();
        return;
    }
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(Client.AccountNumber, WithDrawBalance * -1, vClients);
    Client.AccountBalance -= WithDrawBalance;
}
void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
    if (QuickWithDrawOption == 9)//exit
        return;
    short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);
    if (WithDrawBalance > Client.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    } vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(Client.AccountNumber, WithDrawBalance * -1, vClients);
    Client.AccountBalance -= WithDrawBalance;
}
void PerfromATMMainMenueOption(enMainMenueOptions ATMMainMenueOptions)
{
    switch (ATMMainMenueOptions)
    {
    case enMainMenueOptions::eQucikWithdraw:
    {
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eNormalWithDraw:
    {
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eCheckBalance:
    {
        system("cls");
        CheckBalanceScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eExit:
    {
        Login();
    }
    }

}

//Show Screen
void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    PerfromDepositOption();
}
void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromATMMainMenueOption((enMainMenueOptions)ReadATMMainMenueOption());
}
void ShowNormalWithDrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
    PerfromNormalWithdrawOption();
}
void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQucik Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is " << Client.AccountBalance;
    PerfromQuickWithdrawOption(ReadQuickWithdrawOption());
}
void CheckBalanceScreen()
{
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    cout << "Your Balance Is " << Client.AccountBalance;


}

bool LoadUserInfo(string AccountNumber , string Pin)
{
    if (FindClientByAccountNumber(AccountNumber, Pin,Client))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;
    string AccountNumber, Pin;
    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";
        if (LoginFaild)
        {
            cout << "Invlaid Account Number/PinCode!\n";
        }
        cout << "Enter Account Number? ";
        cin >> AccountNumber;
        cout << "Enter Pin? ";
        cin >> Pin;
        LoginFaild = !LoadUserInfo(AccountNumber, Pin);
    } while (LoginFaild);
    ShowMainMenue();
}

int main()
{
    Login();
}