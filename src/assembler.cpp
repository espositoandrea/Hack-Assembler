#include "assembler.h"
using namespace std;

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

Symbol Assembler::createSymbol(string name, int line)
{
    Symbol s;
    s.name = name;
    s.line = line;
    return s;
}

void Assembler::initializeSymbolTable()
{
    symbolTable.push_back(createSymbol("SP", 0));
    symbolTable.push_back(createSymbol("LCL", 1));
    symbolTable.push_back(createSymbol("ARG", 2));
    symbolTable.push_back(createSymbol("THIS", 3));
    symbolTable.push_back(createSymbol("THAT", 4));
    symbolTable.push_back(createSymbol("R0", 0));
    symbolTable.push_back(createSymbol("R1", 1));
    symbolTable.push_back(createSymbol("R2", 2));
    symbolTable.push_back(createSymbol("R3", 3));
    symbolTable.push_back(createSymbol("R4", 4));
    symbolTable.push_back(createSymbol("R5", 5));
    symbolTable.push_back(createSymbol("R6", 6));
    symbolTable.push_back(createSymbol("R7", 7));
    symbolTable.push_back(createSymbol("R8", 8));
    symbolTable.push_back(createSymbol("R9", 9));
    symbolTable.push_back(createSymbol("R10", 10));
    symbolTable.push_back(createSymbol("R11", 11));
    symbolTable.push_back(createSymbol("R12", 12));
    symbolTable.push_back(createSymbol("R13", 13));
    symbolTable.push_back(createSymbol("R14", 14));
    symbolTable.push_back(createSymbol("R15", 15));
    symbolTable.push_back(createSymbol("SCREEN", 16384));
    symbolTable.push_back(createSymbol("KBD", 24576));
}

void Assembler::firstPass(ifstream& source)
{
    symbolTable.clear();
    initializeSymbolTable();

    currentRamLine = 16;
    currentRomLine = 0;

    string line;
    while(getline(source, line))
    {
        if(line.substr(0,1) == "(" && line.substr(line.size() - 1, 1) == ")")
        {
           symbolTable.push_back(createSymbol(line.substr(1, line.size() - 2), currentRomLine));
        }
        else if(line.substr(0, 2) != "//" && !line.empty())
        {
            currentRomLine++;
        }
    }
}

void Assembler::secondPass(ifstream& source, ofstream& dest)
{
    string line;
    bool firstTime = true;
    while(getline(source, line))
    {

        if(line.substr(0, 2) != "//" && !line.empty())
        {
            if(!firstTime)
                dest << "\n";
            else
                firstTime = false;

            if(line.substr(0,1) != "(" || line.substr(line.size() - 1, 1) != ")")
            {
                while(!line.empty() && isspace(*line.begin()))
                    line.erase(line.begin());

                while(!line.empty() && isspace(*line.rbegin()))
                    line.erase(line.length()-1);

                line.erase(remove(line.begin(), line.end(), ' '), line.end());

                if(line.find("//") != std::string::npos)
                    line = line.substr(0, line.find("//"));

                char * pch;
                char *s = new char[line.length() + 1];
                strcpy(s, line.c_str());
                pch = strtok (s," ");
                line = pch;

                dest << parser(line);
            }
            else
                firstTime = true;
        }
    }
}

string Assembler::parser(string instruction)
{
    string result;

    if(instruction.find("@") != std::string::npos)      //A Instruction
    {
        result = code(instruction.substr(instruction.find("@") + 1, instruction.size() - 1), true, false);
    }
    else                                                //C Instruction
    {
        result = "111";

        size_t comp_start = 0, comp_end = instruction.size();
        if(instruction.find("=") != std::string::npos)
            comp_start = instruction.find("=") + 1;
        if(instruction.find(";") != std::string::npos)
            comp_end = instruction.find(";");

        result += code(instruction.substr(comp_start, comp_end), false, true);


        if(instruction.find("=") != std::string::npos)
            result += code(instruction.substr(0, instruction.find("=") + 1), false, false);
        else
            result += "000";


        if(instruction.find(";") != std::string::npos)
            result += code(instruction.substr(instruction.find(";"),4), false, false);
        else
            result += "000";
    }

    return result;
}

string Assembler::code(string instruction, bool isAInstruction, bool isComp)
{
    string result;
    if(isAInstruction)
    {
        result = "0";
        if(is_number(instruction))
        {
            result += bitset<15>(stoi(instruction, nullptr)).to_string();
        }
        else
        {
            bool found = false;
            for(int i = 0; i < symbolTable.size() && !found; i++)
            {
                if(instruction == symbolTable[i].name)
                {
                    result += bitset<15>(symbolTable[i].line).to_string();
                    found = true;
                }
            }
            if(!found)
            {
                symbolTable.push_back(createSymbol(instruction, currentRamLine));
                result += bitset<15>(currentRamLine).to_string();
                currentRamLine++;
            }
        }
    }
    else
    {
        if(instruction.substr(0, 1) == ";")
        {
            if(instruction.substr(1, 3) == "JGT")
                result = "001";
            else if(instruction.substr(1, 3) == "JEQ")
                result = "010";
            else if(instruction.substr(1, 3) == "JGE")
                result = "011";
            else if(instruction.substr(1, 3) == "JLT")
                result = "100";
            else if(instruction.substr(1, 3) == "JNE")
                result = "101";
            else if(instruction.substr(1, 3) == "JLE")
                result = "110";
            else if(instruction.substr(1, 3) == "JMP")
                result = "111";
        }

        else if(instruction.substr(instruction.size() - 1, 1) == "=")
        {
            if(instruction.substr(0, instruction.size() - 1) == "M")
                result = "001";
            else if(instruction.substr(0, instruction.size() - 1) == "D")
                result = "010";
            else if(instruction.substr(0, instruction.size() - 1) == "MD")
                result = "011";
            else if(instruction.substr(0, instruction.size() - 1) == "A")
                result = "100";
            else if(instruction.substr(0, instruction.size() - 1) == "AM")
                result = "101";
            else if(instruction.substr(0, instruction.size() - 1) == "AD")
                result = "110";
            else if(instruction.substr(0, instruction.size() - 1) == "AMD")
                result = "111";
        }

        if(isComp)
        {
            if(instruction == "0")
                result = "0101010";
            else if(instruction == "1")
                result = "0111111";
            else if(instruction == "-1")
                result = "0111010";
            else if(instruction == "D")
                result = "0001100";
            else if(instruction == "A")
                result = "0110000";
            else if(instruction == "!D")
                result = "0001101";
            else if(instruction == "!A")
                result = "0110001";
            else if(instruction == "-D")
                result = "0001111";
            else if(instruction == "-A")
                result = "0110011";
            else if(instruction == "D+1")
                result = "0011111";
            else if(instruction == "A+1")
                result = "0110111";
            else if(instruction == "D-1")
                result = "0001110";
            else if(instruction == "A-1")
                result = "0110010";
            else if(instruction == "D+A")
                result = "0000010";
            else if(instruction == "D-A")
                result = "0010011";
            else if(instruction == "A-D")
                result = "0000111";
            else if(instruction == "D&A")
                result = "0000000";
            else if(instruction == "D|A")
                result = "0010101";
            else if(instruction == "M")
                result = "1110000";
            else if(instruction == "!M")
                result = "1110001";
            else if(instruction == "-M")
                result = "1110011";
            else if(instruction == "M+1")
                result = "1110111";
            else if(instruction == "M-1")
                result = "1110010";
            else if(instruction == "D+M")
                result = "1000010";
            else if(instruction == "D-M")
                result = "1010011";
            else if(instruction == "M-D")
                result = "1000111";
            else if(instruction == "D&M")
                result = "1000000";
            else if(instruction == "D|M")
                result = "1010101";
        }
    }
    return result;
}
