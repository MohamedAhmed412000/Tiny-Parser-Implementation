#include "Scanner.h"

vector<Token> tokenList;

string readFile(string input){
    int index;
    for (int i = input.length()-1; i > 0; i--) {
        if (!(input[i] == ' ' || input[i] == '\t' || input[i] == '\n')) {
            index = i;
            break;
        }
    }
    return input.substr(0, index+1);
}

void writeFile(string input,string filename) {
    ofstream out;
    out.open(filename, ios_base::app | ios_base::out);
    out << input << "\n";
    out.close();
}

void createFile(string path){
    ofstream output(path);
}

bool isChar(char in){
    return ((in > 96 && in < 123) || (in > 64 && in < 91));
}

bool isNum(char in){
    return (in > 47 && in < 58);
}

bool isOper(char in) {
    if (in == '+' || in == '-' || in == '*' || in == '/' || in == '=' || in == '(' || in == ')' || in == ';' || in == '<')
        return true;
    return false;
}

bool isReservedWord(string in){
    string Word[8] = {"read", "write", "if", "then", "else", "repeat", "until", "end"};
    for (int i = 0; i < 8; i++){
        if (in == Word[i])
            return true;
    }
    return false;
}

bool isBreakChar(char in){
    if (in == ' ' || in == '\t' || in == '\n' || in == '{' || in == '}' || in == ':')
        return true;
    return false;
}

char capitalize(char in){
	if (in > 96 && in < 123)
	 return in - 32;
	return in;
}

string tokenType(string in){
	string out = "";
    for (unsigned int i = 0; i < in.length(); i++)
		out += capitalize(in[i]);
	return out;
}

string Operator(char x){
	switch (x)
	{
	case '+':
		return "PLUS";
	case '-':
		return "MINUS";
	case '*':
		return "MULT";
	case '/':
		return "DIV";
	case '(':
		return "OPENBRACKET";
	case ')':
		return "CLOSEDBRACKET";
	case ';':
		return "SEMICOLON";
	case '=':
		return "EQUAL";
	case '<':
		return "LESSTHAN";
	default:
		return "";
	}
}

string Special(string in) {
    if (in == ":=") return "ASSIGN";
    return "";
}

int index = 0;				// Global Index to use it with every call
bool floatPoint = false;	// used to check if added floating point before or not
bool power = false;			// used to check if added power before or not

Token getToken(string Text){
    int state = 0, s;
    Token x;
    while (state != 13) {
        switch (state) {
        case 0:	// START STATE
            s = index;
            if (isChar(Text[index]))
                state = 1;	// CHAR STATE
			else if ((Text[index] == '-') && (isOper(Text[index-1]) || isBreakChar(Text[index-1])))
                state = 3;	// SIGN STATE
            else if (isNum(Text[index]))
                state = 4;	// NUM STATE
            else if (Text[index] == '{')
                state = 7;	// COMMENT STATE
            else if (isOper(Text[index]))
                state = 8;	// REGULAR OPERATOR STATE
            else if (Text[index] == '<')
                state = 9;	// GREATER OR LESS STATE
            else if (Text[index] == ':')
                state = 10;	// ASSIGN STATE
            break;
        /*=========================================================================*/
        case 1:	// CHAR STATE
            if (isNum(Text[index]))
                state = 2;	// IDENTIFIER NUM STATE
            if (isBreakChar(Text[index]) || (index == (Text.length()-1)) || isOper(Text[index])) {
                int end = isBreakChar(Text[index]) ? index-s : index-s+1;
                end = isOper(Text[index]) ? index-s : end;
                x.Value = Text.substr(s, end);
                if (isReservedWord(x.Value))
					x.Type = tokenType(x.Value);
                else
                    x.Type = "IDENTIFIER";
                state = 13;	// ACCEPTING STATE
            }
            break;
        /*=========================================================================*/
        case 2:	// IDENTIFIER NUM STATE
            if (isChar(Text[index]))
                state = 1;	// CHAR STATE
            if (isBreakChar(Text[index]) || (index == (Text.length()-1)) || isOper(Text[index])) {
                x.Type = "IDENTIFIER";
                int end = isBreakChar(Text[index]) ? index-s : index-s+1;
                end = isOper(Text[index]) ? index-s : end;
                x.Value = Text.substr(s, end);
                state = 13;	// ACCEPTING STATE
            }
            break;
		/*=========================================================================*/
        case 3:	// SIGN STATE
			if (isChar(Text[index]) || isBreakChar(Text[index])) {
				if (!power)	{
				    state = 13;	// ACCEPTING STATE
					x.Value = Text[index-1];
					x.Type = Operator(Text[index-1]);
				}
				else {
					state = 12;	// ERROR STATE
				    x.Value = "Wrong sign added";
				}
            }
			else if (isNum(Text[index]))
				state = 4;	// NUMBER STATE
			else {
				state = 12;	// ERROR STATE
			    x.Value = "Wrong sign added";
        	}
            break;
        /*=========================================================================*/
        case 4:	// CONSTANT NUM STATE

			// still number
            if (isNum(Text[index]))
				state = 4;	// NUMBER STATE

			// error charater after numbers
			if (isChar(Text[index])) {
                state = 12;	// ERROR STATE
                x.Value = "Wrong Constant (Added characters)";
            }
			
			// Floating point part
			if (Text[index] == '.') {
				if (!power) {
					if (!floatPoint)
						state = 5;	// FLOATING POINT STATE
					else {
						state = 12; // ERROR
						x.Value = "Floating point added before";
					}
				}
				else {
					state = 12; // ERROR
					x.Value = "Can't add floating point to power";
				}
			}
			
			// power part
			if (Text[index] == 'e') {
				if (!power)
					state = 6;	// POWER STATE
				else {
					state = 12; // ERROR
					x.Value = "Power part added before";
				}
			}
			
			// end of number
            if (isBreakChar(Text[index]) || (index == (Text.length()-1)) || isOper(Text[index])) {
				floatPoint = false;
				power = false;
				x.Type = "NUMBER";
                int end = isBreakChar(Text[index]) ? index-s : index-s+1;
                end = isOper(Text[index]) ? index-s : end;
                x.Value = Text.substr(s, end);
                state = 13;	// ACCEPTING STATE
            }
            break;
		/*=========================================================================*/
        case 5:	// FLOATING POINT STATE
			floatPoint = true;
            if (isNum(Text[index]))
                state = 4;	// FLOATING NUMBER STATE
			else {
				state = 12;	// ERROR STATE
                x.Value = "Incomplete float number";
			}
            break;
        /*=========================================================================*/
        case 6:	// POWER PART STATE
			power = true;
            if (isNum(Text[index]))
                state = 4;	// POWER PART NUMBERS STATE
			else if (Text[index] == '+' || Text[index] == '-')
				state = 3;	// POWER PART SIGN STATE
			else {
				state = 12;	// ERROR STATE
                x.Value = "Incomplete power part number";
			}
            break;
		/*=========================================================================*/
        case 7:	// COMMENT STATE
            if (Text[index] == '}')
                state = 0;	// START STATE
            if ((index == (Text.length()-1)) && Text[index] != '}') {
                state = 12;	// ERROR STATE
                x.Value = "Incomplete Comment";
            }
            break;
        /*=========================================================================*/
        case 8:	// OPERATOR STATE
            x.Value = Text[index-1];
            x.Type = Operator(x.Value[0]);
            state = 13;	// ACCEPTING STATE
            break;
        /*=========================================================================*/
        case 9:	// GREATER OR LESS STATE
            if (Text[index] == '=')
                state = 11;	// COMPLETE EQUAL STATE
            else{
                x.Value = Text[index-1];
                x.Type = Operator(x.Value[0]);
                state = 13;	// ACCEPTING STATE
            }
            break;
        /*=========================================================================*/
        case 10:	// ASSIGN STATE
            if (Text[index] == '=')
                state = 11;	// COMPLETE EQUAL STATE
            else {
                state = 12;	// ERROR
                x.Value = "Do you mean := (Assign Statement)";
            }
            break;
        /*=========================================================================*/
        case 11:	// COMPLETE EQUAL STATE
            state = 13;	// ACCEPTING STATE
            x.Value = Text.substr(s, 2);
            x.Type = Special(x.Value);
            break;
        /*=========================================================================*/
        case 12:	// ERROR STATE
			floatPoint = false;
			power = false;
            state = 13;	// ACCEPTING STATE
            x.Type = "ERROR";
            while (!(isBreakChar(Text[index]) || (index == (Text.length()-1))))
                index++;
            break;
        /*=========================================================================*/
        default:
            break;
        }
        if (state != 13 && (index != (Text.length()-1)))
            index++;
    }
    return x;
}

string getTokens(string input){
    string tokens = "";
    while ((index != (input.length()-1)) || isOper(tokenList.back().Value[0]) && (index == (input.length()-1))) {
        tokenList.push_back(getToken(input));
        tokens += (tokenList.back().Value + ", " + tokenList.back().Type + "\n");
    }
    index = 0;
    return tokens;
}
