#include "utility.h"
using namespace std;
map<string, string> AUTHENs;
void load_authens() {
    ifstream fin(user_list_file);
    AUTHENs.clear();
    string user, pass;
    while (fin >> user >> pass) {
        AUTHENs[user] = pass;
    }
    fin.close();
}

int login(int sd) {
    char buff[MAX_BUFF];
    send(sd, "USER");
    read(sd, buff, MAX_BUFF);
    string user = buff;
    if (AUTHENs.find(buff) == AUTHENs.end()) {
        return AUTHEN_USER_NOTFOUND;
    }
    send(sd, "PASS");
    read(sd, buff, MAX_BUFF);
    if (AUTHENs[user] != buff) {
        return AUTHEN_INCORRECT_PASSWORD;
    }
    return 0;
}
string exec(const char* cmd) {
    char buff[MAX_BUFF];
    string res;
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        cout << "failed\n";
        throw std::runtime_error("popen() failed!");
    }

    while (!feof(pipe.get())) {
        memset(buff, 0, MAX_BUFF);
        if (fgets(buff, MAX_BUFF, pipe.get()) != nullptr) {
            res += buff;
        }
    }
    res.pop_back();
    return res;
}
int main(int nargs, char* args[]) {
    load_authens();
    int server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sd < 0) {
        return printf("Socket creation failed\n"), 0;
    }
    
    int opt = 1;
    setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    sockaddr_in server;
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = 21;
    server.sin_addr.s_addr = INADDR_ANY;
    if (int k = bind(server_sd, (sockaddr*)&server, sizeof(server))) {
        return printf("Binding failed\n"), 0;
    }
    
    if (int k = listen(server_sd, 5); k == -1) {
        return printf("Listen failed\n"), 0;
    }
    
    sockaddr_in client;
    socklen_t l = sizeof(client);
    int client_sd = accept(server_sd, (sockaddr*)&client, &l);
//     cout << iIP_to_string(client.sin_addr.s_addr) << endl;
//     cout << client.sin_port << endl;
//     return printf("Connected\n"), 0;
    while (1) {
        if (int lg_err = login(client_sd); lg_err != 0) {
            if (lg_err == AUTHEN_USER_NOTFOUND) {
                send(client_sd, "User not found!");
            }
            else if (lg_err == AUTHEN_INCORRECT_PASSWORD) {
                send(client_sd, "Incorrect password!");
            }
            continue;
        }
        send(client_sd, "OK");
        break;
    }

    char buff[MAX_BUFF];
    while (1) {
        read(client_sd, buff, MAX_BUFF);
        if (strcmp(buff, "ls") == 0) {
            string res = exec("ls");
            int n = res.length();
            for (int i = 0; i < n; i += MAX_BUFF) {
                memset(buff, 0, MAX_BUFF);
                memcpy(buff, res.c_str() + i, min(MAX_BUFF, n - i));
                write(client_sd, buff, MAX_BUFF);
            }
            buff[0] = -1;
            write(client_sd, buff, MAX_BUFF);
        }
        else {
            send(client_sd, "Command not found!");
        }
    }
}
