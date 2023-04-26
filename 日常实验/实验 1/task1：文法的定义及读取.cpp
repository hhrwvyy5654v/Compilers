#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <fstream> //�ı�����ͷ�ļ�
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
                start = ch.substr(0, i); // ������-ʱ����ǰ�����Ϊ��ʼ��

                t = i + 2; // ��������-��
                break;
            }
        }
        // ������ս������ս��
        int p = t; // ������һ���ǺŵĽ���λ��
        for (int i = t; i < ch.size(); i++)
        {
            if (!isalpha(ch[i])) // ���������ĸ
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
        for (int i = 0; i < 4; i++) // ѭ���Ĵηֱ𱣴�VTPS;
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
    // 1����������
    ifstream in;
    ofstream out;
    stringstream sstr;
    // 2��ָ���򿪷�ʽ
    in.open("Grammar.txt", ios::in);
    out.open("test.txt", ios::out);

    // 3�����Ĳ���
    Grammar gra(in);
    gra.display();
    return 0;
}
