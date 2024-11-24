#include <stdio.h>
#include <stdlib.h>
#include "interface.c"


int main() {
    printf("Make sure you are in full screen before executing the program.\nMaximise the window....\nPress '1' to continue: ");
    int con;
    scanf("%d", &con);
    if (con!=1){
        exit(0);
    }
    setup_intro();
    printf("Choose your role..\n\
    1 -> Login as User\n\
    2 -> Login as Admin\n\
    3 -> Create a New User Account\n\
    0 -> Exit\n");
    
    initialise_db();
    login_page();

    // user("tatwik_sai");
    // admin("tatwik_sai");

}
