#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <string>
#include <sqlite3.h>

// Funções de login
std::string hashPassword(const std::string& password);
bool createTable(sqlite3* db);
bool createDatabase();
bool registerUser(const std::string& email, const std::string& password);
bool authenticateUser(const std::string& email, std::string& password);
bool resetPassword(const std::string& email,std::string& newPassword);
std::string getPassword();
bool loginSystem();


#endif // LOGINSYSTEM_H
