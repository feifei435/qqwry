#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "libs/qqwry.h"
int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: nali <ip>\n");
        return 0;
    }
    
    char *ip=argv[1];
    char *wrt_path="/etc/QQWry.Dat";
    char country[1024]={'\0'};
    char area[1024]={'\0'};

    //char *test_ip2 = "127.0.0.1";
    //char *test_ip2 = "72.14.207.191";
    //char *test_ip2 = "121.0.23.2";
    qqwrt_get_location(country,area,ip,wrt_path);
    if (strlen(country)>0) {
        printf("%s",country);
    }
    if (strlen(area)>0) {
        if (strlen(country)<=0) {
            printf("unknown");
        } else {
            printf("%s",area);
        }
    }
    printf("\n");
    return 0;
}

