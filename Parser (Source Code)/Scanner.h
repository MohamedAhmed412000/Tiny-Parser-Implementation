#ifndef SCANNER
#define SCANNER

#include "headers.h"

struct Token
{
    string Type;
    string Value;
};

void createFile(string);
void writeFile(string, string);
string readFile(string);
string getTokens(string);

#endif // SCANNER

