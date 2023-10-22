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
    
}

int main()
{
    bool flag = false;
    string function;
    do
    {
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

