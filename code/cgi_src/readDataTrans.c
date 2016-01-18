#include <stdio.h>
#include <string.h>

#define DATATRANS_FILEPATH  "/opt/ip_com_gateway/data_trans.dat"

int main(int argc, char* argv[])
{
    char dataName[32] = {0};
    char srcIp[64] = {0};
    char tarStr[256] = {0};
    char tarType[64] = {0};
    char tarIp[64] = {0};
    int tarPort = 0;
    char srcData[256] = {0};
    char tarData[256] = {0};
    char buf[1024] = {0};
    FILE* p_file = NULL;

    p_file = fopen(DATATRANS_FILEPATH, "r");
    if (!p_file)
        return -1;

    //head
    printf("content-type:text/xml\n\n");

    printf("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>");
    printf("<gatewayDataTrans>\n");

    //tcp data 
    while (fgets(buf, 1024, p_file)){  //no check, not good code
        sscanf(buf, "%[^:]%*[^ ]%*c%[^:]%*c%[^:]%*c%[^:]%*c%[^\r\n]", 
            dataName, srcIp, tarStr, srcData, tarData);
        if (strncmp(tarStr, "tcp", strlen("tcp")) == 0 ||
            strncmp(tarStr, "udp", strlen("udp")) == 0 ) {
            sscanf(tarStr, "%[^ ]%*c%[^ ]%*c%d", 
                tarType, tarIp, &tarPort);
        }        
        else {  //com
            strcpy(tarType, tarStr);
            tarIp[0] = '\0';
            tarPort = 0;
        } 

        printf("<data>");
        printf("<name>%s</name>", dataName);
        printf("<srcTermio>");
        printf("<ip>%s</ip>", srcIp);
        printf("</srcTermio>");

        printf("<tarTermio>");
        printf("<type>%s</type>", tarType);
        printf("<ip>%s</ip>", tarIp);
        printf("<port>%d</port>", tarPort);
        printf("</tarTermio>");

        printf("<srcData>%s</srcData>", srcData);
        printf("<tarData>%s</tarData>", tarData);
        
        printf("</data>\n");
    }

    printf("</gatewayDataTrans>\n");

    fclose(p_file);
    p_file = NULL;

    return 0;
}
