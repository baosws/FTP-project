#include "ftp_functions.h"
using namespace std;

// Input tham số
void input_args(std::vector<string>& args) {
    char buff[MAX_BUFF];
    readline(buff);
    args = parse_args(buff); // tách chuỗi tham số ra thành vector các tham số: "abc.txt def.cpp ghi.xyz" -> {"abc.txt", "def.cpp", "ghi.xyz"} 
}

// Input tham số và đưa ra prompt nếu người dùng chưa nhập
// trả về true nếu có input tham số, false nếu không input tham số
// chuỗi tham số lưu trong args
bool input_args_prompt(const string& arg_prompt, const string& usage_prompt, std::vector<string>& args) {
    bool have_arg = false;
    
    input_args(args);

    if (args.size() > 0) 
        have_arg=true;
    else {
        cout<< arg_prompt;
        input_args(args);

        if (args.size()>0) 
            have_arg=true; 
        else{
            cout << usage_prompt;
        }
    } 
    return have_arg;  
}
void process(int sd) { // sd là socket để gửi lệnh và nhận phản hồi
    char buff[MAX_BUFF];
    int mode = ACTIVE;
    while (1) {
        try {
            cout << "ftp> ";
            string cmd;
            // đọc lệnh: ví dụ 'get abc.txt' thì cmd = "get"
            cin >> cmd;

            // chuỗi prompt hướng dẫn khi người dùng không nhập tham số
            string arg_prompt, usage_prompt;
            vector<string> args;

            if (cmd == "passive"){
                ftp_passive(mode);
            }

            else if (cmd == "ls") {
                input_args(args);
                ftp_ls(sd, mode, join(args));
            }

            else if (cmd == "dir"){
                input_args(args);
                ftp_dir(sd, mode, join(args));
            }

            else if (cmd == "cd"){
                arg_prompt = "(remote-directory) ";
                usage_prompt = "usage: cd remote-directory \n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_cd(sd, args[0]);
            }

            else if (cmd == "get"){
                arg_prompt = "(remote-file) ";
                usage_prompt = "usage: get remote-file [ local-file ]\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_get(sd, mode, args[0]);
            }

            else if (cmd == "mget"){
                arg_prompt = "(remote-files) ";
                usage_prompt = "usage: mget remote-files\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_mget(sd, mode, args);
            }

            else if (cmd == "put"){
                arg_prompt = "(local-file [remote-file]) ";
                usage_prompt = "usage: put local-file remote-file\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_put(sd, mode, args[0]);
            }

            else if (cmd == "mput"){
                arg_prompt = "(local-files) ";
                usage_prompt = "usage: mput local-files\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_mput(sd, mode, args);
            }

            else if (cmd == "pwd")
                ftp_pwd(sd);

            else if (cmd  == "delete"){
                arg_prompt = "(remote-file) ";
                usage_prompt = "usage: delete remote-file\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_delete(sd, args[0]);
            }

            else if (cmd == "mdelete"){
                arg_prompt = "(local-files) ";
                usage_prompt = "usage: mdelete remote-files\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_mdelete(sd, args);
            }

            else if (cmd == "mkdir"){
                arg_prompt = "(directory-name) ";
                usage_prompt = "usage: mkdir directory-name\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_mkdir(sd, args[0]);
            }

            else if (cmd == "rmdir") {
                arg_prompt = "(directory-name) ";
                usage_prompt = "usage: rmdir directory-name\n";
                if (input_args_prompt(arg_prompt,usage_prompt,args))
                    ftp_rmdir(sd, args[0]);
            }

            else if (cmd == "lcd") {
                input_args(args);
                if (args.size() > 0)
                    ftp_lcd(args[0]);
                char buff[MAX_BUFF];
                cout << "Local directory now " << getcwd(buff, MAX_BUFF) << endl;
            }

            else if (cmd == "bye" || cmd == "quit") {
                ftp_quit(sd);
                break;
            }
            else {
                cout << "Invalid command\n";
            }
        }
        catch (string ex) {
            cout << "Error: " << ex << endl;
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
