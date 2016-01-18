#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi_common.h"

#define BUFFER_SIZE 256

#define DEVPARAS_FILEPATH  "/opt/ip_com_gateway/dev_paras.dat"
 
#define KEY_TCP   "tcp"
#define KEY_COM2  "com2"
#define KEY_COM3  "com3"
#define KEY_COM4  "com4"

/*
  Function: Get value from environment
  return: -1 -- not found.
           0 -- find and save the result in addr.
*/
int get_input_value(char* tcpParas, char* com2Paras, 
                    char* com3Paras, char* com4Paras, 
                    int buflen)
{
    char buf[BUFFER_SIZE] = {0};

    if (get_inputs(buf, BUFFER_SIZE) == 0)
        return -1;

    if (get_value_from_query(buf, KEY_TCP, tcpParas, buflen))
        return -1;

    if (get_value_from_query(buf, KEY_COM2, com2Paras, buflen))
        return -1;

    if (get_value_from_query(buf, KEY_COM3, com3Paras, buflen))
        return -1;

    return get_value_from_query(buf, KEY_COM4, com4Paras, buflen);
}

/*
  Function: read device value
  return: -1 -- failed.
           0 -- success.
*/
int writeTermioParas(void)
{
    char tcpParas[KEYBUFFER_SIZE] = {0};
    char com2Paras[KEYBUFFER_SIZE] = {0};
    char com3Paras[KEYBUFFER_SIZE] = {0};
    char com4Paras[KEYBUFFER_SIZE] = {0};
    FILE* p_file = NULL;

    //get value
    if (get_input_value(tcpParas, com2Paras,
        com3Paras, com4Paras, KEYBUFFER_SIZE))
        return -1;


    p_file = fopen(DEVPARAS_FILEPATH, "w");
    if (!p_file)
        return -1;

    fputs(tcpParas, p_file);
    fputs("\r\n", p_file);
    fputs(com2Paras, p_file);
    fputs("\r\n", p_file);
    fputs(com3Paras, p_file);
    fputs("\r\n", p_file);
    fputs(com4Paras, p_file);
    fputs("\r\n", p_file);

    fclose(p_file);
    p_file = NULL;

    return 0;
}

int main(int argc, char** argv)
{    
    int ret = 0;

    ret = writeTermioParas();

    printf("content-type:text/html\n\n");
    printf("%s\n", ret ? "Failed to update data" : "Success to update data");

    return 0;
}
