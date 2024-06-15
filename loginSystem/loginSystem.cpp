#include "loginSystem.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <regex>

using namespace std;

std::string hashPassword(const std::string& password) {
    return "hashed_" + password;
}

std::string getPassword() {
    std::string password;
    char ch;

    while ((ch = _getch()) != '\r') { // '\r' é a tecla Enter no Windows
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                std::cout << "\b \b"; // Move o cursor para trás, apaga o caractere e move o cursor para trás novamente
                password.pop_back();
            }
        } else if (ch == 27) { // Esc para cancelar
            std::cout << "\nPassword input cancelled.\n";
            password.clear();
            break;
        } else {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << std::endl; // Move para a próxima linha após Enter
    return password;
}

/*bool authenticateUser(const std::string& username, std::string& password) {
    sqlite3* db;

}*/

bool registerUser(const std::string& username, std::string& password) {
    sqlite3* db;

    int exit = sqlite3_open("users.db", &db);

    if (exit) {
        std::cerr << "Error opening Database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "INSERT INTO users (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Error preparing the statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    std::string hashedPassword = hashPassword(password);
    sqlite3_bind_text(stmt, 2, hashedPassword.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Erro ao executar statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

void loginSystem() {
    
    int option; 
    char ch;
    const std::regex pattern(R"([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))");

    std::string username;
    std::string password;
    std::string password_aux;
    std::string email;
    
    
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
    
    if (option == 1)
    {
        std::cout << "Starting Login..."<< endl;
        std::cout << "Enter Username: ";
        std::cin  >> username;
        std::cout << "Enter Password: ";
        password = getPassword();
        
    }
    else if (option == 2)
    {
        std::cout << "Starting Registration..."<< endl;
        std::cout << "Enter Email: ";
        std::cin  >> email;
        while(!(std::regex_match(email, pattern))) {
            std::cout << "Email is not valid!"<< endl;
            std::cout << "Enter a valid email: ";
            std::cin  >> email;
        };

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
        };

    }
    else if (option == 3)
    {
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
        };

        std::cout << "Password Reset with Sucess!";
    }
    else if (option == 4)
    {
        std::cout << "Exiting..."<< endl;
    }
}