#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/reboot.h>

int main(int argc, char** argv)
{
    printf("content-type:text/html\n\n");
    printf("<html>");
    printf("<head><title>Start service</title></head>");
    printf("<body><p>Restart host</p></body></html>\n");

    return reboot(RB_AUTOBOOT);
}
