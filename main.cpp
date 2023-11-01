#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <stdlib.h>
#include <bitset>
#include <sstream>
#include <limits.h>
#include <fstream>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

void printVec(vector<string> s) {
    if (s.empty()) { cout << "-"; return; }
    cout << s.at(0);
    for (int i = 1; i < s.size(); i++) cout << ", " << s.at(i);
}

void printVec(vector<char> s) {
    if (s.empty()) return;
    for (int i = 0; i < s.size(); i++) cout<< s.at(i);
}

void printSet(set<int> s) {
    if (s.empty()) { cout << "[]"; return; }
    auto it = s.begin();
    cout << "[" << *s.begin();
    it++;
    while (it != s.end()) {
        cout << ", " << *it;
        it++;
    }
    cout << "]";
}

string fullExpression(vector<string> s) {
    string exp = "";
    if (s.empty()) { return "-"; }
    exp += s.at(0);
    for (int i = 1; i < s.size(); i++) exp += " + " + s.at(i);
    return exp;
}

struct implicationRow {

        set<int> coveredMinterms;
        vector<char> mintermBits;

        void printRow() {
            printSet(coveredMinterms);
            std::cout << "\t\t\t";
            printVec(mintermBits);
            std::cout << endl;
        }
        
        implicationRow combine(const implicationRow& other, const int& difBit) {
            implicationRow temp = implicationRow();
            temp.coveredMinterms = this->coveredMinterms;
            for (int i = 0; i < difBit; i++) temp.mintermBits.push_back(this->mintermBits.at(i));
            temp.mintermBits.push_back('-');
            for (int i = difBit + 1; i < this->mintermBits.size(); i++) temp.mintermBits.push_back(this->mintermBits.at(i));
            temp.coveredMinterms.insert(other.coveredMinterms.begin(), other.coveredMinterms.end());
            std::cout << endl;
            return temp;
        }

        bool covers(const int& minterm) {
            return (coveredMinterms.find(minterm) != coveredMinterms.end());
        }

        string booleanExpression(vector<char> variables) {
            string boolExp = "";
            string temp;
            for (int i = 0; i < variables.size(); i++) {
                if (mintermBits.at(i) == '1') boolExp += variables.at(i);
                else if (mintermBits.at(i) == '0'){
                    temp = variables.at(i);
                    boolExp += temp + "'";
                }
            }
            return boolExp;
        }
    };

inline bool operator<(implicationRow mt1, implicationRow mt2) {
    return tie(mt1.coveredMinterms, mt1.mintermBits) < tie(mt2.coveredMinterms, mt2.mintermBits);
}

inline bool operator!=(implicationRow mt1, implicationRow mt2) {
    return tie(mt1.coveredMinterms, mt1.mintermBits) != tie(mt2.coveredMinterms, mt2.mintermBits);
}

bool logicallyAdj(vector<char> exp1, vector<char> exp2, int& differentBitIndex) {
    if (exp1.size() != exp2.size()) { differentBitIndex = -1; return 0; }
    int incons = 0, tempInd = 0;
    for (auto it1 = exp1.begin(), it2 = exp2.begin(); it1 != exp1.end(); it1++, it2++, tempInd++) {
        if (*it1 != *it2) {
            // // This vv means you have met more than 1 different actual bit (not "-"), so they are not log. adj.
            // if (incons > 0 || (*it1 == '-' && *it2 != '-') || (*it1 != '-' && *it2 == '-')) {
            //     differentBitIndex = -1; return 0;
            // }
            incons++;
            differentBitIndex = tempInd;
        }
    }
    return incons == 1;
}

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
            std::cout << "Invalid character in SoP expression: " << c << endl;
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
            std::cout << "Invalid character in PoS expression: " << c << endl;
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
    std::cout << "Truth Table:" << endl;

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
        std::cout << Variables[i] << " | ";
    }
    std::cout << "Func" << endl;

    for (int i = 0; i < numVariables; i++)
	{
        std::cout << "----";
    }
    std::cout << "-----" << endl;

    int numRows = pow(2, numVariables);
    for (int row = 0; row < numRows; row++)
	{
	    vector<bool> rowValues;

        for (int var = numVariables - 1; var >= 0; var--)
		{
            std::cout << ((row >> var) & 1) << " | ";
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

        std::cout << result << endl;
    }

    canonical_SoP.pop_back();
    canonical_SoP.pop_back();
    canonical_SoP.pop_back();

    std::cout << "canonical SoP : " << canonical_SoP << endl << "canonical PoS : " << canonical_PoS << endl;
}

void generatePrimeImplicants(vector<char>& Variables, vector<vector<bool>>& minterms, set<implicationRow>& primes)
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

    std::cout << "Prime Implicants:" << endl;

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
                            std::cout << "PI: " << primeImplicant << " Covers Minterms: ";
                            for (const auto& m : {term1, term2})
                            {
                                for (int k = 0; k < m.size(); ++k)
                                {
                                    std::cout << m[k];
                                }
                                std::cout << '\t';
                            }
                            std::cout << endl;
                            seen.insert(primeImplicant);
                        }
                    }
                }
            }
        }
    }

    // Use Implication Table and pick minterms with logical distances of 1 (with relatively naive algorithm) to combine
    // 2 maps to continuously push the simplified iteration, and then we scan at the end and add the finals to the PI table



    set<implicationRow> implicants1, implicants2, primeImplicants;
    implicationRow temp;
    int mintermVal = 0, p;
    for (auto i = minterms.begin(); i != minterms.end(); i++) {
        p = Variables.size() - 1;
        mintermVal = 0;
        for (auto j = (*i).begin(); j != (*i).end(); j++, p--) {
            mintermVal += pow(2, p) * *j;
            temp.mintermBits.push_back( *j ? '1' : '0');
        }
        temp.coveredMinterms.insert(mintermVal);
        implicants1.insert(temp);
        temp = implicationRow();
    }
    bool flag = false, indivFinal;
    int dif = 0;
    auto it1 = implicants1.begin(), it2 = implicants1.begin();
    while (!flag) {
        it1 = implicants1.begin(); it2 = implicants1.begin(); // Resets iterators for every run
        while (it1 != implicants1.end()) {
            indivFinal = true;
            it2 = implicants1.begin();
            while (it2 != implicants1.end()) {
                if (logicallyAdj((*it1).mintermBits, (*it2).mintermBits, dif)) {
                    indivFinal = false;
                    // Have to store the iterator contents in a variable to call the struct method
                    implicationRow x = *it1;
                    implicants2.insert(x.combine(*it2, dif));
                }
                it2++;
            }
            if (indivFinal) primeImplicants.insert(*it1);
            // If this current element was not combined with anything, it is already a prime implicant
            it1++;
        }
        if (implicants2.empty()) flag = 1; // This means no combinations were made
        else {
            implicants1.clear();
            for (auto it = implicants2.begin(); it != implicants2.end(); it++) implicants1.insert(*it);
            implicants2.clear();
            } // Shifts the implicants one column to the left, and resumes
    }

    implicationRow tester;
    std::cout << "ALL Prime Implicants:\n-----------------\nCovered Minterms\tIn Binary\n-----------------\n";

    for (auto it = primeImplicants.begin(); it != primeImplicants.end(); it++) {
        tester = *it;
        tester.printRow();
    }
    primes = primeImplicants;
}

void classifyEssentials(set<implicationRow> primes, set<implicationRow> essentials, set<implicationRow> nonEssentials,
    vector<string>& essentialExpressions, vector<string>& nonEssentialExpressions, vector<char> variables) {
    implicationRow temp;
    set<int>::iterator num;
    set<implicationRow>::iterator comp;
    bool flag; // To mark when another implicant covers a minterm

    for (auto it = primes.begin(); it != primes.end(); it++) {

        for (auto num = (*it).coveredMinterms.begin(); num != (*it).coveredMinterms.end(); num++) {
            comp = primes.begin();
            flag = false;
            while (comp != primes.end() && !flag) {
                temp = *comp;
                if (temp.covers(*num) && comp != it) {flag = true; break;} // If we find *another* implicant that covers this minterm
                comp++;
            }
            // If we exit the while loop because we got to the end, then no other implicants
            // cover this minterm, so it is an essential PI and we don't need to keep checking it

            // Otherwise, we keep checking the numbers
            if (!flag) {
                temp = *it;
                if (essentials.find(temp) == essentials.end()) {
                    essentials.insert(temp);
                    essentialExpressions.push_back(temp.booleanExpression(variables));
                }
            }
        }

    }
    for (auto it = primes.begin(); it != primes.end(); it++) {
        // If we never pushed the implicant on the essentials, then it is not essential.
        // While primitive, this way simplifies code
        if (essentials.find(*it) == essentials.end()) {
            temp = *it;
            nonEssentials.insert(temp);
            nonEssentialExpressions.push_back(temp.booleanExpression(variables));

        }
    }

    std::cout << "\nEssential Prime Implicant Expressions: ";
    printVec(essentialExpressions);
    cout << endl;

    std::cout << "\nNon-Essential Prime Implicant Expressions: ";
    printVec(nonEssentialExpressions);
    cout << endl;

    set<int> uncoveredMinterms;

    for (implicationRow i : nonEssentials) {
        for (int x : i.coveredMinterms) {
            auto it = essentials.begin();
            while (it != essentials.end()) {
                temp = *it;
                if (temp.covers(x)) break;
                it++;
            }
            if (it == essentials.end()) uncoveredMinterms.insert(x); // It never met an EPI that covered it
        }
    }

    std::cout << "\nMinterms not Covered by Essential Prime Implicants: ";
    printSet(uncoveredMinterms);
    cout << endl;

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

string solvePITable(vector<string>& EPIs, vector<string>& PIs, vector<vector<bool>>& minterms) {
    vector<string> minimizedExpression;
    string minimized = "";

    // Add all EPIs to the minimized expression
    for (const auto& epi : EPIs) {
        if (epi != "") minimizedExpression.push_back(epi);
    }

    // Cover remaining minterms
    for (const auto& minterm : minterms) {
        if (!isCoveredByEPIs(minterm, EPIs)) {
            string minPI = findMinimumPI(minterm, PIs);
            if (find(minimizedExpression.begin(), minimizedExpression.end(), minPI) == minimizedExpression.end()
                && minPI != "") {
                minimizedExpression.push_back(minPI);
            }
        }
    }

    // Print the minimized expression
    std::cout << "Minimized Boolean Expression: ";
    minimized = fullExpression(minimizedExpression);
    cout << minimized;
    return minimized;
    // auto it = minimizedExpression.begin();
    // if (it != minimizedExpression.end()) {
    //     cout << *it;
    //     minimized += *it;
    //     ++it;
    // }
    // for (; it != minimizedExpression.end(); ++it) {
    //     cout << " + " << *it;
    //     minimized += " + " + *it;
    // }

}

//q7
// Function to generate Gray code sequence.
//vector<int> grayCode(int n) {
//    vector<int> res;
//    for (int i = 0; i < (1 << n); i++) {
//        int val = i ^ (i >> 1);
//        res.push_back(val);
//    }
//    return res;
//}
//
//
//void generateKMap(vector<vector<bool>>& minterms, int numVariables) {
//    if (numVariables > 4 || numVariables < 2) {
//        cout << "K-Map generation only supports 2 to 4 variables." << endl;
//        return;
//    }
//
//    int rows = pow(2, ceil(numVariables / 2.0));
//    int cols = pow(2, floor(numVariables / 2.0));
//
//    vector<vector<int>> kmap(rows, vector<int>(cols, -1)); // Initialize K-Map with -1 (undefined)
//
//    // Generate Gray code for rows and columns
//    vector<int> rowGray = grayCode(ceil(numVariables / 2.0));
//    vector<int> colGray = grayCode(floor(numVariables / 2.0));
//
//    // Fill in the K-Map based on minterms
//    for (const auto& minterm : minterms) {
//        int row = 0, col = 0;
//        for (int i = 0; i < ceil(numVariables / 2.0); ++i) {
//            row = (row << 1) | minterm[i];
//        }
//        for (int i = ceil(numVariables / 2.0); i < numVariables; ++i) {
//            col = (col << 1) | minterm[i];
//        }
//        int rowIndex = find(rowGray.begin(), rowGray.end(), row) - rowGray.begin();
//        int colIndex = find(colGray.begin(), colGray.end(), col) - colGray.begin();
//        kmap[rowIndex][colIndex] = 1;
//    }
//
//    int rowBits = ceil(numVariables / 2.0);
//    int colBits = floor(numVariables / 2.0);
//
//    // Print column labels
//    cout << "  ";
//    for (int j : colGray) {
//        cout << bitset<sizeof(int) * 8>(j).to_string().substr(sizeof(int) * 8 - colBits) << " ";
//    }
//    cout << endl;
//
//    // Print horizontal line
//    cout << " +";
//    for (int j = 0; j < cols; ++j) {
//        cout << string(colBits, '-') << "-+";
//    }
//    cout << endl;
//
//    for (int i : rowGray) {
//        // Print row labels
//        cout << bitset<sizeof(int) * 8>(i).to_string().substr(sizeof(int) * 8 - rowBits) << "|";
//
//
//        int rowIndex = find(rowGray.begin(), rowGray.end(), i) - rowGray.begin();
//        for (int j : colGray) {
//            int colIndex = find(colGray.begin(), colGray.end(), j) - colGray.begin();
//            if (kmap[rowIndex][colIndex] == -1) {
//                cout << "0 ";
//            } else {
//                cout << kmap[rowIndex][colIndex] << " ";
//            }
//        }
//        cout << "|" << endl;
//
//        // Print horizontal line
//        cout << " +";
//        for (int j = 0; j < cols; ++j) {
//            cout << "--+";
//        }
//        cout << endl;
//    }
//}

// Utility function to join the variables
string join(const vector<char>& vec, const string& delim) {
    ostringstream oss;
    for (const auto& ch : vec) {
        oss << ch << delim;
    }
    string result = oss.str();
    return result.substr(0, result.length() - delim.length());
}

void generateWebFiles(const vector<vector<bool>>& minterms, int numVariables, const vector<char>& variables) {
    ofstream htmlFile("Kmap.html");
    htmlFile << R"(
<!DOCTYPE html>
<html>
<head>
    <title>K-Map Generator</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }
        #kmap {
            border-collapse: collapse;
        }
        .cell {
            border: 1px solid black;
            width: 50px;
            height: 50px;
            text-align: center;
        }
        .cell-group1 { background-color: red; }
        .cell-group2 { background-color: blue; }
        .cell-group3 { background-color: yellow; }
        .cell-group4 { background-color: green; }
        .cell-group5 { background-color: orange; }
        .cell-group6 { background-color: purple; }
        .cell-group7 { background-color: pink; }
        .cell-group8 { background-color: brown; }
    </style>
</head>
<body>
    <div id="kmap"></div>
    <script>
    function grayCode(n) {
        if (n === 1) return ['0', '1'];
        const prevGray = grayCode(n - 1);
        const currGray = [];
        for (const code of prevGray) {
            currGray.push('0' + code);
        }
        for (let i = prevGray.length - 1; i >= 0; i--) {
            currGray.push('1' + prevGray[i]);
        }
        return currGray;
    }

    function generateKMap(minterms, numVariables, variables) {
        const kmapDiv = document.getElementById('kmap');
        const rows = Math.pow(2, Math.ceil(numVariables / 2));
        const cols = Math.pow(2, Math.floor(numVariables / 2));
        let kmap = Array.from({ length: rows }, () => Array(cols).fill(-1));

        const rowGray = grayCode(Math.ceil(numVariables / 2));
        const colGray = grayCode(Math.floor(numVariables / 2));

        const rowGrayMap = {};
        const colGrayMap = {};
        rowGray.forEach((code, index) => rowGrayMap[parseInt(code, 2)] = index);
        colGray.forEach((code, index) => colGrayMap[parseInt(code, 2)] = index);

        minterms.forEach(minterm => {
            let row = 0, col = 0;
            for (let i = 0; i < Math.ceil(numVariables / 2); ++i) {
                row = (row << 1) | minterm[i];
            }
            for (let i = Math.ceil(numVariables / 2); i < numVariables; ++i) {
                col = (col << 1) | minterm[i];
            }
            const rowIndex = rowGrayMap[row];
            const colIndex = colGrayMap[col];
            kmap[rowIndex][colIndex] = 1;
        });

        let rowVars = variables.slice(0, Math.ceil(numVariables / 2)).join('');
        let colVars = variables.slice(Math.ceil(numVariables / 2)).join('');

        let tableHtml = '<table>';
        tableHtml += '<tr><th></th>';
        colGray.forEach(code => {
            tableHtml += `<th>${code}(${colVars})</th>`;
        });
        tableHtml += '</tr>';
        for (let i = 0; i < rows; i++) {
            tableHtml += `<tr><td>${rowGray[i]}(${rowVars})</td>`;
            for (let j = 0; j < cols; j++) {
                tableHtml += `<td class=\"cell ${kmap[i][j] === 1 ? 'cell-group1' : ''}\">${kmap[i][j] === -1 ? '0' : kmap[i][j]}</td>`;
            }
            tableHtml += '</tr>';
        }
        tableHtml += '</table>';

        kmapDiv.innerHTML = tableHtml;
    }

    const minterms = )";

    // Write minterms to HTML file
    htmlFile << "[";
    for (const auto& minterm : minterms) {
        htmlFile << "[";
        for (bool bit : minterm) {
            htmlFile << (bit ? "1," : "0,");
        }
        htmlFile.seekp(-1, ios_base::cur);
        htmlFile << "],";
    }
    htmlFile.seekp(-1, ios_base::cur);
    htmlFile << "];" << endl;

    htmlFile << R"(
    const numVariables = )" << numVariables << R"(;
    const variables = [')" << join(variables, "','") << R"('];
    generateKMap(minterms, numVariables, variables);
    </script>
</body>
</html>
    )";

    htmlFile.close();
    cout << "HTML file has been generated." << endl;
}

string convertToWaveDrom(const string& minimizedFunction) {
    stringstream waveDromScript;
    waveDromScript << "[\"|\", ";

    string term;
    string andGroup;
    for (int i = 0; i < minimizedFunction.size(); ++i) {
        char c = minimizedFunction[i];
        char next_c = (i < minimizedFunction.size() - 1) ? minimizedFunction[i + 1] : '\0'; // Avoid out-of-bounds access

        if (c == '+') {
            if (!andGroup.empty()) {
                term += "[\"&\", " + andGroup + "]";
                andGroup.clear();
            }
            if (!term.empty()) {
                waveDromScript << term + ", ";
                term.clear();
            }
        } else if (c == '\'') {
            // Skip this character, as it is already handled by the next_c check below
        } else if (isalpha(c)) {
            if (next_c == '\'') {
                andGroup += "[\"~\", \"" + string(1, c) + "\"], ";
                ++i; // Skip the next character as it is already processed
            } else {
                andGroup += string("\"") + c + "\", ";
            }
        }
    }
    if (!andGroup.empty()) {
        term += "[\"&\", " + andGroup + "]";
    }
    if (!term.empty()) {
        waveDromScript << term;
    }

    waveDromScript << "]";
    return waveDromScript.str();
}


void generateHTMLFile(const std::string& waveDromScript) {
    ofstream htmlFile("circuit.html");

    htmlFile << "<!DOCTYPE html>\n";
    htmlFile << "<head>\n";
    htmlFile << "<style>\n";
    htmlFile << "svg {\n";
    htmlFile << "  transform: scale(2);  /* Scaling the SVG by 2 times its original size */\n";
    htmlFile << "  display: block;\n";   // Ensures the SVG block is centered
    htmlFile << "  margin: auto;\n";     // Centers the SVG horizontally
    htmlFile << "}\n";
    htmlFile << "body {\n";
    htmlFile << "  display: flex;\n";       // Use flexbox for vertical centering
    htmlFile << "  height: 100vh;\n";       // Use full viewport height
    htmlFile << "  align-items: center;\n"; // Centers the content vertically
    htmlFile << "  justify-content: center;\n"; // Centers the content horizontally
    htmlFile << "}\n";
    htmlFile << "</style>\n";
    htmlFile << "</head>\n";
    htmlFile << "<body onload=\"WaveDrom.ProcessAll()\">\n";
    htmlFile << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/wavedrom/3.1.0/skins/default.js\" type=\"text/javascript\"></script>\n";
    htmlFile << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/wavedrom/3.1.0/wavedrom.min.js\" type=\"text/javascript\"></script>\n";
    htmlFile << "<script type=\"WaveDrom\">\n";
    htmlFile << "{ assign:[\n";
    htmlFile << "  [\"out\",\n";
    htmlFile << convertToWaveDrom(waveDromScript) << "\n";
    htmlFile << "  ]\n";
    htmlFile << "]}\n";
    htmlFile << "</script>\n";
    htmlFile << "</body>\n";

    htmlFile.close();
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
        std::cout << "Enter a Boolean function in SoP or PoS form: ";
        getline(cin, function);

        if (isValidSoP(function))
        {
            std::cout << "Valid SoP expression." << endl;
        }
        else if (isValidPoS(function))
        {
            std::cout << "Valid PoS expression." << endl;
        }
        else
        {
            std::cout << "Invalid expression. Try another one!" << endl;
            flag = true;
        }
    } while (flag);

    generateTruthTable(function, Variables, minterms);

    set<implicationRow> primeImplicants, essentialPrimeImplicants, nonEssentialPrimeImplicants;
    vector<string> essentialExpressions, nonEssentialExpressions;

    generatePrimeImplicants(Variables, minterms, primeImplicants);

    classifyEssentials(primeImplicants, essentialPrimeImplicants,
        nonEssentialPrimeImplicants, essentialExpressions, nonEssentialExpressions, Variables);

    generateWebFiles(minterms, Variables.size(), Variables);
    generateHTMLFile(solvePITable(essentialExpressions, nonEssentialExpressions, minterms));

    // implicationRow x;
    // char nums[] = {'1','0','-'};
    // char vars[] = {'a', 'b', 'c'};
    // vector<char> varss;
    // for (char i : vars) varss.push_back(i);
    // for (char i : nums) x.mintermBits.push_back(i);
    // cout << x.booleanExpression(varss);


    //q4




    //q5






//    generateKMap(minterms, numVariables);


    return 0;
}
