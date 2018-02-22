#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Contact
{
    int id;
    string name;
    string surname;
    string phoneNumber;
    string mailAddress;
    string address;

};
vector <Contact> readDataFromFile();
void showConsoleTextInRed(string);
char displayMainMenu();
char displaySearchMenu();
void addContactToContactsBookWindow(vector <Contact>&);
void searchByName(vector <Contact>&);
void searchBySurname(vector <Contact>&);
void showAllContacts(vector <Contact>&);
void modifyContact (int, vector <Contact>&);
void removeContact (int, vector <Contact>&);
void updateContactsBook(vector <Contact>&);
void showDetailedInfo (char, vector <Contact>&);

int main()
{
    vector <Contact> allContacts;
    char mainWindowUserSelection;
    char searchWindowUserSelection;

    allContacts = readDataFromFile();

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

        addContactToContactsBookWindow(allContacts);
        goto mainWindow;
        break;

    case '3':
        system("cls");
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
    contactBook.open("contactBook_v2.txt", ios::in);

    vector <Contact> contactsFromFile;
    vector <string> dividedContact;
    Contact contactInfo;
    string contact;
    while (getline(contactBook, contact))
    {
        stringstream ss(contact);
        dividedContact.clear();
        while( ss.good() )
        {
            string sub;
            getline( ss, sub, '|' );
            dividedContact.push_back(sub);
        }
        contactInfo.id = atoi(dividedContact[0].c_str());
        contactInfo.name = dividedContact[1];
        contactInfo.surname = dividedContact[2];
        contactInfo.phoneNumber = dividedContact[3];
        contactInfo.mailAddress = dividedContact[4];
        contactInfo.address = dividedContact[5];

        contactsFromFile.push_back(contactInfo);
    }
    return contactsFromFile;

}

void updateContactsBook(vector <Contact> &allContacts)
{
    fstream contactBook;
    contactBook.open("contactBook_v2.txt", ios::out | ios::trunc);

    for (int i = 0; i < allContacts.size(); i++)
        contactBook << allContacts[i].id << "|"
                    << allContacts[i].name << "|"
                    << allContacts[i].surname << "|"
                    << allContacts[i].phoneNumber << "|"
                     << allContacts[i].mailAddress << "|"
                    << allContacts[i].address << "|" << endl;

    contactBook.close();
}

void addContactToContactsBookWindow(vector <Contact> &allContacts)
{
    Contact newContact;
    int contactId = 1;

    if (allContacts.size()>0)
    {
        contactId = allContacts.back().id + 1;
    }

    system("cls");

    cout << "Fill contact informations:" << endl << endl;
    newContact.id = contactId;

    cout << "Name: ";
    cin >> newContact.name;

    cout << "Surname: ";
    cin >> newContact.surname;

    cout << "Phone number: ";
    cin.sync();
    getline(cin, newContact.phoneNumber);

    cout << "Email address: ";
    cin >> newContact.mailAddress;

    cout << "Address: ";
    cin.sync();
    getline(cin, newContact.address);

    allContacts.push_back(newContact);
    updateContactsBook(allContacts);

    cout << endl << "Contact has been added to contacts list" << endl;
    Sleep(1500);
}

char displayMainMenu()
{
    char userSelection;
    system("cls");

    showConsoleTextInRed(".:Contacts book application:.");

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

void searchByName(vector <Contact> &allContacts)
{
    string searchedName;
    char userSelection;
    bool matchesFound = false;
    vector <char> availableIDs;
    availableIDs.push_back('0');

    system("cls");
    cout << "Please type a name: ";
    cin >> searchedName;

    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].name == searchedName))
        {
            availableIDs.push_back((char)(allContacts[i].id)+48);
            matchesFound = true;
            cout << allContacts[i].id << ". " << allContacts[i].name << " " << allContacts[i].surname << endl;
        }
    }
    if(!matchesFound)
    {
        cout << endl << "There is no match. Press 0 to go back to main menu" << endl;
    }
    else
    {
        cout << endl << "Press ID No. to edit/see contact details"
             << endl << "Press 0 to go back to main menu" << endl;
    }

    do
    {
        userSelection = getch();
    }
    while ((find(availableIDs.begin(), availableIDs.end(), userSelection) == availableIDs.end()));

    if (userSelection != '0')
    {
        showDetailedInfo(userSelection, allContacts);
    }
}

void searchBySurname(vector <Contact> &allContacts)
{
    string searchedSurname;
    char userSelection;
    bool matchesFound = false;
    vector <char> availableIDs;
    availableIDs.push_back('0');

    system("cls");
    cout << "Please type a surname: ";
    cin >> searchedSurname;

    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].surname == searchedSurname))
        {
            availableIDs.push_back((char)(allContacts[i].id)+48);
            matchesFound = true;
            cout << allContacts[i].id << ". " << allContacts[i].name << " " << allContacts[i].surname << endl;
        }
    }

    if(!matchesFound)
    {
        cout << endl << "There is no match. Press 0 to go back to main menu" << endl;
    }
    else
    {
        cout << endl << "Press ID No. to edit/see contact details"
             << endl << "Press 0 to go back to main menu" << endl;
    }

    do
    {
        userSelection = getch();
    }
    while ((find(availableIDs.begin(), availableIDs.end(), userSelection) == availableIDs.end()));

    if (userSelection != '0')
    {
        showDetailedInfo(userSelection, allContacts);
    }

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

void showAllContacts(vector<Contact> &allContacts)
{
    char userSelection;
    vector <char> availableIDs;
    availableIDs.push_back('0');

    cout << "Contacts: " << endl << endl;

    for (int i = 0; i < allContacts.size(); i++)
    {
        availableIDs.push_back((char)(allContacts[i].id)+48);
        cout << allContacts[i].id << ". " << allContacts[i].name << " " << allContacts[i].surname << endl;
    }

    cout << endl << "Press ID No. to edit/see contact details"
         << endl << "Press 0 to go back to main menu" << endl;

    do
    {
        userSelection = getch();
    }
    while ((find(availableIDs.begin(), availableIDs.end(), userSelection) == availableIDs.end()));

    if (userSelection != '0')
    {
        showDetailedInfo(userSelection, allContacts);
    }

}

void showDetailedInfo (char id, vector <Contact>& allContacts)
{
    char userSelection;
    int convertedId = id - '0';

    system("cls");
    cout << "Contact details:" << endl << endl;

    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].id == convertedId))
        {
            cout << allContacts[i].name << " " << allContacts[i].surname << ", " << allContacts[i].phoneNumber
                 << ", " << allContacts[i].mailAddress << ", " << allContacts[i].address << endl;
        }
    }
    cout << endl << "Press 1 to edit contact" << endl
         << "Press 2 to remove contact" << endl
         << "Press 0 to go back to main menu" << endl;

    do
    {
        userSelection = getch();
    }
    while ((userSelection != '0') && (userSelection != '1') && (userSelection != '2'));

    if (userSelection == '1')
    {
        modifyContact(convertedId, allContacts);
    }
    else if (userSelection == '2')
    {
        removeContact(convertedId, allContacts);
    }
}

void modifyContact (int id, vector <Contact>& allContacts)
{
    Contact newContact;

    system("cls");
    cout << "Update contact informations:" << endl << endl;

    for (int i = 0; i < allContacts.size(); i++)

    {
        if ((allContacts[i].id == id))
        {
            newContact.id = id;

            cout << "Old name: " << allContacts[i].name << endl << "New name: ";
            cin >> newContact.name;

            cout << "Old surname: " << allContacts[i].surname << endl << "New surname: ";
            cin >> newContact.surname;

            cout << "Old phone number: " << allContacts[i].phoneNumber << endl << "New phone number: ";
            cin.sync();
            getline(cin, newContact.phoneNumber);

            cout << "Old email: " << allContacts[i].mailAddress << endl << "New email address: ";
            cin >> newContact.mailAddress;

            cout << "Old address: " << allContacts[i].address << endl << "New address: ";
            cin.sync();
            getline(cin, newContact.address);

            allContacts[i] = newContact;
        }
    }

    updateContactsBook(allContacts);
    cout << endl << "Contact has been modified" << endl;
    Sleep(1500);
}
void removeContact (int id, vector <Contact>& allContacts)
{

    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].id == id))
        {
            allContacts.erase(allContacts.begin()+i);
        }
    }

    updateContactsBook(allContacts);
    cout << endl << "Contact has been removed from contacts list" << endl;
    Sleep(1500);
}

