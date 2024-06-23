#include "loginSystem.h"
#include "sha256.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h>
#include <regex>

#define LOGIN    1
#define REGISTER 2
#define RESET    3
#define EXIT     4

using namespace std;

std::string hashPassword(const std::string& password) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, reinterpret_cast<const BYTE*>(password.c_str()), password.size());
    
    BYTE hash[SHA256_BLOCK_SIZE];
    sha256_final(&ctx, hash);

    // Converter o hash para uma string hexadecimal
    std::stringstream ss;
    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    
    return ss.str();
}

//This function creates a Table in the Database
bool createTable(sqlite3* db) {
    const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "email TEXT NOT NULL UNIQUE,"
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

//This function registers a new user
void registerUser(const std::string& email, std::string& password) {
    sqlite3* db;
    
    int db_res = sqlite3_open("users.db", &db);
    if(db_res != SQLITE_OK) {
        std::cerr << "Error opening Database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Verifies if User already exists
    std::string checkUserSql = "SELECT COUNT(*) FROM users WHERE email = ?;";
    sqlite3_stmt* checkUserStmt;
    if (sqlite3_prepare_v2(db, checkUserSql.c_str(), -1, &checkUserStmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing verification statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_text(checkUserStmt, 1, email.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(checkUserStmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(checkUserStmt, 0);
        if (count > 0) {
            std::cerr << "Error: User already exists!" << std::endl;
            sqlite3_finalize(checkUserStmt);
            sqlite3_close(db);
            return;
        }
    } else {
        std::cerr << "Error verifying User existence: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(checkUserStmt);
        sqlite3_close(db);
        return;
    }
    sqlite3_finalize(checkUserStmt);

    //Inserts New User
    std::string sql = "INSERT INTO users (email, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    std::string hashedPassword = hashPassword(password);
    sqlite3_bind_text(stmt, 2, hashedPassword.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error executaing statement: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Registered with sucess!" << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

//This function checks if the input credentials match the DB information for the login
bool authenticateUser(const std::string& email, std::string& password) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    int db_res = sqlite3_open("users.db", &db);
    if (db_res != SQLITE_OK) {
        std::cerr << "Error opening Database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "SELECT password FROM users WHERE email = ?;";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);

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

bool resetPassword(const std::string& email, std::string& newPassword) {
    sqlite3* db;
    sqlite3_stmt* resetPasswordStmt;
    sqlite3_stmt* checkUserStmt;

    int db_res = sqlite3_open("users.db", &db);
    if (db_res != SQLITE_OK) {
        std::cerr << "Error opening Database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Verifies if User already exists
    std::string checkUserSql = "SELECT COUNT(*) FROM users WHERE email = ?;";
    
    if (sqlite3_prepare_v2(db, checkUserSql.c_str(), -1, &checkUserStmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing verification statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    sqlite3_bind_text(checkUserStmt, 1, email.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(checkUserStmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(checkUserStmt, 0);
        if (count == 0) {
            std::cerr << "Error: User does not exist!" << std::endl;
            sqlite3_finalize(checkUserStmt);
            sqlite3_close(db);
            return false;
        }
    } else {
        std::cerr << "Error verifying User existence: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(checkUserStmt);
        sqlite3_close(db);
        return false;
    }

    //Resets User Password
    std::string sql = "UPDATE users SET password = ? WHERE email = ?;";
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &resetPasswordStmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error prepararing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    std::string hashedPassword = hashPassword(newPassword);
    sqlite3_bind_text(resetPasswordStmt, 1, hashedPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(resetPasswordStmt, 2, email.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(resetPasswordStmt) != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(resetPasswordStmt);
        sqlite3_close(db);
        return false;
    }

    std::cout << "Password Reset with Success" << std::endl;
    sqlite3_finalize(resetPasswordStmt);
    sqlite3_close(db);
    return true;
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
bool loginSystem() {

    if (!createDatabase()) {
        std::cerr << "Failed initializing the Database." << std::endl;
        return false;
    }

    int option; 
    char ch;
    const std::regex pattern(R"([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))");

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
            case LOGIN:
                std::cout << "Starting Login..."<< endl;
                std::cout << "Enter email: ";
                std::cin  >> email;
                while(!(std::regex_match(email, pattern))) {
                    std::cout << "Email is not valid!"<< endl;
                    std::cout << "Enter a valid email: ";
                    std::cin  >> email;
                }
                std::cout << "Enter Password: ";
                password = getPassword();
                if (authenticateUser(email, password)) {
                    std::cout << "Login successful!" << std::endl;
                    return true;
                } else {
                    std::cout << "Login failed. Please check your email and password." << std::endl;
                    break;
                }

            case REGISTER:
                std::cout << "Starting Registration..."<< endl;
                std::cout << "Enter Email: ";
                std::cin  >> email;
                while(!(std::regex_match(email, pattern))) {
                    std::cout << "Email is not valid!"<< endl;
                    std::cout << "Enter a valid email: ";
                    std::cin  >> email;
                }
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
                registerUser(email, password);
                break;
            
            case RESET:
                std::cout << "Starting Password Reset..."<< endl;
                std::cout << "Enter email: ";
                std::cin  >> email;
                while(!(std::regex_match(email, pattern))) {
                    std::cout << "Email is not valid!"<< endl;
                    std::cout << "Enter a valid email: ";
                    std::cin  >> email;
                }
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
                resetPassword(email, password);
                break;

            case EXIT:
                 std::cout << "Exiting..."<< endl;
                 break;
            
            default:
                std::cout << "Invalid option! Try again." << std::endl;
                break;
        }

    } while ((option != 4));

    return false;
}