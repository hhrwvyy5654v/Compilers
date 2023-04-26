#include "globals.h"
#include "util.h"
#include "scan.h"

//词法分析，自前向后扫描

//枚举类型，DFA状态
typedef enum
{
  START,
  INASSIGN,
  INCOMMENT,
  INNUM,
  INID,
  DONE,
  INLE,
  INME,
  INUPDOX
} StateType;

// 标识符或保留字的词素,字符数组存储
char tokenString[MAXTOKENLEN + 1];

// 源代码行的输入缓冲区的长度
#define BUFLEN 256

static char lineBuf[BUFLEN]; // 保持当前的行数
static int linepos = 0;      // 在LineBuf中的当前位置
static int bufsize = 0;      // 当前缓冲区字符串的大小
static int EOF_flag = FALSE; // 纠正了EOF时ungetNextChar的行为

// getNextChar从lineBuf获取下一个非空字符，如果lineBuf已耗尽，则读取新行
static int getNextChar(void)
{
  if (!(linepos < bufsize)) //读取新行
  {
    lineno++;
    if (fgets(lineBuf, BUFLEN - 1, source))
    {
      if (EchoSource)
        fprintf(listing, "%d: %s", lineno, lineBuf);
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    {
      EOF_flag = TRUE;
      return EOF;
    }
  }
  else
    return lineBuf[linepos++]; //返回下一个字符
}

// ungetNextChar在lineBuf中回退一格
// 该函数用于区分包含I/不包含E，相当于JUMP
static void ungetNextChar(void)
{
  if (!EOF_flag)
    linepos--;
}

//预留字查找表
static struct
{
  char *str;
  TokenType tok;
} reservedWords[MAXRESERVED] = {{"if", IF}, {"then", THEN}, {"else", ELSE}, {"end", END}, {"repeat", REPEAT}, {"until", UNTIL}, {"read", READ}, {"write", WRITE}};

//查找标识符以查看它是否是保留字！最为简单的线性查找
static TokenType reservedLookup(char *s)
{
  int i;
  for (i = 0; i < MAXRESERVED; i++)
    if (!strcmp(s, reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

/*
getToken()函数基于状态转换实现，返回下一个词法单元。
通过该函数中的状态转移，我们可以推测TINY函数的赋值符号为:
=，{}内为注释，每一句以;结束，
标识符只能由字母组成，支持+、-、*、/、<等简单的运算符。*/

//返回下一个词法单元
TokenType getToken(void)
{
  int tokenStringIndex = 0; //索引
  TokenType currentToken;   //当前词法单元
  StateType state = START;  //当前状态—总是从头开始
  int save;                 //标志，当前字符是否有效
  while (state != DONE)     //匹配到一个词素之后就结束
  {
    int c = getNextChar();
    save = TRUE;
    switch (state) //状态转移
    {
    //起始状态 INASSIGN：输入:之后，等待输入=，从而赋值状态
    case START:
      if (isdigit(c))
        state = INNUM; //数字
      else if (isalpha(c))
        state = INID; // id
      else if (c == ':')
        state = INASSIGN;                                               //冒号
      else if ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r')) //忽略空白字符
        save = FALSE;
      else if (c == '{') //注释
      {
        save = FALSE;
        state = INCOMMENT;
      }
      else
      {
        state = DONE;
        switch (c)
        {
        case EOF:
          save = FALSE;
          currentToken = ENDFILE; //结束
          break;
        //枚举符号
        case '=':
          currentToken = EQ;
          break;
        case '+':
          currentToken = PLUS;
          break;
        case '-':
          currentToken = MINUS;
          break;
        case '*':
          currentToken = TIMES;
          break;
        case '/':
          currentToken = OVER;
          break;
        case '(':
          currentToken = LPAREN;
          break;
        case ')':
          currentToken = RPAREN;
          break;
        case ';':
          currentToken = SEMI;
          break;
        case ',':
          currentToken = COMMA;
          break;
        case '%':
          currentToken = PERCENT;
          break;
        case '\'':
          currentToken = INUPDOX;
          break;
        default:
          currentToken = ERROR;
          break;
        }
      }
      break;
    //注释语句内：输入"{"之后，进入注释状态，编译器忽略注释内容，直至遇到"}"结束
    case INCOMMENT:
      save = FALSE;
      if (c == EOF)
      {
        state = DONE;
        currentToken = ENDFILE;
      }
      //直到遇到"}"为止
      else if (c == '}')
        state = START;
      break;

    case INUPDOX:
      if (c == '\'')
      {
        save = FALSE;
        state = DONE;
        currentToken = STR;
      }
      else if (!(linepos < bufsize))
      {
        save = FALSE;
        state = DONE;
        currentToken = ERROR;
        strcpy(tokenString, "Missing \" \' \" !");
        tokenStringIndex += 15;
      }
      break;

    case INASSIGN: //冒号之后，TINY的语法，赋值语句
      state = DONE;
      if (c == '=')
        currentToken = ASSIGN;
      else
      {
        ungetNextChar();
        save = FALSE; //不保存，回退，出错
        currentToken = ERROR;
      }
      break;
    case INNUM: //输入数字之后，进入数字的匹配状态，遇到非数字结束
      if (!isdigit(c))
      {
        ungetNextChar();    //回退
        save = FALSE;       //不保存现在的字符
        state = DONE;       //完成一个词素的匹配
        currentToken = NUM; //类型为NUM
      }
      break;
    //输入字母之后，进入标识符的匹配状态，可能为用户自定义或预留符，
    //遇到非字母结束 DONE：成功匹配到一个词法单元，完成匹配
    case INID:
      if (!isalpha(c))
      {
        ungetNextChar();
        save = FALSE; //不是字符，代表ID结束，DONE
        state = DONE;
        currentToken = ID;
      }
      break;
    case DONE: //错位情况
    default:   //永远不应该发生
      fprintf(listing, "Scanner Bug: state= %d\n", state);
      state = DONE;
      currentToken = ERROR;
      break;
    }
    // save为true
    if ((save) && (tokenStringIndex <= MAXTOKENLEN))
      tokenString[tokenStringIndex++] = (char)c;
    //匹配到一个词素
    if (state == DONE)
    {
      tokenString[tokenStringIndex] = '\0';
      if (currentToken == ID)
        currentToken = reservedLookup(tokenString); //检测是否为保留字
    }
  }
  //全局变量
  if (TraceScan)
  {
    fprintf(listing, "\t%d: ", lineno);
    //在util.c中，将一个词法单元及其词汇表打印到listing文件中，这个函数里面都是文件打印语句
    printToken(currentToken, tokenString);
  }
  return currentToken;
}