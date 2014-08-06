////////////////////////////////////////////////////////////
//
// yoda - Command line utility for Mac OS & Linux
//
// Copyright (C) 2014 Abner Coimbre (http://abnercoimbre.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the author(s) be held liable for any damages arising from the use of this software.
//
// With that said, you may use, modify, and distribute the code freely.
////////////////////////////////////////////////////////////
//
// yoda parses the given source code file, recognizing
// conditionals of the form:
//
// if (operand1 == operand2)
//
// or
//
// if (operand1 != operand2)
//
// and swaps the order of the operands upon user request. Number of spaces doesn't matter.
// Useful if you need to conform your code to a certain coding
// standard. See #1 on the list of http://blog.codinghorror.com/new-programming-jargon/
//
// [Note: The newly-swapped code will be saved with a .YODA extension that
// should be removed later (original file stays intact).]
////////////////////////////////////////////////////////////

#include <iostream>
using std::cout; using std::cin; using std::cerr; using std::endl;

#include <fstream>
using std::ifstream; using std::ofstream; using std::ios;

#include <sstream>
using std::stringstream;

#include <string>
using std::string;

#include <algorithm>
using std::remove;

////////////////////////////////////////////////////////////
/// is_valid_conditional
///
/// Checks if the given string is a valid conditional of the form:
///
/// if (arg1 == arg2)
///
/// or
///
/// if (arg1 != arg2)
///
/// [Note: Number of spaces doesn't matter.]
///
/// @param str - the string to check if it's a valid if-statement
/// @return - true if str is a valid conditional of the specified form
///
////////////////////////////////////////////////////////////
bool is_valid_conditional(const string &str)
{
   string str_copy = str;
   str_copy.erase(remove(str_copy.begin(), str_copy.end(), ' '), str_copy.end());

   if (str_copy.find("if(") != string::npos)
       if (str_copy.find("==") != string::npos || str_copy.find("!=") != string::npos)
           return true;

   return false;
}

////////////////////////////////////////////////////////////
/// user_wants_swap
///
/// Asks the user whether or not they want to perform a yoda swap.
///
/// @return - true if user wants to do a swap
///
////////////////////////////////////////////////////////////
bool user_wants_swap()
{
    string answer;

    while (answer != "y" && answer != "n")
    {
        cout << "Perform swap? (y/n) ";
        cin >> answer;
    }

    return answer == "y" ? true : false;
}

////////////////////////////////////////////////////////////
/// yoda_swap
///
/// Takes a pointer to a string that is expected to be a valid
/// conditional of the form described in the function is_valid_conditional
/// and swaps the order of the operands.
///
/// @param *str - string to modify and perform the yoda swap on
///
////////////////////////////////////////////////////////////
void yoda_swap(string *str)
{
    string comparison_operator = str->find("==") != string::npos ? "==" : "!=";

    string str_copy = str->substr(str->find("(") + 1);
    string::size_type end_pos = str_copy.find(comparison_operator);
    string first_operand = str_copy.substr(0, end_pos);
    first_operand.erase(remove(first_operand.begin(), first_operand.end(), ' '), first_operand.end());

    str_copy = str_copy.substr(end_pos + 2);
    end_pos = str_copy.rfind(")");
    string second_operand = str_copy.substr(0, end_pos);
    second_operand.erase(remove(second_operand.begin(), second_operand.end(), ' '), second_operand.end());

    string swapped_str = str->substr(0, str->find("(") + 1);
    swapped_str += second_operand + ' ' + comparison_operator + ' ' + first_operand + ")";

    cout << "Swap successful: " << swapped_str.substr(swapped_str.find("if")) << endl;

    str->assign(swapped_str);
}

////////////////////////////////////////////////////////////
/// exit_if: self-explanatory
////////////////////////////////////////////////////////////
void exit_if(const bool &something_wrong, const string &msg)
{
    if (something_wrong)
    {
        cerr << msg << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    /* Stop if no argument was provided */
    exit_if(argc != 2, "Need the name of a single text file.");
    string file_location = argv[1];
    /* * * * * * * * * * * * * * * * * */

    /* Stop if file doesn't exist or is invalid */
    ifstream file(file_location.c_str(), ios::in);
    exit_if(!file.is_open(), file_location + ": No such file or directory");
    /* * * * * * * * * * * * * * * * * * * * * */

    /* Intro */
    cout << "Yoda Swap v0.1" << endl;
    /* * * * */

    /* Begin */
    ofstream output((file_location + ".YODA").c_str(), ofstream::out);

    string line;
    while (getline(file, line))
    {
        if (is_valid_conditional(line))
        {
            cout << endl << line.substr(line.find("if")) << endl;
            if (user_wants_swap())
                yoda_swap(&line);
        }
        output << line << endl;
    }
    /* * * * * * * * */

    cout << "\nDone. New text file is called " << file_location + ".YODA" << endl;

    return EXIT_SUCCESS;
}
