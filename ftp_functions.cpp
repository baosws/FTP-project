#include "ftp_functions.h"
using namespace std;

void login(int sd) {
    char buff[MAX_BUFF];
    cout << "> Username: ";
    readline(buff);
    send(sd, "USER " + string(buff) + "\r\n");

    cout << recv(sd, buff);
    if (get_return_code(buff) != 331) {
        throw string("Login failed.");
    }
    else {
        cout << "> Password: ";
        readline(buff);
        send(sd, "PASS " + string(buff) + "\r\n");
        memset(buff, 0, MAX_BUFF);
        cout << recv(sd, buff);
        if (get_return_code(buff) != 230) {
            throw string("Login failed.");
        }
    }
}

int connect_server(unsigned int server_ip, unsigned short port) {
    int server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sd == -1) {
        throw string("Socket creation failed");
    }
    
    sockaddr_in server;
    memset((void*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = server_ip;
    if (connect(server_sd, (sockaddr*)&server, sizeof(server)) == -1) {
        throw string("Connection error");
    }
    return server_sd;
}

int client_listen(unsigned short& port) {
    int client_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sd < 0) {
        throw string("Socket creation failed");
    }
    sockaddr_in client;
    memset((void*)&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr =  INADDR_ANY;
    client.sin_port = htons(0);
    if (bind(client_sd, (sockaddr*)&client, sizeof(client)) == -1) {
        close(client_sd);
        throw string("Binding failed");
    }
    if (listen(client_sd, 1) == -1) {
        close(client_sd);
        throw string("Listening failed");
    }
    socklen_t len = sizeof(client);
    getsockname(client_sd, (sockaddr*)&client, &len);
    port = ntohs(client.sin_port);
    return client_sd;
}

int create_data_socket(int sd, int mode) {
    char buff[MAX_BUFF];
    if (mode == PASSIVE) {
        send(sd, "PASV\r\n");
        cout << recv(sd, buff);
        unsigned char h[4], p[2];
        int ret_code;
        sscanf(buff, "%d %*s %*s %*s (%hhu,%hhu,%hhu,%hhu,%hhu,%hhu)",
               &ret_code, h , h + 1, h + 2, h + 3, p + 1, p);
        if (ret_code != 227) {
            return -1;
        }
        unsigned int ip = *((unsigned int*)h);
        unsigned short port = *((unsigned short*)p);
        return connect_server(ip, port);
    }
    else {
        sockaddr_in addr;
        socklen_t len = sizeof(addr);
        getsockname(sd, (sockaddr*)&addr, &len);
        unsigned int ip = addr.sin_addr.s_addr;
        unsigned char* h = (unsigned char*)&ip;
        unsigned short port;
        unsigned char* p = (unsigned char*)&port;
        int client_sd = client_listen(port);
        sprintf(buff, "PORT %hhu,%hhu,%hhu,%hhu,%hhu,%hhu\r\n", h[0], h[1], h[2], h[3], p[1], p[0]);
        send(sd, buff);
        cout << recv(sd, buff);

        return client_sd;
    }
}

/* ------------------------------------------------- */
void func_passive(int& mode) {
    if (mode == PASSIVE) {
        cout << "Passive mode is off.\n";
        mode = ACTIVE;
    }
    else {
        cout << "Passive mode is on.\n";
        mode = PASSIVE;
    }
}
void func_ls(int sd, int mode, string args = "") {
    int client_sd = create_data_socket(sd, mode), data_sd = client_sd;
    send(sd, "NLST " + args + "\r\n");
    
    char buff[MAX_BUFF];
    cout << recv(sd, buff);
    if (mode == ACTIVE) {
        data_sd = accept(client_sd);
    }
    cout << recv(data_sd, buff);
    cout << recv(sd, buff);
    close(client_sd);
    close(data_sd);
}
void func_dir(int sd, int mode, string args = "") {
    int client_sd = create_data_socket(sd, mode), data_sd = client_sd;
    send(sd, "LIST " + args + "\r\n");
    
    char buff[MAX_BUFF];
    cout << recv(sd, buff);
    if (mode == ACTIVE) {
        data_sd = accept(client_sd);
    }
    cout << recv(data_sd, buff);
    cout << recv(sd, buff);
    close(client_sd);
    close(data_sd);
}
void func_cd(int sd, string dir) {
    char buff[MAX_BUFF];
    send(sd, "CWD " + dir + "\r\n");
    cout << recv(sd, buff);
}
void func_pwd(int sd) {
    char buff[MAX_BUFF];
    send(sd, "PWD\r\n");
    cout << recv(sd, buff);
}
void func_get(int sd, int mode, string filename) {
    int client_sd = create_data_socket(sd, mode), data_sd = client_sd;
    send(sd, "RETR " + filename + "\r\n");
    
    char buff[MAX_BUFF];
    cout << recv(sd, buff);

    if (mode == ACTIVE) {
        data_sd = accept(client_sd);
    }
    
    memset(buff, 0, MAX_BUFF);
    int cnt = read(data_sd, buff, MAX_BUFF);
    int file_desc = open(filename.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0666);
    
    write(file_desc, buff, cnt);
    close(file_desc);
    cout << recv(sd, buff);

    close(client_sd);
    close(data_sd);
}
void func_mget(int sd, int mode, vector<string> filenames) {
    for (string& filename : filenames) {
        func_get(sd, mode, filename);
    }
}
void func_put(int sd, int mode, string filename) {
    int client_sd = create_data_socket(sd, mode), data_sd = client_sd;
    send(sd, "STOR " + filename + "\r\n");
    
    char buff[MAX_BUFF];
    cout << recv(sd, buff);

    if (mode == ACTIVE) {
        data_sd = accept(client_sd);
    }
    
    int file_desc = open(filename.c_str(), O_RDONLY);
    if (file_desc == -1) {
        throw string("No such file on the local directory\n");
    }
    struct stat obj;
    stat(filename.c_str(), &obj);
    int size = obj.st_size;
    memset(buff, 0, MAX_BUFF);
    read(file_desc, buff, size);
    write(data_sd, buff, size);
    
    close(file_desc);
    close(data_sd);
    close(client_sd);
    cout << recv(sd, buff);
}
void func_mput(int sd, int mode, vector<string> filenames) {
    for (string& filename : filenames) {
        func_put(sd, mode, filename);
    }
}
void func_quit(int sd) {
    char buff[MAX_BUFF];
    send(sd, "QUIT\r\n");
    cout << recv(sd, buff);
}