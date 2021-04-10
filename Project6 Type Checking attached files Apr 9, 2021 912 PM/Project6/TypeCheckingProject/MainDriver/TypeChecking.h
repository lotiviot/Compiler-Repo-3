#pragma once
#include "ErrorMsg.h"
#include "Env.h"

using namespace absyn;

namespace semantics
{

	class TypeChecking 
	{
	private:
		symbol::Env										env;
		ErrorMsg										*errormsg;
		symbol::SymbolTable<symbol::SymTabEntry>		lv;
								//used to store current for-loop variables which
								//cannot be updated, and store the information
								//about current break is legal or illegal
								//You don't need to use it in project #5

	//constants
		static const string			breakSign;
		bool						errorFlag;

	public:
		TypeChecking(ErrorMsg *r)
			: errormsg(r), errorFlag(false) 
		{}

		void error(const Absyn *a, string msg)
		{
			errormsg->error(a->getLineno(), a->getColno(), msg);
			errorFlag = true;
		}

		bool hasSemanticError() const 
		{ return errorFlag; }

		void checkInt(const types::Type* et, const Absyn* a) {
			if (! et->coerceTo(&types::INT()) )
				error(a, "int required");
		}

		//insert a variable into the var/function symbol table
		void insertVar(string name, symbol::SymTabEntry entry);

		//insert a function into the var/function symbol table
		void insertFunc(string name, symbol::SymTabEntry entry);

		//insert a type into the type symbol table
		void insertType(string name, symbol::SymTabEntry entry);

		const types::Type* visit(const Absyn *v);
		const types::Type* visit(const Exp *e);
		const types::Type* visit(const SimpleVar *v);
//		const types::Type* visit(const FieldVar *v);
		const types::Type* visit(const SubscriptVar *v);
		const types::Type* visit(const Var *v);
		const types::Type* visit(const OpExp *e);
		const types::Type* visit(const VarExp *e);
		const types::Type* visit(const NilExp *e);
		const types::Type* visit(const IntExp *e);
		const types::Type* visit(const StringExp *e);
		const types::Type* visit(const CallExp *e);
//		const types::Type* visit(const RecordExp *e);
		const types::Type* visit(const SeqExp *e);
		const types::Type* visit(const AssignExp *e);
		const types::Type* visit(const IfExp *e);
		const types::Type* visit(const WhileExp *e);
		const types::Type* visit(const ForExp *e);
		const types::Type* visit(const BreakExp *e);
		const types::Type* visit(const LetExp *e);
		const types::Type* visit(const ArrayExp *e);
//		const types::Type* visit(const FunctionDec *d);
		const types::Type* visit(const VarDec *d);
		const types::Type* visit(const TypeDec *d);
		const types::Type* visit(const Dec *d);
		const types::Type* visit(const NameTy *t);
//		const types::Type* visit(const RecordTy *t);
		const types::Type* visit(const ArrayTy *t);
		const types::Type* visit(const Ty *t);
		/*
		const types::Type* visit(const FieldList *f);
		const types::Type* visit(const ExpList *e);
		const types::Type* visit(const DecList *v);
		const types::Type* visit(const FieldExpList *f);
		*/
	};
} // end of namespace semantics
