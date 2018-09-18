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

/**
 * @brief A symbol in the symbol table.
 * A symbol is basically a variable: a certain name given
 * to a line of the RAM/ROM.
 */
struct Symbol
{
    string name;                            ///The name of the symbol.
    unsigned int line;                      ///The line indicated by the symbol.
};

/**
 * @brief The main Assembler.
 * This class contains all the functions of the assembler.
 */
class Assembler
{
private:
    vector<Symbol> symbolTable;             ///The table of symbols.

    unsigned int currentRomLine;            ///The current line of the ROM (used to store symbols).
    unsigned int currentRamLine;            ///The current line of the RAM (used to store symbols).

    void initializeSymbolTable();           ///Initializes the table of symbols by adding default symbols.
    Symbol createSymbol(string, int);       ///Creates a symbol.
    string parser(string);                  ///Breaks the instructions into pieces and starts decoding them.
    string A_InsructionTranslate(string);        ///Translates an A instruction.
    string C_InsructionTranslate(string, bool);        ///Translates a C instruction.

public:
    void firstPass(ifstream&);              ///Gets all variables and labels and stores them in the table of symbols.
    void secondPass(ifstream&, ofstream&);  ///Gets all lines of code and starts decoding them.
};

///Checks if a string is a number.
bool is_number(const std::string& s);

#endif // ASSEMBLER_H
