#if !defined(_APP_COMMON_H_)
#define _APP_COMMON_H_

long app_common_getCurrentTime();

char *app_common_chars2Hexstr(const char *chars, int chars_len);
char *app_common_hexstr2Chars(const char *hexstr, int *chars_len);

#endif // _APP_COMMON_H_
