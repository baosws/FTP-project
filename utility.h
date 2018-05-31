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
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/stat.h>

#define MAX_BUFF (1 << 18)
// utils functions
int send(int sd, std::string msg);
char* recv(int sd, char* buff, int* cnt = NULL, int len = MAX_BUFF);
char* inet_ntoa(unsigned int ip);
int get_return_code(char* msg);
void readline(char* buff, int len = MAX_BUFF, FILE* stream = stdin);
std::vector<std::string> parse_args(const std::string& args);
int accept(int sd);
std::string join(const std::vector<std::string>& v, const std::string& sep = "");
