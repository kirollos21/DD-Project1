#include <iostream>
#include <string>
using namespace std;

bool isValidSoP(const string& sop)
{
    int openBrackets = 0;
    bool prevWasVariable = false;

    for (char c : sop)
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            prevWasVariable = true;
        }
        else if (c == '\'')
        {
            if (!prevWasVariable)
            {
                cout << "Invalid SoP expression." << endl;
                return false;
            }
            prevWasVariable = true;
        }
        else if (c == '+')
        {
            if (!prevWasVariable)
            {
                cout << "Invalid SoP expression." << endl;
                return false;
            }
            prevWasVariable = false;
        }
        else if (c == '(')
        {
            openBrackets++;
            if (prevWasVariable)
            {
                cout << "Invalid SoP expression." << endl;
                return false;
            }
            prevWasVariable = false;
        }
        else if (c == ')')
        {
            if (openBrackets <= 0)
            {
                cout << "Invalid SoP expression." << endl;
                return false;
            }
            openBrackets--;
            prevWasVariable = true;
        }
        else if (c != ' ')
        {
            cout << "Invalid character in SoP expression: " << c << endl;
            return false;
        }
    }

    if (openBrackets != 0)
    {
        cout << "Invalid SoP expression." << endl;
        return false;
    }

    return true;
}

bool isValidPoS(const string& pos)
{
    int openBrackets = 0;
    bool prevWasVariable = false;

    for (char c : pos)
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            if (prevWasVariable)
            {
                cout << "Invalid PoS expression." << endl;
                return false;
            }
            prevWasVariable = true;
        }
        else if (c == '+')
        {
            if (!prevWasVariable)
            {
                cout << "Invalid PoS expression." << endl;
                return false;
            }
            prevWasVariable = false;
        }
        else if (c == '(')
        {
            if (prevWasVariable)
            {
                cout << "Invalid PoS expression." << endl;
                return false;
            }
            openBrackets++;
            prevWasVariable = false;
        }
        else if (c == ')')
        {
            if (!prevWasVariable)
            {
                cout << "Invalid PoS expression." << endl;
                return false;
            }
            openBrackets--;
            prevWasVariable = false;
        }
        else if (c == '\'')
        {
            if (!prevWasVariable)
            {
                cout << "Invalid PoS expression." << endl;
                return false;
            }
            prevWasVariable = true;
        }
        else if (c != ' ')
        {
            cout << "Invalid character in PoS expression: " << c << endl;
            return false;
        }
    }

    if (openBrackets != 0)
    {
        cout << "Invalid SoP expression." << endl;
        return false;
    }

    return true;
}

int main()
{
    bool flag;
    string function;
    do
    {
        flag = false;
        cout << "Enter a Boolean function in SoP or PoS form: ";
        getline(cin, function);

        if (isValidSoP(function))
        {
            cout << "Valid SoP expression." << endl;
        }
        else if (isValidPoS(function))
        {
            cout << "Valid PoS expression." << endl;
        }
        else
        {
            cout << "Invalid expression. Try another one!" << endl;
            flag = true;
        }
    } while (flag);

    return 0;
}
