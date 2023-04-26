#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <fstream> //文本操作头文件
#include <map>
using namespace std;

struct node
{
    string start;
    vector<char> str;
    vector<char> O;
    node(string ch) // 初始化部分
    {
        int t;
        for (int i = 0; i < ch.size(); i++)
        {
            if (ch[i] == '-') // 保存开始符号
            {
                start = ch.substr(0, i); // 当遇到-时保存前面的作为开始。

                t = i + 2; // 跳过符号-》
                break;
            }
        }
        // 保存非终结符或非终结符
        int p = t; // 保存上一个记号的结束位置
        for (int i = t; i < ch.size(); i++)
        {
            if (!isalnum(ch[i])) // 如果不是字母也就是说是运算符，则开始把前面的字符串保存在str[i],运算符保存在O中
            {
                O.push_back(ch[i]);
            }
            else
            {

                str.push_back(ch[i]);
            }
        }
    }
    void display()
    {
        cout << start << "->";
        for (int j = 0; j < str.size(); j++)
            cout << str[j];
    }
};
class Grammar
{
private:
    vector<string> V;
    vector<string> T;
    map<string, char> Book;

    vector<node> P;
    string S;
    int type;

public:
    Grammar(ifstream &in)
    {
        string temp;
        stringstream sstr;
        for (int i = 0; i < 4; i++) // 循环四次分别保存VTPS;
        {
            getline(in, temp);
            sstr.clear();
            sstr << temp;

            if (i == 0)
                while (getline(sstr, temp, ','))
                {
                    V.push_back(temp);
                    Book[temp] = 'V';
                }

            else if (i == 1)
                while (getline(sstr, temp, ','))
                {
                    T.push_back(temp);
                    Book[temp] = 'T';
                }

            else if (i == 2)
                while (getline(sstr, temp, ','))
                {
                    node te = node(temp);
                    P.push_back(te);
                }
            else
                S = temp;
        }
    }
    void display() // 将文法输出的函数
    {

        for (int i = 0; i < V.size(); i++)
        {
            cout << V[i];
            if (i != V.size() - 1)
                cout << ",";
        }
        cout << endl;
        for (int i = 0; i < T.size(); i++)
        {
            cout << T[i];
            if (i != T.size() - 1)
                cout << ",";
        }
        cout << endl;

        for (int i = 0; i < P.size(); i++)
        {
            int l = 0;
            P[i].display();
            if (i != P.size() - 1)
                cout << ",";
        }
        cout << endl;
        cout << S << endl;
    }

    /*
    分类遵循以下规则：
    (1)3型文法遵循规范
        第一点：左边必须只有一个字符，且必须是非终结符；
        第二点：其右边最多只能有两个字符，且当有两个字符时必须有一个为终结符而另一个为非终结符。当右边只有一个字符时，此字符必须为终结符。
        第三点：对于3型文法中的所有产生式，其右边有两个字符的产生式，这些产生式右边两个字符中终结符和非终结符的相对位置一定要固定，也就是说如果一个产生式右边的两个字符的排列是：终结符＋非终结符，那么所有产生式右边只要有两个字符的，都必须前面是终结符而后面是非终结符。反之亦然，要么，就全是：非终结符＋终结符。

    (2)2型文法遵循规范：
        第一点：与3型文法的第一点相同，即：左边必须有且仅有一个非终结符。
        第二点：2型文法所有产生式的右边可以含有若干个终结符和非终结符（只要是有限的就行，没有个数限制）。

    (3)1型文法如何判断：
        第一点：1型文法所有产生式左边可以含有一个、两个或两个以上的字符，但其中必须至少有一个非终结符。
        第二点：与2型文法第二点相同。

    (4)最后是0型文法，只要你能描述出来，都属于这个类型，即0型。
    */

    bool check1() // 判断产生式左部长度是否都为1;
    {

        for (int i = 0; i < P.size(); i++)
        {
            if (P[i].start.size() > 1)
                return false;
        }
        return true;
    }
    bool check2()
    {
        for (int i = 0; i < P.size(); i++)
        {
        }
        int a1 = 0, a2 = 0;
        cout << P.size() << endl;
        for (int i = 0; i < P.size(); i++)
        {
            string temp1, temp2;
            temp1.push_back(P[i].str[0]);
            cout << temp1 << endl;
            if (P[i].str.size() == 1 && Book[temp1] != 'T')
                return false;              // 长度为1，但不是非终结符
            else if (P[i].str.size() == 2) // 长度为2但不全符合左线性或右线性
            {
                temp2.push_back(P[i].str[1]);
                cout << temp1 << " " << temp2 << endl;
                cout << Book[temp1] << " " << Book[temp2] << "-" << endl;
                if ((!a1 && !a2) && ((Book[temp1] == 'T' && Book[temp2] == 'V') || (Book[temp1] == 'V' && Book[temp2] == 'T')))
                {
                    a1 = Book[temp1];
                    a2 = Book[temp2];
                }
                else if (a1 && a2 && (Book[temp1] != a1 || Book[temp2] != a2))
                    return false;
            }
        }
        return true;
    }
    bool check3() // 判断P中的所有语法规则，左部是否全为终结符号
    {
        for (int i = 0; i < P.size(); i++)
        {
            bool flag = false; // 衡量是否出现过V的标志变量
            for (int j = 0; j < P[i].start.size(); j++)
            {
                string temp;
                temp.push_back(P[i].start[j]);
                cout << Book[temp] << " " << i << endl;
                if (Book[temp] == 'V')
                    flag = true;
            }
            if (!flag)
                return false;
        }
        return true;
    }
    void classification()
    {
        int flag1 = 1;
        if (check1()) // 判断产生式左部长度是否都为1;如果是则是2型或3型语法
        {
            if (check2()) // 判断右部是否为一个时全部为非终结符，2时则看是否都满足左线性文法或为右线性文法
            {
                type = 3; // 3型文法
            }
            else
            {
                type = 2; // 否则为2型文法
            }
        }
        else
        {
            if (check3()) // 判断P中的所有语法规则，左部是否全为终结符号
            {
                type = 1;
            }
            else
            {
                type = 0;
            }
        }
    }
    int getType() { return type; }
};

int main()
{
    // 1创建流对象
    ifstream in;
    ofstream out;

    for (int i = 1; i < 9; i++)
    {
        string ch = "./Grammar/Grammar";

        ch += char(i + 48);
        ch += ".txt";
        cout << ch << endl;
        // 2、指定打开方式
        in.open(ch, ios::in);

        // 3、核心部分
        // 建立文法
        Grammar gra(in);
        // 文法展示
        gra.display();
        // 文法分类，分类后的结果保存在Grammar类的type上
        gra.classification();

        cout << "该文法类型为" << gra.getType() << "型文法" << endl;
        in.close();
    }

    return 0;
}
