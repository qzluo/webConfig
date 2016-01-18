#include <stdio.h>
#include <string.h>

#define DEVPARAS_FILEPATH  "/opt/ip_com_gateway/dev_paras.dat"

int main(int argc, char* argv[])
{
int tcpport = 0;
    int baud = 0;
    int databit = 0;
    int stopbit = 0;
    char validatebit = 0;
    char buf[256] = {0};
    FILE* p_file = NULL;

    p_file = fopen(DEVPARAS_FILEPATH, "r");
    if (!p_file)
        return -1;

    //head
    printf("content-type:text/xml\n\n");

    printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
    printf("<temios>");

    //tcp data
    while (fgets(buf, 256, p_file)){
        if (strncmp(buf, "tcp", strlen("tcp")) == 0) {
            sscanf(buf, "%*[^:]%*c%d", &tcpport);
            printf("<tcp>");
            printf("<port>%d</port>", tcpport);
            printf("</tcp>");
        }        
        else if (strncmp(buf, "com2", strlen("com2")) == 0) {
            sscanf(buf, "%*[^:]%*c%d%*c%d%*c%d%*c%c", 
                &baud, &databit, &stopbit, &validatebit);

            printf("<com2>");
            printf("<baud>%d</baud>", baud);
            printf("<databit>%d</databit>", databit);
            printf("<stopbit>%d</stopbit>", stopbit);
            printf("<validatebit>%c</validatebit>", validatebit);
            printf("</com2>");
        } 
        else if (strncmp(buf, "com3", strlen("com3")) == 0) {
            sscanf(buf, "%*[^:]%*c%d%*c%d%*c%d%*c%c", 
                &baud, &databit, &stopbit, &validatebit);

            printf("<com3>");
            printf("<baud>%d</baud>", baud);
            printf("<databit>%d</databit>", databit);
            printf("<stopbit>%d</stopbit>", stopbit);
            printf("<validatebit>%c</validatebit>", validatebit);
            printf("</com3>");
        } 
        else if (strncmp(buf, "com4", strlen("com4")) == 0) {
            sscanf(buf, "%*[^:]%*c%d%*c%d%*c%d%*c%c", 
                &baud, &databit, &stopbit, &validatebit);

            printf("<com4>");
            printf("<baud>%d</baud>", baud);
            printf("<databit>%d</databit>", databit);
            printf("<stopbit>%d</stopbit>", stopbit);
            printf("<validatebit>%c</validatebit>", validatebit);
            printf("</com4>");
        } 
    }

    printf("</temios>\n");

    fclose(p_file);
    p_file = NULL;

    return 0;
}
