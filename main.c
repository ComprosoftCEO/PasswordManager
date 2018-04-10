#include <PasswordManager.h>
#include <terminal.h>
#include <stdio.h>

//Stores the main public database object
pPM_Obj_t db;

int main() {
    db = new_password_database();

    if (!db) {
        printf("Failed to create database!\n");
        return 1;
    }

    run_terminal();
    return 0;
}
