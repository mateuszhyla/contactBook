#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <vector>

using namespace std;

struct Contact
{
    int id;
    string name;
    string surname;
    string phoneNumber;
    string mailAdress;
    string adress;

};
vector <Contact> readDataFromFile();
void showConsoleTextInRed(string);
char displayMainMenu();
char displaySearchMenu();
bool searchByName(vector <Contact>);
bool searchBySurname(vector <Contact>);
bool showAllContacts(vector<Contact>);
void saveNewContactToContactsBook(Contact);
Contact addContactToContactsBookWindow(int);


int main()
{
    vector <Contact> allContacts;
    int contactId;
    char mainWindowUserSelection;
    char searchWindowUserSelection;

    allContacts=readDataFromFile();

    mainWindow:
    mainWindowUserSelection=displayMainMenu();

    switch (mainWindowUserSelection)
    {

    case '1':
        searchWindowUserSelection=displaySearchMenu();

        switch (searchWindowUserSelection)
        {
        case '1':
            searchByName(allContacts);
            break;

        case '2':
            searchBySurname(allContacts);
            break;

        default:
            break;
        }
        goto mainWindow;
        break;

    case '2':
        contactId=allContacts.size();
        allContacts.push_back(addContactToContactsBookWindow(contactId));
        goto mainWindow;
        break;

    case '3':
        showAllContacts(allContacts);
        goto mainWindow;
        break;

    case '0':
        exit(0);
    }

    return 0;
}

void showConsoleTextInRed(string header)
{
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(console, 12);
cout << header << endl << endl;
SetConsoleTextAttribute(console, 15);

}

vector <Contact> readDataFromFile()
{
fstream contactBook;
contactBook.open("contactBook.txt", ios::in);

vector <Contact> contactsFromFile;
Contact contactInfo;
string contactId;
while (getline(contactBook, contactId))
    {
        contactInfo.id = atoi(contactId.c_str());
        getline(contactBook, contactInfo.name);
        getline(contactBook, contactInfo.surname);
        getline(contactBook, contactInfo.phoneNumber);
        getline(contactBook, contactInfo.mailAdress);
        getline(contactBook, contactInfo.adress);

        contactsFromFile.push_back(contactInfo);
    }
    return contactsFromFile;

}

void saveNewContactToContactsBook(Contact newContact)
{
fstream contactBook;
contactBook.open("contactBook.txt", ios::out | ios::app);

    contactBook << newContact.id << endl
                << newContact.name << endl
                << newContact.surname << endl
                << newContact.phoneNumber << endl
                << newContact.mailAdress << endl
                << newContact.adress << endl;

    contactBook.close();
}

Contact addContactToContactsBookWindow(int contactId)
{
    system("cls");

    Contact newContact;

    cout << "Fill contact informations:" << endl << endl;
    newContact.id = contactId;
    cout << "Name: ";          cin >> newContact.name;
    cout << "Surname: ";       cin >> newContact.surname;
    cout << "Phone number: ";  cin >> newContact.phoneNumber;
    cout << "Email adress: ";  cin >> newContact.mailAdress;
    cout << "Adress: ";        cin.sync(); getline(cin, newContact.adress);

    saveNewContactToContactsBook(newContact);

    cout << endl << "Contact has been added to contacts list" << endl;
    Sleep(1500);

    return newContact;
}

char displayMainMenu()
{
    char userSelection;
    system("cls");

    showConsoleTextInRed(".:Contacts book aplication:.");

    cout << "1. Search for contact" << endl;
    cout << "2. Add new contact to your contact list" << endl;
    cout << "3. Show all contacts from your contact list" << endl;
    cout << "0. Close application" << endl << endl;
    cout << "Select a number to start an action..." << endl;

    do
    {
        userSelection = getch();
    }
    while ((userSelection != '1') && (userSelection != '2') && (userSelection != '3') && (userSelection != '0'));

    return userSelection;
}

bool searchByName(vector <Contact> allContacts)
{
    system("cls");
    string searchedName;
    char userSelection;
    bool matchesFound = false;
    cout << "Please type a name: ";
    cin >> searchedName;

    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].name == searchedName))
        {
            matchesFound = true;
            cout << allContacts[i].id+1 << ". " << allContacts[i].name << " " << allContacts[i].surname << endl;
        }
    }
    if(!matchesFound)
    {
        cout << endl << "There is no match. Press 0 to go back to main menu" << endl;
    }
    else
        cout << endl << "Press 0 to go back to main menu" << endl;

    do
    {
        userSelection = getch();
    }
    while (userSelection != '0');

    return true;
}

bool searchBySurname(vector <Contact> allContacts)
{
    system("cls");
    string searchedSurname;
    char userSelection;
    bool matchesFound=false;
    cout << "Please type a surname: ";

    cin >> searchedSurname;
    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].surname == searchedSurname))
        {
            matchesFound = true;
            cout << allContacts[i].id+1 << ". " << allContacts[i].name << " " << allContacts[i].surname << endl;
        }
    }
    if(!matchesFound)
    {
        cout << endl << "There is no match. Press 0 to go back to main menu" << endl;
    }
    else
        cout << endl << "Press 0 to go back to main menu" << endl;

    do
    {
        userSelection = getch();
    }
    while (userSelection != '0');

    return true;
}

char displaySearchMenu()
{

    char userSelection;
    system("cls");

    showConsoleTextInRed(".:Search contact:.");

    cout << "1. By name..." << endl;
    cout << "2. By surname..." << endl;
    cout << "0. Back" << endl << endl;
    cout << "Select a number to start an action..." << endl;

do
{
    userSelection = getch();
}
while ((userSelection != '1') && (userSelection != '2') && (userSelection != '0'));

    return userSelection;
}

bool showAllContacts(vector<Contact> allContacts)
{
    char userSelection;
    system("cls");

    cout << "Contacts: " << endl << endl;

    for (int i = 0; i < allContacts.size(); i++)
    {
        cout << allContacts[i].id+1 << ". " << allContacts[i].name << " " << allContacts[i].surname << endl;
    }
    cout << endl << "Press 0 to go back to main menu" << endl;

    do
    {
        userSelection = getch();
    }

    while (userSelection != '0');

    return true;
}