#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <fstream> //文本操作头文件
using namespace std;

struct node
{
    string start;
    vector<string> str;
    vector<char> O;
    node(string ch)
    {
        int t;
        for (int i = 0; i < ch.size(); i++)
        {
            if (ch[i] == '-')
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
            if (!isalpha(ch[i])) // 如果不是字母
            {
                string temp = ch.substr(p, i - p);
                str.push_back(temp);
                O.push_back(ch[i]);
                p = i + 1;
            }
            else if (isalpha(ch[i]) && i == ch.size() - 1)
            {
                string temp = ch.substr(p, i - p + 1);
                str.push_back(temp);
            }
        }
    }
    void display()
    {
        int l = 0;
        cout << start << "->" << str[l++];

        for (int j = l; j < str.size(); j++)
            cout << O[j - 1] << str[j];
    }
};
class Grammar
{
private:
    vector<string> V;
    vector<string> T;
    vector<node> P;
    string S;

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
                }

            else if (i == 1)
                while (getline(sstr, temp, ','))
                {
                    T.push_back(temp);
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
    void display()
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
};

int main()
{
    // 1创建流对象
    ifstream in;
    ofstream out;
    stringstream sstr;
    // 2、指定打开方式
    in.open("Grammar.txt", ios::in);
    out.open("test.txt", ios::out);

    // 3、核心部分
    Grammar gra(in);
    gra.display();
    return 0;
}
