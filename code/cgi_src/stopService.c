#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    char* file_path[256] = {0};
    pid_t pid = 0;
    printf("content-type:text/html\n\n");
    printf("<html>");
    printf("<head><title>Start service</title></head>");
    printf("<body><p>Start service</p></body></html>\n");

    pid = vfork();
    if (pid == 0) {
        strcpy(file_path, "/home/sky/ehomeserver/bin/ehservice");
	execl(file_path, file_path, "stop", NULL);
        return -1;
    }

    return 0;
}
