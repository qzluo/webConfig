#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cgi_common.h"

#define MAX_DATA_LEN   1024

#define DATATRANS_FILEPATH  "/opt/ip_com_gateway/data_trans.dat"
 
#define KEY_DATA   "data"


/*
  Function: Get value from environment
  return: -1 -- not found.
           0 -- find and save the result in addr.
*/
int get_input_data(char* datas, int buflen)
{
    char buf[MAX_DATA_LEN] = {0};

    if (get_inputs(buf, MAX_DATA_LEN) == 0)
        return -1;

    return get_value_from_query(buf, KEY_DATA, datas, buflen);
}

/*
  Function: write datas
  return: -1 -- failed.
           0 -- success.
*/
int appendDataTrans(void)
{
    char datas[MAX_DATA_LEN] = {0};
    FILE* p_file = NULL;

    //get value
    if (get_input_data(datas, MAX_DATA_LEN))
        return -1;

    p_file = fopen(DATATRANS_FILEPATH, "a");
    if (!p_file)
        return -1;

    fputs(datas, p_file);
    fputs("\r\n", p_file);

    fclose(p_file);
    p_file = NULL;

    return 0;
}

int main(int argc, char** argv)
{    
    int ret = 0;

    ret = appendDataTrans();

    printf("content-type:text/html\n\n");
    printf("%d\n", ret ? 0 : 1);

    return 0;
}
