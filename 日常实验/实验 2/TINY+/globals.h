/****************************************************/
/* 文件：globals.h */
/* 用于TINY编译器的全局类型和变量 */
/* 必须在其他包含文件之前出现 */
/* 编译器构建：原则与实践 */
/* Kenneth C. Louden */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

// MAXRESERVED = 保留字的数量
#define MAXRESERVED 8

//定义记号
typedef enum
{
   ENDFILE,
   ERROR,
   /* 保留字 */
   IF,
   THEN,
   ELSE,
   END,
   REPEAT,
   UNTIL,
   READ,
   WRITE,
   TRUE1,
   FALSE1,
   OR,
   AND,
   NOT,
   INT,
   BOOL1,
   STRING,
   FLOAT,
   DOUBLE,
   DO,
   WHILE,
   /*多字符标记 */
   ID,
   NUM,
   STR,
   /* 特殊符号 */
   ASSIGN,
   EQ,
   LT,
   MT,
   ME,
   LE,
   PLUS,
   MINUS,
   TIMES,
   OVER,
   LPAREN,
   RPAREN,
   SEMI,
   COMMA,
   UPDOX,
   PERCENT
} TokenType;

extern FILE *source;  // 源代码文本文件
extern FILE *listing; // 列出输出文本文件
extern FILE *code;    // 用于TM模拟器的代码文本文件

extern int lineno; //列表的源行号

//用于解析的语法树
typedef enum
{
   StmtK,
   ExpK
} NodeKind;
typedef enum
{
   IfK,
   RepeatK,
   AssignK,
   ReadK,
   WriteK
} StmtKind;
typedef enum
{
   OpK,
   ConstK,
   IdK
} ExpKind;

// ExpType用于类型检查
typedef enum
{
   Void,
   Integer,
   Boolean
} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
{
   struct treeNode *child[MAXCHILDREN];
   struct treeNode *sibling;
   int lineno;
   NodeKind nodekind;
   union
   {
      StmtKind stmt;
      ExpKind exp;
   } kind;
   union
   {
      TokenType op;
      int val;
      char *name;
   } attr;
   ExpType type; /* for type checking of exps */
} TreeNode;

/**************************************************/
/***********   Flags for tracing       ************/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;
#endif
