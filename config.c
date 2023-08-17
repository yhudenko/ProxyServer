#include "config.h"

int loadConfig(struct DNSConfig* config)
{
    printf("Loading config...\n");
    FILE* configFile = fopen("config.config", "r");
    if (configFile == NULL){
        perror("Fail open file");
        return 1;
    }

    char line[MAX_CONFIG_LINE_LENGTH];

    while (fgets(line, sizeof(line), configFile))
    {
        if (line == NULL){
            perror("Fail read file");
            return 1;
        }

        //Skip empty lines
        if (line[0] == '#' || line[0] == '\n'){
            continue;
        }

        char* key = strtok(line, " ");
        if (key == NULL){
            printf("Undefinited: %s", line);
            continue;
        }

        if (strcmp(key, "server_address") == 0){
            if (getConfigLine(&config->server_address) != 0)
                return 1;
            printf("server_address: %s\n", config->server_address);
        }
        else if (strcmp(key, "server_reply") == 0){
            if (getConfigLine(&config->server_reply) != 0)
                return 1;
            printf("server_reply: %s\n", config->server_reply);
        }
        else if (strcmp(key, "blacklist") == 0){
            while (1)
            {
                fgets(line, sizeof(line), configFile);
                if (line == NULL){
                    perror("Fail read file");
                    return 1;
                }

                if (line[0] == '#' || line[0] == '\n'){
                    continue;
                }
                if (line[0] == '}'){
                    break;
                }
                size_t len = strlen(line);
                line[len - 1] = '\0';

                char* variable = (char*)malloc(len * sizeof(char));
                if (variable == NULL){
                    perror("Malloc error\n");
                    return 1;
                }

                strcpy(variable, line);
                if (addBlacklistItem(&config, variable)){
                    return 1;
                }
                printf("Blacklist%d: %s\n", config->blacklist_length, config->blacklist[config->blacklist_length - 1]);
            }
        }
    }
    // Закрываем файл
    fclose(configFile);
    return 0;
}

void clearConfig(struct DNSConfig* config)
{
    for (int i = 0; i < config->blacklist_length; ++i){
        if (config->blacklist[i] != NULL){
            free(config->blacklist[i]);
            config->blacklist[i] = NULL;
        }   
    }
    config->blacklist_length = 0;
    if (config->server_address != NULL){
        free(config->server_address);
    }
    if (config->server_reply != NULL){
        free(config->server_reply);
    }
}

int isBlacklisted(struct DNSConfig* config, char* domain)
{
    for (int i = 0; i < config->blacklist_length; ++i)
    {
        if (strcmp(domain, config->blacklist[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int getConfigLine(char** variable)
{
    char* temp_value = strtok(NULL, "=");
    if (temp_value == NULL){
        perror("Fail read line");
        return 1;
    }
    temp_value = temp_value + 1;
    size_t len = strlen(temp_value);
    if (len > 0){
        temp_value[len - 1] = '\0';
    }
    len = strlen(temp_value) + 1;
    *variable = (char*)malloc(len * sizeof(char));

    if (*variable == NULL){
        perror("Malloc error\n");
        return 1;
    }
    
    strcpy(*variable, temp_value);
    return 0;
}

int addBlacklistItem(struct DNSConfig** config, char* item)
{
    int len = (*config)->blacklist_length;
    if (len == MAX_CONFIG_LINE_LENGTH){
        return 1;
    }
    (*config)->blacklist[len] = item;
    (*config)->blacklist_length = len + 1;
    return 0;
}
