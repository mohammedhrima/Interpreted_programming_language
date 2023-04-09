#include <string>
#include <iostream>
#include <list>
#include <sstream>

std::string
removeSpaces(const std::string &str)
{
    std::string s(str);
    int j = 0;
    int N = s.size();

    for (int i = 0; i < N; ++i)
    {
        if (s[i] != ' ')
        {
            s[j] = s[i];
            j++;
        }
    }

    s.resize(j);
    return s;
}

//
// output is a list of strings which map to tokens
//
void tokenize(
    const std::string &str,
    std::list<std::string> &tokens)
{
    std::string num;
    int N = str.size();

    for (int i = 0; i < N; ++i)
    {
        char c = str[i];
        if (isdigit(c))
        {
            num += c;
        }
        else
        {
            if (!num.empty())
            {
                tokens.push_back(num);
                num.clear();
            }
            std::string token;
            token += c;
            tokens.push_back(token);
        }
    }

    if (!num.empty())
    {
        tokens.push_back(num);
        num.clear();
    }
}

// Rules
// exp    : term
//        | exp + term
//        | exp - term
//        ;
// term   : factor
//        | factor * term
//        | factor / term
//        ;
// factor : number
//        | ( exp )
//        ;

class Calculator
{
public:
    Calculator(const std::string &expression);

    void next();
    int exp();
    int term();
    int factor();
    int toInt(const std::string &s);

private:
    std::list<std::string> mTokens;
    std::string mCurrent;
};

Calculator::Calculator(const std::string &expression)
{
    std::string s = removeSpaces(expression);
    // std::cout << s << std::endl;

    tokenize(s, mTokens);

    // std::list<std::string>::iterator it, itEnd;
    // for (it = mTokens.begin(); it != mTokens.end(); ++it) {
    //     std::cout << *it << " ";
    // }
    // std::cout << std::endl;

    mCurrent = mTokens.front();
}

void Calculator::next()
{
    mTokens.pop_front();

    if (!mTokens.empty())
    {
        mCurrent = mTokens.front();
    }
    else
    {
        mCurrent = std::string();
    }
}

int Calculator::exp()
{
    int result = term();
    while (mCurrent == "+" || mCurrent == "-")
    {
        if (mCurrent == "+")
        {
            next();
            result += term();
        }
        if (mCurrent == "-")
        {
            next();
            result -= term();
        }
    }
    return result;
}

int Calculator::term()
{
    int result = factor();
    while (mCurrent == "*" || mCurrent == "/")
    {
        if (mCurrent == "*")
        {
            next();
            result *= factor();
        }
        if (mCurrent == "/")
        {
            next();
            //
            // Could simply be:
            // result /= term();
            //
            // But we need to deal with divide by 0
            //
            int denominator = factor();
            if (denominator != 0)
            {
                result /= denominator;
            }
            else
            {
                result = 0;
            }
        }
    }
    return result;
}

int Calculator::factor()
{
    int result;

    if (mCurrent == "(")
    {
        next();
        result = exp();
        next();
    }
    else
    {
        result = toInt(mCurrent);
        next();
    }

    return result;
}

int Calculator::toInt(const std::string &s)
{
    std::stringstream ss;
    ss << s;
    int x;
    ss >> x;
    return x;
}

int calculate(std::string s)
{
    Calculator calculator(s);
    return calculator.exp();
}

int main()
{
    std::string expression;

    // "3+2*2" = 7
    // " 3/2 " = 1
    // " 3+5 / 2 " = 5

    // my code evalute this to 2, while it's expected to be 7
    expression = "1+2*2+1";
    std::cout << expression << " -> "
              << calculate(expression) << std::endl;

    expression = "3+2*2";
    std::cout << expression << " -> "
              << calculate(expression) << std::endl;

    expression = "3/2";
    std::cout << expression << " -> "
              << calculate(expression) << std::endl;

    expression = "3+5 / 2";
    std::cout << expression << " -> "
              << calculate(expression) << std::endl;

    expression = "(1 + 22) * 3";
    std::cout << expression << " -> "
              << calculate(expression) << std::endl;

    expression = "1 + 1";
    std::cout << expression << " -> "
              << calculate(expression) << std::endl;

    expression = "100000000/1/2/3/4/5/6/7/8/9/10";
    std::cout << expression << " -> "
              << calculate(expression) << std::endl;

    return 0;
}