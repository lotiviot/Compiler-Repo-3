#pragma once

#include <string>
#include <unordered_map>

#include "Absyn.h"
#include "Types.h"

using namespace std;

namespace symbol
{
	struct SymTabEntry
	{
		int	level;	//at which level, the type/variable/function is defined
		types::Type*	info;		//containing type information 
		const absyn::Absyn*	node;	//pointing back to the AST node of this entry

		SymTabEntry(int l=0, types::Type* i=NULL, const absyn::Absyn * a=NULL)
			: level(l), info(i), node(a)
		{}
	};

	template <class Entry> 
	class SymbolTable
	{
	private:
		//Define some macros for convenience
		typedef unordered_map<string, Entry>	HashTable;
		typedef typename list<HashTable>::iterator		Iterator;
		
		//list of the symbol tables for nested scope levels
		//The first hashtable is always for the current scope
		//The last hashtable is always for the global scope
		//in general, ith hashtable represents the parent scope of (i-1)th hashtable
		list<HashTable>		tables;

		//tracing the level of types, variables, and functions
		int					level;	//the level of current scope

		string				dumpinfo;
	public:
		//constructor: create the hashtable for the global scope
		SymbolTable(void) : level(0), dumpinfo("")
		{
			//push the global scope
			tables.push_back( unordered_map<string, Entry>() );
		}

		//check if a lexeme is contained in the symbol table list
		//search from the head to tail
		bool contains(string); 

		//similar to contains, but only search in the current scope
		bool localContains(string);
		
		//similar to contains, but only search in the global scope
		bool globalContains(string);

		//Retrieve the value associated with the given lexeme in the hashtable list
		//search from the head to tail
		//an exception is thrown if the lexeme doesn't exist
		Entry& lookup(string);

		//Retrieve the value associated with the given lexeme in the global level only
		//an exception is thrown if the lexeme doesn't exist
		Entry& globalLookup(string);

		//insert a lexeme and binder to the current scope, i.e. the last hashtable in the list
		//if it exists, an exception will be thrown
		void insert(string lexeme, const Entry value); 

		//create a new scope as the current scope
		void beginScope(); 

		//destroy the current scope, and its parent becomes the current scope
		void endScope(); 

		//get the level of current scope
		int getLevel() const;

		//dump the current scope
		//It should be called at the beginning of the endScope function.
		void dump();

		//to remove all scopes from the hashtable list so that
		//all remaining scopes will be dumped.
		SymbolTable::~SymbolTable();
	};

} //end of namespace Environment

#include "SymbolTable.cpp"


