/*
PROGRAMMER: Darien Kidwell
Project 6
Project Due: 4/18/2021
INSTRUCTOR: Dr. Zhijang Dong
Desc: This is the implementation of typechecking.cpp which checks
for types and reports type-related semantic errors
*/


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

	/* Begin My Implementation */
	const types::Type* TypeChecking::visit(const SubscriptVar *v)
	{
		//add your implementation here
		//syntax: lvalue[index_exp]
		//lvalue must be ARRAY
		//index_exp must be INT

		// Type checking on lvalue
		// Giving its data type to t
		const types::Type* t = visit(v->getVar());

		// if t is not an ARRAY, report error
		if (dynamic_cast<const types::ARRAY*>(t) == NULL)
		{
			error(v, "ARRAY required");
		}
		// Type checking on index_exp
		// Giving its type to te
		const types::Type* te = visit(v->getIndex());

		// if te is not an INT, report error
		if (dynamic_cast<const types::INT*>(te) == NULL)
		{
			error(v->getIndex(), "int required");
		}

		// if t is not ARRAY, return INT - else, return array element type at the given location
		if (dynamic_cast<const types::ARRAY*>(t) == NULL)
			return new types::INT();
		else
			return ((types::ARRAY*)t)->getElement();
		



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

		// Typechecking on left_exp
		// Giving its type to lt
		const types::Type* lt = visit(e->getLeft());
		
		// Typechecking on right_exp
		// Giving its type to rt
		const types::Type* rt = visit(e->getRight());
		
		// Getting the operator (int because the type of operator is stored in an array in Absyn::OpExp and is accessible by index)
		int oper = e->getOper();


		// if lt or rt are functions, acquire the result of the function to use for the respective exp
		if (dynamic_cast<const types::FUNCTION*>(lt) != NULL)
			lt = dynamic_cast<const types::FUNCTION*>(lt)->getResult();
		if (dynamic_cast<const types::FUNCTION*>(rt) != NULL)
			rt = dynamic_cast<const types::FUNCTION*>(rt)->getResult();
		

		// Arithmetic Operator
		if (oper < 4)
		{
			if (dynamic_cast<const types::INT*>(lt) == NULL)
			{
				error(e->getLeft(), "int required");
			}
			if (dynamic_cast<const types::INT*>(rt) == NULL)
			{
				error(e->getRight(), "int required");
			}
			return new types::INT();
		}
		else if (oper > 5)	// GT, LT, GE, LE
		{
			if ((dynamic_cast<const types::INT*>(lt) == NULL) 
				&& (dynamic_cast<const types::STRING*>(lt) == NULL))
			{
				error(e->getLeft(), "int or string required");
			}
			if ((dynamic_cast<const types::INT*>(rt) == NULL) 
				&& (dynamic_cast<const types::STRING*>(rt) == NULL))
			{
				error(e->getRight(), "int or string required");
			}
			if (!((visit(e->getLeft()))->coerceTo(visit(e->getRight()))))
			{
				error(e, "Operands must have the same type");
			}
			return new types::INT();
		}
		else	// Equal or Not Equal
		{
			if (dynamic_cast<const types::INT*>(lt) == NULL
				&& dynamic_cast<const types::STRING*>(lt) == NULL
				&& dynamic_cast<const types::ARRAY*>(lt) == NULL
				&& dynamic_cast<const types::RECORD*>(lt) == NULL
				&& dynamic_cast<const types::NIL*>(lt) == NULL)
			{
				error(e->getLeft(), "exps must be int, string, array, record, or nil");
			}
			if (dynamic_cast<const types::INT*>(rt) == NULL
				&& dynamic_cast<const types::STRING*>(rt) == NULL
				&& dynamic_cast<const types::ARRAY*>(rt) == NULL
				&& dynamic_cast<const types::RECORD*>(rt) == NULL
				&& dynamic_cast<const types::NIL*>(rt) == NULL)
			{
				error(e->getRight(), "exps must be int, string, array, record, or nil");
			}

			if (dynamic_cast<const types::NIL*>(lt) != NULL)
			{
				if (dynamic_cast<const types::ARRAY*>(rt) == NULL
					&& dynamic_cast<const types::RECORD*>(rt) == NULL)
				{
					error(e->getRight(), "exp must be array or record type");
				}
			}

			if (dynamic_cast<const types::NIL*>(rt) != NULL)
			{
				if (dynamic_cast<const types::ARRAY*>(lt) == NULL
					&& dynamic_cast<const types::RECORD*>(lt) == NULL)
				{
					error(e->getLeft(), "exp must be array or record type");
				}
			}

			if (!(visit(e->getLeft()))->coerceTo((visit(e->getRight()))))
			{
				error(e, "Operands must have the same type");
			}
			if (dynamic_cast<const types::NIL*>(lt) != NULL
				&& dynamic_cast<const types::NIL*>(rt) != NULL)
			{
				error(e, "Both exps cannot be nil");
			}
			return new types::INT();
		}
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

		// Assign function name to fname
		string fname = e->getFunc();

		// Check if fname is already defined
		if (!(env.getVarEnv()->contains(fname)))
		{
			error(e, "undefined function name");
			return new types::INT();
		}
		else	// if fname is defined
		{
			// look up fname and assign its type to t
			const types::Type* t = env.getVarEnv()->lookup(fname).info->actual();

			// if t is not a function, report an error
			if (dynamic_cast<const types::FUNCTION*>(t) == NULL)
			{
				error(e, "this is not a function type");
				return new types::INT();
			}
			else
			{
				
				// c_arg and c_par
				// c_arg = the first argument
				// c_par = the first parameter
				const ExpList* c_arg = e->getArgs();

				vector<const types::Type*> c_par;
				c_par = dynamic_cast<const types::FUNCTION*>(env.getVarEnv()->lookup(fname).info)->getFieldType();
				vector<const types::Type*>::iterator cp_iter = c_par.begin();

				// until either c_arg or c_par is NULL
				// perform typechecking on each and give their types to ta and tp
				// check that ta and tp are compatible types
				// if not, report an error
				while (c_arg != NULL && cp_iter != c_par.end())
				{
					const types::Type* ta = visit(c_arg->getHead());
					const types::Type* tp = (*cp_iter);

					if (!(ta->coerceTo(tp)))
					{
						error(e->getArgs()->getHead(), "wrong argument type");
					}
					cp_iter++;
					c_arg = c_arg->getRest();
				}

				// if c_par runs out before c_arg
				if (c_arg != NULL && cp_iter == c_par.end())
				{
					error(e, "too many arguments");
				}

				// if c_arg runs out before c_par
				if (c_arg == NULL && cp_iter != c_par.end())
				{
					error(e, "too few arguments");
				}

				return t;
			}
		}


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

		// get the ExpList and give the values to exps
		const ExpList* exps = e->getList();
		const types::Type* t;

		// if the list is empty
		if (exps == NULL)
		{
			return new types::VOID();
		}
		else
		{
			// perform typechecking on each exp
			while (exps != NULL)
			{
				t = visit(exps->getHead());
				exps = exps->getRest();
			}
		}
		return t;


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

		// typechecking on lvalue and exp, giving their types to t and te
		const types::Type* t = visit(e->getVar());
		const types::Type* te = visit(e->getExp());
		
		// if te and t are not compatible types
		if (!(te->coerceTo(t)))
		{
			error(e, "Type mismatch in assignment");
		}
		return new types::VOID();

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

		// typechecking on test and giving the type to t
		const types::Type* t = visit(e->getTest());
		
		// if t is not an int
		if (dynamic_cast<const types::INT*>(t) == NULL)
		{
			error(e->getTest(), "int required");
		}

		// typechecking on the then exp and giving its type to t1
		const types::Type* t1 = visit(e->getThenClause());

		// if-then statement (no else clause)
		if (e->getElseClause() == NULL)
		{
			if (dynamic_cast<const types::VOID*>(t1) == NULL)
			{
				error(e->getThenClause(), "valueless expression required");
				return new types::VOID();
			}
		}
		else // if-then-else statement
		{
			// typechecking on the else exp, giving its type to t2
			const types::Type* t2 = visit(e->getElseClause());

			// if t1 is compatible with t2
			if (t1->coerceTo(t2))
			{
				return t2;
			}
			else if (t2->coerceTo(t1))	// if t2 is compatible with t1
			{
				return t1;
			}
			else	// t1 and t2 are not compatible
			{
				error(e, "Operands must have the same type");
				return t1;
			}
		}
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

		// typechecking on the test exp and giving its type to t
		const types::Type* t = visit(e->getTest());

		// if t is not an int
		if (dynamic_cast<const types::INT*>(t) == NULL)
		{
			error(e, "valueless expression required");
		}

		// typechecking on the body of the while and giving its type to t1
		const types::Type* t1 = visit(e->getBody());

		// if t1 is not void
		if (dynamic_cast<const types::VOID*>(t1) == NULL)
		{
			error(e, "void required");
		}
		return new types::VOID();

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

		// initialize vname and begin new scope for var/function symbol table
		string vname;
		env.getVarEnv()->beginScope();

		// typechecking on vname and giving the type to t
		const types::Type* t = visit(e->getVar());

		// looking up the type of vname in the symbol table and giving the type to t1
		vname = e->getVar()->getName();
		const types::Type* t1 = env.getVarEnv()->lookup(vname).info->actual();
		

		// if t1 is not an int
		if (dynamic_cast<const types::INT*>(t1) == NULL)
		{
			error(e, "int required");
		}

		// typechecking on exp2 and giving the type to t2
		const types::Type* t2 = visit(e->getHi());

		// if t2 is not an int
		if (dynamic_cast<const types::INT*>(t2) == NULL)
		{
			error(e->getHi(), "int required");
		}

		// typechecking on exp3 and giving the type to t3
		const types::Type* t3 = visit(e->getBody());

		// if t3 is not void
		if (dynamic_cast<const types::VOID*>(t3) == NULL)
		{
			error(e->getBody(), "void required");
			
		}

		// end scope for var/function symbol table
		env.getVarEnv()->endScope();

		return new types::VOID();
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

		return new types::VOID();

		/*Algorithm:
			return VOID if  you don't want bonus points.
		*/
	}

	const types::Type* TypeChecking::visit(const LetExp *e)
	{
		//add your implementation here
		//syntax: let decls in exps end

		// begin new scope for var/function and tyope symbol table
		env.getVarEnv()->beginScope();
		env.getTypeEnv()->beginScope();

		// get the dec list
		const DecList* dec_list = e->getDecs();

		// if the dec list exists
		if (dec_list != NULL)
		{
			// get the first dec
			const Dec* decl = dec_list->getHead();
			// while the dec_list is not NULL
			while (dec_list != NULL)
			{
				// typechecking on the current dec and update position in the list
				visit(decl);
				dec_list = dec_list->getRest();
				if (dec_list != NULL)
					decl = dec_list->getHead();
			}
		}
		// typechecking on exps and giving its type to t
		const types::Type* t = visit(e->getBody());

		// end scope for var/function and type symbol table
		env.getTypeEnv()->endScope();
		env.getVarEnv()->endScope();


		return t;
		/*
		Algorithm:
			1.	Create a new scope for var/function symbol table
			2.	Create a new scope for type symbol table
			3.	for each decl in the declaration list
					perform type checking on the decl
			4.	Perform type checking on exps and save its data type to t
			5.	if t is an VOID, report an error (???) -- stated in class that this is not necessary
			6.	end scope of both symbol tables
			7.	return t;

		*/
	}

	const types::Type* TypeChecking::visit(const ArrayExp *e)
	{
		//add your implementation here
		//syntax: array_type [exp1] of exp2

		// initialize Type* t
		const types::Type* t;

		// if array_type does not exist
		if (!env.getTypeEnv()->contains(e->getType()))
		{
			error(e, "undefined type name");
			// let t be ARRAY of INT
			t = new types::ARRAY(new types::INT());
		}
		else
		{
			// look up type in the symbol table and give the type to t
			t = env.getTypeEnv()->lookup(e->getType()).info->actual();
			
			// if t is not ARRAY
			if (dynamic_cast<const types::ARRAY*>(t) == NULL)
			{
				error(e, "array required");
				// let t be ARRAY of INT
				t = new types::ARRAY(new types::INT());
			}
		}

		// typechecking on array size exp and giving the type to t1
		const types::Type* t1 = visit(e->getSize());

		// if size is not an int
		if (dynamic_cast<const types::INT*>(t1) == NULL)
		{
			error(e->getSize(), "int required");
		}

		// typechecking on exp2 and giving its type to t2
		const types::Type* t2 = visit(e->getInit());
		if (!(t2->coerceTo(((types::ARRAY*)t)->getElement())))
		{
			error(e->getInit(), "array initializer has the wrong type");
		}

		// This did not work correctly when placed at the top of the algorithm or within the suggested if, so I put it down here and it did
		// if the array does not exist
		if (!env.getTypeEnv()->contains(e->getType()))
			error(e, "array required");


		return t;
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

		// initialize variables
		const types::Type* type;
		const types::Type* tt;

		string vname = d->getName();

		// if the variable is already defined, locally
		if (env.getVarEnv()->localContains(vname))
		{
			error(d, "variable already defined");	
		}
		// if the type is given
		if (d->getType() != NULL)
		{
			//  let tt be int
			tt = new types::INT();

			// if type is not defined
			if (!(env.getTypeEnv()->contains(d->getType()->getName())))
			{
				error(d, "undefined type name");
				
			}
			else
			{
				// look up given type and give its value to tt
				tt = env.getTypeEnv()->lookup(d->getType()->getName()).info->actual();

				// typeechecking on exp1 and giving its value to t1
				const types::Type* t1 = visit(d->getInit());

				// if t1 is not compatible with tt
				if (!(t1->coerceTo(tt)))
				{
					error(d->getInit(), "wrong type of initializing expression");

				}

				// insert vname into the var/function symbol table
				insertVar(d->getName(), SymTabEntry(env.getVarEnv()->getLevel(), (types::Type*)tt, d));
			}
		}
		else
		{
			// if t1 is nil
			if (dynamic_cast<const types::NIL*>(visit(d->getInit())) != NULL)
				error(d->getInit(), "type of nil cannot be inferred");

			// insert vname into the var/function symbol table
			insertVar(d->getName(), SymTabEntry(env.getVarEnv()->getLevel(), (types::Type*)visit(d->getInit()), d));
		}

		return NULL;

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
