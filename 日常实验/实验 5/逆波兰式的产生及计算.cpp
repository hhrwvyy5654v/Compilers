#include <bits/stdc++.h>
using namespace std;

vector<string> anti_polish;  //用vector容器保存逆波兰式
map<char, int> priority;     // 定义优先级
string operators = "+-*/()"; //定义操作符号

//简单表达式转换成逆波兰式
void convert(string str)
{
    stack<char> opt;          //设置符号栈
    opt.push('#');            //在堆栈的后面插入一个'#'，象征着结束
    string tempStr, tempFlag; // 存储临时变量
    //遍历简单表达
    for (int i = 0; i < str.size(); i++)
    {
        //检查运算符是否为真正的运算符或数字的一部分，如果i等于零，则运算符+&-应被视为数字的一部分
        if (((str[i] == '-' || str[i] == '+' || str[i] == '.') &&
             (i == 0 || string("+-/*(").find(str[i - 1]) != string::npos)) ||
            isdigit(str[i]))
        {
            tempStr = str[i] != '+' ? str.substr(i, 1) : "";
            while (i + 1 < str.size() && operators.find(str[i + 1]) == string::npos)
            {
                tempStr += str[++i];
            }
            //对于负数，将其视作为单目运算符,"-"与一个正数的复合后记作@,@符号放在字符串尾部
            if (tempStr[0] == '-')
            {
                tempStr.erase(0, 1);
                tempFlag = "@";
            }
            else
            {
                tempFlag = "";
            }
            //将临时字符压入逆波兰式容器中
            anti_polish.push_back(tempStr);
            if (!tempFlag.empty())
            {
                anti_polish.push_back(tempFlag);
                tempFlag.clear();
            }
        }
        //对于操作符
        else
        {
            //如果是左括号，则放入符号栈中
            if (str[i] == '(')
            {
                opt.push(str[i]);
            }
            //如果是右括号
            else if (str[i] == ')')
            {
                //不断弹出符号栈顶元素放入队列，直到弹出左括号
                while (opt.top() != '(')
                {
                    anti_polish.emplace_back(1, opt.top());
                    opt.pop();
                }
                opt.pop();
            }
            //如果是非括号操作符
            else
            {
                //根据优先级在逆波兰式容器尾部添加一个新元素
                while (priority[str[i]] <= priority[opt.top()] && str[i] != '^')
                {
                    anti_polish.emplace_back(1, opt.top());
                    opt.pop();
                }
                opt.push(str[i]);
            }
        }
    }
    //遍历结束后，将符号栈中剩余的符号逐个弹出到输出逆波兰式容器中
    while (opt.size() > 1)
    {
        anti_polish.emplace_back(1, opt.top());
        opt.pop();
    }
}

//逆波兰式的计算
double calculator()
{
    //设置一个输出栈
    stack<double> temp;
    //从左到右遍历逆波兰式
    for (const auto &i : anti_polish)
    {
        try
        {
            //将字符串变为双精度浮点数
            temp.push(stod(i));
        }
        catch (...)
        {
            //当发生异常,意味着不是数字而是运算符,在这里启动计算器
            char c = i[0];
            double result = 0;
            //如果是一个负数
            if (c == '@')
            {
                result -= temp.top(); //当前结果减去栈顶的数字
                temp.pop();           //弹出栈顶元素
                temp.push(result);    //将当前结果压入栈顶
            }
            //如果是加号
            else if (c == '+')
            {
                result += temp.top(); //当前结果与栈顶数字相加
                temp.pop();           //弹出栈顶元素
                result += temp.top(); //当前结果与栈顶数字相加
                temp.pop();           //弹出栈顶元素
                temp.push(result);    //将当前结果压入栈顶
            }
            //如果是减号
            else if (c == '-')
            {
                result += temp.top();         //当前结果与栈顶数字相加
                temp.pop();                   //弹出栈顶元素
                result = temp.top() - result; //当前结果=栈顶数字-当前结果
                temp.pop();                   //弹出栈顶元素
                temp.push(result);            //将当前结果压入栈顶
            }
            //如果是乘号
            else if (c == '*')
            {
                result += temp.top(); //当前结果与栈顶数字相加
                temp.pop();           //弹出栈顶元素
                result *= temp.top(); //当前结果乘等于栈顶数字
                temp.pop();           //弹出栈顶元素
                temp.push(result);    //将当前结果压入栈顶
            }
            //如果是除号
            else if (c == '/')
            {
                result += temp.top();         //当前结果与栈顶数字相加
                temp.pop();                   //弹出栈顶元素
                result = temp.top() / result; //当前结果=栈顶数字/当前结果
                temp.pop();                   //弹出栈顶元素
                temp.push(result);            //将当前结果压入栈顶
            }
        }
    }
    return temp.top();
}

int main()
{
    //根据运算规则确定运算符优先级
    priority['*'] = priority['/'] = 3;
    priority['+'] = priority['-'] = 2;
    priority['('] = 1;
    priority['#'] = 0;
    string expression, str;
    cin >> expression;
    cout << "原式:";
    for (int i = 0; i < expression.size() - 1; i++)
    {
        str += expression[i];
        cout << expression[i];
    }
    cout << endl;
    convert(str);
    cout << "逆波兰:";
    for (int i = 0; i < anti_polish.size(); i++)
    {
        cout << (i ? " " : "") << anti_polish[i];
    }
    cout << endl;
    cout << "得出：" << setiosflags(ios::fixed) << setprecision(6) << calculator() << endl;
    return 0;
}
