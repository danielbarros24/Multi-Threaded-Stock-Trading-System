#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <string>
#include <sqlite3.h>

// Funções de login
std::string hashPassword(const std::string& password);
bool createTable(sqlite3* db);
bool createDatabase();
bool registerUser(const std::string& username, const std::string& password);
bool authenticateUser(const std::string& username, std::string& password);
std::string getPassword();
void loginSystem();


#endif // LOGINSYSTEM_H
