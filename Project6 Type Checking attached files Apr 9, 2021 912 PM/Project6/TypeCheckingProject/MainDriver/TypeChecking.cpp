#include "TypeChecking.h"

using namespace symbol;


namespace semantics
{
	const string		TypeChecking::breakSign = "breaksign";

		//insert a variable into the var/function symbol table
	void TypeChecking::insertVar(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a function into the var/function symbol table
	void TypeChecking::insertFunc(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getVarEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getVarEnv()->lookup(name);
			ss << "function " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getVarEnv()->insert( name, entry );
	}

	//insert a type into the type symbol table
	void TypeChecking::insertType(string name, symbol::SymTabEntry entry)
	{
		string			msg;
		stringstream	ss(msg);

		if ( env.getTypeEnv()->localContains(name) )
		{
			symbol::SymTabEntry		old = env.getTypeEnv()->lookup(name);
			ss << "variable " << name << " is already defined at line " << old.node->getLineno();
			error(entry.node, ss.str());

		}
		else
			env.getTypeEnv()->insert( name, entry );
	}

	const types::Type* TypeChecking::visit(const Absyn *v)
	{
		if ( dynamic_cast<const Exp *>(v) != NULL )
			return visit( dynamic_cast<const Exp *>(v) );
		else if ( dynamic_cast<const Var *>(v) != NULL )
			return visit( dynamic_cast<const Var *>(v) );
		else if ( dynamic_cast<const Dec *>(v) != NULL )
			return visit( dynamic_cast<const Dec *>(v) );
		else
			throw runtime_error("invalid node");
	}

	const types::Type* TypeChecking::visit(const Exp *e)
	{
		if (dynamic_cast<const OpExp*>(e) != NULL)			return visit((const OpExp*)e);
		else if (dynamic_cast<const VarExp*>(e) != NULL)	return visit((const VarExp*)e);
		else if (dynamic_cast<const NilExp*>(e) != NULL)	return visit((const NilExp*)e);
		else if (dynamic_cast<const IntExp*>(e) != NULL)	return visit((const IntExp*)e);
		else if (dynamic_cast<const StringExp*>(e) != NULL) return visit((const StringExp*)e);
		else if (dynamic_cast<const CallExp*>(e) != NULL)	return visit((const CallExp*)e);
//		else if (dynamic_cast<const RecordExp*>(e) != NULL) return visit((const RecordExp*)e);
		else if (dynamic_cast<const SeqExp*>(e) != NULL)	return visit((const SeqExp*)e);
		else if (dynamic_cast<const AssignExp*>(e) != NULL) return visit((const AssignExp*)e);
		else if (dynamic_cast<const IfExp*>(e) != NULL)		return visit((const IfExp*)e);
		else if (dynamic_cast<const WhileExp*>(e) != NULL)	return visit((const WhileExp*)e);
		else if (dynamic_cast<const ForExp*>(e) != NULL)	return visit((const ForExp*)e);
		else if (dynamic_cast<const BreakExp*>(e) != NULL)	return visit((const BreakExp*)e);
		else if (dynamic_cast<const LetExp*>(e) != NULL)	return visit((const LetExp*)e);
		else if (dynamic_cast<const ArrayExp*>(e) != NULL)	return visit((const ArrayExp*)e);
		else throw new runtime_error("ExpType.visit(Exp*)");
	}

	const types::Type* TypeChecking::visit(const Var *v)
	{
		if (dynamic_cast<const SimpleVar *>(v) != NULL)			return visit((const SimpleVar *) v);
//		else if (dynamic_cast<const FieldVar *>(v) != NULL)		return visit((const FieldVar *) v);
		else if (dynamic_cast<const SubscriptVar *>(v) != NULL) return visit((const SubscriptVar *)v);
		else throw new runtime_error("ExpType.visit(Var*)");
	}

	const types::Type* TypeChecking::visit(const Ty *t)
	{
		if (dynamic_cast<const NameTy *>(t) != NULL)			return visit((const NameTy *) t);
		else if (dynamic_cast<const ArrayTy *>(t) != NULL)		return visit((const ArrayTy *) t);
//		else if (dynamic_cast<const RecordTy *>(t) != NULL)		return visit((const RecordTy *)t);
		else throw new runtime_error("ExpType.visit(Ty*)");
	}

	const types::Type* TypeChecking::visit(const Dec *d)
	{
		if (dynamic_cast<const TypeDec *>(d) != NULL)			return visit((const TypeDec *) d);
		else if (dynamic_cast<const VarDec *>(d) != NULL)		return visit((const VarDec *) d);
//		else if (dynamic_cast<const FunctionDec *>(d) != NULL)	return visit((const FunctionDec *)d);
		else throw new runtime_error("ExpType.visit(Dec*)");
	}

	const types::Type* TypeChecking::visit(const SimpleVar *v)
	{
		if ( !(env.getVarEnv()->contains(v->getName())) )
		{
			error( v, "undefined variable");
			//undeclared variables is treated as INT variable
			insertVar( v->getName(), SymTabEntry(env.getVarEnv()->getLevel(),
																new types::INT(),
																v) );
			return new types::INT();
		}
		else
		{
			const types::Type*	t = env.getVarEnv()->lookup(v->getName()).info->actual();

			if (dynamic_cast<const types::FUNCTION *>(t) != NULL )
			{
				error( v, "function with the same name exists");
				//undeclared variables is treated as INT variable
				return new types::INT();
			}
			return t;
		}
	}

/*	const types::Type* TypeChecking::visit(const FieldVar *v)
	{
		//add your implementation here
		//syntax: lvalue.fieldname
		
		Algorithm:
			1.	Perform type checking on lvalue, and get its data type (say t)
			2.	if t is not a record type
					report an error
					return INT
				else
			3.		cast t to RECORD *;
			4.		For each filed in the RECORD definition
						if the fieldname is the one we are looking for
							return the type of current field
			5.		report an error for non-existing field
			6.		return INT.
	}
*/
	const types::Type* TypeChecking::visit(const SubscriptVar *v)
	{
		//add your implementation here
		//syntax: lvalue[index_exp]
		/*
		Algorithm:
			1.	Perform type checking on lvalue, and get its data type (say t)
			2.	if t is not ARRAY type
					report an error
			3.	Perform type checking on index_exp, and get its data type (say te)
			4.	if te is not INT
					report an error
			5.	if t is not ARRAY
					return INT
				else
					return the type of array element which can be
					found at ((ARRAY *)t)
		*/
	}


	const types::Type* TypeChecking::visit(const OpExp *e)
	{
		//add your implementation here
		//syntax: left_exp Operator right_exp
		/*
		Algorithm:
			1.	Perform type checking on left_exp, and get its data type (say lt)
			2.	Perform type checking on right_exp, and get its data type (say rt)
			3.	if Operator is one of +, -, *, /
					if lt is not an INT, report an error
					if rt is not an INT, report an error
					return INT
			4.	else if Operator is one of >, >=, <, <=
					if lt is not an INT/STRING, report an error
					if rt is not an INT/STRING, report an error
					if lt and rt are not compatible
						report an error
					return INT;
			5.	else //i.e. =, <> 
					if lt is not an INT/STRING/ARRAY/RECORD/NIL, report an error
					if rt is not an INT/STRING/ARRAY/RECORD/NIL, report an error
					if lt and rt are not compatible
						report an error
					if both lt and rt are NIL
						report an error
					return INT
		*/
	}

	const types::Type* TypeChecking::visit(const VarExp *e)
	{
		const types::Type*		t = visit( e->getVar() );
		return t->actual();
	}

	const types::Type* TypeChecking::visit(const NilExp *e)
	{
		return new types::NIL();
	}

	const types::Type* TypeChecking::visit(const IntExp *e)
	{
		return new types::INT();
	}

	const types::Type* TypeChecking::visit(const StringExp *e)
	{
		return new types::STRING();
	}

	const types::Type* TypeChecking::visit(const CallExp *e)
	{
		//add your implementation here
		//syntax: fname(exp1, exp2, ..., expn)
		/*
		Algorithm:
			things that can go wrong:
				1. fname is undefined
				2. fname is defined as variable
				3. the type of exp_i doesn't match the type of param_i
				4. the # of expressions doesn't match the # of parameters

			1.	check if fname is defined by looking up the symbol table
			2.	if fname is not defined, report an error, and return INT
			3.	if fname is defined, get its data type, say t
			4.	if t is not FUNCTION, report an error and return INT;
				convert t to FUNCTION *.
			5.	Let c_arg refers to the first argument (argument list can be found in CallExp)
				Let c_par refers to the first parameter (parameter list can be found in FUNCTION)
			6.	repeat as long as both c_arg and c_par are not NULL
					perform type checking on c_arg and get its type, see ta
					if ( ta is not compatible with type of c_par )
						report an error
					update c_arg to refer to next argument
					update c_par to refer to next parameter
				end repeat
			7.	if (c_arg is not null && c_par is null )
					report an error: too many arguments provided
			8.	if (c_arg is null && c_par is not null )
					report an error: too few arguments provided
			9.	return the result type of the function (can be found in t)
		*/
	}

/*	const types::Type* TypeChecking::visit(const RecordExp *e)
	{
		//add your implementation here
		//syntax: record_type {f1=exp1, ..., fn=expn}
	}
*/

	const types::Type* TypeChecking::visit(const SeqExp *e)
	{
		//add your implementation here
		//syntax: exp1; exp2; exp3; ....; expn
		/*
		Algorithm:
			for each expression exp_i in the list
				perform type checking on exp_i and save its data type to t
			return t;
		*/
	}

	const types::Type* TypeChecking::visit(const AssignExp *e)
	{
		//add your implementation here
		//syntax: lvalue := exp
		/*
		Algorithm:
			1.	perform type checking on lvalue and save its data type to t
			2.	perform type checking on exp and save its data type to te
			3.	if ( te is NOT compatible with t )
					report an error
			4.	return VOID
		*/
	}

	const types::Type* TypeChecking::visit(const IfExp *e)
	{
		//add your implementation here
		//syntax: if test then
		//				exp1
		//			else
		//				exp2

		/*
		Algorithm:
			1.	perform type checking on test and save its data type to t
			2.	if t is not INT, report an error
			3.	perform type checking on exp1 and save its data type to t1
			4.	if it is a if-then satement (no else-clause)
					if t1 is not VOID, report an error
					return VOID;
			5.	else (if-then-else expression)
					perform type checking on exp2 and save its data type to t2
					if t1 is compatible with t2
						return t2
					else if t2 is compatible with t1
						return t1
					else
						report an error;
						return t1
		*/

	}

	const types::Type* TypeChecking::visit(const WhileExp *e)
	{
		//add your implementation here
		//syntax: while test do exp1
		/*
		Algorithm:
			1.	perform type checking on test and save its data type to t
			2.	if t is not INT, report an error
			3.	perform type checking on exp1 and save its data type to t1
			4.	if t1 is not VOID, report an error
			5.	return VOID;
		*/
	}

	const types::Type* TypeChecking::visit(const ForExp *e)
	{
		//add your implementation here
		//syntax: for vname := exp1 to exp2 do exp3
		/*
		Algorithm:
			1.	Create a new scope for var/function symbol table
			2.	Perform type checking on (vname := exp1), which is treated 
				as a variable declaration
			3.	lookup var/function symbol table to find and save the data type of vname
				to t1;
			4.	if t1 is not INT, report an error
			5.	Perform type checking on exp2, and save its data type to t2
			6.	if t2 is not INT, report an error
			7.	Perform type checking on exp3, and save its data type to t3
			8.	if t3 is not VOID, report an error
			9.	end the scope of var/function symbol table
			10.	return VOID;
		*/
	}

	const types::Type* TypeChecking::visit(const BreakExp *e)
	{
		//add your implementation here
		/*Algorithm:
			return VOID if  you don't want bonus points.
		*/
	}

	const types::Type* TypeChecking::visit(const LetExp *e)
	{
		//add your implementation here
		//syntax: let decls in exps end
		/*
		Algorithm:
			1.	Create a new scope for var/function symbol table
			2.	Create a new scope for type symbol table
			3.	for each decl in the declaration list
					perform type checking on the decl
			4.	Perform type checking on exps and save its data type to t
			5.	if t is an VOID, report an error (???)
			6.	end scope of both symbol tables
			7.	return t;

		*/
	}

	const types::Type* TypeChecking::visit(const ArrayExp *e)
	{
		//add your implementation here
		//syntax: array_type [exp1] of exp2
		/*
		Algorithm:
			1.	if array_type exists. 
					If it doesn't exist, report an error;
					Let t be ARRAY of INT
			2.	else
					lookup type symbol table to find and save its type
					to t;
					if t is not ARRAY, 
						report an error;
						Let t be ARRAY of INT
			3.	perform type checking on exp1 and save its type to t1
			4.	if t1 is not INT, report an error
			5.	perform type checking on exp2 and save its type to t2
			6.	if t2 is not compatible to ((ARRAY *)t)->getElement();
					report an error
			7.	return t;
		*/
	}

/*	const types::Type* TypeChecking::visit(const FunctionDec *d)
	{
		//add your implementation 
		//syntax: function fname(p1:type1, ..., pn:typen) : rtype = exp1
	}
*/

	const types::Type* TypeChecking::visit(const VarDec *d)
	{
		//add your implementation here
		// syntax: var vname : Type = exp1
		/*
		Algorithm:
			1.	if vname is defined locally  (use localContains function)
					report an error
			2.	if Type is provided
					Let tt be INT
					if Type doesn't exist in type symbol table
						report an error
					else
						lookup type symbol table to find and save 
						its type information to tt;
					Perform type checking on exp1 and save its type to t1
					if t1 is not compatible with tt
						report an error
					insert vname into the var/function symbol table
			3.	else (Type is not provided)
					Perform type checking on exp1 and save its type to t1
					insert vname into the var/function symbol table
			4.	return NULL;
		*/
	}

	const types::Type* TypeChecking::visit(const TypeDec *d)
	{
		const types::Type*		type;
		types::NAME*			name = new types::NAME( d->getName() );
	
		//find type redefine in the consecutive type declarations 
		const absyn::TypeDec*	td = d->getNext();
		while ( td != NULL ) {
			if ( td->getName() == d->getName() )
				error( td, "type redefined" );
			td = td->getNext();
		}

		name->bind( new types::INT() );	//just for avoiding the self loop, later it
							//will be replaced by actual value
		
		insertType(d->getName(), SymTabEntry(env.getVarEnv()->getLevel(), name, d));	

		if ( d->getNext() != NULL )
			visit( d->getNext() );
		type = visit( d->getTy() );

		name->bind( (types::Type *)type );
		env.getTypeEnv()->lookup(d->getName()) = SymTabEntry(env.getVarEnv()->getLevel(),
																name,
																d);	

		if ( name->isLoop() ) {
			error( d, "illegal cycle found in type definition" );
			name->bind( new types::INT() );
		}
		return NULL;

	}


	const types::Type* TypeChecking::visit(const NameTy *t)
	{
		if ( !(env.getTypeEnv()->contains(t->getName())) )
		{
			error(t, "undefined type name");
			return new types::INT();
		}

		return env.getTypeEnv()->lookup(t->getName()).info;
	}

/*	const types::Type* TypeChecking::visit(const RecordTy *t)
	{
		const absyn::FieldList*		fl = t->getFields();

		if ( fl == NULL ) {
			//empty record
			return new types::RECORD( "", NULL, NULL );
		}
		else {
			types::RECORD		*r = NULL, *tail = NULL, *head = NULL;
	
			while ( fl != NULL ) {
				if ( !env.getTypeEnv()->contains(fl->getType()) )
					r = new types::RECORD(	fl->getName(),
											new types::INT(),
											NULL );
				else
					r = new types::RECORD(	fl->getName(),
											env.getTypeEnv()->lookup(fl->getType()).info,
											NULL );
				if ( head == NULL )
					head = tail = r;
				else {
					tail->setRest(r);
					tail = r;	
				}
				fl = fl->getRest();
			}
			return head;
		}	
	}
*/
	const types::Type* TypeChecking::visit(const ArrayTy *t)
	{
		if ( !(env.getTypeEnv()->contains(t->getName())) )
		{
			error(t, "undefined type");
			return new types::INT();
		}

		return new types::ARRAY( env.getTypeEnv()->lookup(t->getName()).info );
	}


	
} // end of namespace semantics



/*
Bonus points:
1. Break expression
	algorithm:
	1. create a symbol table say lv (lv is actually a member data of class TypeChecking;
	2. everytime entering a loop:
			create a new scope for lv,
			insert breakSign into lv, its data type is INT
	3. everytime exiting a loop;
			destroy the scope for lv
	4. everytime entering a function declaration
			create a new scope for lv,
			insert breakSign into lv, its data type is VOID
	5. everytime exiting a function;
			destroy the scope for lv
	6. in visit(BreakExp)
			lookup lv symbol table to find the definition of breakSign
			and get its data type t

			if t is VOID, report an error

2. No modification to loop variable
	algorithm:
	1. Everytime entering a for loop
			create a new scope for lv
			insert the loop variable into lv
	2. Every time leaving a for loop
			destroy the scope for lv
	3. In visit(AssignExp)
			if Var in the assignment expression is a simple var
				retrieve information (say v1) of var from the var/function symbol table
				retrieve information (say v2) of var from the lv symbol table
				if v1 and v2 points to the same node in the AbstractSyntaxTree,
					report an error

*/
