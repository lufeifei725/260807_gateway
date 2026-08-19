#include "app_common.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * 获取当前时间的毫秒数
 * @return 返回从1970年1月1日00:00:00到现在的毫秒数
 */
/**
 * 获取当前时间的毫秒数
 * @return 返回从1970年1月1日00:00:00 UTC到现在的毫秒数
 */
long app_common_getCurrentTime()
{
    struct timeval tv;  // 定义timeval结构体变量，用于存储时间值
    gettimeofday(&tv, NULL);  // 获取当前时间，精确到微秒
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;  // 将秒转换为毫秒，并将微秒转换为毫秒后相加，返回总毫秒数
    
}

char *app_common_chars2Hexstr(const char *chars, int chars_len)
{
    char *hexstr = (char *)malloc(chars_len * 2 + 1);
    if (!hexstr) {
        return NULL;
    }
    for (int i = 0; i < chars_len; i++) {
        snprintf(hexstr + i * 2, 3, "%02X", (unsigned char)chars[i]);
    }
    hexstr[chars_len * 2] = '\0';
    return hexstr;
}

char *app_common_hexstr2Chars(const char *hexstr, int *chars_len)
{
    int hexstr_len = strlen(hexstr);
    *chars_len = hexstr_len  / 2;
    char *chars = (char *)malloc(*chars_len);
    for(int i = 0; i < hexstr_len; i+=2) {
        sscanf(hexstr + i, "%02X", (unsigned int*)(chars+i/2));
    }
    return chars;   

}