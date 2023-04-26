#ifndef _SCAN_H_
#define _SCAN_H_

// MAXTOKENLEN是一个标记的最大尺寸
#define MAXTOKENLEN 40

// tokenString数组存储每个标记的词素
extern char tokenString[MAXTOKENLEN + 1];

// 函数getToken返回源文件中的下一个令牌
TokenType getToken(void);

#endif
