#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

using namespace std;

// GLOBAL VARIABLES
const string digits = "-.0123456789";  
const string separator = "[,;|\"(){}?<>]";


const string keywordsARR[28] = {"include", "int", "bool", "double", "string", "char", "const",
                            "return", "exit", "if", "else", "for", "while", "cout", 
                            "switch", "case", "break", "vector", "do", "using", "namespace",
                            "cout", "cin", "continue", "void", "long", "float", "endl"};
const string identifier_keywords = "_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const string operators = "%+-!=/*";

int main() {

    //==============================================================================
    //
    //         Display information about team 
    //         members and project objective.  
    //
    //==============================================================================

    cout << endl;
    cout << "Member Name: Emmanuel Montoya" << endl;
    cout << "Member Name: Tony Dinh" << endl;
    cout << "Member Name: Rouni Assaf" << endl;
    cout << "Date: March 27, 2024" << endl;
    cout << "Project 1: Remove Whitespaces, Comments & Tokenize" << endl;
    cout << "Class: CSPC 323 SPRING 2024" << endl;
    cout << "Professor: Keshav Daga" << endl;
    cout << endl << endl;

    // Initialize local variables
    string temp;
    string myString;
    ifstream file;

    //======================= OPEN TXT FILE ==================
    file.open("test.txt");         // <---INPUT FILE NAME
    if(!file.is_open())
    {
        cout<<"Error opening the file\n";
        exit(0);
    }

    // Convert txt files into a string
    while(!file.eof()) 
    {
      getline(file, temp);
        myString += temp + "\n";
    } 
    //==============================================================================
    //
    //         Deletes White Space and Comments
    //
    //==============================================================================
    string output;
    int count = 0;

        while (count < myString.length()) {
        if (myString[count] == '/') {
            if (myString[count + 1] == '/') {
                while (myString[count] != '\n')
                    count++;
            } else if (myString[count + 1] == '*') {
                while (myString[count] != '*' || myString[count + 1] != '/') {
                    count++;
                }
                count += 2;
            } else {
                output += myString[count];
            }
        } else if (myString[count] == ' ' && myString[count + 1] == ' ') {
            while (myString[count] != ' ' || myString[count + 1] != ' ') {
                count++;
            }
            count += 2;
        } else {
            // Check if the line is empty before adding it to the output
            if (!(myString[count] == '\n' && output.back() == '\n'))
                output += myString[count];
            count++;
        }
    }

    cout << "=======================NO COMMENT & WHITESPACE =============================\n\n";
    cout << output;

    //==============================================================================
    //
    //         Initializes the token types  
    //
    //==============================================================================

    // Create states of finite machine & initialize the first state
    enum stateCurrent {literalState, operatorState, separatorState, keywordState};
    stateCurrent stateCurrent = literalState;

    // Initialize & declare variables
    vector<string> myKeywords;
    vector<string> myIdentifiers;
    vector<string> myOperators;
    vector<string> mySeparators;
    vector<string> myLiterals;
    char myChar;                            // To hold each character of string
    int index = 0;                          // To count string
    string insert ="";                      // To combine char into one string
    int found;                              // To determine if item is found in list above

    //==============================================================================
    //
    //         Detects LITERALS 
    //
    //==============================================================================

  while (index < output.length()) {
    myChar = output[index];
    switch (stateCurrent)
    {
        // LITERAL
        case literalState:
            if (myChar == '"') {
                insert = '"'; // Clear insert for new string
                stateCurrent = literalState; // Stay in literal state
                index++; // Move past the opening quote
                while (index < output.length() && output[index] != '"') {
                    insert += output[index];
                    index++;
                }
              insert += '"';
                if (index < output.length()) {
                    myLiterals.push_back(insert); // Insert string into list
                    stateCurrent = literalState; // Stay in literal state
                    index++; // Move past the closing quote
                }
            } else {
                found = digits.find(myChar);
                if (found != string::npos) {
                    insert = myChar; // Start building a new literal
                    stateCurrent = literalState;
                    index++;
                    while (index < output.length() && digits.find(output[index]) != string::npos) {
                        insert += output[index];
                        index++;
                    }
                    myLiterals.push_back(insert); // Insert string into list
                    stateCurrent = literalState; // Stay in literal state
                    insert = "";
                } else {
                    stateCurrent = operatorState;
                }
            }
            break;

    //==============================================================================
    //
    //         Detects OPERATORS
    //
    //==============================================================================

            // OPERATOR
            case operatorState:
                found = operators.find(myChar);
                if(found < operators.length())
                {

                    insert = insert + myChar;
                    stateCurrent = operatorState;
                    index++;
                    found = operators.find(output[index]);
                    if(found > operators.length())
                    {
                        myOperators.push_back(insert);
                        stateCurrent = literalState;
                        insert = "";
                    }
                }
                else{
                    stateCurrent = separatorState;
                }
            break;

    //==============================================================================
    //
    //         Detects SEPERATORS
    //
    //==============================================================================

        // SEPARATOR
        case separatorState:
            found = separator.find(myChar);
            if (found < separator.length()) {
                insert = insert + myChar;
                stateCurrent = separatorState;
                index++;
                found = separator.find(output[index]);
                // Check if the next character is also a separator
                if (found < separator.length()) {
                    // If the next separator character is different, add a space
                    if (found != separator.find(myChar)) {
                        insert += ' ';
                    }
                }
                if (found > separator.length()) {
                    mySeparators.push_back(insert);
                    stateCurrent = literalState;
                    insert = "";
                }
            } else {
                stateCurrent = keywordState;
            }
        break;


    //==============================================================================
    //
    //         Detects KEYWORDS
    //
    //==============================================================================

            // KEYWORD
            case keywordState:

                //cout << "key   "<<insert <<endl;
                found = identifier_keywords.find(toupper(myChar));
                if(found<identifier_keywords.length())
                {
                    insert = insert + myChar;
                    stateCurrent = keywordState;
                    index++;
                    found = identifier_keywords.find(toupper(output[index]));
                    if(found > identifier_keywords.length())
                    {
                        // Determine insert is keywords
                        for(int i=0; i<27; i++)
                        {
                            if(insert == keywordsARR[i])
                            {
                                myKeywords.push_back(insert);
                                stateCurrent = literalState;
                                insert = "";
                            }
                        }
                        myIdentifiers.push_back(insert);
                        stateCurrent = literalState;
                        insert = "";   
                    }
                }
                else{
                    stateCurrent = literalState;
                    index++;
                    insert = "";

                }

            break;

        }
    }

    //==============================================================================
    //
    //         Organizes and outputs all
    //         of the Tokens into a table
    //
    //==============================================================================

    vector<string>::iterator ptr;
    // Display the output

    cout << "\n================================ LITERAL ================================\n\n";

    for (ptr = myLiterals.begin(); ptr < myLiterals.end(); ptr++) 
    cout << *ptr << "  ";        

    cout<<"\n\n================================ IDENTIFIER ================================\n"<<endl;

    for (ptr = myIdentifiers.begin(); ptr < myIdentifiers.end(); ptr++) 
    cout << *ptr << "  ";     

    cout<<"\n\n================================ KEYWORDS ================================\n"<<endl;

    for (ptr = myKeywords.begin(); ptr < myKeywords.end(); ptr++) 
    cout << *ptr << "  ";     

    cout<<"\n\n================================ OPERATOR ================================\n"<<endl;

    for (ptr = myOperators.begin(); ptr < myOperators.end(); ptr++) 
    cout << *ptr << "  ";     

    cout<<"\n\n================================ SEPARATOR ================================\n"<<endl;

    for (ptr = mySeparators.begin(); ptr < mySeparators.end(); ptr++) 
    cout << *ptr << "  ";     

    cout << endl;

    return 0;
}