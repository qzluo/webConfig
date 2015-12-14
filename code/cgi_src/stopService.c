#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char* file_path[256] = {0};
    pid_t pid = 0;
    printf("content-type:text/html\n\n");
    printf("<html><head>");
    printf("<meta http-equiv='Content-Type' content='text/html; charset=gb2312' />");
    printf("<title>Start service</title></head>");
    printf("<body><p>停止了服务</p></body></html>\n");

    pid = vfork();
    if (pid == 0) {
        strcpy(file_path, "/home/sky/ehomeserver/bin/ehservice");
	    execl(file_path, file_path, "stop", NULL);
        return -1;
    }

    return 0;
}
