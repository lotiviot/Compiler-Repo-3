#pragma once
#include <iostream>
#include <string>
#include "Absyn.h"

using namespace std;

namespace absyn {
	class Print
	{
	private:
		ostream*		out;

	public:
		Print(ostream& o) {out=&o;}
		/* Print Exp class types. Indent d spaces. */
		void prExp(const Exp *e, int d);
	private:
		void indent(int d);
		void say(bool b);
		void say(string s);
		void say(char* s);
		void say(int i);
		void sayln(string s);
		void prVar(const SimpleVar *v, int d);
		void prVar(const FieldVar *v, int d);
		void prVar(const SubscriptVar *v, int d);
		/* Print A_var types. Indent d spaces. */
		void prVar(const Var *v, int d);
		void prExp(const OpExp *e, int d);
		void prExp(const VarExp *e, int d);
		void prExp(const NilExp *e, int d);
		void prExp(const IntExp *e, int d);
		void prExp(const StringExp *e, int d);
		void prExp(const CallExp *e, int d);
		void prExp(const RecordExp *e, int d);
		void prExp(const SeqExp *e, int d);
		void prExp(const AssignExp *e, int d);
		void prExp(const IfExp *e, int d);
		void prExp(const WhileExp *e, int d);
		void prExp(const ForExp *e, int d);
		void prExp(const BreakExp *e, int d);
		void prExp(const LetExp *e, int d);
		void prExp(const ArrayExp *e, int d);
		void prDec(const FunctionDec *d, int i);
		void prDec(const VarDec *d, int i);
		void prDec(const TypeDec *d, int i);
		void prDec(const Dec *d, int i);
		void prTy(const NameTy *t, int i);
		void prTy(const RecordTy *t, int i);
		void prTy(const ArrayTy *t, int i);
		void prTy(const Ty *t, int i);
		void prFieldlist(const FieldList *f, int d);
		void prExplist(const ExpList *e, int d);
		void prDecList(const DecList *v, int d);
		void prFieldExpList(const FieldExpList *f, int d);
	};
}  // end of namespace Absyn
