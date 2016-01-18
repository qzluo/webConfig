#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WEBSRV_CMD_SET_SERVICE     0x01
#define WEBSRV_CMD_GET_SERVICE     0x02

#define WEBSRV_CMD_SERVICE_START   0x01
#define WEBSRV_CMD_SERVICE_STOP    0x02

#define WEBSRV_CMD_RSP_SUCCESS              0x01
#define WEBSRV_CMD_RSP_ERROR                0x02
#define WEBSRV_CMD_RSP_NOT_RECOGNIZED       0x03

//define cmd and response structure
typedef struct {
    int cmd;
    char data[16];
} TagWebSrvCmd, TagWebSrvCmdBack;

int oneTcpSession(const char* ipAddr, int port, 
                  const char* indata, int indata_len, 
                  char* outdata, int outdata_len)
{
    int cfd = -1;
    struct sockaddr_in addr = {0};
    int ret = -1;
    struct timeval timeout = {3, 0}; 

    //1. open
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1)
        return -1;

    //1.1 setup timeout    
    setsockopt(cfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
    setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

    //2. prepare address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipAddr);

    //3. connect
    ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        close(cfd);
        return -1;
    }

    //4. communicate
    ret = write(cfd, indata, indata_len);
    if (ret == -1) {
        close(cfd);
        return -1;
    }

    ret = read(cfd, outdata, sizeof(outdata_len));
    if (ret == -1) {
        close(cfd);
        return -1;
    }

    return 0;
}

//op -- WEBSRV_CMD_SERVICE_START, WEBSRV_CMD_SERVICE_STOP
int setEhService(int op)
{
    TagWebSrvCmd cmdSend = {0};
    TagWebSrvCmdBack cmdRsp = {0};

    cmdSend.cmd = WEBSRV_CMD_SET_SERVICE;
    cmdSend.data[0] = op;

    if (oneTcpSession("127.0.0.1", 5678, (const char*)&cmdSend, 
        sizeof(cmdSend), (char*)&cmdRsp, sizeof(cmdRsp)))
        return -1;

    return cmdRsp.cmd == WEBSRV_CMD_RSP_SUCCESS ? 0 : -1;
}

int main(int argc, char** argv)
{
    char* file_path[256] = {0};
    pid_t pid = 0;
    printf("content-type:text/html\n\n");
    printf("<html><head>");
    printf("<meta http-equiv='Content-Type' content='text/html; charset=gb2312' />");
    printf("<title>Start service</title></head>");

    if (setEhService(WEBSRV_CMD_SERVICE_START) == 0) 
        printf("<body><p>启动服务成功</p></body></html>\n");
    else
        printf("<body><p>启动服务失败</p></body></html>\n");

    return 0;
}
