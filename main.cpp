#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
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
    bool prevWasVariable;

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

void generateTruthTable(const string& function)
{
	vector <char> Variables;
    bool result, temp1, temp2;
    cout << "Truth Table:" << endl;
    
    int numVariables = 0;
    for (char c : function)
	{
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		{
			Variables.push_back(c);
        }
    }
    
    sort(Variables.begin(), Variables.end());
	Variables.erase(unique(Variables.begin(), Variables.end()), Variables.end());
    numVariables = Variables.size();

    for (int i = 0; i < numVariables; i++)
	{
        cout << Variables[i] << " | ";
    }
    cout << "Func" << endl;

    for (int i = 0; i < numVariables; i++)
	{
        cout << "----";
    }
    cout << "-----" << endl;

    int numRows = pow(2, numVariables);
    for (int row = 0; row < numRows; row++)
	{
        for (int var = numVariables - 1; var >= 0; var--)
		{
            cout << ((row >> var) & 1) << " | ";
        }
        
        if (isValidSoP(function))
        {
            result = false;
            temp2 = true;
            for (int i = 0; i < function.size(); i++)
            {
                char c = function[i];
                char next_c = function[i+1];
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
                {
                    if (next_c == '\'')
                    {
                        temp1 = !((row >> (numVariables - 1 - (find(Variables.begin(), Variables.end(), c) - Variables.begin()))) & 1);
                        i++;
                    }
                    else
                    {
                        temp1 = ((row >> (numVariables - 1 - (find(Variables.begin(), Variables.end(), c) - Variables.begin()))) & 1);
                    }
                    temp2 &= temp1;
                }
                if ((c == '+') || (i+1 >= function.size()))
                {
                    result |= temp2;
                    temp2 = true;
                }
            }
        }
        else
        {
            for (int i = 0; i < function.size(); i++)
            {
                
            }
        }
        
        cout << result << endl;
    }
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
            generateTruthTable(function);
        }
        else if (isValidPoS(function))
        {
            cout << "Valid PoS expression." << endl;
            generateTruthTable(function);
        }
        else
        {
            cout << "Invalid expression. Try another one!" << endl;
            flag = true;
        }
    } while (flag);

    return 0;
}
