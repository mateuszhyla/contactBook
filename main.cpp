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
    int contactsOwnerId;
    string name;
    string surname;
    string phoneNumber;
    string mailAddress;
    string address;
};

struct User
{
    int id;
    string nickname;
    string password;
};

string convertIntToString (int);
string convertContactStructureToSingleLine(Contact);
string convertUserStructureToSingleLine(User);
int readContactsDataFromFile(int, vector <Contact>&);
vector <User> readUsersDataFromFile();
void showConsoleTextInRed(string);
char displayMainMenu();
void registerNewUser(vector <User>&);
User logIn (vector <User>&);
void updateUsersDataBase(User, string);
char manageContactsMenu (User);
char displayContactsMenu(User);
void addContactToContactsBookWindow(int, int);
void showAllContacts(vector <Contact>&, int);
void modifyContact (int, vector <Contact>&, int);
void removeContact (int, vector <Contact>&);
void showDetailedInfo (string, vector <Contact>&, int);
void changePassword (User&);
void updateContactsBook(Contact, string);
char displaySearchMenu();
void searchByName(vector <Contact>&, int);
void searchBySurname(vector <Contact>&, int);

int main()
{
    vector <User> userDataBase;
    User loggedUser;
    char mainMenuUserSelection;
    char contactsMenuUserSelection;


mainMenuWindow:
    userDataBase = readUsersDataFromFile();
    mainMenuUserSelection = displayMainMenu();

    switch (mainMenuUserSelection)
    {

    case '1':
        registerNewUser(userDataBase);
        goto mainMenuWindow;
        break;

    case '2':

        loggedUser = logIn(userDataBase);
        if (loggedUser.id != 0)
        {
            contactsMenuUserSelection = manageContactsMenu(loggedUser);
        }
        goto mainMenuWindow;
        break;

    case '0':
        exit(0);
    }


    return 0;
}

string convertIntToString (int id)
{
    ostringstream ss;
    ss << id;
    string str =ss.str();

    return str;
}

vector <User> readUsersDataFromFile()
{
    vector <User> existingUsers;
    fstream usersDataBase;
    usersDataBase.open("usersDataBase.txt", ios::in);

    vector <string> dividedUser;
    User userInfo;
    string user;

    while (getline(usersDataBase, user))
    {
        stringstream ss(user);
        dividedUser.clear();
        while( ss.good() )
        {
            string sub;
            getline( ss, sub, '|' );
            dividedUser.push_back(sub);
        }
        userInfo.id = atoi(dividedUser[0].c_str());
        userInfo.nickname = dividedUser[1];
        userInfo.password = dividedUser[2];

        existingUsers.push_back(userInfo);
    }
    usersDataBase.close();
    return existingUsers;
}

void showConsoleTextInRed(string header)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 12);
    cout << header;
    SetConsoleTextAttribute(console, 15);
}

char displayMainMenu()
{
    char userSelection;

    system("cls");

    showConsoleTextInRed(".:Contacts book application:.");
    cout << endl << endl;
    cout << "1. Register" << endl;
    cout << "2. Log in" << endl;
    cout << "0. Close application" << endl << endl;
    cout << "Select a number to start an action..." << endl;

    do
    {
        userSelection = getch();
    }
    while ((userSelection != '1') && (userSelection != '2') && (userSelection != '0'));

    return userSelection;
}

void registerNewUser(vector <User> &existingUsers)
{
    string nickName, password;
    User newUser;
    int i = 0, contactId = 1;

    cout << endl << "Type new username: ";
    cin >> nickName;

    while (i < existingUsers.size())
    {
        if (existingUsers[i].nickname == nickName)
        {
            cout << "Username already exists. Type different username: ";
            cin >> nickName;
            i = 0;
        }
        else
        {
            i++;
        }
    }

    cout << "Type a password: ";
    cin >> password;


    if (existingUsers.size()>0)
    {
        contactId = existingUsers.back().id + 1;
    }
    newUser.id = contactId;
    newUser.nickname = nickName;
    newUser.password = password;

    existingUsers.push_back(newUser);
    updateUsersDataBase(newUser, "add");

    cout << endl << "User's account has been created!" << endl;
    Sleep(1500);
}

User logIn (vector <User> &allUsers)
{
    string nickName, password;
    User matchedUser;
    matchedUser.id = 0;
    system("cls");
    cout << "Write your username: ";
    cin >> nickName;
    int i = 0;

    while (i < allUsers.size())
    {
        if (allUsers[i].nickname == nickName)
        {
            for (int attempts = 0; attempts < 3; attempts++)
            {
                cout << "Write your password. Login attempts remaining: " << 3-attempts << ": ";
                cin >> password;
                if (allUsers[i].password == password)
                {
                    matchedUser = allUsers[i];
                    cout << "Logged in correctly!";
                    Sleep(1000);
                    return allUsers[i];
                }
            }
            cout << "3 logon attempts failed. Please wait 5 seconds until next try" << endl;
            Sleep(5000);
            return matchedUser;
        }
        i++;
    }
    cout << "There is no such username in database. Check your credentials" << endl;
    Sleep(1500);
    return matchedUser;
}

string convertUserStructureToSingleLine(User userToConvert)
{
    string convertedUser;

    convertedUser = convertIntToString(userToConvert.id) + "|";
    convertedUser += userToConvert.nickname+ "|";
    convertedUser += userToConvert.password + "|";

    return convertedUser;

}

void updateUsersDataBase(User userToUpdate, string action)
{
    string convertedUser = convertUserStructureToSingleLine(userToUpdate);
    string tempLine;
    fstream usersDataBase;
    string userId;
    usersDataBase.open("usersDataBase.txt", ios::in | ios::out | ios::app);

    if (action == "add")
    {
        usersDataBase << convertedUser << endl;
        usersDataBase.close();
    }
    else
    {
        userId = convertIntToString(userToUpdate.id);
        ofstream tempUserDataBase("tempUserDataBase.txt");

        while(usersDataBase >> tempLine)
        {
            if(tempLine.substr(0, userId.size()) != userId)
            {
                tempUserDataBase << tempLine << endl;
            }
            else
            {
                tempUserDataBase << convertedUser << endl;
            }
        }
        usersDataBase.close();
        remove("usersDataBase.txt");
        tempUserDataBase.close();
        rename("tempUserDataBase.txt", "usersDataBase.txt");
    }
}

char manageContactsMenu(User currentUserInfo)
{
    char contactsWindowUserSelection;
    char searchWindowUserSelection;
    vector <Contact> allContacts;
    int contactId;

contactsWindow:
    contactId = readContactsDataFromFile(currentUserInfo.id, allContacts);
    contactsWindowUserSelection=displayContactsMenu(currentUserInfo);

    switch (contactsWindowUserSelection)
    {

    case '1':
        searchWindowUserSelection = displaySearchMenu();

        switch (searchWindowUserSelection)
        {
        case '1':
            searchByName(allContacts, currentUserInfo.id);
            break;

        case '2':
            searchBySurname(allContacts, currentUserInfo.id);
            break;

        default:
            break;
        }
        goto contactsWindow;
        break;

    case '2':
        addContactToContactsBookWindow(currentUserInfo.id, contactId);
        goto contactsWindow;
        break;

    case '3':
        showAllContacts(allContacts, currentUserInfo.id);
        goto contactsWindow;
        break;

    case '4':
        changePassword (currentUserInfo);
        goto contactsWindow;

    case '0':
        return contactsWindowUserSelection;
    }
}

char displayContactsMenu(User loggedUser)
{
    char userSelection;
    system("cls");

    showConsoleTextInRed(".:Manage contacts:.");
    cout << "  User: " << loggedUser.nickname;
    cout << endl << endl;
    cout << "1. Search for contact" << endl;
    cout << "2. Add new contact to your contact list" << endl;
    cout << "3. Show all contacts from your contact list" << endl;
    cout << "4. Change password" << endl;
    cout << "0. Log off" << endl << endl;

    cout << "Select a number to start an action..." << endl;

    do
    {
        userSelection = getch();
    }
    while ((userSelection != '1') && (userSelection != '2') && (userSelection != '3')&& (userSelection != '4') && (userSelection != '0'));

    return userSelection;
}

int readContactsDataFromFile(int userId, vector <Contact> &allContacts)
{
    fstream contactBook;
    contactBook.open("contactBook.txt", ios::in);
    vector <string> dividedContact;
    Contact contactInfo;
    string contact;
    int contactId = 1;
    allContacts.clear();
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
        contactInfo.contactsOwnerId = atoi (dividedContact[1].c_str());
        contactInfo.name = dividedContact[2];
        contactInfo.surname = dividedContact[3];
        contactInfo.phoneNumber = dividedContact[4];
        contactInfo.mailAddress = dividedContact[5];
        contactInfo.address = dividedContact[6];
        contactId = contactInfo.id + 1;
        if (contactInfo.contactsOwnerId == userId)
        {
            allContacts.push_back(contactInfo);
        }
    }
    contactBook.close();
    return contactId;

}

void changePassword (User &currentUserInfo)
{
    string oldPassword, newPassword;
    system ("cls");
    cout << "Type old password: ";

    cin >> oldPassword;
    while (oldPassword != currentUserInfo.password)
    {
        system("cls");
        cout << "Password is incorrect. Please type again: ";
        cin >> oldPassword;
    }
    cout << endl << "Type new password: ";
    cin >> newPassword;

    currentUserInfo.password = newPassword;
    updateUsersDataBase(currentUserInfo, "update");

    cout << endl << "New password has been set" << endl;
    Sleep(1500);
}

void addContactToContactsBookWindow(int userId, int contactId)
{
    Contact newContact;

    system("cls");

    cout << "Fill contact informations:" << endl << endl;

    newContact.id = contactId;
    newContact.contactsOwnerId = userId;

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

    updateContactsBook(newContact, "add");

    cout << endl << "Contact has been added to contacts list" << endl;
    Sleep(1500);
}

void showAllContacts(vector<Contact> &allContacts, int userId)
{
    string userSelection;
    vector <string> availableIDs;
    availableIDs.push_back("0");

    system("cls");
    cout << "Contacts: " << endl << endl;

    for (int i = 0; i < allContacts.size(); i++)
    {
        availableIDs.push_back(convertIntToString(allContacts[i].id));
        cout << allContacts[i].id << ". " << allContacts[i].name << " " << allContacts[i].surname << endl;
    }

    cout << endl << "Press ID No. to edit/see contact details"
         << endl << "Press 0 to go back to main menu" << endl;

    do
    {
        cin >> userSelection;
    }
    while ((find(availableIDs.begin(), availableIDs.end(), userSelection) == availableIDs.end()));

    if (userSelection != "0")
    {
        showDetailedInfo(userSelection, allContacts, userId);
    }

}

string convertContactStructureToSingleLine(Contact contactToConvert)
{
    string convertedContact;

    convertedContact = convertIntToString(contactToConvert.id) + "|";
    convertedContact += convertIntToString(contactToConvert.contactsOwnerId) + "|";
    convertedContact += contactToConvert.name + "|";
    convertedContact += contactToConvert.surname + "|";
    convertedContact += contactToConvert.phoneNumber + "|";
    convertedContact += contactToConvert.mailAddress + "|";
    convertedContact += contactToConvert.address + "|";

    return convertedContact;
}

void updateContactsBook(Contact contactToModify, string action)
{
    string convertedContact = convertContactStructureToSingleLine(contactToModify);
    string tempLine;
    fstream contactBook;
    string contactId;
    contactBook.open("contactBook.txt", ios::in | ios::out | ios::app);

    if (action == "add")
    {
        contactBook << convertedContact << endl;
        contactBook.close();
    }
    else
    {
        contactId = convertIntToString(contactToModify.id);
        convertedContact = convertContactStructureToSingleLine(contactToModify);
        ofstream tempContactBook("tempContactBook.txt");

        while(contactBook >> tempLine)
        {
            if(tempLine.substr(0, contactId.size()) != contactId)
            {
                tempContactBook << tempLine << endl;
            }
            else
            {
                if (action == "remove")
                {
                }
                else if (action == "modify")
                {
                    tempContactBook << convertedContact << endl;
                }
            }
        }
        contactBook.close();
        remove("contactBook.txt");
        tempContactBook.close();
        rename("tempContactBook.txt", "contactBook.txt");
    }
}

char displaySearchMenu()
{
    char userSelection;

    system("cls");
    showConsoleTextInRed(".:Search contact:.");
    cout << endl << endl;
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

void searchByName(vector <Contact> &allContacts, int userId)
{
    string searchedName;
    string userSelection;
    bool matchesFound = false;
    vector <string> availableIDs;
    availableIDs.push_back("0");

    system("cls");
    cout << "Please type a name: ";
    cin >> searchedName;

    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].name == searchedName))
        {
            availableIDs.push_back(convertIntToString(allContacts[i].id));
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
        cin >> userSelection;
    }
    while ((find(availableIDs.begin(), availableIDs.end(), userSelection) == availableIDs.end()));

    if (userSelection != "0")
    {
        showDetailedInfo(userSelection, allContacts, userId);
    }
}

void searchBySurname(vector <Contact> &allContacts, int userId)
{
    string searchedSurname;
    string userSelection;
    bool matchesFound = false;
    vector <string> availableIDs;
    availableIDs.push_back("0");

    system("cls");
    cout << "Please type a surname: ";
    cin >> searchedSurname;

    for (int i = 0; i < allContacts.size(); i++)
    {
        if ((allContacts[i].surname == searchedSurname))
        {
            availableIDs.push_back(convertIntToString(allContacts[i].id));
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
        cin >> userSelection;
    }
    while ((find(availableIDs.begin(), availableIDs.end(), userSelection) == availableIDs.end()));

    if (userSelection != "0")
    {
        showDetailedInfo(userSelection, allContacts, userId);
    }

}

void showDetailedInfo (string contactId, vector <Contact>& allContacts, int userId)
{
    char userSelection;
    int convertedId = atoi(contactId.c_str());

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
        modifyContact(convertedId, allContacts, userId);
    }
    else if (userSelection == '2')
    {
        removeContact(convertedId, allContacts);
    }
}

void modifyContact (int contactId, vector <Contact>& allContacts, int userId)
{
    Contact newContact;

    system("cls");
    cout << "Update contact informations:" << endl << endl;

    for (int i = 0; i < allContacts.size(); i++)

    {
        if ((allContacts[i].id == contactId))
        {
            newContact.id = contactId;
            newContact.contactsOwnerId = userId;

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

            break;
        }
    }

    updateContactsBook(newContact, "modify");
    cout << endl << "Contact has been modified" << endl;
    Sleep(1500);
}

void removeContact (int id, vector <Contact>& allContacts)
{
    char userSelection;
    Contact contactToRemove;
    cout << endl << "Are you sure? (y/n)" << endl;
    do
    {
        userSelection = getch();
    }
    while ((userSelection != 'y') && (userSelection != 'n'));


    if (userSelection == 'y')
    {
        for (int i = 0; i < allContacts.size(); i++)
        {
            if ((allContacts[i].id == id))
            {
                contactToRemove = allContacts[i];
                break;
            }
        }

        updateContactsBook(contactToRemove, "remove");
        cout << endl << "Contact has been removed from contacts list" << endl;
        Sleep(1500);
    }

}

