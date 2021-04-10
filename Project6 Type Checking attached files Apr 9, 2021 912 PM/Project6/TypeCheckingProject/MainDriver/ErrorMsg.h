#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <list>

using std::string;
using std::list;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

//Class used to report errors when a unrecognized token occurs
class ErrorMsg
{
private:
	string		fileName;	//the file being processed
	int			lineNum;	//trace the line no of current token
	list<int>	linePos;	//contains end position of each line, in reverse order
public:
	void reset(string f)
	{
		fileName = f;
		lineNum = 1;
		linePos.clear();
		linePos.push_back(0);
	}

	//should be invoked whenever a '\n' occurs
	//in order to trace the line #.
	void newline(int pos) {
		lineNum++;
		linePos.push_front(pos);
	}

	//print error message for the current token
	void error(int pos, string msg) {
		int num = lineNum;
		list<int>::iterator it;

		for(it=linePos.begin(); it!=linePos.end() && *it>= pos; it++)
			num--;
		cerr << fileName << ":" << num << "." << pos-*it <<": " << msg << endl;

		displayCode(num, pos - *it );
	}

	void error(int lineno, int column, string msg) {
		cerr << fileName << ":" << lineno << "." << column <<": " << msg << endl;

		displayCode(lineno, column);
	}

	void displayCode(int lineno, int column)
	{
		ifstream	ifs(fileName);
		string		line;

		for(int i=0; i<lineno; i++)
			getline(ifs, line);
		cerr << line << endl;


		for(unsigned int i=0; i<line.size(); i++)
		{
			if ( line[i] != '\t' && line[i] != '\b' && line[i] != '\f' )
				line[i] = ' ';
		}
		line += " ";
		line[column-1] = '^';
		cerr << line << endl << endl;
	}
};

