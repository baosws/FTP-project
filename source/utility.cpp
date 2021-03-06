#include "../header/utility.h"
using namespace std;
string PWD;

#define MAX_BUFF (1 << 18)
// utils functions
int send(int sd, string msg) {
    return write(sd, msg.c_str(), msg.length());
}
char* recv(int sd, char* buff, int* cnt, int len) {
    memset(buff, 0, len);
    if (cnt != NULL) {
        *cnt = read(sd, buff, len);
    }
    else {
        read(sd, buff, len);
    }
    return buff;
}

char* inet_ntoa(unsigned int ip) {
    in_addr tmp;
    tmp.s_addr = ip;
    return inet_ntoa(tmp);
}

int get_return_code(char* msg) {
    int res;
    sscanf(msg, "%d", &res);
    return res;
}
void readline(char* buff, int len, FILE* stream) {
    memset(buff, 0, len);
    fgets(buff, len, stream);
    int n = strlen(buff);
    while (buff[n - 1] == '\r' || buff[n - 1] == '\n')
        buff[--n] = 0;
}
vector<string> parse_args(const string& args) {
    FILE* pipe = popen((PWD + "/args_parser " + args).c_str(), "r");
    int n;
    fscanf(pipe, "%d\n", &n);
    char buff[MAX_BUFF];
    vector<string> res;
    for (int i = 0; i < n; ++i) {
        readline(buff, MAX_BUFF, pipe);
        res.emplace_back(buff);
    }
    fclose(pipe);
    return res;
}
int accept(int sd) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    return accept(sd, (sockaddr*)&addr, &len);
}
string join(const vector<string>& v, const string& sep) {
    string res;
    if (v.size()) {
        for (int i = 0; i + 1 < (int)v.size(); ++i)
            res += v[i] + sep;
        res += v.back();
    }
    return res;
}
