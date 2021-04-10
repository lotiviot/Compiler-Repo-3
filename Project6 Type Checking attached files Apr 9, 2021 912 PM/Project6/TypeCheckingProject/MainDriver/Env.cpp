#include "Env.h"

namespace symbol
{
	//constructor
	//should add all built-in types and predefined functions into symbol tables
	Env::Env(void)
	{
		// Initialize venv with predefined library functions.
		vector<const types::Type*>	params;

		params.push_back( new types::INT() );
		venv.insert( "iprint", 
					SymTabEntry(0,
						new types::FUNCTION(	"iprint",
												params,
												new types::VOID()),
						NULL) );

		params.clear();
		params.push_back( new types::STRING() );
		venv.insert( "print", 
					SymTabEntry(0,
						new types::FUNCTION(	"print",
												params,
												new types::VOID()),
						NULL) );

		params.clear();
		venv.insert( "flush", 
					SymTabEntry(0,
						new types::FUNCTION(	"flush", 
												params,
												new types::VOID()),
						NULL) );
		venv.insert( "getchar", 
					SymTabEntry(0,
						new types::FUNCTION(	"getchar", 
												params,
												new types::STRING()),
						NULL) );
		params.clear();
		params.push_back( new types::STRING() );
		venv.insert( "ord", 
					SymTabEntry(0,
						new types::FUNCTION(	"ord",
												params,
												new types::INT()),
						NULL)	);
		
		
		params.clear();
		params.push_back( new types::INT() );
		venv.insert( "chr", 
					SymTabEntry(0,
						new types::FUNCTION(	"chr",
												params,
												new types::STRING()),
						NULL) );

		params.clear();
		params.push_back( new types::STRING() );
		venv.insert( "size", 
					SymTabEntry(0,
						new types::FUNCTION(	"size",
												params,
												new types::INT()),
						NULL) );
		
		params.clear();
		params.push_back( new types::STRING() );
		params.push_back( new types::INT() );
		params.push_back( new types::INT() );
		venv.insert( "substring", 
					SymTabEntry(0,
						new types::FUNCTION(	"substring",
											params,
											new types::INT()),
						NULL) );

		params.clear();
		params.push_back( new types::STRING() );
		params.push_back( new types::STRING() );
		venv.insert(	"concat", 
					SymTabEntry(0,
						new types::FUNCTION(	"concat",
												params,
												new types::INT()),
						NULL) );
	
		params.clear();
		params.push_back( new types::INT() );
		venv.insert( "not", 
					SymTabEntry(0,
						new types::FUNCTION(	"not",
												params,
												new types::INT()),
						NULL) );

		params.clear();
		params.push_back( new types::INT() );
		venv.insert( "exit", 
					SymTabEntry(0,
						new types::FUNCTION(	"exit",
												params,
												new types::VOID()),
						NULL) );
	

		// Initialize tenv with predefined types.
		tenv.insert("int", SymTabEntry(0, new types::INT(), NULL) );
		tenv.insert("string", SymTabEntry(0, new types::STRING(), NULL));
	}
	
	//return a pointer to tenv or venv
	SymbolTable<SymTabEntry>* Env::getTypeEnv()
	{
		return &tenv;
	}

	SymbolTable<SymTabEntry>* Env::getVarEnv()
	{
		return &venv;
	}
	

} //end of namespace Symbol