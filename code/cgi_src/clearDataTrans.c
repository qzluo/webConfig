#include <stdio.h>

#define DATATRANS_FILEPATH  "/opt/ip_com_gateway/data_trans.dat"
 
/*
  Function: write datas
  return: -1 -- failed.
           0 -- success.
*/
int clearDataTrans(void)
{
    FILE* p_file = fopen(DATATRANS_FILEPATH, "w");
    if (!p_file)
        return -1;

    fclose(p_file);
    p_file = NULL;

    return 0;
}

int main(int argc, char** argv)
{    
    int ret = 0;

    ret = clearDataTrans();

    printf("content-type:text/html\n\n");
    printf("%d\n", ret ? 0 : 1);

    return 0;
}
