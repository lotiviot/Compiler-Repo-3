#pragma once
#include "SymbolTable.h"

namespace symbol
{
	class Env
	{
	private:
		SymbolTable<SymTabEntry>		tenv;	//for type identifiers
		SymbolTable<SymTabEntry>		venv;	//for variable/function identifiers
	public:

		//constructor
		//should add all built-in types and predefined functions into symbol tables
		Env(void);

		//return a pointer to tenv or venv
		SymbolTable<SymTabEntry>* getTypeEnv();
		SymbolTable<SymTabEntry>* getVarEnv();
	};

} //end of namespace Symbol