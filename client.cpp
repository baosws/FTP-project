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

#define ACTIVE 0
#define PASSIVE 1
// tạo port để truyền data
int establish_data_socket(int sd, int mode) {
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

void process(int sd) { // sd là socket để gửi lệnh và nhận phản hồi
    char buff[MAX_BUFF];
    int data_sd = -1;
    int cur_mode = ACTIVE;
    while (1) {
        cout << "ftp> ";
        string cmd;
        // đọc lệnh: ví dụ 'get abc.txt' thì cmd = "get"
        cin >> cmd;
        // đọc tham số: 'mget abc.txt def.cpp ghi.xyz' thì buff="abc.txt def.cpp ghi.xyz"
        readline(buff);
        vector<string> args = parse_args(buff); // tách chuỗi tham số ra thành vector các tham số: "abc.txt def.cpp ghi.xyz" -> {"abc.txt", "def.cpp", "ghi.xyz"} 
        // có phải lệnh liên quan đến server không? mấy lệnh này lưu trong map<string,string> server_commands trong utility.h
        bool is_svcmd = server_commands.find(cmd) != server_commands.end();
        // có phải lệnh trao đổi dữ liệu không? mấy cái này lưu trong set<string> data_commands trong utility.h
        bool is_datacmd = data_commands.find(cmd) != data_commands.end();

        // xử lý lệnh liên quan đến server
        if (is_svcmd) {
            int client_sd = -1;
            // nếu có trao đổi data thì phải tạo socket trao đổi data
            if (is_datacmd) {
                client_sd = establish_data_socket(sd, cur_mode);
            }
            // sau đó gửi command sau khi đã được dịch thành lệnh chuẩn lên server, ví dụ: ls -> NLST, dir -> LIST, get -> RETR, put -> STOR,... chuyển cái này dùng map server_commands<string, string>
            if (args[0] != "") send(sd, (server_commands[cmd] +  " " + args[0] + "\r\n").c_str()); // phải có \r\n ở cuối
			else send(sd, (server_commands[cmd] + "\r\n").c_str());
            cout << recv(sd, buff); // sau khi gửi xong thì nhận phản hồi và in ra stdout
            if (cmd == "bye" || cmd == "quit") { // lệnh thoát
                break;
            }
            if (is_datacmd) {
                // tạo socket trao đổi dữ liệu. phải tạo 2 lần như vậy vì chế độ passive và active tạo hơi khác nhau.
                if (cur_mode == ACTIVE) {
                    sockaddr_in addr;
                    socklen_t len = sizeof(addr);
                    data_sd = accept(client_sd, (sockaddr*)&addr, &len);
                }
                else {
                    data_sd = client_sd;
                }
                if (cmd == "ls" || cmd == "dir") {
                    // nhận data từ socket trao đổi dữ liệu và in ra stdout
                    cout << recv(data_sd, buff);
                    // nhận thêm xác nhận đã gửi xong data từ socket sd
                    cout << recv(sd, buff);
                }
                else if (cmd == "get") {
					char f[MAX_BUFF];
					//send(sd, (server_commands[cmd] +  " " + args[0] + "\r\n").c_str());
					recv(data_sd, f);
					int file_desc = open(args[0].c_str(), O_CREAT | O_EXCL | O_WRONLY, 0666);
					write(file_desc, f, strlen(f));
					close(file_desc);
					cout << recv(sd, buff);
                }
                else if (cmd == "put") {
                    // ...
                }
                // else if(...)

                // đóng socket. phải đóng vì 2 cái này tạo mới mỗi lần gửi/nhận data
                close(data_sd);
                close(client_sd);
            }
        }
        // xử lý lệnh chỉ liên quan client
        else {
            // nếu là lệnh passive/active thì thay đổi mode
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
            else if (cmd == "lcd") {
                // do something, use chdir()
            }
        }
    }
}

int main(int nargs, char* args[]) {
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
}
