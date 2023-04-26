#include <bits/stdc++.h>
using namespace std;

char terminator[3] = {'a', 'b', '#'};                         // 存放终结符
char non_terminator[2] = {'S', 'B'};                          // 存放非终结符
char *generative[4] = {"E->S#", "S->BB#", "B->aB#", "B->b#"}; // 存放产生式

// ACTION表
char *action[10][3] = {"S3#", "S4#", NULL,
                       NULL, NULL, "acc",
                       "S6#", "S7#", NULL,
                       "S3#", "S4#", NULL,
                       "r3#", "r3#", NULL,
                       NULL, NULL, "r1#",
                       "S6#", "S7#", NULL,
                       NULL, NULL, "r3#",
                       "r2#", "r2#", NULL,
                       NULL, NULL, "r2#"};

// GOTO表
__attribute__((unused)) vector<int> v1;
int goto1[10][2] = {1, 2,
                    0, 0,
                    0, 5,
                    0, 8,
                    0, 0,
                    0, 0,
                    0, 9,
                    0, 0,
                    0, 0,
                    0, 0};

int a[10]; // 分析栈
char b[10], c[10], c1;
int top1, top2, top3, top, m, n;
string s1;

int main()
{
    int g, h, i, j, k, l, p, y, z, count;
    char x, copy[10], copy1[10];
    top1 = 0;
    top2 = 0;
    top3 = 0;
    top = 0; // 左字符
    a[0] = 0;
    b[0] = '#';
    count = 0; // 步骤计数
    z = 0;
    cout << "请输入表达式:" << endl;

    // 读取表达式
    do
    {
        cin >> c1;
        c[top3] = c1;
        top3 = top3 + 1;
    } while (c1 != '#');

    // 输出步骤、状态栈、符号栈、输入串、ACTION、GOTO表单
    cout << left << setw(12) << "步骤";
    cout << left << setw(12) << "状态栈";
    cout << left << setw(13) << "符号栈";
    cout << left << setw(14) << "输入串";
    cout << left << setw(10) << "ACTION";
    cout << left << setw(8) << "GOTO" << endl;

    do
    {
        y = z; // y,z指向状态栈栈顶
        m = 0;
        n = 0;
        g = top; // 输入的串长度
        j = 0;
        k = 0;
        x = c[top]; // 当前字符
        count++;

        // 打印序列号
        cout << left << setw(10) << count;

        // 输出状态栈
        while (m <= top1)
        {
            s1.clear();
            cout << a[m];
            m = m + 1;
        }
        for (int u = m; u < 10; ++u)
        {
            cout << " ";
        }

        // 输出符号栈
        while (n <= top2)
        {
            s1.push_back(b[n]);
            n = n + 1;
        }
        cout << left << setw(10) << s1;
        s1.clear();

        // 输出输入串
        while (g <= top3)
        {
            s1 += c[g];
            g = g + 1;
        }
        cout << left << setw(10) << s1;
        s1.clear();

        // 查询动作表
        switch (x)
        {
        case 'a':
            j = 0;
            break;
        case 'b':
            j = 1;
            break;
        case '#':
            j = 2;
            break;
        }
        // 当action表项为空
        if (action[y][j] == NULL)
        {
            cout << left << setw(10) << "error";
            getchar();
            getchar();
            return 0;
        }
        // 当action表项不为空
        else
        {
            strcpy(copy, action[y][j]);
        }

        // 处理移进
        if (copy[0] == 'S')
        {
            z = copy[1] - '0'; // 状态栈栈顶元素，移除结尾符号
            top1 = top1 + 1;
            top2 = top2 + 1;
            a[top1] = z;
            b[top2] = x;
            top = top + 1;
            i = 0;
            while (copy[i] != '#')
            {
                s1 += copy[i];
                i++;
            }
            // 输出对应的ACTION值
            cout << left << setw(10) << s1 << endl;
            s1.clear();
        }

        // 处理归约
        else if (copy[0] == 'r')
        {
            i = 0;
            while (copy[i] != '#')
            {
                s1 += copy[i];
                i++;
            }
            cout << left << setw(10) << s1;
            s1.clear();

            // 输出对应的goto值
            h = copy[1] - '0';
            strcpy(copy1, generative[h]);
            if (copy1[0] == 'S')
            {
                k = 0;
            }
            else if (copy1[0] == 'B')
            {
                k = 1;
            }
            l = strlen(generative[h]) - 4;
            top1 = top1 - l + 1;
            top2 = top2 - l + 1;
            y = a[top1 - 1];
            p = goto1[y][k];
            a[top1] = p;
            b[top2] = copy1[0];
            z = p;
            cout << left << setw(10) << p << endl;
        }
    } while (action[y][j] != "acc");
    cout << left << setw(10) << "acc" << endl;
    getchar();
    getchar();
    return 0;
}
