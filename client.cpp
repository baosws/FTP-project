#include "utility.h"
using namespace std;

// login vào server
void login(int sd) {
    char buff[MAX_BUFF];
    cout << "> Username: ";
    readline(buff);
    send(sd, ("USER " + string(buff) + "\r\n").c_str());

    cout << recv(sd, buff);
    if (get_return_code(buff) != 331) {
        throw string("Login failed.");
    }
    else {
        cout << "> Password: ";
        readline(buff);
        send(sd, ("PASS " + string(buff) + "\r\n").c_str());
        memset(buff, 0, MAX_BUFF);
        cout << recv(sd, buff);
        if (get_return_code(buff) != 230) {
            throw string("Login failed.");
        }
    }
}

// tạo socket kết nối tới server $server_ip, port $port
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
    client.sin_addr.s_addr =  INADDR_ANY; // (inet_addr("192.168.56.1"));
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
    cout << inet_ntoa(client.sin_addr.s_addr) << endl;
    port = ntohs(client.sin_port);
    return client_sd;
}

#define ACTIVE 0
#define PASSIVE 1
// tạo port để truyền data
int establish_data_port(int sd, int mode) {
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

void process(int sd) {
    char buff[MAX_BUFF];
    int data_sd = -1;
    int cur_mode = ACTIVE;
    while (1) {
        cout << "ftp> ";
        string cmd;
        cin >> cmd;
        readline(buff);
        vector<string> args = parse_args(buff);
        
        if (server_commands.find(cmd) != server_commands.end()) {
            if (cmd == "passive") {
                if (cur_mode == ACTIVE) {
                    cur_mode = PASSIVE;
                    cout << "Passive mode on.\n";
                }
                else {
                    cur_mode = ACTIVE;
                    cout << "Passive mode off.\n";
                }
            }
            else {
                int client_sd = -1;
                if (data_commands.find(cmd) != data_commands.end()) {
                    client_sd = establish_data_port(sd, cur_mode);
                }
                send(sd, (server_commands[cmd] + "\r\n").c_str());
                cout << recv(sd, buff);
                if (data_commands.find(cmd) != data_commands.end()) {
                    if (cur_mode == ACTIVE) {
                        sockaddr_in addr;
                        socklen_t len = sizeof(addr);
                        data_sd = accept(client_sd, (sockaddr*)&addr, &len);
                    }
                    else {
                        data_sd = client_sd;
                    }
                }
                if (cmd == "bye") {
                    break;
                }
                if (data_commands.find(cmd) != data_commands.end()) {
                    cout << recv(data_sd, buff);
                    cout << recv(sd, buff);
                    close(data_sd);
                    close(client_sd);
                }
            }
        }
    }
}

int main(int nargs, char* args[])
try {
    if (nargs < 2) {
        throw string("Server missing");
    }
    
    int server_sd = connect_server(inet_addr(args[1]), 21);
    cout << "Connected to " << args[1] << endl;
    char buff[MAX_BUFF];
    cout << recv(server_sd, buff);
    
    if (get_return_code(buff) != 220) {
        throw string("Connection refused");
    }
    else {
        login(server_sd);
    }
    send(server_sd, "SYST\r\n");
    cout << recv(server_sd, buff);
    
    process(server_sd);
}
catch (string ex) {
    cerr << "Err: " << ex << endl;
}
