#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "database.h"
#include <string.h>



sqlite3 *db;
char *err_msg = 0;
int rc;

void check_sqlite_error() {
    if (rc != SQLITE_OK && rc != SQLITE_DONE && rc != SQLITE_ROW) {
        fprintf(stderr, "SQLite error (%d): %s\n", rc, sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(1);
    }
}

void initialise_db(){
    rc = sqlite3_open("database.db", &db);
    check_sqlite_error();

    const char *sql_users_table = "CREATE TABLE IF NOT EXISTS Users(id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, password TEXT, name TEXT,age INT, gender TEXT, email TEXT);";
    rc = sqlite3_exec(db, sql_users_table, 0, 0, &err_msg);
    check_sqlite_error();

    const char *sql_trains_table = "CREATE TABLE IF NOT EXISTS Trains ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "t_name TEXT, "
                    "s_station TEXT, "
                    "e_station TEXT, "
                    "stations TEXT, "
                    "coach_cap INTEGER, "
                    "sleeper_seats INTEGER, "
                    "ac3_seats INTEGER, "
                    "ac2_seats INTEGER, "
                    "ac1_seats INTEGER, "
                    "general_seats INTEGER);";
    rc = sqlite3_exec(db, sql_trains_table, 0, 0, &err_msg);
    check_sqlite_error();

    const char *sql_train_schdedules_table = "CREATE TABLE IF NOT EXISTS TrainSchedules ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "t_id INTEGER, "
                    "dep_time TEXT, "
                    "arr_time TEXT, "
                    "dep_date TEXT, "
                    "arr_date TEXT, "
                    "moving INTEGER, "
                    "sleeper_count INTEGER, "
                    "ac3_count INTEGER, "
                    "ac2_count INTEGER, "
                    "ac1_count INTEGER, "
                    "general_count INTEGER, "
                    "canceled TEXT);";
    rc = sqlite3_exec(db, sql_train_schdedules_table, 0, 0, &err_msg);
    check_sqlite_error();


    const char *sql_reservation_table = "CREATE TABLE IF NOT EXISTS Reservation ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "t_id INTEGER, "
                    "sch_id INTEGER, "
                    "user_id INTEGER, "
                    "reservaton_date TEXT, "
                    "seats TEXT, "
                    "passengers TEXT, "
                    "status TEXT);";
    rc = sqlite3_exec(db, sql_reservation_table, 0, 0, &err_msg);
    check_sqlite_error();



}

void addUser(const char *username, const char *password, const char *name, int age,const char *gender,const char *email) {
    const char *sql = "INSERT INTO Users (username, password, name, age, gender, email) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    check_sqlite_error();

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, age);
    sqlite3_bind_text(stmt, 5, gender, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, email, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    check_sqlite_error();

    sqlite3_finalize(stmt);
    check_sqlite_error();
}

int user_or_email_exists(const char *username, const char *email) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT 1 FROM Users WHERE username = ? OR email = ? LIMIT 1;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 1;
    } else {
        sqlite3_finalize(stmt);
        return 0;
    }
}

int validate_user_admin(const char *username, const char *password, int u_a) {
    sqlite3_stmt *stmt;
    char sql[200];
    char table[50];
    if (u_a == 1){
        strcpy(table, "Users");
    }
    else if (u_a == 2){
        strcpy(table, "Admin");
    } 
    sprintf(sql, "SELECT * FROM %s WHERE username = ? AND password = ?", table);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    check_sqlite_error();

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 1;
    } else {
        sqlite3_finalize(stmt);
        return 0;
    }
}

int get_user_id(const char *username) {
    const char *query = "SELECT id FROM Users WHERE username = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    check_sqlite_error();

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    int user_id = -1;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        user_id = sqlite3_column_int(stmt, 0);
    } else if (rc == SQLITE_DONE) {
        printf("No user found with username '%s'.\n", username);
    }

    sqlite3_finalize(stmt);
    return user_id;
}

void print_table_data(const char *table_name) {
    sqlite3_stmt *stmt;
    char sql[256];

    snprintf(sql, sizeof(sql), "SELECT * FROM %s;", table_name);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    check_sqlite_error();

    printf("\nTable: %s\n", table_name);
    printf("--------------------------------------------------\n");

    int col_count = sqlite3_column_count(stmt);
    for (int i = 0; i < col_count; i++) {
        printf("%s\t", sqlite3_column_name(stmt, i));
    }
    printf("\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < col_count; i++) {
            const char *col_text = (const char *)sqlite3_column_text(stmt, i);
            printf("%s\t", col_text ? col_text : "NULL");
        }
        printf("\n");
    }

    sqlite3_finalize(stmt);
}

void close_db(){
    sqlite3_close(db);
}

void print_all_tables() {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to retrieve tables: %s\n", sqlite3_errmsg(db));
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *table_name = (const char *)sqlite3_column_text(stmt, 0);
        if (table_name) {
            print_table_data(table_name);
        }
    }

    sqlite3_finalize(stmt);
}

sqlite3_stmt *execute_command(char *query){
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    check_sqlite_error();
    
    return stmt;
}
