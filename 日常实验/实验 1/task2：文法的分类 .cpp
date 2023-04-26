#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <fstream> //�ı�����ͷ�ļ�
#include <map>
using namespace std;

struct node
{
    string start;
    vector<char> str;
    vector<char> O;
    node(string ch) // ��ʼ������
    {
        int t;
        for (int i = 0; i < ch.size(); i++)
        {
            if (ch[i] == '-') // ���濪ʼ����
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
            if (!isalnum(ch[i])) // ���������ĸҲ����˵�����������ʼ��ǰ����ַ���������str[i],�����������O��
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
        for (int i = 0; i < 4; i++) // ѭ���Ĵηֱ𱣴�VTPS;
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
    void display() // ���ķ�����ĺ���
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
    ������ѭ���¹���
    (1)3���ķ���ѭ�淶
        ��һ�㣺��߱���ֻ��һ���ַ����ұ����Ƿ��ս����
        �ڶ��㣺���ұ����ֻ���������ַ����ҵ��������ַ�ʱ������һ��Ϊ�ս������һ��Ϊ���ս�������ұ�ֻ��һ���ַ�ʱ�����ַ�����Ϊ�ս����
        �����㣺����3���ķ��е����в���ʽ�����ұ��������ַ��Ĳ���ʽ����Щ����ʽ�ұ������ַ����ս���ͷ��ս�������λ��һ��Ҫ�̶���Ҳ����˵���һ������ʽ�ұߵ������ַ��������ǣ��ս�������ս������ô���в���ʽ�ұ�ֻҪ�������ַ��ģ�������ǰ�����ս���������Ƿ��ս������֮��Ȼ��Ҫô����ȫ�ǣ����ս�����ս����

    (2)2���ķ���ѭ�淶��
        ��һ�㣺��3���ķ��ĵ�һ����ͬ��������߱������ҽ���һ�����ս����
        �ڶ��㣺2���ķ����в���ʽ���ұ߿��Ժ������ɸ��ս���ͷ��ս����ֻҪ�����޵ľ��У�û�и������ƣ���

    (3)1���ķ�����жϣ�
        ��һ�㣺1���ķ����в���ʽ��߿��Ժ���һ�����������������ϵ��ַ��������б���������һ�����ս����
        �ڶ��㣺��2���ķ��ڶ�����ͬ��

    (4)�����0���ķ���ֻҪ��������������������������ͣ���0�͡�
    */

    bool check1() // �жϲ���ʽ�󲿳����Ƿ�Ϊ1;
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
                return false;              // ����Ϊ1�������Ƿ��ս��
            else if (P[i].str.size() == 2) // ����Ϊ2����ȫ���������Ի�������
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
    bool check3() // �ж�P�е������﷨�������Ƿ�ȫΪ�ս����
    {
        for (int i = 0; i < P.size(); i++)
        {
            bool flag = false; // �����Ƿ���ֹ�V�ı�־����
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
        if (check1()) // �жϲ���ʽ�󲿳����Ƿ�Ϊ1;���������2�ͻ�3���﷨
        {
            if (check2()) // �ж��Ҳ��Ƿ�Ϊһ��ʱȫ��Ϊ���ս����2ʱ���Ƿ������������ķ���Ϊ�������ķ�
            {
                type = 3; // 3���ķ�
            }
            else
            {
                type = 2; // ����Ϊ2���ķ�
            }
        }
        else
        {
            if (check3()) // �ж�P�е������﷨�������Ƿ�ȫΪ�ս����
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
    // 1����������
    ifstream in;
    ofstream out;

    for (int i = 1; i < 9; i++)
    {
        string ch = "./Grammar/Grammar";

        ch += char(i + 48);
        ch += ".txt";
        cout << ch << endl;
        // 2��ָ���򿪷�ʽ
        in.open(ch, ios::in);

        // 3�����Ĳ���
        // �����ķ�
        Grammar gra(in);
        // �ķ�չʾ
        gra.display();
        // �ķ����࣬�����Ľ��������Grammar���type��
        gra.classification();

        cout << "���ķ�����Ϊ" << gra.getType() << "���ķ�" << endl;
        in.close();
    }

    return 0;
}
