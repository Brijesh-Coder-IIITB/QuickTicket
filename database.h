#ifndef DATABASE_H
#define DATABASE_H

void initialise_db();
void addUser(const char *username, const char *password, const char *name, int age,const char *gender,const char *email);
void print_all_tables();
sqlite3_stmt *execute_command(char *query);
int get_user_id(const char *username);
int user_or_email_exists(const char *username, const char *email);
int validate_user_admin(const char *username, const char *password, int u_a);
void close_db();

#endif