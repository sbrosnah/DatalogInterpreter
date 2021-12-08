#include "Lexer.h"
#include "Parser.h"
#include "DatalogProgram.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char** argv) {

    string filename = argv[1];
    //cout << filename << endl;
    ifstream inFile;

    inFile.open(filename);
    string line;

    Lexer* lexer = new Lexer();
    Parser* parser = new Parser();

    string input;
    while(getline(inFile, line)){
        input += (line + "\n");
    }

    lexer->Run(input);
    //lexer->ToString();

    DatalogProgram* datalogProgram = parser->Run(lexer->tokens);

    Interpreter* interpreter = new Interpreter(datalogProgram);
    interpreter->Run();
    interpreter->ToString();

    delete interpreter;
    delete parser;
    delete lexer;

    return 0;
}