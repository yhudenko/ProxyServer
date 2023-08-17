#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLACKLIST_SIZE 1024
#define CONFIG_FILE "config.txt"
#define MAX_CONFIG_LINE_LENGTH 100

enum ConfigParam{
    UNDEFINITED,
    BLACKLIST,
    SERVER_ADDRESS,
    SERVER_REPLY
};

struct Test{
    enum ConfigParam gfdg;
    char* param_word;
};

struct DNSConfig {
    int blacklist_length;
    char* blacklist[MAX_BLACKLIST_SIZE];
    char* server_address;
    char* server_reply;
};

int loadConfig(struct DNSConfig* config);
void clearConfig(struct DNSConfig* config);
int isBlacklisted(struct DNSConfig* config, char* domain);

int getConfigLine(char** variable);
int addBlacklistItem(struct DNSConfig** config, char* item);

#endif //CONFIG_H