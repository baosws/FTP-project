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
void ftp_passive(int& mode);
void ftp_ls(int sd, int mode, const std::string& args);
void ftp_dir(int sd, int mode, const std::string& args);
void ftp_cd(int sd, const std::string& dir);
void ftp_pwd(int sd);
void ftp_get(int sd, int mode, const std::string& filename);
void ftp_mget(int sd, int mode, const std::vector<std::string>& filenames);
void ftp_put(int sd, int mode, const std::string& filename);
void ftp_mput(int sd, int mode, const std::vector<std::string>& filenames);
void ftp_delete(int sd, const std::string& filename);
void ftp_mdelete(int sd, const std::vector<std::string>& filenames);
void ftp_mkdir(int sd, const std::string& dir);
void ftp_rmdir(int sd, const std::string& dir);
void ftp_lcd(const std::string& dir);
void ftp_quit(int sd);
