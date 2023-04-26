#include <bits/stdc++.h>
using namespace std;
#define MAXN 100

//全局变量定义
char terminator[MAXN];                     //终结符数组
char String[MAXN][MAXN];                   //转换后的文法规则
char grammar_rules[MAXN][MAXN];            //文法规则
char FIRSTOP_Collection[MAXN][MAXN];       // FIRSTOP集数组
int num, FIRSTOP_Marker[MAXN] = {0};       //文法规则数、FIRSTOP标记
char compare_priority_symbols[MAXN][MAXN]; //比较优先符号

//判断字符ch是否是终极符
int terminator_judgement(char ch)
{
    for (int i = 0; terminator[i] != '\0'; i++)
    {
        if (ch == terminator[i])
        {
            return 1;
        }
    }
    return 0;
}

//获取字符ch在算符优先关系表中的下标
int subscript_judgement(char ch)
{
    for (int i = 0; terminator[i] != '\0'; i++)
    {
        if (ch == terminator[i])
        {
            return i;
        }
    }
    return -1;
}

// 根据FIRSTOP集求的定义计算
void FIRSTOP(char ch)
{
    int i, j, k, m, n;
    //(1) 若X ∈ Vt，则FIRST(X) = {X}。(即定义1)
    for (i = 0; i < num; i++)
    {
        if (grammar_rules[i][0] == ch)
        {
            break;
        }
    }
    /*
    (2）若X ∈ Vn，且有产生式X → a……，a∈Vt，则a∈FIRST(X) (非终结符，将首个终结符加入First集)
    (3）若X ∈ VN，X → ε，则 ε ∈ FIRST(X) (直接推导)
    (4）若X→Y1,Y2,……,Yn ∈ Vn，而有产生式X → Y1,Y2,……,Yn。当Y1,Y2,……,Y(i-1) 直接推出ε时，
    则FIRST(Y1) - ε, FIRST(Y2) - ε, …… , FIRST(Y(i-1) - ε) ,FIRST(Yi) 都包含在FIRST(X)中(无ε)
    (5）当(4）中所有Yi 都推出 ε时，则最后的FIRST(X) = FIRST(Y1)∪FIRST(Y2)∪……∪FIRST(Yn)∪{ε}
    反复运用(2）-(5）步骤计算，直到每个符号的FIRST集合不再增大为止
    */
    if (FIRSTOP_Marker[i] == 0)
    {
        n = FIRSTOP_Collection[i][0] + 1;
        m = 0;
        do
        {
            if (m == 2 || grammar_rules[i][m] == '|')
            {
                if (terminator_judgement(grammar_rules[i][m + 1]))
                {
                    FIRSTOP_Collection[i][n] = grammar_rules[i][m + 1];
                    n++;
                }
                else
                {
                    if (terminator_judgement(grammar_rules[i][m + 2]))
                    {
                        FIRSTOP_Collection[i][n] = grammar_rules[i][m + 2];
                        n++;
                    }
                    if (grammar_rules[i][m + 1] != ch)
                    {
                        FIRSTOP(grammar_rules[i][m + 1]);
                        for (j = 0; j < num; j++)
                        {
                            if (grammar_rules[j][0] == grammar_rules[i][m + 1])
                                break;
                        }
                        for (k = 0; k < FIRSTOP_Collection[j][0]; k++)
                        {
                            int t;
                            for (t = 0; t < n; t++)
                            {
                                if (FIRSTOP_Collection[i][t] == FIRSTOP_Collection[j][k + 1])
                                    break;
                            }
                            if (t == n)
                            {
                                FIRSTOP_Collection[i][n] = FIRSTOP_Collection[j][k + 1];
                                n++;
                            }
                        }
                    }
                }
            }
            m++;
        } while (grammar_rules[i][m] != '\0');
        FIRSTOP_Collection[i][n] = '\0';
        FIRSTOP_Collection[i][0] = --n;
        FIRSTOP_Marker[i] = 1;
    }
}

//算符优先表
void arithmetic_priority_table()
{
    char rules[MAXN][MAXN];
    int i, j, k, t, l, m, n, x = 0, y = 0;
    for (i = 0; i < num; i++)
    {
        FIRSTOP(grammar_rules[i][0]);
    }
    for (i = 0; i < num; i++)
    {
        rules[x][y] = grammar_rules[i][0];
        y++;
        for (j = 1; grammar_rules[i][j] != '\0'; j++)
        {
            if (grammar_rules[i][j] == '|')
            {
                rules[x][y] = '\0';
                x++;
                y = 0;
                rules[x][y] = grammar_rules[i][0];
                y++;
                rules[x][y++] = '-';
                rules[x][y++] = '>';
            }
            else
            {
                rules[x][y] = grammar_rules[i][j];
                y++;
            }
        }
        rules[x][y] = '\0';
        x++;
        y = 0;
    }

    //输出转化后的文法规则串
    cout << "转化后的文法为:" << endl;
    for (i = 0; i < x; i++)
    {
        cout << rules[i] << endl;
    }
    for (i = 0; i < x; i++)
    {
        String[i][0] = rules[i][0];
        for (j = 3, l = 1; rules[i][j] != '\0'; j++, l++)
        {
            String[i][l] = rules[i][j];
        }
        String[i][l] = '\0';
    }
    for (i = 0; i < x; i++)
    {
        for (j = 1; rules[i][j + 1] != '\0'; j++)
        {
            if (terminator_judgement(rules[i][j]) && terminator_judgement(rules[i][j + 1]))
            {
                m = subscript_judgement(rules[i][j]);
                n = subscript_judgement(rules[i][j + 1]);
                compare_priority_symbols[m][n] = '=';
            }
            if (rules[i][j + 2] != '\0' && terminator_judgement(rules[i][j]) && terminator_judgement(rules[i][j + 2]) && !terminator_judgement(rules[i][j + 1]))
            {
                m = subscript_judgement(rules[i][j]);
                n = subscript_judgement(rules[i][j + 2]);
                compare_priority_symbols[m][n] = '=';
            }
            if (terminator_judgement(rules[i][j]) && !terminator_judgement(rules[i][j + 1]))
            {
                for (k = 0; k < num; k++)
                {
                    if (grammar_rules[k][0] == rules[i][j + 1])
                    {
                        break;
                    }
                }
                m = subscript_judgement(rules[i][j]);
                for (t = 0; t < FIRSTOP_Collection[k][0]; t++)
                {
                    n = subscript_judgement(FIRSTOP_Collection[k][t + 1]);
                    compare_priority_symbols[m][n] = '<';
                }
            }
        }
    }
    m = subscript_judgement('#');
    for (t = 0; t < FIRSTOP_Collection[0][0]; t++)
    {
        n = subscript_judgement(FIRSTOP_Collection[0][t + 1]);
        compare_priority_symbols[m][n] = '<';
    }
    n = subscript_judgement('#');
    compare_priority_symbols[n][n] = '=';
}

//判断是否是算符文法
void whether_arithmetic_grammar()
{
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; grammar_rules[i][j] != '\0'; j++)
        {
            if (grammar_rules[i][0] < 'A' || grammar_rules[i][0] > 'Z')
            {
                cout << "不是算符文法!" << endl;
            }
            if (grammar_rules[i][j] >= 'A' && grammar_rules[i][j] <= 'Z')
            {
                if (grammar_rules[i][j + 1] >= 'A' && grammar_rules[i][j + 1] <= 'Z')
                {
                    cout << "不是算符文法!" << endl;
                }
            }
        }
    }
    cout << endl;
}

//打印输出所有变量的FIRSTOP集
void show_FIRSTOP()
{
    cout << endl
         << "所有非终结符的FIRSTOP集如下:" << endl;
    for (int i = 0; i < num; i++)
    {
        cout << "FIRSTOP(" << grammar_rules[i][0] << ")={ " << FIRSTOP_Collection[i][1];
        for (int j = 1; j < FIRSTOP_Collection[i][0]; j++)
        {
            cout << "," << FIRSTOP_Collection[i][j + 1];
        }
        cout << " }" << endl;
    }
    cout << endl;
}

int main()
{
    int i, j, k = 0;
    FILE *path;
    char ch[MAXN];
    path = fopen("./error_grammar.txt", "r");
    if (path == NULL)
    {
        cout << "文件读取失败";
        exit(1);
    }
    else
    {
        cout << "文件读取成功\n";
        char line[80];
        while (fgets(ch, MAXN, path) != NULL)
        {
            char *find;
            find = strchr(ch, '\n'); //查找换行符并替换
            if (find)
            {
                *find = '\0';
            }
            FIRSTOP_Collection[i][0] = 0;
            memcpy(grammar_rules[i], ch, sizeof(ch));
            cout << ch << endl;
            i++;
        }
    }
    fclose(path);
    num = i;
    whether_arithmetic_grammar(); //判断是否是算符文法
    for (i = 0; i < num; i++)
    {
        for (j = 0; grammar_rules[i][j] != '\0'; j++)
        {
            if ((grammar_rules[i][j] < 'A' || grammar_rules[i][j] > 'Z') && grammar_rules[i][j] != '-' && grammar_rules[i][j] != '>' && grammar_rules[i][j] != '|')
            {
                terminator[k++] = grammar_rules[i][j];
            }
        }
    }
    terminator[k] = '#';
    terminator[k + 1] = '\0';
    arithmetic_priority_table();
    show_FIRSTOP(); //输出每个非终结符的FIRSTOP集
    return 0;
}
