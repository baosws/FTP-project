#pragma once
#include <stdio.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <bitset>
#include <string>
#include <unistd.h>
#include <map>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <thread>

#define MAX_BUFF (1 << 16)

std::map<std::string, std::string> server_commands =
{{"ls", "NLST"},
{"dir", "LIST"},
{"cd", "CWD"},
{"mkdir","MKD"},
{"mrdir","RMD"},
{"delete","DELE"},
{"passive", "PASV"},
{"get", "RETR"},
{"pwd", "PWD"},
{"bye", "QUIT"},
{"quit", "QUIT"}
};
std::set<std::string> data_commands = {"ls", "dir", "get", "put"};
// utils functions
int send(int sd, const char* msg) {
    return write(sd, msg, strlen(msg));
}
char* recv(int sd, char* buff, int* cnt = NULL, int len = MAX_BUFF) {
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

void readline(char* buff, int len = MAX_BUFF, FILE* stream = stdin) {
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
