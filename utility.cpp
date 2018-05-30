#include "utility.h"

#define MAX_BUFF (1 << 18)
// utils functions
int send(int sd, std::string msg) {
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

std::vector<std::string> parse_args(char const* args) {
    std::stringstream ss;
    ss << args;
    std::vector<std::string> res;
    for (std::string s; ss >> s;) {
        res.push_back(s);
    }
    return res;
}
int accept(int sd) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    return accept(sd, (sockaddr*)&addr, &len);
}
std::string join(const std::vector<std::string>& v, const std::string& sep) {
    std::string res;
    if (v.size()) {
        for (int i = 0; i + 1 < (int)v.size(); ++i)
            res += v[i] + sep;
        res += v.back();
    }
    return res;
}
