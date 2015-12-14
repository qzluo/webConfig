#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")

#else

#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#endif

#define _TEST

#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define BUFFER_SIZE  1024
#define KEYBUFFER_SIZE 256

#define IPADDR "ipaddr"

#define NETWORK_FILE    "/home/sky/ehomeserver/bin/setip-eth0"

/*  
  return: >0 : validate
          <=0: invalidate
*/
int check_ipaddr_validity(const char *str)
{
#ifdef _WIN32
    int ret = inet_addr(str);
    return  (ret < 0) ? -1 : 1;
#else
    struct in_addr addr = {0};
    return inet_pton(AF_INET, str, &addr);
#endif
}

/*
  Get input string from environment.
  return: string length
*/
int get_inputs(char* inputstring, int buflen)
{
    int length = 0;
    char* method = NULL;

    method = getenv("REQUEST_METHOD");
    if(method == NULL)
        return 0;    

    if (!strcmp(method, "POST")) {
        length = MIN(atoi(getenv("CONTENT_LENGTH")), buflen); 
        if(length != 0) {
            fread(inputstring, sizeof(char), 
                length, stdin);
            inputstring[MIN(length, buflen)] = 0;
        }
    }
    else if(!strcmp(method, "GET")) {
        length = MIN(strlen(getenv("QUERY_STRING")), buflen);
        strncpy(inputstring, getenv("QUERY_STRING"), length);
    }

    return length;
}

/*
  Function: Get value from inputstring used in html.
            the inputstring is in the format：
            key1=value1&key2=value2&...
  return: -1 -- value with the key is not found.
           0 -- find and save the result in value.
*/
int get_value_from_query(const char* inputstring, const char* key, char* value, int buflen)
{
    char keybuf_temp[KEYBUFFER_SIZE] = {0};
    char valuebuf_temp[KEYBUFFER_SIZE] = {0};
    char* keyStart = NULL;
    char* valueStart = NULL;

    keyStart = (char*)inputstring;
    while (keyStart) {
        valueStart = strstr(keyStart, "=");
        if (!valueStart)    //not find '='
            return -1;

        if (keyStart == valueStart)
            return -1;

        //copy key
        strncpy(keybuf_temp, keyStart, (valueStart - keyStart));
        keybuf_temp[valueStart - keyStart] = 0;
        
        //find next key
        valueStart++;
        keyStart = strstr(valueStart, "&");

        //copy value
        if (keyStart) {
            strncpy(valuebuf_temp, valueStart, (keyStart - valueStart));
            valuebuf_temp[keyStart - valueStart] = 0;
            keyStart++;
        }
        else
            strcpy(valuebuf_temp, valueStart);

        //compare key
        if (strcmp(key, keybuf_temp) == 0) {
            //copy value
            strncpy(value, valuebuf_temp, buflen);
            return 0;
        }
    }

    return -1;
}

void test_get_value_from_query(void)
{
    char inputstring[KEYBUFFER_SIZE] = {0};
    char key[KEYBUFFER_SIZE] = {0};
    char value[KEYBUFFER_SIZE] = {0};
    int ret = 0;

    strcpy(inputstring, "ipaddr=192.168.1.112&submask=255.255.255.0");
    strcpy(key, "ipaddr");

    printf("inputstring: %s, key: %s\n", inputstring, key);
    ret = get_value_from_query(inputstring, key, value, KEYBUFFER_SIZE);
    if (ret == 0) 
        printf("find %s, value: %s\n", key, value);
    else
        printf("not find %s\n", key);

    printf("========================================================\n");
    strcpy(key, "submask");

    printf("inputstring: %s, key: %s\n", inputstring, key);
    ret = get_value_from_query(inputstring, key, value, KEYBUFFER_SIZE);
    if (ret == 0) 
        printf("find %s, value: %s\n", key, value);
    else
        printf("not find %s\n", key);

    printf("========================================================\n");
    strcpy(key, "any");

    printf("inputstring: %s, key: %s\n", inputstring, key);
    ret = get_value_from_query(inputstring, key, value, KEYBUFFER_SIZE);
    if (ret == 0) 
        printf("find %s, value: %s\n", key, value);
    else
        printf("not find %s\n", key);

}

/*
  Function: Get ip address from environment
  return: -1 -- not found.
           0 -- find and save the result in ipaddr.
*/
int get_ipaddr(char* ipaddr, int buflen)
{
    char buf[BUFFER_SIZE] = {0};

    if (get_inputs(buf, BUFFER_SIZE) == 0)
        return -1;

    return get_value_from_query(buf, IPADDR, ipaddr, buflen);
}

/*
  Function: write ip address to file
  return: -1 -- not write.
           0 -- write.
*/
int get_ip_and_writefile(const char* filename)
{
    char ipaddr[KEYBUFFER_SIZE] = {0};
    char buf[BUFFER_SIZE] = {0};
    FILE* p_file = NULL;
    int ret = 0;

    if (get_ipaddr(ipaddr, KEYBUFFER_SIZE)) 
        return -1;

    //check ip address
    if (check_ipaddr_validity(ipaddr) <= 0)
        return -1;

    sprintf(buf, "ifconfig eth0 %s", ipaddr);

    //write to file
    p_file = fopen(filename, "w");
    if (p_file == NULL)
        return -1;

    ret = fwrite(buf, sizeof(char), strlen(buf), p_file);
    if (ret < 0) {
        fclose(p_file);
        return -1;
    }

    fclose(p_file);

    return 0;
}

int main(int argc, char** argv)
{
    char retbuf[KEYBUFFER_SIZE] = {0};

    if (get_ip_and_writefile(NETWORK_FILE))
        strcpy(retbuf, "写IP地址失败");
    else
        strcpy(retbuf, "写IP地址成功");

    printf("content-type:text/html\n\n");
    printf("<html><head>");
    printf("<meta http-equiv='Content-Type' content='text/html; charset=gb2312' />");
    
    printf("<title>Start service</title></head>");
    printf("<body><p>%s</p></body></html>\n", retbuf);

    return 0;
}
