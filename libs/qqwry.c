#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#define BE_32(x) ((((uint8_t*)(x))[0]<<24) |\
                  (((uint8_t*)(x))[1]<<16) |\
                  (((uint8_t*)(x))[2]<<8) |\
                  ((uint8_t*)(x))[3])

#define LE_32(x) ((((uint8_t*)(x))[3]<<24) |\
                  (((uint8_t*)(x))[2]<<16) |\
                  (((uint8_t*)(x))[1]<<8) |\
                  ((uint8_t*)(x))[0])

#define LE_24(x) ((((uint8_t*)(x))[2]<<16) |\
                  (((uint8_t*)(x))[1]<<8) |\
                  ((uint8_t*)(x))[0])

#define REDIRECT_TYPE_1 0x01
#define REDIRECT_TYPE_2 0x02
            
static uint32_t ip2long(char *ip) {
    uint32_t ip_long=0;
    uint8_t ip_len=strlen(ip);
    uint32_t ip_sec=0;
    int8_t ip_level=3;
    uint8_t i,n;
    for (i=0;i<=ip_len;i++) {
        if (i!=ip_len && ip[i]!='.' && ip[i]<48 || ip[i]>57) {
            continue;
        }
        if (ip[i]=='.' || i==ip_len) {
            /*too many .*/
            if (ip_level==-1) {
                return 0;
            }
            for (n=0;n<ip_level;n++) {
                ip_sec*=256;
            }
            ip_long+=ip_sec;
            if (i==ip_len) {
                break;
            }
            ip_level--;
            ip_sec=0;
        } else {
            /*char '0' == int 48*/
            ip_sec=ip_sec*10+(ip[i]-48);
        }
    }
    return ip_long;
}
static uint32_t search_index(uint32_t ip,FILE *qqwrt_file) {
    uint32_t index_ip;
    unsigned char head[8];
    unsigned char index_bytes[7];
    fread(head,8,1,qqwrt_file);
    uint32_t index_start,index_end,index_mid;
    index_start = (uint32_t)LE_32(&head[0]);
    index_end = (uint32_t)LE_32(&head[4]);
    while (1) {
        if ((index_end-index_start)==7) {
            break;
        }
        //printf("index:%u:%u\n",index_start,index_end);
        index_mid=index_end/7 - index_start/7;
        if (index_mid%2==0) {
            index_mid=index_mid/2;
        } else {
            index_mid=(index_mid+1)/2;
        }
        index_mid=index_start+index_mid*7;
        fseek(qqwrt_file,index_mid,SEEK_SET);
        fread(index_bytes,7,1,qqwrt_file);
        index_ip=(uint32_t)LE_32(&index_bytes[0]);
        //printf("index_ip:%u\n",index_ip);
        if (index_ip==ip) {
            break;
        } else if (index_ip<ip) {
            index_start=index_mid;
        } else {
            index_end=index_mid;
        }
    }
    if (index_ip>ip) {
        fseek(qqwrt_file,index_start,SEEK_SET);
        fread(index_bytes,7,1,qqwrt_file);
    }
    return (uint32_t)LE_24(&index_bytes[4]);
}
static int readOrJumpRead(char *location,FILE *qqwrt_file,uint32_t data_index) {
    unsigned char c;
    unsigned char data_index_bytes[3];
    uint32_t jump_data_index=0;
    if (data_index) {
        fseek(qqwrt_file,data_index,SEEK_SET);
    }
    c=fgetc(qqwrt_file);
    switch (c) {
        case REDIRECT_TYPE_2:
        case REDIRECT_TYPE_1:
            fread(data_index_bytes,3,1,qqwrt_file);
            jump_data_index=LE_24(&data_index_bytes[0]);
            fseek(qqwrt_file,jump_data_index,SEEK_SET);
            break;
        default:
            location[strlen(location)]=c;
    }
    while (c=fgetc(qqwrt_file)) {
        location[strlen(location)]=c;
    }
    if (jump_data_index!=0) {
        fseek(qqwrt_file,data_index+4,SEEK_SET);
    }
    return 1;

}
static int is_cz88(char *str) {
    int i;
    int l=strlen(str)-7;
    for (i=0;i<l;i++) {
        if (str[i]=='C' 
            && str[i+1]=='Z'
            && str[i+2]=='8'
            && str[i+3]=='8'
            && str[i+4]=='.'
            && str[i+5]=='N'
            && str[i+6]=='E'
            && str[i+7]=='T'
        ) {
            return 1;
        }
    }
    return 0;
}
int qqwrt_get_location_by_long(char *country,char *area,uint32_t ip,char *wrt_path) {
    FILE *qqwrt_file;
    unsigned char data_index_bytes[3];
    uint32_t data_index;
    uint32_t area_offset;
    unsigned char c;

    qqwrt_file = fopen(wrt_path,"rb");
    if (!qqwrt_file) {
        fprintf(stderr,"can not open file:%s",wrt_path);
        return 0;
    }
    data_index = search_index(ip,qqwrt_file);
    //fprintf(stderr,"index:%u:%u\n",ftell(qqwrt_file),data_index);

    fseek(qqwrt_file,data_index+4,SEEK_SET);
    switch (c=fgetc(qqwrt_file)) {
        case REDIRECT_TYPE_1:
    //fprintf(stderr,"mod_1\n");
            fread(data_index_bytes,3,1,qqwrt_file);
            data_index=LE_24(&data_index_bytes[0]);
            readOrJumpRead(country,qqwrt_file,data_index);
            //location[strlen(location)]=' ';
            readOrJumpRead(area,qqwrt_file,0);
            break;
        case REDIRECT_TYPE_2:
    //fprintf(stderr,"mod_2\n");
            area_offset=data_index+8;
            fread(data_index_bytes,3,1,qqwrt_file);

            data_index=LE_24(&data_index_bytes[0]);
            fseek(qqwrt_file,data_index,SEEK_SET);
            while (c=fgetc(qqwrt_file)) {
                country[strlen(country)]=c;
            }
            //location[strlen(location)]=' ';
            readOrJumpRead(area,qqwrt_file,area_offset);
            break;
        default:
    //fprintf(stderr,"mod_d\n");
            country[strlen(country)]=c;
            while (c=fgetc(qqwrt_file)) {
                country[strlen(country)]=c;
            }
            while (c=fgetc(qqwrt_file)) {
                area[strlen(area)]=c;
            }
    }
    if (is_cz88(country)) {
        country[0]='\0';
    }
    if (is_cz88(area)) {
        area[0]='\0';
    }
    return 1;
} 
int qqwrt_get_location(char *country,char *area,char *ip,char *wrt_path) {
    qqwrt_get_location_by_long(country,area,ip2long(ip),wrt_path);
    return 1;
}
