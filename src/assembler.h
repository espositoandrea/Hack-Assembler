#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <ctype.h>
#include <algorithm>

#include <QDebug>
using namespace std;

struct Symbol
{
    string name;
    unsigned int line;
};

class Assembler
{
public:
    vector<Symbol> symbolTable;

    unsigned int currentRomLine;
    unsigned int currentRamLine;

    void firstPass(ifstream&);
    void initializeSymbolTable();
    Symbol createSymbol(string, int);

    void secondPass(ifstream&, ofstream&);

    string parser(string);
    string code(string, bool, bool);
};

bool is_number(const std::string& s);

#endif // ASSEMBLER_H
