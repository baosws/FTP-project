#include "utility.h"
using namespace std;

// tạo socket kết nối tới server $server_ip, port $port
int connect(char* server_ip, unsigned short port) {
    int server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sd < 0) {
        throw "Socket creation failed";
    }
    
    sockaddr_in server;
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(server_ip);
    if (connect(server_sd, (sockaddr*)&server, sizeof(server)) == -1) {
        throw "Connection error";
    }
    return server_sd;
}

// login vào server
void login(int sd) {
    char buff[MAX_BUFF];
    cout << "> Username: ";
    readline(buff);
    send(sd, ("USER " + string(buff) + "\r\n").c_str());
    
    cout << recv(sd, buff);
    if (get_return_code(buff) != 331) {
        throw "Login failed.";
    }
    else {
        cout << "> Password: ";
        readline(buff);
        send(sd, ("PASS " + string(buff) + "\r\n").c_str());
        memset(buff, 0, MAX_BUFF);
        cout << recv(sd, buff);
        if (get_return_code(buff) != 230) {
            throw "Login failed.";
        }
    }
}

#define ACTIVE 0
#define PASSIVE 1
// tạo port để truyền data
int establish_data_port(char const* rep, int mode) {
    if (mode == PASSIVE) {
        unsigned char h[4], p[2];
        int ret_code;
        sscanf(rep, "%d %*s %*s %*s (%hhu,%hhu,%hhu,%hhu,%hhu,%hhu)",
               &ret_code, h , h + 1, h + 2, h + 3, p + 1, p);
        if (ret_code != 227) {
            return -1;
        }
        unsigned int ip = *((unsigned int*)h);
        unsigned short port = *((unsigned short*)p);
        return connect(inet_ntoa(ip), port);
    }
    else {
        
    }
}

void process(int sd) {
    char buff[MAX_BUFF];
    int data_sd = -1;
    int cur_mode = PASSIVE;
    while (1) {
        cout << "ftp> ";
        readline(buff);
        vector<string> args = parse_args(buff);
        string cmd = args[0];
        
        if (server_commands.find(cmd) != server_commands.end()) {
            if (data_commands.find(cmd) != data_commands.end()) {
                if (cur_mode == PASSIVE) {
                    send(sd, "PASV\r\n");
                    cout << recv(sd, buff);
                }
                data_sd = establish_data_port(buff, cur_mode);
            }
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
                send(sd, (server_commands[cmd] + "\r\n").c_str());
                cout << recv(sd, buff);
                if (get_return_code(buff) == 221) {
                    break;
                }
                if (data_commands.find(cmd) != data_commands.end()) {
                    cout << recv(data_sd, buff);
                    cout << recv(sd, buff);
                }
            }
        }
    }
}

int main(int nargs, char* args[]) {
    try {
        if (nargs < 2) {
            throw "Server missing";
        }
        
        int server_sd = connect(args[1], 21);
        cout << "Connected to " << args[1] << endl;
        char buff[MAX_BUFF];
        cout << recv(server_sd, buff);
        
        if (get_return_code(buff) != 220) {
            throw "Connection refused";
        }
        else {
            login(server_sd);
        }
        send(server_sd, "SYST\r\n");
        cout << recv(server_sd, buff);
        
        process(server_sd);
    }
    catch (char const* ex) {
        cerr << "Err: " << ex << endl;
    }
}
