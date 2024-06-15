#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <string>
#include <sqlite3.h>

// Funções de login
std::string hashPassword(const std::string& password);
std::string getPassword();
bool registerUser(const std::string& username, const std::string& password);
void loginSystem();


#endif // LOGINSYSTEM_H
