#pragma once
#include "utility.h"

// login vào server
void login(int sd);
// tạo socket kết nối tới server $server_ip, port $port
int connect_server(unsigned int server_ip, unsigned short port);
int client_listen(unsigned short& port);
#define ACTIVE 0
#define PASSIVE 1
// tạo port để truyền data
int create_data_socket(int sd, int mode);
/* ------------------------------------------------- */
void func_passive(int& mode);
void func_ls(int sd, int mode, std::string args);
void func_dir(int sd, int mode, std::string args);
void func_cd(int sd, std::string dir);
void func_pwd(int sd);
void func_get(int sd, int mode, std::string filename);
void func_mget(int sd, int mode, std::vector<std::string> filenames);
void func_put(int sd, int mode, std::string filename);
void func_mput(int sd, int mode, std::vector<std::string> filenames);
void func_lcd(int sd, std::string dir); // to do
void func_delete(int sd, std::string filename); // to do
void func_mdelete(int sd, std::vector<std::string> filenames); // to do
void func_mkdir(int sd, std::string dir); // to do
void func_rmdir(int sd, std::string dir); // to do
void func_quit(int sd);
