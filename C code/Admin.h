#ifndef ADMIN_H
#define ADMIN_H

struct Admin{
    char username[40];
    char password[40];
};
void Main_menu();
int Admin_page(struct Admin *admin);
void Admin_sign_up(struct Admin *admin);
void Admin_login(struct Admin *admin);
void Unmask_password(char *stored, char *out);

#endif