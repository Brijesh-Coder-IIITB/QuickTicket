#ifndef UTILS_H
#define UTILS_H

#define MAX_STATIONS 2000
#define MAX_STATION_NAME 60

#define RESET_COLOR "\033[0m"
#define BLUE "\033[0;34m"
#define ERROR "\033[0;31m"
#define SUCCESS "\033[0;32m"
#define INNER_QUERY "\033[1;36m"
#define CHOICE "\033[0;94m"

typedef struct Node {
    char *data;             
    struct Node *prev;
    struct Node *next;     
} Node;

typedef struct DoublyLinkedList {
    Node *head;             
    Node *tail;             
} DoublyLinkedList;

typedef struct {
    char t_name[300];
    char stations[150][200];
    int coach_cap, sleeper, general, ac3, ac2, ac1;
} Train;

DoublyLinkedList* create_list();
void insert_at_end(DoublyLinkedList *list, const char *data);
void traverse_list(DoublyLinkedList *list, int order);
void free_list(DoublyLinkedList *list);

int split_string(const char *input, int rows, int cols, char output[rows][cols]);
void merge_strings(char *output, char arr[][200], int count);
int compare_strings(const char *str1, const char *str2);
void format_string(const char *input, char *output);
int re_check(char *pattern, char *input);
int is_departure_before_arrival(char *dep_date, char *dep_time, char *arr_date, char *arr_time);
void center_text(const char *text, int width, char* colour);
void center_group_text(const char *text, int console_width);
int is_date_time_in_future(const char *date_str, const char *time_str);



#endif