#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "utils.h"

int is_date_time_in_future(const char *date_str, const char *time_str) {
    struct tm input_time = {0};
    time_t current_time;
    time_t input_time_t;

    if (sscanf(date_str, "%d-%d-%d", &input_time.tm_year, &input_time.tm_mon, &input_time.tm_mday) != 3) {
        fprintf(stderr, "Invalid date format\n");
        return 0;
    }
    if (sscanf(time_str, "%d:%d", &input_time.tm_hour, &input_time.tm_min) != 2) {
        fprintf(stderr, "Invalid time format\n");
        return 0;
    }

    input_time.tm_year -= 1900;
    input_time.tm_mon -= 1;
    input_time.tm_sec = 0;  

    input_time_t = mktime(&input_time);

    current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    local_time->tm_hour += 5;
    local_time->tm_min += 30;
    mktime(local_time);
    current_time = mktime(local_time);

    return difftime(input_time_t, current_time) >= 0 ? 1 : 0;
}

void format_string(const char *input, char *output) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalnum((unsigned char)input[i])) {
            output[j++] = tolower((unsigned char)input[i]);
        }
    }
    output[j] = '\0';
}

int compare_strings(const char *str1, const char *str2) {
    char formatted_str1[1000], formatted_str2[1000];
    
    format_string(str1, formatted_str1);
    format_string(str2, formatted_str2);
    
    return strcmp(formatted_str1, formatted_str2) == 0 ? 1 : 0;
}

int split_string(const char *input, int rows, int cols, char output[rows][cols]) {
    char temp[cols * rows];
    strcpy(temp, input);

    int count = 0;
    char *token = strtok(temp, ",");

    while (token != NULL && count < rows) {
        strncpy(output[count], token, cols - 1);
        output[count][cols - 1] = '\0';
        count++;
        token = strtok(NULL, ",");
    }

    return count;
}

void merge_strings(char *output, char arr[][200], int count) {
    int i;
    output[0] = '\0';

    for (i = 0; i < count; i++) {
        strcat(output, arr[i]);
        if (i < count - 1) {
            strcat(output, ",");
        }
    }
}

void center_text(const char *text, int width, char* colour) {
    int textLength = strlen(text);
    int padding = (width - textLength) / 2;

    for (int i = 0; i < padding; i++) {
        putchar(' ');
    }
    printf("%s%s%s\n", colour, text, "\033[0m");
}

void center_group_text(const char *text, int console_width) {
    const char *line = text;
    char buffer[1024];

    while (*line != '\0') {
        int i = 0;
        while (*line != '\0' && *line != '\n' && i < sizeof(buffer) - 1) {
            buffer[i++] = *line++;
        }
        buffer[i] = '\0';

        if (*line == '\n') line++;

        int text_length = strlen(buffer);
        int padding = (console_width - text_length) / 2;
        if (padding < 0) padding = 0;

        for (int j = 0; j < padding; j++) {
            putchar(' ');
        }
        printf("%s\n", buffer);
    }
}

int re_check(char *pattern, char *input) {
    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        printf("Could not compile regex\n");
        return 0;
    }

    int match = regexec(&regex, input, 0, NULL, 0);
    regfree(&regex);

    return match == 0;
}

int is_departure_before_arrival(char *dep_date, char *dep_time, char *arr_date, char *arr_time) {
    struct tm dep_tm = {0}, arr_tm = {0};
    time_t dep_epoch, arr_epoch;

    sscanf(dep_date, "%d-%d-%d", &dep_tm.tm_year, &dep_tm.tm_mon, &dep_tm.tm_mday);
    sscanf(dep_time, "%d:%d", &dep_tm.tm_hour, &dep_tm.tm_min);

    sscanf(arr_date, "%d-%d-%d", &arr_tm.tm_year, &arr_tm.tm_mon, &arr_tm.tm_mday);
    sscanf(arr_time, "%d:%d", &arr_tm.tm_hour, &arr_tm.tm_min);

    dep_tm.tm_year -= 1900;
    arr_tm.tm_year -= 1900;
    dep_tm.tm_mon -= 1;
    arr_tm.tm_mon -= 1;

    dep_epoch = mktime(&dep_tm);
    arr_epoch = mktime(&arr_tm);

    return dep_epoch < arr_epoch;
}

DoublyLinkedList* create_list() {
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void insert_at_end(DoublyLinkedList *list, const char *data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = strdup(data);
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
}

void traverse_list(DoublyLinkedList *list, int order) {
    Node *current = (order==0)?list->head:list->tail;
    while (current != NULL) {
        if (((order==0)?current->next:current->prev) != NULL){
            printf("%s \u2794 ", current->data);
        }
        else{
            printf("%s", current->data);
        }
        current = (order==0)?current->next:current->prev;
    }
    printf("\n");
}

void free_list(DoublyLinkedList *list) {
    Node *current = list->head;
    Node *next_node;
    
    while (current != NULL) {
        next_node = current->next;
        free(current->data);
        free(current);
        current = next_node;
    }
    
    free(list);
}