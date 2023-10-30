#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <set>
#include <map>
using namespace std;

bool isValidSoP(string& sop)
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
                return false;
            }
            prevWasVariable = true;
        }
        else if (c == '+')
        {
            if (!prevWasVariable)
            {
                return false;
            }
            prevWasVariable = false;
        }
        else if (c == '(')
        {
            openBrackets++;
            if (prevWasVariable)
            {
                return false;
            }
            prevWasVariable = false;
        }
        else if (c == ')')
        {
            if (openBrackets <= 0)
            {
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
        return false;
    }

    return true;
}

bool isValidPoS(string& pos)
{
    int openBrackets = 0;
    bool prevWasVariable = false;

    for (char c : pos)
    {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            if (prevWasVariable)
            {
                return false;
            }
            prevWasVariable = true;
        }
        else if (c == '+')
        {
            if (!prevWasVariable)
            {
                return false;
            }
            prevWasVariable = false;
        }
        else if (c == '(')
        {
            if (prevWasVariable)
            {
                return false;
            }
            openBrackets++;
            prevWasVariable = false;
        }
        else if (c == ')')
        {
            if (!prevWasVariable)
            {
                return false;
            }
            openBrackets--;
            prevWasVariable = false;
        }
        else if (c == '\'')
        {
            if (!prevWasVariable)
            {
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
        return false;
    }

    return true;
}

void generateTruthTable(string& function, vector <char>& Variables, vector<vector<bool>>& minterms)
{
    bool result, temp1, temp2;
    vector<vector<bool>> truthTable;
    vector<bool> minterm;
    string canonical_PoS = "";
    string canonical_SoP = "";
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
	    vector<bool> rowValues;
	    
        for (int var = numVariables - 1; var >= 0; var--)
		{
            cout << ((row >> var) & 1) << " | ";
            rowValues.push_back(((row >> var) & 1));
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
                        temp1 = !rowValues[(find(Variables.begin(), Variables.end(), c) - Variables.begin())];
                        i++;
                    }
                    else
                    {
                        temp1 = rowValues[(find(Variables.begin(), Variables.end(), c) - Variables.begin())];
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
            result = true;
            temp2 = false;
            for (int i = 0; i < function.size(); i++)
            {
                char c = function[i];
                char next_c = function[i+1];
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
                {
                    if (next_c == '\'')
                    {
                        temp1 = !rowValues[(find(Variables.begin(), Variables.end(), c) - Variables.begin())];
                        i++;
                    }
                    else
                    {
                        temp1 = rowValues[(find(Variables.begin(), Variables.end(), c) - Variables.begin())];
                    }
                    temp2 |= temp1;
                }
                if (c == ')')
                {
                    result &= temp2;
                    temp2 = false;
                }
            }
        }
        
        if (result)
        {
            for (int i = 0; i < numVariables; i++)
            {
                canonical_SoP += Variables[i];
                if (!rowValues[i])
                {
                    canonical_SoP += '\'';
                    minterm.push_back(0);
                }
                else
                {
                    minterm.push_back(1);
                }
            }
            minterms.push_back(minterm);
            minterm.clear();
            canonical_SoP += " + ";
        }
        else
        {
            canonical_PoS += "(";
            for (int i = 0; i < numVariables; i++)
            {
                canonical_PoS += Variables[i];
                if (!rowValues[i])
                {
                    canonical_PoS += '\'';
                }
                canonical_PoS += " + ";
            }
            canonical_PoS.pop_back();
            canonical_PoS.pop_back();
            canonical_PoS.pop_back();
            canonical_PoS += ") ";
        }
        
        cout << result << endl;
    }
    
    canonical_SoP.pop_back();
    canonical_SoP.pop_back();
    canonical_SoP.pop_back();
    
    cout << "canonical SoP : " << canonical_SoP << endl << "canonical PoS : " << canonical_PoS << endl;
}

void generatePrimeImplicants(vector<char>& Variables, vector<vector<bool>>& minterms)
{
    vector<vector<vector<bool>>> groups;
    int maxOnes = 0;
    
    for (const auto& minterm : minterms)
    {
        int onesCount = count(minterm.begin(), minterm.end(), true);
        maxOnes = max(maxOnes, onesCount);
    }
    groups.resize(maxOnes + 1);

    for (const auto& minterm : minterms)
    {
        int onesCount = count(minterm.begin(), minterm.end(), true);
        groups[onesCount].push_back(minterm);
    }

    cout << "Prime Implicants:" << endl;

    set<string> seen;
    
    for (int i = 0; i < groups.size(); ++i)
    {
        for (const auto& term1 : groups[i])
        {
            for (int j = i + 1; j < groups.size(); ++j)
            {
                for (const auto& term2 : groups[j])
                {
                    int diffBits = 0;
                    int diffIndex = -1;

                    for (int k = 0; k < term1.size(); ++k)
                    {
                        if (term1[k] != term2[k])
                        {
                            diffBits++;
                            diffIndex = k;
                        }
                    }
                    
                    if (diffBits == 1)
                    {
                        string primeImplicant;
                        for (int k = 0; k < term1.size(); ++k)
                        {
                            if (k != diffIndex)
                            {
                                if (term1[k] || term2[k])
                                {
                                    primeImplicant += Variables[k];
                                }
                            }
                        }
                        
                        if (seen.find(primeImplicant) == seen.end()) {
                            cout << "PI: " << primeImplicant << " Covers Minterms: ";
                            for (const auto& m : {term1, term2})
                            {
                                for (int k = 0; k < m.size(); ++k)
                                {
                                    cout << m[k];
                                }
                                cout << '\t';
                            }
                            cout << endl;
                            seen.insert(primeImplicant);
                        }
                    }
                }
            }
        }
    }
}

bool coversMinterm(const string& pi, const vector<bool>& minterm) {
    // Assuming that the variables are in the same order as in the minterm vector
    // e.g., if minterm is {true, false} for "AB", then pi should be "AB" or "A'B" etc.
    for (int i = 0; i < pi.size(); ++i) {
        char var = pi[i];
        bool isComplement = (i + 1 < pi.size() && pi[i + 1] == '\'');

        // Check if the variable or its complement is present in the minterm
        if ((isComplement && minterm[i]) || (!isComplement && !minterm[i])) {
            return false;
        }

        // Skip the complement symbol
        if (isComplement) {
            ++i;
        }
    }
    return true;
}


bool isCoveredByEPIs(const vector<bool>& minterm, const vector<string>& EPIs) {
    // Loop through each EPI and check if it covers the minterm
    for (const auto& epi : EPIs) {
        if (coversMinterm(epi, minterm)) {
            return true;
        }
    }
    return false;
}

string findMinimumPI(const vector<bool>& minterm, const vector<string>& PIs) {
    string minPI;
    int minSize = INT_MAX;

    // Loop through each PI and check if it covers the minterm
    for (const auto& pi : PIs) {
        if (coversMinterm(pi, minterm)) {
            if (pi.size() < minSize) {
                minSize = pi.size();
                minPI = pi;
            }
        }
    }
    return minPI;
}


void solvePITable(vector<string>& EPIs, vector<string>& PIs, vector<vector<bool>>& minterms) {
    set<string> minimizedExpression;

    // Add all EPIs to the minimized expression
    for (const auto& epi : EPIs) {
        minimizedExpression.insert(epi);
    }

    // Cover remaining minterms
    for (const auto& minterm : minterms) {
        if (!isCoveredByEPIs(minterm, EPIs)) {
            string minPI = findMinimumPI(minterm, PIs);
            minimizedExpression.insert(minPI);
        }
    }

    // Print the minimized expression
    cout << "Minimized Boolean Expression: ";
    auto it = minimizedExpression.begin();
    if (it != minimizedExpression.end()) {
        cout << *it;
        ++it;
    }
    for (; it != minimizedExpression.end(); ++it) {
        cout << " + " << *it;
    }
    cout << endl;
}

//q7
void printGrayCode(int n, int len) {
    for (int i = len - 1; i >= 0; --i) {
        cout << ((n >> i) & 1);
    }
}

void generateKMap(vector<vector<bool>>& minterms, int numVariables) {
    if (numVariables > 4 || numVariables < 2) {
        cout << "K-Map generation only supports 2 to 4 variables." << endl;
        return;
    }

    int rows = pow(2, ceil(numVariables / 2.0));
    int cols = pow(2, floor(numVariables / 2.0));

    vector<vector<int>> kmap(rows, vector<int>(cols, -1)); // Initialize K-Map with -1 (undefined)

    // Fill in the K-Map based on minterms
    for (const auto& minterm : minterms) {
        int row = 0, col = 0;
        for (int i = 0; i < ceil(numVariables / 2.0); ++i) {
            row = (row << 1) | minterm[i];
        }
        for (int i = ceil(numVariables / 2.0); i < numVariables; ++i) {
            col = (col << 1) | minterm[i];
        }
        kmap[row][col] = 1;
    }

    // Print column labels
    cout << "  ";
    for (int j = 0; j < cols; ++j) {
        printGrayCode(j, floor(numVariables / 2.0));
        cout << " ";
    }
    cout << endl;

    // Print horizontal line
    cout << " +";
    for (int j = 0; j < cols; ++j) {
        for (int k = 0; k < floor(numVariables / 2.0); ++k) {
            cout << "-";
        }
        cout << "+";
    }
    cout << endl;

    for (int i = 0; i < rows; ++i) {
        // Print row labels
        printGrayCode(i, ceil(numVariables / 2.0));
        cout << "|";

        for (int j = 0; j < cols; ++j) {
            if (kmap[i][j] == -1) {
                cout << "0";
            } else {
                cout << kmap[i][j];
            }
            cout << "|";
        }
        cout << endl;

        // Print horizontal line
        cout << " +";
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < floor(numVariables / 2.0); ++k) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
    }
}


int main()
{
    bool flag;
    string function;
    vector <char> Variables;
    vector<vector<bool>> minterms;
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

    generateTruthTable(function, Variables, minterms);
    
    generatePrimeImplicants(Variables, minterms);

    //q3



    //q4




    //q5







//    // Dummy data for testing (You should replace these data from q3 and q4)
//    vector<string> EPIs = {"AB", "AC"};  // Replace with your actual EPIs
//    vector<string> PIs = {"AB", "AC", "BC"};  // Replace with your actual PIs
//    minterms = {{1, 0, 1}, {1, 1, 0}};  // Replace with your actual minterms
//    // Call the function to solve the PI table and print the minimized Boolean expression
//    solvePITable(EPIs, PIs, minterms);
    int numVariables = 4;

    generateKMap(minterms, numVariables);

    return 0;
}