#include "loginSystem.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <regex>

using namespace std;

std::string hashPassword(const std::string& password) {
    return "hashed_" + password;
}

//This function creates a Table in the Database
bool createTable(sqlite3* db) {
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "email TEXT NOT NULL UNIQUE,"
                      "username TEXT NOT NULL UNIQUE, "
                      "password TEXT NOT NULL);";
    char* errorMessage;
    int db_res = sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage);

    if (db_res != SQLITE_OK) {
        std::cerr << "Error creating Table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}

//This function creates the Database
bool createDatabase() {
    sqlite3* db;
    int db_res = sqlite3_open("users.db", &db);
    if (db_res != SQLITE_OK)  {
        std::cerr << "Error opening Database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!createTable(db)) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

//This function checks if the input credentials match the DB information for the login
bool authenticateUser(const std::string& username, std::string& password) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    int db_res = sqlite3_open("users.db", &db);
    if (db_res != SQLITE_OK) {
        std::cerr << "Error opening Database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "SELECT password FROM users WHERE username = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        std::string storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

        if (storedPassword == hashPassword(password)) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return true;
        } else {
            std::cerr << "Incorrect Password" << std::endl;
        }
    } else {
        std::cerr << "User does not exist" << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

// This functions receives the User Input for Password
std::string getPassword() {
    std::string password;
    char ch;

    while ((ch = _getch()) != '\r') {
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                std::cout << "\b \b";
                password.pop_back();
            }
        } else if (ch == 27) {
            std::cout << "\nPassword input cancelled.\n";
            password.clear();
            break;
        } else {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
}

// This functions is the Menu Routine that allows the user to access the platform
void loginSystem() {

    if (!createDatabase()) {
        std::cerr << "Failed initializing the Database." << std::endl;
        return;
    }

    int option; 
    char ch;
    const std::regex pattern(R"([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))");

    std::string username;
    std::string password;
    std::string password_aux;
    std::string email;
    
    
    do {
        std::cout << "____________________________________________________";
        std::cout << endl << endl << endl;
        std::cout << "              Welcome to the Login Page             ";
        std::cout << endl << endl << endl;
        std::cout << "___________________    MENU    _____________________";
        std::cout << endl << endl;

        std::cout << "|1 - LOGIN          |"<< endl;
        std::cout << "|2 - REGISTER       |"<< endl;
        std::cout << "|3 - RESET PASSWORD |"<< endl;
        std::cout << "|4 - EXIT           |"<< endl;
        std::cout << endl << endl;
        std::cout << "Enter your option: ";

        std::cin  >> option;
        
        switch (option) {
            case 1:
                std::cout << "Starting Login..."<< endl;
                std::cout << "Enter Username: ";
                std::cin  >> username;
                std::cout << "Enter Password: ";
                password = getPassword();
                if (authenticateUser(username, password)) {
                    std::cout << "Login successful!" << std::endl;
                } else {
                    std::cout << "Login failed. Please check your username and password." << std::endl;
                }
                break;

            case 2:
                std::cout << "Starting Registration..."<< endl;
                std::cout << "Enter Email: ";
                std::cin  >> email;
                while(!(std::regex_match(email, pattern))) {
                    std::cout << "Email is not valid!"<< endl;
                    std::cout << "Enter a valid email: ";
                    std::cin  >> email;
                }
                std::cout << "Enter Username: ";
                std::cin  >> username;
                std::cout << "Enter Password: ";
                password = getPassword();
                std::cout << "Re-Enter Password: ";
                password_aux = getPassword();
                while (password != password_aux) {
                    std::cout << "Passwords do not match!"<< endl;
                    std::cout << "Enter Password: ";
                    password = getPassword();
                    std::cout << "Re-Enter Password: ";
                    password_aux = getPassword();
                }
                break;
            
            case 3:
                std::cout << "Starting Password Reset..."<< endl;
                std::cout << "Enter New Password: ";
                password = getPassword();
                std::cout << "Re-Enter New Password: ";
                password_aux = getPassword();
                while (password != password_aux) {
                    std::cout << "Passwords do not match!"<< endl;
                    std::cout << "Enter New Password: ";
                    password = getPassword();
                    std::cout << "Re-Enter New Password: ";
                    password_aux = getPassword();
                }

                std::cout << "Password Reset with Sucess!";
                break;

            case 4:
                 std::cout << "Exiting..."<< endl;
                 break;
            
            default:
                std::cout << "Invalid option! Try again." << std::endl;
                break;
        }

    } while (option != 4);
}