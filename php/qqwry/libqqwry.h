#include <inttypes.h>
/*
 * 请自己给addr1和addr2分配内存，我的建议是:addr1为64字节，addr2为128字节。
 * 这个库不会自己分配内存，而是把结果写入addr1和addr2的内存。
 * 这样的设计可以使得调用方更方便地使用自己的内存机制。
 *
 * addr1是大的范围，例如南宁市
 * addr2是小的范围，例如邕宁区
 */
int qqwry_get_location(char *addr1,char *addr2,const char *ip,FILE *qqwry_file);
int qqwry_get_location_by_long(char *addr1,char *addr2,const uint32_t ip,FILE *qqwry_file);
