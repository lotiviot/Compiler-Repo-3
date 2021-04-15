/*Tristan Lotivio*/
/*Project 5: TypeChecking.cpp*/
/* 4/7/2021 */
/* Dr. Zhijiang Dong */
/*implementation for symboltable.cpp which is all the basic functionality for the symboltable hashtable*/

#include <iostream>
#include <sstream>

using namespace std;

namespace symbol
{

	//get the level of current scope
	template<class Entry>
	int SymbolTable<Entry>::getLevel() const
	{
		return level;
	}

	//Save the information of current scope to the string dumpinfo
	//It should be called at the beginning of the endScope function.
	template<class Entry>
	void SymbolTable<Entry>::dump()
	{
		int			level = tables.size();
		string		indent;
		HashTable	current = *(tables.begin());
		HashTable::iterator	it = current.begin();

		for(int i=0; i<level; i++)
			indent += "    ";
		string			info;
		stringstream	ss(info);

		for(; it != current.end(); ++it)
			ss << indent << it->first << endl;
		ss << indent << "-------------------------------" << endl;
		dumpinfo += ss.str();
	}

	//to remove all scopes from the hashtable list so that
	//all remaining scopes will be dumped.
	template<class Entry>
	SymbolTable<Entry>::~SymbolTable()
	{
		//delete all remaining scopes 
		for(unsigned int i=0; i<tables.size(); i++)
			endScope();

		cout << "*******************Symbol Table************************" << endl;
		cout << dumpinfo << endl;
		cout << "*******************Symbol Table************************" << endl;
	}


	//Retrieve the value associated with the given lexeme in the hashtable list
	//search from the head to tail
	//an exception is thrown if the lexeme doesn't exist
	template<class Entry>
	Entry& SymbolTable<Entry>::lookup(string lexeme)
	{
		for(Iterator it=tables.begin(); it!=tables.end(); it++)
		{
			HashTable		&current = *it;
			HashTable::iterator	vit = current.find(lexeme);

			if ( vit != current.end() )
			{
				return current[lexeme];	//found the lexeme
			}
		}
		throw runtime_error("The given Lexeme " + lexeme + " doesn't exist!") ;
	}

	//Retrieve the value associated with the given lexeme in the global level only
	//an exception is thrown if the lexeme doesn't exist
	template<class Entry>
	Entry& SymbolTable<Entry>::globalLookup(string lexeme)
	{
		Iterator	prev;
		Iterator	cur = tables.begin();

		while( cur != tables.end() )
		{
			prev = cur;
			cur ++;
		}

		HashTable		&current = *prev;

		if ( current.find(lexeme) != current.end() )
		{
			return current[lexeme];	//found the lexeme
		}
		throw runtime_error("The given Lexeme " + lexeme + " doesn't exist!") ;
	}

	//create a new scope as the current scope
	template<class Entry>
	void SymbolTable<Entry>::beginScope()
	{
		level ++;
		tables.push_front( HashTable() );
	}

	//destroy the current scope, and its parent becomes the current scope
	template<class Entry>
	void SymbolTable<Entry>::endScope()
	{
		dump();

		//remove current scope level from the symbol table
		if ( level >= 0 )
		{
			tables.pop_front();
			level --;
		}
	};
	//asdf

	/****************************
	Provide implementation of all other member functions here
	****************************/
	
	//check if a lexeme is contained in the symbol table list
	//search from the head to tail
	template<class Entry>
	bool SymbolTable<Entry>::contains(string lexeme)
	{
		/* put your implementation here */
		for (Iterator it = tables.begin(); it != tables.end(); it++)
		{	
			//initiate the iterator and find the value
			//return true if the value exists and false otherwise
			HashTable& current = *it;
			HashTable::iterator	vit = current.find(lexeme);

			if (vit != current.end())
			{
				return true;
			}
		}
		return false;
		
	}


	template<class Entry>
	bool SymbolTable<Entry>::localContains(string lexeme)
	{
		/* put your implementation here */
		//in the specific table
		//initiate the iterator and find the value
		//return true if the value exists and false otherwise
		Iterator it = tables.begin();
		HashTable& current = *it;
		HashTable::iterator	vit = current.find(lexeme);
		if (vit != current.end()) {
			return true;
		}
		else {
			return false;
		}
		
	}

	//check if a lexeme is contained in the global level
	template<class Entry>
	bool SymbolTable<Entry>::globalContains(string lexeme)
	{
		//at the last hashtable
		//initiate the iterator and find the value
		//return true if the value exists and false otherwise
		HashTable current = *(tables.end());
		HashTable::iterator it = current.begin();
		while (it != current.end()) {
			if (it->first == lexeme)
				return true; 
		}
		return false;
	}

	//insert a lexeme and binder to the current scope, i.e. the first hashtable in the list
	//if it exists, an exception will be thrown
	template<class Entry>
	void SymbolTable<Entry>::insert(string lexeme, const Entry value)
	{
		//if already inserted throw an error
		if (localContains(lexeme)) {
			throw runtime_error("Error: " + lexeme + "already exists in this current scope.");
		}

		//if else insert the lexeme
		else {
			Iterator it = tables.begin();
			it->insert({ lexeme, value });
		}
	}

} //end of namespace Environment



