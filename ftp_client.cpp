#include "ftp_functions.h"
using namespace std;
void process(int sd) { // sd là socket để gửi lệnh và nhận phản hồi
    char buff[MAX_BUFF];
    int mode = ACTIVE;
    while (1) {
        cout << "ftp> ";
        string cmd;
        // đọc lệnh: ví dụ 'get abc.txt' thì cmd = "get"
        cin >> cmd;
        // đọc tham số: 'mget abc.txt def.cpp ghi.xyz' thì buff="abc.txt def.cpp ghi.xyz"
        readline(buff);
        vector<string> args = parse_args(buff); // tách chuỗi tham số ra thành vector các tham số: "abc.txt def.cpp ghi.xyz" -> {"abc.txt", "def.cpp", "ghi.xyz"} 
        if (cmd == "passive")
            func_passive(mode);
        else if (cmd == "ls")
            func_ls(sd, mode, join(args));
        else if (cmd == "dir")
            func_dir(sd, mode, join(args));
        else if (cmd == "cd")
            func_cd(sd, args[0]);
        else if (cmd == "get")
            func_get(sd, mode, args[0]);
        else if (cmd == "mget")
            func_mget(sd, mode, args);
        else if (cmd == "put")
            func_put(sd, mode, args[0]);
        else if (cmd == "mput")
            func_mput(sd, mode, args);
        else if (cmd == "pwd")
            func_pwd(sd);
        // else if (...)
        else if (cmd == "bye" || cmd == "quit") {
            func_quit(sd);
            break;
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
