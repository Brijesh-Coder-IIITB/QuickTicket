#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h"
#include "database.h"
#include "utils.h"
#include <unistd.h>
#include <sys/ioctl.h>


int console_width;

void setup_intro();
void user(char*);
void admin(char*);
void u_option1();
void u_option2_3(int option, char *date);
void u_option4(char *username);
void u_option5();
void u_option6_7(int option, char* username);
void u_option9(char *username);
void u_option10(char *username);

void a_option1();
void a_option2_3(int option, int id);
void a_option4();
void a_option5();
void a_option6();
void a_option7();
void a_option8_9();

void print_line(int width, char *colour){
    for (int i = 0; i < width; i++) {
        #ifdef _WIN32
        printf("%s%c", colour, 196);
        #else
        printf("%s\u2500", colour);
        #endif

    }
    printf("\033[0m");
}

void print_table(char *headers[], int cols, char data[][cols][500], int rows) {
    int col_widths[cols];
    int spacing = 5;
    char *line_color = HEADER_COLOR;

    for (int j = 0; j < cols; j++) {
        col_widths[j] = strlen(headers[j]) + spacing;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int data_len = strlen(data[i][j]) + spacing;
            if (data_len > col_widths[j]) {
                col_widths[j] = data_len;
            }
        }
    }

    printf(HEADER_COLOR);
    for (int j = 0; j < cols; j++) {
        printf("%-*s ", col_widths[j], headers[j]);
    }
    printf(RESET_COLOR "\n");

    for (int j = 0; j < cols; j++) {
        for (int k = 0; k < col_widths[j]; k++){
            #ifdef _WIN32
            printf("%s%c", line_color, 196);
        #else
            printf("%s\u2500", line_color);
        #endif
        }
        printf(" ");
    }
    printf("\n");

    printf(DATA_COLOR);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%-*s ", col_widths[j], data[i][j]);
        }
        printf("\n");
    }
    printf(RESET_COLOR);

}

void login_page(){
    int choice;
    char username[50];
    char password[50];
    authentication:
        printf(CHOICE);
        printf("\nEnter your choice: ");
        printf(RESET_COLOR);
        if (scanf("%d", &choice) != 1) { 
            printf(ERROR);
            printf("[-] Choose from one of the above options only!\n");
            printf(RESET_COLOR);
            while (getchar() != '\n');
            goto authentication;
        }

        switch (choice){
            case 1:
                printf("\tTo login as user...\n");
                user:
                printf(INNER_QUERY);
                printf("\tEnter your Username: ");
                printf(RESET_COLOR);
                scanf("%49s", username);

                printf(INNER_QUERY);
                printf("\tEnter your Password: ");
                printf(RESET_COLOR);
                scanf("%49s", password);
                if (validate_user_admin(username, password, 1)){
                    user(username);
                }
                else{
                    printf(ERROR);
                    printf("\t[-]Invalid Username or Password!\n");
                    printf(RESET_COLOR);
                    goto user;
                }
                break;
            case 2:
                printf("\tTo login as admin...\n");
                admin:
                printf(INNER_QUERY);
                printf("\tEnter your Username: ");
                printf(RESET_COLOR);
                scanf("%49s", username);

                printf(INNER_QUERY);
                printf("\tEnter your Password: ");
                printf(RESET_COLOR);
                scanf("%49s", password);
                if (validate_user_admin(username, password, 2)){
                    admin(username);
                }
                else{
                    printf(ERROR);
                    printf("\t[-]Invalid Username or Password!\n");
                    printf(RESET_COLOR);
                    goto admin;
                }
                break;
            case 3:
                printf("\tTo Create a New User Account...\n");
                create_account:
                printf(INNER_QUERY);
                printf("\tChoose your Username: ");
                printf(RESET_COLOR);
                scanf("%49s", username);

                printf(INNER_QUERY);
                printf("\tChoose your Password: ");
                printf(RESET_COLOR);
                scanf("%49s", password);

                char name[100], gender[10], email[100];
                int age;

                printf(INNER_QUERY);
                printf("\tName: ");
                printf(RESET_COLOR);
                getchar();
                fgets(name, sizeof(name), stdin);

                name[strcspn(name, "\n")] = '\0';

                printf(INNER_QUERY);
                printf("\tEmail: ");
                printf(RESET_COLOR);
                scanf("%99s", email);

                age:
                printf(INNER_QUERY);
                printf("\tAge: ");
                printf(RESET_COLOR);
                if (scanf("%d", &age) != 1) { 
                    printf(ERROR);
                    printf("\t[-] Invalid Age!\n");
                    printf(RESET_COLOR);
                    while (getchar() != '\n');
                    goto age;
                }
                if (age < 10 || age > 120){
                    printf(ERROR);
                    printf("\t[-]Either you have entered an invaid age or your not eligible to create an account!\n");
                    printf(RESET_COLOR);
                    goto age;
                }

                printf("\tWhats your gender...\n\t1 \u2192 Male\n\t2 \u2192 Female\n\t3 \u2192 Other\n");
                int gen_code;
                gender:
                printf(INNER_QUERY);
                printf("\tGender: ");
                printf(RESET_COLOR);
                if (scanf("%d", &gen_code) != 1) { 
                    printf(ERROR);
                    printf("\t[-] Invalid input!\n");
                    printf(RESET_COLOR);
                    while (getchar() != '\n');
                    goto gender;
                }
                switch (gen_code){
                    case 1:
                        strcpy(gender, "Male");
                        break;
                    case 2:
                        strcpy(gender, "Female");
                        break;
                    case 3:
                        strcpy(gender, "Other");
                        break;
                    default:
                        printf(ERROR);
                        printf("\t[-]Invaid input!\n");
                        printf(RESET_COLOR);
                        goto gender;
                }

                if (user_or_email_exists(username, email)){
                    printf(ERROR);
                    printf("\t[-]User already exists with same email or username!\n");
                    printf(RESET_COLOR);
                    goto create_account;
                }
                else{
                    addUser(username, password, name, age, gender, email);
                    user(username);
                }
                break;
            case 0:
                close_db();
                exit(0);
                break;
            default:
                printf(ERROR);
                printf("[-] Choose from one of the above options only!\n");
                printf(RESET_COLOR);
                goto authentication;
        }
}

void user(char *username){
    setup_intro();
    printf("Hey, %s\n", username);
    printf("[+] You have LoggedIn as User...\n\n");

    printf("Main Menu:\n");
    printf("\t1  \u2192 View all Trains\n"
        "\t2  \u2192 Search Trains from Source to Destination\n"
        "\t3  \u2192 Search Trains from Source to Destination on a Date\n"
        "\t4  \u2192 Book a Ticket\n"
        "\t5  \u2192 View Train Map\n"
        "\t6  \u2192 View Reservation History\n"
        "\t7  \u2192 View Active Reservations\n"
        "\t8  \u2192 Modify a Reservation\n"
        "\t9  \u2192 Cancel a Reservation\n"
        "\t10 \u2192 Update Profile\n"
        "\t0  \u2192 Exit\n");
    
    int choice = -1;
    choice:
    while (choice != 0){
        input:
        printf(CHOICE);
        printf("\nEnter your choice: ");
        printf(RESET_COLOR);
        if (scanf("%d", &choice) != 1) { 
            printf(ERROR);
            printf("[-] Choose from one of the above options only!\n");
            printf(RESET_COLOR);
            while (getchar() != '\n');
            goto input;
        }

        switch (choice){
            case 1:
                u_option1();
                break;
            case 2:
                u_option2_3(2, "");
                break;
            case 3:
                printf("\tDate Format: yyyy-mm-dd\n");
                printf(INNER_QUERY);
                printf("\tDate of Travel: ");
                printf(RESET_COLOR);
                char date[20];
                scanf("%s", date);
                u_option2_3(3, date);
                break;
            case 4:
                u_option4(username);
                break;
            case 5:
                u_option5();
                break;
            case 6:
                u_option6_7(6, username);
                break;
            case 7:
                u_option6_7(7, username);
                break;
            case 8:
                printf("8");
                break;
            case 9:
                u_option9(username);
                break;
            case 10:
                u_option10(username);
                break;
            case 0:
                exit(0);
                break;
            default:
                printf(ERROR);
                printf("[-] Choose from one of the above options only!\n");
                printf(RESET_COLOR);
                goto choice;
        }
    }
}

void u_option1(){
    char *query = 
        "SELECT Trains.t_name, TrainSchedules.id, TrainSchedules.dep_date, TrainSchedules.dep_time, "
                      "TrainSchedules.arr_date, TrainSchedules.arr_time, TrainSchedules.moving, "
                      "Trains.s_station, Trains.e_station "
                      "FROM TrainSchedules "
                      "JOIN Trains ON TrainSchedules.t_id = Trains.id "
                      "ORDER BY TrainSchedules.t_id, TrainSchedules.id, TrainSchedules.dep_date";
    sqlite3_stmt *stmt = execute_command(query);
    char last_train_name[300] = "";
    char *headers[] = {"ID", "Departure", "Arrival", "Journey Type"};
    int cols = sizeof(headers) / sizeof(headers[0]);
    char data[1000][cols][500];
    int rows = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *train_name = (const char *)sqlite3_column_text(stmt, 0);
        int schedule_id = sqlite3_column_int(stmt, 1);
        const char *dep_date = (const char *)sqlite3_column_text(stmt, 2);
        const char *dep_time = (const char *)sqlite3_column_text(stmt, 3);
        const char *arr_date = (const char *)sqlite3_column_text(stmt, 4);
        const char *arr_time = (const char *)sqlite3_column_text(stmt, 5);
        const char *moving = (const char *)sqlite3_column_text(stmt, 6);
        const char *s_station = (const char *)sqlite3_column_text(stmt, 7);
        const char *e_station = (const char *)sqlite3_column_text(stmt, 8);

        if (strcmp(train_name, last_train_name) != 0) {
            if (strlen(last_train_name) > 0) {
                print_table(headers, cols, data, rows);
            }
            char form_t_name[200];
            sprintf(form_t_name, "%s (%s \u21D4 %s)\n", train_name, s_station, e_station);
            printf("\n");
            center_text(form_t_name, 70, "\033[1;31m");
            strcpy(last_train_name, train_name);
            rows = 0;
        }

        if (is_date_time_in_future(dep_date, dep_time)){
            sprintf(data[rows][0], "%d", schedule_id);
            snprintf(data[rows][1], 100, "%s %s", dep_date, dep_time);
            snprintf(data[rows][2], 100, "%s %s", arr_date, arr_time);
            strcpy(data[rows][3], (strcmp(moving, "0") == 0) ? "Forward" : "Return");

            rows++;
        }
    }
    print_table(headers, cols, data, rows);
    sqlite3_finalize(stmt);
}

// change to not to print train name if no schedule matches the user specification
void u_option2_3(int option, char *date) {
    char from[200], to[200];

    printf(INNER_QUERY);
    printf("\tFrom Location: ");
    printf(RESET_COLOR);
    scanf("%s", from);

    printf(INNER_QUERY);
    printf("\tTo Location: ");
    printf(RESET_COLOR);
    scanf("%s", to);

    char *query =
        "SELECT Trains.t_name, TrainSchedules.id, TrainSchedules.dep_date, TrainSchedules.dep_time, "
        "TrainSchedules.arr_date, TrainSchedules.arr_time, TrainSchedules.moving, "
        "Trains.s_station, Trains.e_station, Trains.stations "
        "FROM TrainSchedules "
        "JOIN Trains ON TrainSchedules.t_id = Trains.id "
        "ORDER BY Trains.id, TrainSchedules.dep_date, TrainSchedules.dep_time, TrainSchedules.id";

    sqlite3_stmt *stmt = execute_command(query);

    char last_train_name[300] = "";
    char *headers[] = {"ID", "Departure", "Arrival"};
    int cols = sizeof(headers) / sizeof(headers[0]);
    char data[500][cols][500];
    int rows = 0;
    int satifying_query = 0;
    char output[MAX_STATIONS][MAX_STATION_NAME];
    int forward;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *train_name = (const char *)sqlite3_column_text(stmt, 0);
        int schedule_id = sqlite3_column_int(stmt, 1);
        const char *dep_date = (const char *)sqlite3_column_text(stmt, 2);
        const char *dep_time = (const char *)sqlite3_column_text(stmt, 3);
        const char *arr_date = (const char *)sqlite3_column_text(stmt, 4);
        const char *arr_time = (const char *)sqlite3_column_text(stmt, 5);
        const char *moving = (const char *)sqlite3_column_text(stmt, 6);
        const char *s_station = (const char *)sqlite3_column_text(stmt, 7);
        const char *e_station = (const char *)sqlite3_column_text(stmt, 8);
        const char *stations = (const char *)sqlite3_column_text(stmt, 9);

        if (strcmp(train_name, last_train_name) != 0) {
            satifying_query = 0;
            int count = split_string(stations, MAX_STATIONS, MAX_STATION_NAME, output);
            int s = 0;
            forward = 2737;

            if (strcmp(moving, "0") == 0) {
                for (int i = 0; i < count; i++) {
                    if (s == 1 && compare_strings(output[i], to)) {
                        forward = 0;
                        satifying_query = 1;
                        break;
                    } else if (compare_strings(output[i], from)) {
                        s = 1;
                    }
                }
            } else {
                for (int i = count - 1; i >= 0; i--) {
                    if (s == 1 && compare_strings(output[i], to)) {
                        forward = 1;
                        satifying_query = 1;
                        break;
                    } else if (compare_strings(output[i], from)) {
                        s = 1;
                    }
                }
            }

            if (satifying_query) {
                if (strlen(last_train_name) > 0) {
                    print_table(headers, cols, data, rows);
                }
                char form_t_name[200];
                sprintf(form_t_name, "%s (%s \u21D4 %s)\n", train_name, s_station, e_station);
                printf("\n");
                center_text(form_t_name, 50, "\033[1;31m");
                strcpy(last_train_name, train_name);
                rows = 0;
            }
        }

        if (satifying_query && (strcmp(moving, (forward == 0)?"0":"1") == 0)) {
            if (option == 2){
                sprintf(data[rows][0], "%d", schedule_id);
                snprintf(data[rows][1], 100, "%s %s", dep_date, dep_time);
                snprintf(data[rows][2], 100, "%s %s", arr_date, arr_time);

                rows++;
            }
            else if (option == 3 && (strcmp(date, dep_date)==0)){
                sprintf(data[rows][0], "%d", schedule_id);
                snprintf(data[rows][1], 100, "%s %s", dep_date, dep_time);
                snprintf(data[rows][2], 100, "%s %s", arr_date, arr_time);

                rows++;
            }
        }
    }
    print_table(headers, cols, data, rows);
    sqlite3_finalize(stmt);
}

void u_option4(char *username){
    // Getting the schedule id
    train_id:
    printf(INNER_QUERY);
    printf("\tEnter the Train ID: ");
    printf(RESET_COLOR);
    int id;
    if (scanf("%d", &id) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    // Getting the data no of seats of each type available
    char *query1 = 
        "SELECT sleeper_count, ac3_count, ac2_count, ac1_count, general_count, canceled, t_id "
        "FROM TrainSchedules WHERE id = ?";
    sqlite3_stmt *stmt = execute_command(query1);

    sqlite3_bind_int(stmt, 1, id);

    int t_id = -1;
    int sleeper_count = 0, ac3_count = 0, ac2_count = 0, ac1_count = 0, general_count = 0;
    int coach_cap = 0, sleeper_seats = 0, ac3_seats = 0, ac2_seats = 0, ac1_seats = 0, general_seats = 0;
    char *canceled = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sleeper_count = sqlite3_column_int(stmt, 0);
        ac3_count = sqlite3_column_int(stmt, 1);
        ac2_count = sqlite3_column_int(stmt, 2);
        ac1_count = sqlite3_column_int(stmt, 3);
        general_count = sqlite3_column_int(stmt, 4);
        t_id = sqlite3_column_int(stmt, 6);

        const char *temp = (const char *)sqlite3_column_text(stmt, 5);
        if (temp != NULL) {
            canceled = malloc(strlen(temp) + 1);
            strcpy(canceled, temp);
        }

    } else {
        printf(ERROR);
        printf("\t[-] Make sure you have entered a valid id(Check option 1, 2, 3 to get id)\n");
        printf(RESET_COLOR);
        goto train_id;
        
    }
    sqlite3_finalize(stmt);

    char *query2 = 
        "SELECT coach_cap, sleeper_seats, ac3_seats, ac2_seats, ac1_seats, general_seats, t_name "
        "FROM Trains WHERE id = ?";

    stmt = execute_command(query2);
    sqlite3_bind_int(stmt, 1, t_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        coach_cap = sqlite3_column_int(stmt, 0);
        sleeper_seats = sqlite3_column_int(stmt, 1);
        ac3_seats = sqlite3_column_int(stmt, 2);
        ac2_seats = sqlite3_column_int(stmt, 3);
        ac1_seats = sqlite3_column_int(stmt, 4);
        general_seats = sqlite3_column_int(stmt, 5);
        char *t_name = (char*)sqlite3_column_text(stmt, 6);
    } else {
        printf(ERROR);
        printf("[-] No Train exist with ID: %d\n", t_id);
        printf(RESET_COLOR);
    }

    sqlite3_finalize(stmt);

    // No of seats left for each class
    int rem_sle = sleeper_seats - sleeper_count;
    int rem_ac3 = ac3_seats - ac3_count;
    int rem_ac2 = ac2_seats - ac2_count;
    int rem_ac1 = ac1_seats - ac1_count;
    int rem_gen = general_seats - general_count;

    char cancelled_seats[1000][20];
    int count = split_string(canceled, 1000, 7, cancelled_seats);
    for (int i = 0; i < count; i++){
        if (cancelled_seats[i][0] == 's'){
            rem_sle += 1;
        }
        else if (cancelled_seats[i][0] == 'a'){
            if (cancelled_seats[i][2] == '1'){
                rem_ac1 += 1;
            }
            else if (cancelled_seats[i][2] == '2'){
                rem_ac2 += 1;
            }
            else if (cancelled_seats[i][2] == '3'){
                rem_ac3 += 1;
            }
        }
        else if (cancelled_seats[i][0] == 'g'){
            rem_gen += 1;
        }
    }
    
    // Taking the user input regarding passenger info and seat preferences
    int n_tickets;
    printf(INNER_QUERY);
    printf("\tNumber of tickets: ");
    printf(RESET_COLOR);
    if (scanf("%d", &n_tickets) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    printf("\tSeat Types: \n");
    printf("\t  1 \u2192 Sleeper  : %d\n", rem_sle);
    printf("\t  2 \u2192 AC 3 Tier: %d\n", rem_ac3);
    printf("\t  3 \u2192 AC 2 Tier: %d\n", rem_ac2);
    printf("\t  4 \u2192 AC 1 Tier: %d\n", rem_ac1);
    printf("\t  5 \u2192 General  : %d\n", rem_gen);

    char passengers[n_tickets][200];
    int seat_type[n_tickets];
    int age[n_tickets];
    int gender[n_tickets];

    int ts=rem_sle, ta1=rem_ac1, ta2=rem_ac2, ta3=rem_ac3, tg=rem_gen;
    for (int i = 0; i < n_tickets; i++){
        printf(INNER_QUERY);
        printf("\tName of passenger %d: ", i+1);
        printf(RESET_COLOR);
        getchar();
        fgets(passengers[i], 199, stdin);
        passengers[i][strcspn(passengers[i], "\n")] = '\0';
        printf(INNER_QUERY);
        printf("\tAge of %s: ", passengers[i]);
        printf(RESET_COLOR);
        if (scanf("%d", &age[i]) != 1) { 
            printf(ERROR);
            printf("\t[-] Invalid Age!\n");
            printf(RESET_COLOR);
            while (getchar() != '\n');
            return;
        }
        printf("\t1 \u2192 Male\n\t2 \u2192 Female\n");
        printf(INNER_QUERY);
        printf("\tGender: ");
        printf(RESET_COLOR);
        if (scanf("%d", &gender[i]) != 1) { 
            printf(ERROR);
            printf("\t[-] Invalid Input!\n");
            printf(RESET_COLOR);
            while (getchar() != '\n');
            return;
        }
        seat:
        printf(INNER_QUERY);
        printf("\tSeat type for %s: ", passengers[i]);
        printf(RESET_COLOR);
        if (scanf("%d", &seat_type[i]) != 1) { 
            printf(ERROR);
            printf("\t[-] Invalid Input!\n");
            printf(RESET_COLOR);
            while (getchar() != '\n');
            return;
        }
        switch (seat_type[i])
        {
        case 1:
            if (!(ts > 0)){
                printf(ERROR);
                printf("\t[-] This class is occupied. Please choose another!\n");
                printf(RESET_COLOR);
                goto seat;
            }
            ts -= 1;
            break;
        case 2:
            if (!(ta3 > 0)){
                printf(ERROR);
                printf("\t[-] This class is occupied. Please choose another!\n");
                printf(RESET_COLOR);
                goto seat;
            }
            ta3 -= 1;
            break;
        case 3:
            if (!(ta2 > 0)){
                printf(ERROR);
                printf("\t[-] This class is occupied. Please choose another!\n");
                printf(RESET_COLOR);
                goto seat;
            }
            ta2 -= 1;
            break;
        case 4:
            if (!(ta1 > 0)){
                printf(ERROR);
                printf("\t[-] This class is occupied. Please choose another!\n");
                printf(RESET_COLOR);
                goto seat;
            }
            ta1 -= 1;
            break;
        case 5:
            if (!(tg > 0)){
                printf(ERROR);
                printf("\t[-] This class is occupied. Please choose another!\n");
                printf(RESET_COLOR);
                goto seat;
            }
            tg -= 1;
            break;
        default:
            printf(ERROR);
            printf("\t[-] Invalid choice Enter one of the above mentioned choices!\n");
            printf(RESET_COLOR);
            goto seat;
        }
        printf("\n");
    }

    // Checking if the requested seats are present in canceled seats
    int indexes[n_tickets];
    memset(indexes, -1, sizeof(indexes));
    for (int j = 0; j < n_tickets; j++){
        for (int i = 0; i < count; i++){
            int cont = 0;
            for (int k = 0; k <= j; k++){
                if (indexes[k] == i){
                    cont = 1;
                    break;
                }
            }
            if (cont){
                continue;
            }
            if (cancelled_seats[i][0] == 's' && seat_type[j] == 1){
                indexes[j] = i;
                rem_sle -= 1;
                break;
            }
            else if (cancelled_seats[i][0] == 'a'){
                
                if (cancelled_seats[i][2] == '1' && seat_type[j] == 4){
                    indexes[j] = i;
                    rem_ac1 -= 1;
                    break;
                }
                else if (cancelled_seats[i][2] == '2' && seat_type[j] == 3){
                    indexes[j] = i;
                    rem_ac2 -= 1;
                    break;
                }
                else if (cancelled_seats[i][2] == '3' && seat_type[j] == 2){
                    indexes[j] = i;
                    rem_ac3 -= 1;
                    break;
                }
            }
            else if (cancelled_seats[i][0] == 'g' && seat_type[j] == 5){
                indexes[j] = i;
                rem_gen -= 1;
                break;
            }
        }
    }

    // Alloting the canceled seats if prpesent some new seats
    char seats_alloted[n_tickets][10];
    for (int i = 0; i < n_tickets; i++){
        if (indexes[i] == -1){
            if (seat_type[i] == 1 && rem_sle > 0){
                sleeper_count += 1;
                int c_no = (sleeper_count/coach_cap) + 1;
                char c[20] = "s-";
                sprintf(c + strlen(c), "%d-", c_no);
                sprintf(c + strlen(c), "%d", sleeper_count);
                strcpy(seats_alloted[i], c);
                rem_sle -= 1;
            }
            else if (seat_type[i] == 2 && rem_ac3 > 0){
                ac3_count += 1;
                int c_no = (ac3_count/coach_cap) + 1;
                char c[20] = "ac3-";
                sprintf(c + strlen(c), "%d-", c_no);
                sprintf(c + strlen(c), "%d", ac3_count);
                strcpy(seats_alloted[i], c);
                rem_ac3 -= 1;
            }
            else if (seat_type[i] == 3 && rem_ac2 > 0){
                ac2_count += 1;
                int c_no = (ac2_count/coach_cap) + 1;
                char c[20] = "ac2-";
                sprintf(c + strlen(c), "%d-", c_no);
                sprintf(c + strlen(c), "%d", ac2_count);
                strcpy(seats_alloted[i], c);
                rem_ac2 -= 1;
            }
            else if (seat_type[i] == 4 && rem_ac1 > 0){
                ac1_count += 1;
                int c_no = (ac1_count/coach_cap) + 1;
                char c[20] = "ac1-";
                sprintf(c + strlen(c), "%d-", c_no);
                sprintf(c + strlen(c), "%d", ac1_count);
                strcpy(seats_alloted[i], c);
                rem_ac1 -= 1;
            }
            else if (seat_type[i] == 5 && rem_gen > 0){
                general_count += 1;
                int c_no = (general_count/coach_cap) + 1;
                char c[20] = "g-";
                sprintf(c + strlen(c), "%d-", c_no);
                sprintf(c + strlen(c), "%d", general_count);
                strcpy(seats_alloted[i], c);
                rem_gen -= 1;
            }
        }
        else{
            strcpy(seats_alloted[i], cancelled_seats[indexes[i]]);
        }
    }  

    // Formatting data to update the database
    // Canceled seats new data
    char new_cancelled[1000] = "";
    int is_f = 1;
    for (int i = 0; i < count; i++){
        if (indexes[i] == -1){
            if (is_f){
                is_f = 0;
            }
            else{
                strcat(new_cancelled, ",");
            }
            strcat(new_cancelled, cancelled_seats[i]);
        }
    }

    // Seats and the passenger data
    char seats_data[2000] = "";
    char passengers_data[2000] = "";
    for (int i = 0; i < n_tickets; i++){
        strcat(seats_data, seats_alloted[i]);

        strcat(passengers_data, passengers[i]);
        strcat(passengers_data, ",");
        char buffer[20];

        sprintf(buffer, "%d", age[i]);
        strcat(passengers_data, buffer);
        strcat(passengers_data, ",");

        sprintf(buffer, "%d", gender[i]);
        strcat(passengers_data, buffer);
        if (i != n_tickets-1){
            strcat(seats_data, ",");
            strcat(passengers_data, ",");
        }
    }

    // Date of reservation (Current date)
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[11];
    strftime(date, sizeof(date), "%Y-%m-%d", tm_info);
    
    // Getting the user_id corresponding to username
    int u_id = get_user_id(username);


    char *status = "RESERVED";

    // Inserting into Reservation Table
    char *query4 = "INSERT INTO Reservation (t_id, sch_id, user_id, reservaton_date, seats, passengers, status) "
                "VALUES (?, ?, ?, ?, ?, ?, ?)";

    stmt = execute_command(query4);

    sqlite3_bind_int(stmt, 1, t_id);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, u_id);
    sqlite3_bind_text(stmt, 4, date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, seats_data, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, passengers_data, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, status, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-] SQL Error while inserting into Reservations.\n");
        printf(RESET_COLOR);
    }

    sqlite3_finalize(stmt);

    // Updating seats availablity in TrainSchedules
    char *query5 = "UPDATE TrainSchedules SET sleeper_count = ?, ac3_count = ?, ac2_count = ?, "
                "ac1_count = ?, general_count = ?, canceled = ? WHERE id = ?";

    stmt = execute_command(query5);

    sqlite3_bind_int(stmt, 1, sleeper_count);
    sqlite3_bind_int(stmt, 2, ac3_count);
    sqlite3_bind_int(stmt, 3, ac2_count);
    sqlite3_bind_int(stmt, 4, ac1_count);
    sqlite3_bind_int(stmt, 5, general_count);
    sqlite3_bind_text(stmt, 6, new_cancelled, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\tSQL Error while inserting into TrainSchedules.\n");
        printf(RESET_COLOR);
    }

    sqlite3_finalize(stmt);

    printf(SUCCESS);
    printf("\t[+] Your ticket has been sucessfully reserved. View more under 'Reservations'.\n");
    printf(RESET_COLOR);

    if (canceled != NULL) {
        free(canceled);
    }
}

void u_option5(){
    int id;
    printf(INNER_QUERY);
    printf("\tTrain ID: ");
    printf(RESET_COLOR);
    if (scanf("%d", &id) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }
    char *query = 
        "SELECT Trains.stations, TrainSchedules.moving , Trains.t_name "
        "FROM TrainSchedules "
        "JOIN Trains ON TrainSchedules.t_id = Trains.id "
        "WHERE TrainSchedules.id = ?";

    sqlite3_stmt *stmt = execute_command(query);
    

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *stations = (const char *)sqlite3_column_text(stmt, 0);
        int moving = (int)sqlite3_column_int(stmt, 1);
        const char *t_name = (const char *)sqlite3_column_text(stmt, 2);
        char st_arr[MAX_STATIONS][MAX_STATION_NAME];
        int c = split_string(stations,MAX_STATIONS, MAX_STATION_NAME, st_arr);
        DoublyLinkedList *list = create_list();
        for (int i = 0; i < c; i++){
            insert_at_end(list, st_arr[i]);
        }

        printf("\n\033[1;31m%s\n", t_name);
        printf(RESET_COLOR);
        traverse_list(list, moving);
        free_list(list);
    } else {
        printf(ERROR);
        printf("\t[-] Make sure you have entered a valid id(Check option 1, 2, 3 to get id)\n");
        printf(RESET_COLOR);
    }

    sqlite3_finalize(stmt);
}

void u_option6_7(int option, char *username){
    char *query = 
        "SELECT "
        "    r.id AS reservation_id, "
        "    r.t_id AS train_id, "
        "    r.sch_id AS schedule_id, "
        "    r.user_id, "
        "    r.reservaton_date, "
        "    r.seats, "
        "    r.passengers, "
        "    r.status, "
        "    t.t_name AS train_name, "
        "    ts.dep_time, "
        "    ts.arr_time, "
        "    ts.dep_date, "
        "    ts.arr_date, t.s_station, t.e_station "
        "FROM "
        "    Reservation r "
        "JOIN "
        "    Trains t ON r.t_id = t.id "
        "JOIN "
        "    TrainSchedules ts ON r.sch_id = ts.id "
        "WHERE "
        "    r.user_id = ?;";
    sqlite3_stmt *stmt = execute_command(query);
    int user_id = get_user_id(username);
    sqlite3_bind_int(stmt, 1, user_id);
    

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int reservation_id = (int)sqlite3_column_int(stmt, 0);
        int train_id = (int)sqlite3_column_int(stmt, 1);
        int schedule_id = (int)sqlite3_column_int(stmt, 2);
        const char *reservation_date = (const char *)sqlite3_column_text(stmt, 4);
        const char *seats = (const char *)sqlite3_column_text(stmt, 5);
        const char *passengers = (const char *)sqlite3_column_text(stmt, 6);
        const char *status = (const char *)sqlite3_column_text(stmt, 7);
        const char *train_name = (const char *)sqlite3_column_text(stmt, 8);
        const char *dep_time = (const char *)sqlite3_column_text(stmt, 9);
        const char *arr_time = (const char *)sqlite3_column_text(stmt, 10);
        const char *dep_date = (const char *)sqlite3_column_text(stmt, 11);
        const char *arr_date = (const char *)sqlite3_column_text(stmt, 12);
        const char *s_station = (const char *)sqlite3_column_text(stmt, 13);
        const char *e_station = (const char *)sqlite3_column_text(stmt, 14);
        if (option == 7 && (is_date_time_in_future(dep_date, dep_time) == 0 || strcmp(status, "RESERVED") != 0)){
            continue;
        }
        /*
        green: status reserved and journey not done yet.
        blue: status reserved and journey done.
        red: status cancled\\
        */
        char form_t_name[200];
        sprintf(form_t_name, "%s (%s \u21D4 %s)\n", train_name, s_station, e_station);
        printf("\n");
        center_text(form_t_name, 50, "\033[1;31m");
        printf(HEADER_COLOR"RID             :" RESET_COLOR "%d\n", reservation_id);
        printf(HEADER_COLOR"Reservation Date:" RESET_COLOR "%s\n", reservation_date);
        printf(HEADER_COLOR"Status          :" RESET_COLOR "%s\n", status);
        printf(HEADER_COLOR"Departure       :" RESET_COLOR "%s %s\n", dep_date, dep_time);
        printf(HEADER_COLOR"Arrival         :" RESET_COLOR "%s %s\n\n", arr_date, arr_time);

        char seats_arr[20][15];
        int count = split_string(seats, 20, 15, seats_arr);
        char passengers_arr[50][100];
        split_string(passengers, 50, 100, passengers_arr);

        char *headers[] = {"Name", "Age", "Gender", "Seat"};
        int cols = sizeof(headers) / sizeof(headers[0]);

        char data[count][4][500];
        int rows = sizeof(data) / sizeof(data[0]);
        
        int j = 0;
        for (int i = 0; i < count; i++){
            strcpy(data[i][0], passengers_arr[j++]);
            strcpy(data[i][1], passengers_arr[j++]);
            strcpy(data[i][2], (strcmp(passengers_arr[j++], "1") == 0)?"Male":"Female");
            strcpy(data[i][3], seats_arr[i]);
        }
        print_table(headers, cols, data, rows);
        printf("\n");
    }

    sqlite3_finalize(stmt);
}

void u_option9(char *username){
    int u_id=get_user_id(username), r_id, sch_id;
    printf(INNER_QUERY);
    printf("\tReservation id: ");
    printf(RESET_COLOR);
    if (scanf("%d", &r_id) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    char *sql = "SELECT seats, status, sch_id FROM Reservation WHERE user_id = ? AND id = ?;";
    sqlite3_stmt *stmt = execute_command(sql);

    sqlite3_bind_int(stmt, 1, u_id);
    sqlite3_bind_int(stmt, 2, r_id);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const char *seats = (const char *)sqlite3_column_text(stmt, 0);
        const char *status = (const char *)sqlite3_column_text(stmt, 1);
        sch_id = sqlite3_column_int(stmt, 2);

        if (strcmp("RESERVED", status) != 0){
            printf(ERROR);
            printf("\t[-] This reseravtion is already cancelled!\n");
            printf(RESET_COLOR);
            return;
        }


        
        char *sql1 = "UPDATE Reservation SET status = \"CANCELLED\" WHERE id = ?;";
        sqlite3_stmt *stmt1 = execute_command(sql1);

        sqlite3_bind_int(stmt1, 1, r_id);

        int rc1= sqlite3_step(stmt1);
        if (rc1 != SQLITE_DONE) {
            printf(ERROR);
            printf("\t[-]Failed to update reservation.\n");
            printf(RESET_COLOR);
        }

        sqlite3_finalize(stmt1);


        char *sql3 = "SELECT canceled FROM TrainSchedules WHERE id = ?;";
        sqlite3_stmt *stmt3 = execute_command(sql3);

        sqlite3_bind_int(stmt3, 1, sch_id);
        char prev_canceled[3000] = ""; 
        int rc3 = sqlite3_step(stmt3);
        if (rc3 == SQLITE_ROW) {
            char *cancelled = (char *)sqlite3_column_text(stmt3, 0);
            strcpy(prev_canceled, cancelled);
        } else if (rc3 == SQLITE_DONE) {
            printf(ERROR);
            printf("\t[-]No row found for ID: %d\n", sch_id);
            printf(RESET_COLOR);
        } else {
            printf(ERROR);
            printf("\t[-]Failed to fetch data\n");
            printf(RESET_COLOR);
        }

        sqlite3_finalize(stmt3);




        char *sql2 = "UPDATE TrainSchedules SET canceled = ? WHERE id = ?;";
        sqlite3_stmt *stmt2 = execute_command(sql2);
        
        char updated_seats[3000] = "";
        strcpy(updated_seats, prev_canceled);
        if (updated_seats[0] != 0){
            strcat(updated_seats, ",");
        }
        strcat(updated_seats, seats);

        sqlite3_bind_text(stmt2, 1, updated_seats, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt2, 2, sch_id);

        int rc2 = sqlite3_step(stmt2);
        if (rc2 == SQLITE_DONE) {
            printf(SUCCESS);
            printf("\t[+]Reservation with ID %d has been updated to CANCELLED.\n", r_id);
            printf(RESET_COLOR);
        } else {
            printf(ERROR);
            printf("\t[-]Failed to update reservation.\n");
            printf(RESET_COLOR);
        }

        sqlite3_finalize(stmt2);

    } else if (rc == SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-]Make sure you have entered the correct reservation id\n");
        printf(RESET_COLOR);
    }
    sqlite3_finalize(stmt);

}

void u_option10(char *username){
    int u_id = get_user_id(username);
    int choice;
    printf("\tWhat would you like to update?\n"
    "\t\t1 \u2192 Name\n"
    "\t\t2 \u2192 Age\n"
    "\t\t3 \u2192 Gender\n"
    "\t\t4 \u2192 Passowrd\n"
    "\t\t5 \u2192 Email\n"
    "\t\t0 \u2192 Save Data and Exit\n");
    char *query = "SELECT password, name, age, gender, email FROM Users WHERE id = ?;";
    sqlite3_stmt *stmt = execute_command(query);

    sqlite3_bind_int(stmt, 1, u_id);
    char password[100], name[100], email[100], gender[100];
    int age;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(password, sqlite3_column_text(stmt, 0));
        strcpy(name, sqlite3_column_text(stmt, 1));
        age = sqlite3_column_int(stmt, 2);
        strcpy(gender, sqlite3_column_text(stmt, 3));
        strcpy(email, sqlite3_column_text(stmt, 4));
        printf("\n\tMy Profile:\n");
        printf("\t\tPassword: %s\n", password);
        printf("\t\tName: %s\n", name);
        printf("\t\tAge: %d\n", age);
        printf("\t\tGender: %s\n", gender);
        printf("\t\tEmail: %s\n", email);
    }
    sqlite3_finalize(stmt);

    choice:
    printf(INNER_QUERY);
    printf("\n\tChoice: ");
    printf(RESET_COLOR);
    if (scanf("%d", &choice) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }
    switch (choice){
        case 1:{
            printf(INNER_QUERY);
            printf("\tName: ");
            printf(RESET_COLOR);
            getchar();
            fgets(name, 99, stdin);
            name[strcspn(name, "\n")] = '\0';
            goto choice;
            break;
        }
        case 2:{
            printf(INNER_QUERY);
            printf("\tAge: ");
            printf(RESET_COLOR);
            if (scanf("%d", &age) != 1) { 
                printf(ERROR);
                printf("\t[-] Invalid Input!\n");
                printf(RESET_COLOR);
                while (getchar() != '\n');
                return;
            }
            goto choice;
            break;
        }
        case 3:{
            int temp;
            printf("\t1 \u2192 Male\n\t2 \u2192 Female\n");
            printf(INNER_QUERY);
            printf("\tGender: ");
            printf(RESET_COLOR);
            if (scanf("%d", &temp) != 1) { 
                printf(ERROR);
                printf("\t[-] Invalid Input!\n");
                printf(RESET_COLOR);
                while (getchar() != '\n');
                return;
            }
            if (temp!=1 && temp!=2){
                printf(ERROR);
                printf("\t[-] Choose one of the valid options only.\n");
                printf(RESET_COLOR);
            }
            strcpy(gender, (temp==1)?"Male":"Female");
            goto choice;
            break;
        }
        case 4:{
            char old_pass[100];
            printf(INNER_QUERY);
            printf("\tOld Password: ");
            printf(RESET_COLOR);
            scanf("%s", old_pass);
            if (strcmp(old_pass, password) == 0){
                printf(INNER_QUERY);
                printf("\tEnter New Password: ");
                printf(RESET_COLOR);
                scanf("%s", password);
            }
            else{
                printf(ERROR);
                printf("\t[-] The password isn't correct\n");
                printf(RESET_COLOR);
            }
            goto choice;
            break;
        }
        case 5:{
            printf(INNER_QUERY);
            printf("Email: ");
            printf(RESET_COLOR);
            scanf("%s", email);
            goto choice;
            break;
        }
        case 0:{
            char *sql = "UPDATE Users SET password = ?, name = ?, email = ?, gender = ?, age = ? WHERE id = ?;";
            sqlite3_stmt *stmt = execute_command(sql);

            sqlite3_bind_text(stmt, 1, password, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, gender, -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 5, age);
            sqlite3_bind_int(stmt, 6, u_id);
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            printf(SUCCESS);
            printf("\t[+] Updated your data sucessfully.\n");
            printf(RESET_COLOR);
            break;
        }
        default:{
            printf(ERROR);
            printf("\t[-] Choose only froom one of the above choices.\n");
            printf(RESET_COLOR);
            goto choice;
        }

    }

}

void admin(char *username){
    setup_intro();
    printf("Hey, %s\n\n", username);
    printf("\u2022 You have LoggedIn as Admin...\n\n");

    printf("Main Menu:\n");
    printf("\t1 \u2192 View Trains\n"
        "\t2 \u2192 View Schedules\n"
        "\t3 \u2192 View Schedules of a Train\n"
        "\t4 \u2192 Add Train\n"
        "\t5 \u2192 Remove Train\n"
        "\t6 \u2192 Add Schedule\n"
        "\t7 \u2192 Remove Schedule\n"
        "\t8 \u2192 View Reservations\n"
        "\t9 \u2192 View Reservations of a Train Schedule\n"
        "\t0 \u2192 Exit\n");
    
    int choice = -1;
    while (choice != 0){
        choice:
        printf(CHOICE);
        printf("\nEnter your choice: ");
        printf(RESET_COLOR);
        if (scanf("%d", &choice) != 1) { 
            printf(ERROR);
            printf("[-] Choose from one of the above options only!\n");
            printf(RESET_COLOR);
            while (getchar() != '\n');
            goto choice;
        }
        else{
            getchar();
            printf("\n");
        }

        switch (choice){
            
            case 1:
                a_option1();
                break;
            case 2:
                a_option2_3(2, -1);
                break;
            case 3:{
                int id;
                printf(INNER_QUERY);
                printf("\tTrain ID: ");
                printf(RESET_COLOR);
                if (scanf("%d", &id) != 1) { 
                    printf(ERROR);
                    printf("\t[-] Invalid Input!\n");
                    printf(RESET_COLOR);
                    while (getchar() != '\n');
                }
                else{
                    a_option2_3(3, id);
                }
                break;
            }
            case 4:
                a_option4();
                break;
            case 5:
                a_option5();
                break;
            case 6:
                a_option6();
                break;
            case 7:
                a_option7();
                break;
            case 8:
                a_option8_9(8);
                break;
            case 9:
                a_option8_9(9);
                break;
            case 0:
                exit(0);
                break;
            default:
                printf(ERROR);
                printf("[-] Choose from one of the above options only!\n");
                printf(RESET_COLOR);
                goto choice;
        }
    }
}

void a_option1(){
    char *query = "SELECT id, t_name, s_station, e_station, coach_cap, "
     "sleeper_seats, ac3_seats, ac2_seats, ac1_seats, general_seats, stations FROM Trains";
     sqlite3_stmt *stmt = execute_command(query);

    char *headers[] = {"SID", "Train", "From", "To", "Coach Capacity", "Sleeper", "AC(Tire-3)", "AC(Tire-2)", "AC(Tire-1)", "General"};
    int cols = sizeof(headers) / sizeof(headers[0]);
    char data[1000][cols][500];
    int rows = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        const char *id = (const char *)sqlite3_column_text(stmt, 0);
        const char *train_name = (const char *)sqlite3_column_text(stmt, 1);
        const char *from = (const char *)sqlite3_column_text(stmt, 2);
        const char *to = (const char *)sqlite3_column_text(stmt, 3);
        const char *coach_cap = (const char *)sqlite3_column_text(stmt, 4);
        const char *sleeper = (const char *)sqlite3_column_text(stmt, 5);
        const char *ac3 = (const char *)sqlite3_column_text(stmt, 6);
        const char *ac2 = (const char *)sqlite3_column_text(stmt, 7);
        const char *ac1 = (const char *)sqlite3_column_text(stmt, 8);
        const char *gen = (const char *)sqlite3_column_text(stmt, 9);

        
        strcpy(data[rows][0], id);
        strcpy(data[rows][1], train_name);
        strcpy(data[rows][2], from);
        strcpy(data[rows][3], to);
        strcpy(data[rows][4], coach_cap);
        strcpy(data[rows][5], sleeper);
        strcpy(data[rows][6], ac3);
        strcpy(data[rows][7], ac2);
        strcpy(data[rows][8], ac1);
        strcpy(data[rows][9], gen);

        rows++;
    }
    if (rows == 0){
        printf("[-] No Trains Present.\n");
    }
    else{
        print_table(headers, cols, data, rows);
    }
    sqlite3_finalize(stmt);
}

void a_option2_3(int option, int id){
    char *query = (id == -1) ?
            "SELECT Trains.t_name, TrainSchedules.id, TrainSchedules.dep_date, TrainSchedules.dep_time, "
            "TrainSchedules.arr_date, TrainSchedules.arr_time, TrainSchedules.moving, "
            "Trains.s_station, Trains.e_station, "
            "TrainSchedules.sleeper_count, TrainSchedules.ac3_count, TrainSchedules.ac2_count, "
            "TrainSchedules.ac1_count, TrainSchedules.general_count, TrainSchedules.canceled "
            "FROM TrainSchedules "
            "JOIN Trains ON TrainSchedules.t_id = Trains.id "
            "ORDER BY TrainSchedules.t_id, TrainSchedules.id, TrainSchedules.dep_date"
            :
            "SELECT Trains.t_name, TrainSchedules.id, TrainSchedules.dep_date, TrainSchedules.dep_time, "
            "TrainSchedules.arr_date, TrainSchedules.arr_time, TrainSchedules.moving, "
            "Trains.s_station, Trains.e_station, "
            "TrainSchedules.sleeper_count, TrainSchedules.ac3_count, TrainSchedules.ac2_count, "
            "TrainSchedules.ac1_count, TrainSchedules.general_count, TrainSchedules.canceled "
            "FROM TrainSchedules "
            "JOIN Trains ON TrainSchedules.t_id = Trains.id "
            "WHERE Trains.id = ? "
            "ORDER BY TrainSchedules.t_id, TrainSchedules.id, TrainSchedules.dep_date";

    sqlite3_stmt *stmt = execute_command(query);
    (id==-1)?-1:sqlite3_bind_int(stmt, 1, id);
    char last_train_name[300] = "";
    char *headers[] = {"ID", "Departure", "Arrival", "Journey Type", "Sleeper", "AC(Tire-3)", "AC(Tire-2)", "AC(Tire-1)", "General"};
    int cols = sizeof(headers) / sizeof(headers[0]);
    char data[1000][cols][500];
    int rows = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *train_name = (const char *)sqlite3_column_text(stmt, 0);
        int schedule_id = sqlite3_column_int(stmt, 1);
        const char *dep_date = (const char *)sqlite3_column_text(stmt, 2);
        const char *dep_time = (const char *)sqlite3_column_text(stmt, 3);
        const char *arr_date = (const char *)sqlite3_column_text(stmt, 4);
        const char *arr_time = (const char *)sqlite3_column_text(stmt, 5);
        const char *moving = (const char *)sqlite3_column_text(stmt, 6);
        const char *s_station = (const char *)sqlite3_column_text(stmt, 7);
        const char *e_station = (const char *)sqlite3_column_text(stmt, 8);

        int s = sqlite3_column_int(stmt, 9);
        int ac3 = sqlite3_column_int(stmt, 10);
        int ac2 = sqlite3_column_int(stmt, 11);
        int ac1 = sqlite3_column_int(stmt, 12);
        int g = sqlite3_column_int(stmt, 13);
        const char *canceled = sqlite3_column_text(stmt, 14);

        char cancelled_seats[1000][20];
        int count = split_string(canceled, 1000, 7, cancelled_seats);

        for (int i = 0; i < count; i++){
            if (cancelled_seats[i][0] == 's'){
                s -= 1;
            }
            else if (cancelled_seats[i][0] == 'a'){
                if (cancelled_seats[i][2] == '1'){
                    ac1 -= 1;
                }
                else if (cancelled_seats[i][2] == '2'){
                    ac2 -= 1;
                }
                else if (cancelled_seats[i][2] == '3'){
                    ac3 -= 1;
                }
            }
            else if (cancelled_seats[i][0] == 'g'){
                g -= 1;
            }
        }

        if (strcmp(train_name, last_train_name) != 0) {
            if (strlen(last_train_name) > 0) {
                print_table(headers, cols, data, rows);
            }
            char form_t_name[200];
            sprintf(form_t_name, "%s (%s \u21D4 %s)\n", train_name, s_station, e_station);
            printf("\n");
            center_text(form_t_name, 150, "\033[1;31m");
            strcpy(last_train_name, train_name);
            rows = 0;
        }

        
        sprintf(data[rows][0], "%d", schedule_id);
        snprintf(data[rows][1], 300, "%s %s", dep_date, dep_time);
        snprintf(data[rows][2], 300, "%s %s", arr_date, arr_time);
        strcpy(data[rows][3], (strcmp(moving, "0") == 0) ? "Forward" : "Return");
        snprintf(data[rows][4], 300, "%d", s);
        snprintf(data[rows][5], 300, "%d", ac3);
        snprintf(data[rows][6], 300, "%d", ac2);
        snprintf(data[rows][7], 300, "%d", ac1);
        snprintf(data[rows][8], 300, "%d", g);

        rows++;
    }
    if (rows == 0){
        printf(ERROR);
        printf("\t[-] No Trains found with Train ID: %d\n", id);
        printf(RESET_COLOR);
    }
    else{
        print_table(headers, cols, data, rows);
    }
    sqlite3_finalize(stmt);
}

void a_option4(){
    Train train;
    printf("\u2022Provide the details of the Train.\n\t\033[1;36mTrain Name: ");
    printf(RESET_COLOR);
    fgets(train.t_name, 299, stdin);
    train.t_name[strcspn(train.t_name, "\n")] = '\0';

    printf("\n\t\u2022Enter the names of stations where train stops.\n\t\u2022Enter '0' if nothing more\n\n");
    char station[200] = "";
    int i = 0;
    while (1){
        printf(INNER_QUERY);
        printf("\tStation %d: ", i+1);
        printf(RESET_COLOR);
        fgets(station, 199, stdin);
        station[strcspn(station, "\n")] = '\0';
        if (strcmp(station, "0") == 0){
            break;
        }
        strcpy(train.stations[i], station);
        i++;
    }
    if (i < 2){
        printf(ERROR);
        printf("\t[-] You must atleast have 2 stations.\n");
        printf(RESET_COLOR);
        return;
    }
    printf(INNER_QUERY);
    printf("\n\tCoach Capacity: ");
    printf(RESET_COLOR);
    if (scanf("%d", &train.coach_cap) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    printf(INNER_QUERY);
    printf("\tSleeper Seats: ");
    printf(RESET_COLOR);
    if (scanf("%d", &train.sleeper) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    printf(INNER_QUERY);
    printf("\tGeneral Seats: ");
    printf(RESET_COLOR);
    if (scanf("%d", &train.general) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    printf(INNER_QUERY);
    printf("\tAC(Tire 3) Seats: ");
    printf(RESET_COLOR);
    if (scanf("%d", &train.ac3) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    printf(INNER_QUERY);
    printf("\tAC(Tire 2) Seats: ");
    printf(RESET_COLOR);
    if (scanf("%d", &train.ac2) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    printf(INNER_QUERY);
    printf("\tAC(Tire 1) Seats: ");
    printf(RESET_COLOR);
    if (scanf("%d", &train.ac1) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    char merged_string[1000];
    merge_strings(merged_string, train.stations, i);

    char query[2000];

    snprintf(query, sizeof(query),
             "INSERT INTO Trains (t_name, s_station, e_station, stations, coach_cap, "
             "sleeper_seats, ac3_seats, ac2_seats, ac1_seats, general_seats) "
             "VALUES ('%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, %d);",
             train.t_name, train.stations[0], train.stations[i-1], merged_string,
             train.coach_cap, train.sleeper, train.ac3, train.ac2, train.ac1, train.general);
    sqlite3_stmt *stmt = execute_command(query);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-]SQL Error while inserting into Trains.\n");
        printf(RESET_COLOR);
    }

    sqlite3_finalize(stmt);

    printf(SUCCESS);
    printf("\t[+] Train '%s' has been added to database.\n", train.t_name);
    printf(RESET_COLOR);

}

void a_option5(){
    int t_id;
    printf(INNER_QUERY);
    printf("\tTrain ID: ");
    printf(RESET_COLOR);
    if (scanf("%d", &t_id) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    char query[400];
    snprintf(query, sizeof(query), "SELECT EXISTS(SELECT 1 FROM Trains WHERE id = %d);", t_id);
    sqlite3_stmt *stmt = execute_command(query);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!sqlite3_column_int(stmt, 0)){
            printf(ERROR);
            printf("\t[-] No Train found with ID: %d\n", t_id);
            printf(RESET_COLOR);
            return;
        }
    }


    printf("\n\tAre you sure to remove this Train?\n\t\u2022All the schedules associated with this train will be removed.\n\t\u2022All the registraions will be canceled.\n\t\u2022Press '1' to conform deletion.\n");
    printf(INNER_QUERY);
    printf("\n\tConformation: ");
    printf(RESET_COLOR);
    int con = 0;
    if (scanf("%d", &con) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }
    if (con != 1){
        return;
    }

    
    snprintf(query, sizeof(query), "DELETE FROM Trains WHERE id = %d;", t_id);
    stmt = execute_command(query);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-]SQL Error while deleting the train.\n");
        printf(RESET_COLOR);
        return;
    }

    snprintf(query, sizeof(query), "DELETE FROM TrainSchedules WHERE t_id = %d;", t_id);
    stmt = execute_command(query);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-]SQL Error while deleting from TrainSchedules.\n");
        printf(RESET_COLOR);
        return;
    }

    snprintf(query, sizeof(query), "UPDATE Reservation SET status = 'TRN-CNL' WHERE t_id = %d;", t_id);
    stmt = execute_command(query);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-]SQL Error while updating from Reservation.\n");
        printf(RESET_COLOR);
        return;
    }
    printf(SUCCESS);
    printf("\t[+]Removed the Train(ID: %d) and Updated the data.\n", t_id);
    printf(RESET_COLOR);

}

void a_option6(){
    char *date_pattern = "^[0-9]{4}-[0-9]{2}-[0-9]{2}$";
    char *time_pattern = "^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])$";
    char dep_date[15], arr_date[15], dep_time[15], arr_time[15];
    int t_id;
    printf("\tDetails of Train Schedule\n");
    printf(INNER_QUERY);
    printf("\tTrain ID: ");
    printf(RESET_COLOR);
    if (scanf("%d", &t_id) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    char query[400];
    snprintf(query, sizeof(query), "SELECT EXISTS(SELECT 1 FROM Trains WHERE id = %d);", t_id);
    sqlite3_stmt *stmt = execute_command(query);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!sqlite3_column_int(stmt, 0)){
            printf(ERROR);
            printf("\t[-] No Train found with ID: %d\n", t_id);
            printf(RESET_COLOR);
            return;
        }
    }
    printf("\n\t\u2022Date Format: yyyy-mm-dd\n\t\u2022Time Format: hh:mm\n");

    dep_date:
    printf(INNER_QUERY);
    printf("\n\tDeparture Date: ");
    printf(RESET_COLOR);
    scanf("%s", dep_date);
    if (!re_check(date_pattern, dep_date)){
        printf(ERROR);
        printf("\t[-]Make sure to enter the date in the specified format.");
        printf(RESET_COLOR);
        goto dep_date;
    }
    dep_time:
    printf(INNER_QUERY);
    printf("\tDeparture Time: ");
    printf(RESET_COLOR);
    scanf("%s", dep_time);
    if (!re_check(time_pattern, dep_time)){
        printf(ERROR);
        printf("\t[-]Make sure to enter the time in the specified format.\n");
        printf(RESET_COLOR);
        goto dep_time;
    }
    arr_date:
    printf(INNER_QUERY);
    printf("\tArrival Date: ");
    printf(RESET_COLOR);
    scanf("%s", arr_date);
    if (!re_check(date_pattern, arr_date)){
        printf(ERROR);
        printf("\t[-]Make sure to enter the date in the specified format.\n");
        printf(RESET_COLOR);
        goto arr_date;
    }
    arr_time:
    printf(INNER_QUERY);
    printf("\tArrival Time: ");
    printf(RESET_COLOR);
    scanf("%s", arr_time);
    if (!re_check(time_pattern, arr_time)){
        printf(ERROR);
        printf("\t[-]Make sure to enter the time in the specified format.\n");
        printf(RESET_COLOR);
        goto arr_time;
    }

    if (!is_departure_before_arrival(dep_date, dep_time, arr_date, arr_time)){
        printf(ERROR);
        printf("\t[-]The departure must be before arrival.\n");
        printf(RESET_COLOR);
        return;
    }

    printf("\tJourney Type:\n\t\t1 \u2192 Forward Journey\n\t\t2 \u2192 Return Journey\n");
    int moving;
    journey:    
    printf(INNER_QUERY);
    printf("\tJourney: ");
    printf(RESET_COLOR);
    if (scanf("%d", &moving) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }
    if (moving != 1 && moving != 2){
        printf(ERROR);
        printf("\t[-]Choose from the above options only.\n");
        printf(RESET_COLOR);
        goto journey;
    }
    moving = (moving==1)?0:1;

    snprintf(query, sizeof(query),
             "INSERT INTO TrainSchedules (t_id, dep_date, dep_time, arr_date, arr_time, "
             "moving, sleeper_count, ac3_count, ac2_count, ac1_count, general_count, canceled) "
             "VALUES (%d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, %d, '');",
             t_id, dep_date, dep_time, arr_date, arr_time,
             moving, 0, 0, 0, 0, 0);
    stmt = execute_command(query);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("[-]SQL Error while inserting into Trains.\n");
        printf(RESET_COLOR);
    }

    sqlite3_finalize(stmt);

    printf(SUCCESS);
    printf("[+] Schedule for the Train(ID: %d) has been added.\n", t_id);
    printf(RESET_COLOR);


}

void a_option7(){
    int sch_id;
    printf(INNER_QUERY);
    printf("\tTrain Schedule ID: ");
    printf(RESET_COLOR);
    if (scanf("%d", &sch_id) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }

    char query[400];
    snprintf(query, sizeof(query), "SELECT EXISTS(SELECT 1 FROM TrainSchedules WHERE id = %d);", sch_id);
    sqlite3_stmt *stmt = execute_command(query);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!sqlite3_column_int(stmt, 0)){
            printf(ERROR);
            printf("\t[-] No Train Schedule found with ID: %d\n", sch_id);
            printf(RESET_COLOR);
            return;
        }
    }


    printf("\n\tAre you sure to remove this Schecule?\n\t\u2022All the registraions of this schedule will be canceled.\n\t\u2022Press '1' to conform deletion.\n");
    printf(INNER_QUERY);
    printf("\n\tConformation: ");
    printf(RESET_COLOR);
    int con = 0;
    if (scanf("%d", &con) != 1) { 
        printf(ERROR);
        printf("\t[-] Invalid Input!\n");
        printf(RESET_COLOR);
        while (getchar() != '\n');
        return;
    }
    if (con != 1){
        return;
    }

    snprintf(query, sizeof(query), "DELETE FROM TrainSchedules WHERE id = %d;", sch_id);
    stmt = execute_command(query);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-]SQL Error while deleting from TrainSchedules.\n");
        printf(RESET_COLOR);
        return;
    }

    snprintf(query, sizeof(query), "UPDATE Reservation SET status = 'TRN-CNL' WHERE sch_id = %d;", sch_id);
    stmt = execute_command(query);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf(ERROR);
        printf("\t[-]SQL Error while updating from Reservation.\n");
        printf(RESET_COLOR);
        return;
    }
    printf(SUCCESS);
    printf("[+]Removed the Train Schedule(ID: %d) and Updated the data.\n", sch_id);
    printf(RESET_COLOR);
}

void a_option8_9(int option){
    char query[400] = 
        "SELECT "
        "    r.reservaton_date, "
        "    r.seats, "
        "    r.status, "
        "    t.t_name, "
        "    ts.dep_time, "
        "    ts.arr_time, "
        "    ts.dep_date, "
        "    ts.arr_date, "
        "    u.name, r.sch_id "
        "FROM "
        "    Reservation r "
        "JOIN "
        "    Trains t ON r.t_id = t.id "
        "JOIN "
        "    Users u ON r.user_id = u.id "
        "JOIN "
        "    TrainSchedules ts ON r.sch_id = ts.id";
    int sch_id;
    char where_query[100];
    if (option == 9){
        printf(INNER_QUERY);
        printf("\tTrain Schedule ID: ");
        printf(RESET_COLOR);
        if (scanf("%d", &sch_id) != 1) { 
            printf(ERROR);
            printf("\t[-] Invalid Input!\n");
            printf(RESET_COLOR);
            while (getchar() != '\n');
            return;
        }
        printf("\n");
        snprintf(where_query, sizeof(where_query), " WHERE r.sch_id = %d;", sch_id);
    }
    strcat(query, (option==8)?";": where_query);
    sqlite3_stmt *stmt = execute_command(query); 
    int rows = 0;
    char *headers[] = {"SID", "Train", "Status", "Name", "Seats", "Reservation Date", "Departure", "Arrival"};
    int cols = sizeof(headers) / sizeof(headers[0]);
    char data[1000][cols][500];   

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *reservation_date = (const char *)sqlite3_column_text(stmt, 0);
        const char *seats = (const char *)sqlite3_column_text(stmt, 1);
        const char *status = (const char *)sqlite3_column_text(stmt, 2);
        const char *train_name = (const char *)sqlite3_column_text(stmt, 3);
        const char *dep_time = (const char *)sqlite3_column_text(stmt, 4);
        const char *arr_time = (const char *)sqlite3_column_text(stmt, 5);
        const char *dep_date = (const char *)sqlite3_column_text(stmt, 6);
        const char *arr_date = (const char *)sqlite3_column_text(stmt, 7);
        const char *name = (const char *)sqlite3_column_text(stmt, 8);
        const char *sch_id = (const char *)sqlite3_column_text(stmt, 9);

        strcpy(data[rows][0], sch_id);
        strcpy(data[rows][1], train_name);
        strcpy(data[rows][2], status);
        strcpy(data[rows][3], name);
        strcpy(data[rows][4], seats);
        strcpy(data[rows][5], reservation_date);
        snprintf(data[rows][6], sizeof(data[rows][5]), "%s %s", dep_date, dep_time);
        snprintf(data[rows][7], sizeof(data[rows][6]), "%s %s", arr_date, arr_time);

        rows++;
    }
    if (rows == 0){
        printf(ERROR);
        printf("\t[-] No Reservations found.\n");
        printf(RESET_COLOR);
    }
    else{
        print_table(headers, cols, data, rows);
    sqlite3_finalize(stmt);
    }
}

void setup_intro() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    console_width = w.ws_col;
    system("clear");
    usleep(100);

    char *red = "\033[0;31m";
    center_group_text("\033[1;32m"
        "                            \n"
        "_ | _  _ __  _    _|_ _ \n"
        "\\^/(/_|(_ (_)|||(/_    |_(_)"
        "\033[0m", console_width
    );

    center_group_text("\033[1;32m" 
        "                                                                                                                                         \n"
        "                                                                          ,----,                                                         \n"
        "                                                                        ,/   .`|                                                         \n"
        "    ,----..                                                ,-.        ,`   .'  :                            ,-.                 ___     \n"
        "   /   /   \\                      ,--,                 ,--/ /|      ;    ;     /   ,--,                 ,--/ /|               ,--.'|_   \n"
        "  /   .     :             ,--,  ,--.'|               ,--. :/ |    .'___,/    ,'  ,--.'|               ,--. :/ |               |  | :,'  \n"
        " .   /   ;.  \\          ,'_ /|  |  |,                :  : ' /     |    :     |   |  |,                :  : ' /                :  : ' :  \n"
        ".   ;   /  ` ;     .--. |  | :  `--'_        ,---.   |  '  /      ;    |.';  ;   `--'_        ,---.   |  '  /       ,---.   .;__,'  /   \n"
        ";   |  ; \\ ; |   ,'_ /| :  . |  ,' ,'|      /     \\  '  |  :      `----'  |  |   ,' ,'|      /     \\  '  |  :      /     \\  |  |   |    \n"
        "|   :  | ; | '   |  ' | |  . .  '  | |     /    / '  |  |   \\         '   :  ;   '  | |     /    / '  |  |   \\    /    /  | :__,'| :    \n"
        ".   |  ' ' ' :   |  | ' |  | |  |  | :    .    ' /   '  : |. \\        |   |  '   |  | :    .    ' /   '  : |. \\  .    ' / |   '  : |__  \n"
        "'   ;  \\; /  |   :  | : ;  ; |  '  : |__  '   ; :__  |  | ' \\ \\       '   :  |   '  : |__  '   ; :__  |  | ' \\ \\ '   ;   /|   |  | '.'| \n"
        " \\   \\  ',  . \\  '  :  `--'   \\ |  | '.'| '   | '.'| '  : |--'        ;   |.'    |  | '.'| '   | '.'| '  : |--'  '   |  / |   ;  :    ; \n"
        "  ;   :      ; | :  ,      .-./ ;  :    ; |   :    : ;  |,'           '---'      ;  :    ; |   :    : ;  |,'     |   :    |   |  ,   /  \n"
        "   \\   \\ .'`--\"   `--`----'     |  ,   /   \\   \\  /  '--'                        |  ,   /   \\   \\  /  '--'        \\   \\  /     ---`-'   \n"
        "    `---`                        ---`-'     `----'                                ---`-'     `----'                `----'               \n"
        "\033[0m", console_width
    );

    print_line(console_width, red);

    printf("\n\n");
}
