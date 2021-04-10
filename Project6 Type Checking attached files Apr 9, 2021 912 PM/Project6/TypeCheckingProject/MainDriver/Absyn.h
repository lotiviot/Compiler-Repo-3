#pragma once
#include <string>

using namespace std;


namespace absyn
{

	/*This namespace defines all kinds of nodes in the abstract syntax tree*/
	
	//Ancestor of all nodes
	class Absyn;

	//All kinds of expression nodes
	class ArrayExp;
	class Exp;			//parent of all expression nodes
	class AssignExp;
	class BreakExp;
	class CallExp;
	class ForExp;
	class IfExp;
	class IntExp;
	class LetExp;
	class NilExp;
	class OpExp;
	class RecordExp;
	class SeqExp;
	class StringExp;
	class VarExp;
	class WhileExp;

	//All kinds of type nodes
	class Ty;  // parent of all type nodes
	class ArrayTy;
	class NameTy;
	class RecordTy;

	//All kinds of declaration nodes
	class Dec; // parent of all declaration nodes
	class FunctionDec;
	class TypeDec;
	class VarDec;

	//all kinds of variable nodes
	class Var; //parent of all variable nodes
	class FieldVar;
	class SimpleVar;
	class SubscriptVar;

	//all types of list nodes
	class DecList;
	class ExpList;
	class FieldExpList;
	class FieldList;

	/*****************************************************************/
	/*****************************************************************/
	/*                       Definition                              */
	/*****************************************************************/
	/*****************************************************************/
	//abstract base class, the ancestor of all type of tree nodes
	class Absyn {
	private:
		int		lineno;
		int		colno;
	public:
		//ructor
		Absyn(int line, int col) : lineno(line), colno(col) {}

		int getLineno( void ) const { return lineno; }
		int getColno( void )  const { return colno; }

		//pure virtual destructor
		//its only purpose is to make the class abstract.
		virtual ~Absyn() = 0 {}
	};

	//abstract base class, the ancestor of all type of expression nodes
	class Exp : public Absyn {
	public:
		Exp(int line, int col) : Absyn(line, col) {}
		//pure virtual destructor
		//its only purpose is to make the class abstract.
		virtual ~Exp()=0 {}
	};

	//abstract base class, the ancestor of all type of type nodes
	class Ty : public  Absyn {
	public:
		Ty(int line, int col) : Absyn(line, col) {}
		//pure virtual destructor
		//its only purpose is to make the class abstract.
		virtual ~Ty()=0 {}
	};

	//abstract base class, the ancestor of all type of variable nodes
	class Var : public  Absyn {
	public:
		Var(int line, int col) : Absyn(line, col) {}
		//pure virtual destructor
		//its only purpose is to make the class abstract.
		virtual ~Var()=0 {}
	};

	//abstract base class, the ancestor of all type of declaration nodes
	class Dec : public  Absyn {
	public:
		Dec(int line, int col) : Absyn(line, col) {}
		//pure virtual destructor
		//its only purpose is to make the class abstract.
		virtual ~Dec()=0 {}
	};

	class ArrayExp : public Exp {
	private:
		string		type;		//type of the array
		Exp			*size;		//size of the array
		Exp			*init;		//initial value of the array
	public:
		ArrayExp(int line, int col, string t, Exp *s, Exp *i)
			: Exp(line, col), type(t), size(s), init(i)
		{}

		string getType( void ) const  { return type; }
		const Exp * getSize( void ) const  { return size; }
		const Exp * getInit( void ) const  { return init; }
		~ArrayExp() { delete size; delete init; }
	};

	class ArrayTy : public Ty {
	private:
		string		name;
	public:
		ArrayTy(int line, int col, string t)
			: Ty(line, col), name(t)
		{}

		string getName( void ) const { return name;}
		~ArrayTy() {}
	};

	class AssignExp : public Exp {
	private:
		Var		*var;
		Exp		*exp;
	public:
		AssignExp(int line, int col, Var *v, Exp *e)
			:Exp(line, col), var(v), exp(e)
		{}

		const Var* getVar( void ) const { return var; }
		const Exp* getExp( void ) const { return exp; }

		~AssignExp()  { delete var; delete exp; }
	};

	class BreakExp : public Exp {
	public:
		BreakExp(int line, int col)
			: Exp(line, col)
		{}

		~BreakExp() {}
	};

	class DecList {
	private:
		Dec			*head;
		DecList		*rest;
	public:
		DecList(Dec *h, DecList *r)
			: head(h), rest(r)
		{}

		const Dec* getHead( void ) const { return head; }
		const DecList* getRest( void ) const { return rest; }
		~DecList() { delete head; delete rest; }
	};

	class ExpList {
	private:
		Exp			*head;
		ExpList		*rest;
	public:
		ExpList(Exp *h, ExpList *r)
			: head(h), rest(r)
		{}

		const Exp* getHead( void ) const { return head; }
		const ExpList* getRest( void ) const { return rest; }
		~ExpList() { delete head; delete rest; }
	};

	class FieldExpList : public Absyn {
	private:
		string			name;
		Exp				*init;
		FieldExpList	*rest;
	public:
		FieldExpList(int line, int col, string n, Exp *i, FieldExpList *t) 
			: Absyn(line, col), name(n), init(i), rest(t)
		{}

		string getName( void ) const { return name; }
		const Exp* getInit( void ) const { return init; }
		const FieldExpList* getRest( void ) const { return rest; }
		~FieldExpList() { delete init; delete rest; }
	};

	class FieldList : public Absyn {
	private:
		string		name;
		string		type;
		FieldList	*rest;
		bool		escape;
	public:
		FieldList(int line, int col, string n, string t, FieldList *x)
			:Absyn(line, col), name(n), type(t), rest(x), escape(true)
		{}

		string getName( void ) const { return name; }
		string getType( void ) const { return type; }
		const FieldList* getRest( void ) const { return rest; }
		bool getEscape( void ) const { return escape; }

		~FieldList() { delete rest; }
	};

	class FieldVar : public Var {
	private:
		Var			*var;
		string		field;
	public:
		FieldVar(int line, int col, Var *v, string f)
			: Var(line, col), var(v), field(f)
		{}

		const Var* getVar( void ) const { return var; }
		string getField( void ) const { return field; }

		~FieldVar() { delete var; }
	};

	class IfExp : public Exp {
	private:
		Exp			*test;
		Exp			*thenclause;
		Exp			*elseclause; /* optional */
	public:
		IfExp(int line, int col, Exp *x, Exp *y) 
			: Exp(line, col), test(x), thenclause(y), elseclause(NULL)
		{}
		
		IfExp(int line, int col, Exp *x, Exp *y, Exp *z) 
			: Exp(line, col), test(x), thenclause(y), elseclause(z)
		{}

		const Exp* getTest( void ) const { return test; }
		const Exp* getThenClause( void ) const { return thenclause; }
		const Exp* getElseClause( void ) const { return elseclause; }

		~IfExp() { delete test; delete thenclause; delete elseclause; }
	};

	class IntExp : public Exp {
	private:
		int		value;
	public:
		IntExp(int line, int col, int v)
			: Exp(line, col), value(v)
		{}

		int getValue( void ) const { return value; }

		~IntExp() {}
	};

	class LetExp : public Exp {
	private:
		DecList			*decs;
		Exp				*body;
	public:
		LetExp(int line, int col, DecList *d, Exp *b) 
			: Exp(line, col), decs(d), body(b)
		{}

		const DecList* getDecs( void ) const { return decs; }
		const Exp* getBody( void ) const { return body; }

		~LetExp() { delete decs; delete body; }
	};

	class NameTy : public Ty {
	private:
		string		name;
	public:
		NameTy(int line, int col, string n)
			: Ty(line, col), name(n)
		{}

		string getName( void ) const { return name; }

		~NameTy() {}
	};

	class NilExp : public Exp {
	public:
		NilExp(int line, int col)
			: Exp(line, col)
		{}

		~NilExp() {}
	};

	class OpExp : public Exp {
	public:
		enum OpType {PLUS=0, MINUS=1, MUL=2, DIV=3, EQ=4, NE=5, LT=6, LE=7, GT=8, GE=9};
	private:
		Exp			*left, *right;
		OpType		oper;
	public:
		OpExp(int line, int col, Exp *l, OpType o, Exp *r) 
			: Exp(line, col), left(l), oper(o), right(r)
		{}

		const Exp* getLeft( void ) const { return left; }
		const Exp* getRight( void ) const { return right; }
		OpType getOper( void ) const { return oper; }

		~OpExp() { delete left; delete right; }
	};
	
	class RecordExp : public Exp {
	private:
		string			type;
		FieldExpList	*fields;
	public:
		RecordExp(int line, int col, string t, FieldExpList *f) 
			: Exp(line, col), type(t), fields(f)
		{}

		string getType( void ) const { return type; }
		const FieldExpList* getFields( void ) const { return fields; }

		~RecordExp() { delete fields; }
	};

	class RecordTy : public Ty {
	private:
		FieldList		*fields;
	public:
		RecordTy(int line, int col, FieldList *f) 
			: Ty(line, col), fields(f)
		{}

		const FieldList* getFields( void ) const { return fields; }
		~RecordTy() { delete fields; }
	};

	class SeqExp : public Exp {
	private:
		ExpList			*list;
	public:
		SeqExp(int line, int col, ExpList *l) 
			:Exp(line, col), list(l)
		{}

		const ExpList* getList( void ) const { return list; }

		~SeqExp() { delete list; }
	};
	
	class SimpleVar : public Var {
	private:
		string		name;
	public:
		SimpleVar (int line, int col, string n)
			: Var(line, col), name(n)
		{}

		string getName( void ) const { return name; }
		~SimpleVar() {}
	};

	class StringExp : public Exp {
	private:
		string		value;
	public:
		StringExp(int line, int col, string v)
			: Exp(line, col), value(v)
		{}

		string getValue( void ) const { return value; }
		~StringExp() {}
	};

	class SubscriptVar : public Var {
	private:
		Var			*var;
		Exp			*index;
	public:
		SubscriptVar(int line, int col, Var *v, Exp *i)
			: Var(line, col), var(v), index(i)
		{}

		const Var* getVar( void ) const { return var; }
		const Exp* getIndex( void ) const { return index; }
		~SubscriptVar() { delete var; delete index; }
	};

	class TypeDec : public Dec {
	private:
		string		name;
		Ty			*ty;
		TypeDec		*next;
	public:
		TypeDec(int line, int col, string n, Ty *t, TypeDec *x) 
			: Dec(line, col), name(n), ty(t), next(x)
		{}

		string getName( void ) const { return name; }
		const Ty* getTy( void ) const { return ty; }
		const TypeDec* getNext( void ) const { return next; }
		void setNext( TypeDec *n) { next = n; }

		~TypeDec() { delete ty; delete next; }
	};

	class VarDec : public Dec {
	private:
		string		name;
		bool		escape;
		NameTy		*type; /* optional */
		Exp			*init;
	public:
		VarDec(int line, int col, string n, NameTy *t, Exp *i)
			: Dec(line, col), name(n), type(t), init(i), escape(true)
		{}

		string getName( void ) const { return name; }
		bool getEscape( void ) const { return escape; }
		const NameTy* getType( void ) const { return type; }
		const Exp* getInit( void ) const { return init; }
		~VarDec() { delete type; delete init; }
	};

	class VarExp : public Exp {
	private:
		Var			*var;
	public:
		VarExp(int line, int col, Var *v)
			: Exp(line, col), var(v)
		{}

		const Var* getVar( void ) const { return var; }
		~VarExp() { delete var; }
	};

	class WhileExp : public Exp {
	private:
		Exp			*test, *body;
	public:
		WhileExp(int line, int col, Exp *t, Exp *b)
			: Exp(line, col), test(t), body(b)
		{}

		const Exp* getTest( void ) const { return test; }
		const Exp* getBody( void ) const { return body; }
		~WhileExp() { delete test; delete body; }
	};

	class CallExp : public Exp {
	private:
		string		func;
		ExpList		*args;
	public:
		CallExp(int line, int col, string f, ExpList *a)
			: Exp(line, col), func(f), args(a)
		{}

		string getFunc( void ) const { return func; }
		const ExpList* getArgs( void ) const { return args; }

		~CallExp() { delete args; }
	};

	class ForExp : public Exp {
	private:
		VarDec		*var;
		Exp			*hi, *body;
	public:
		ForExp(int line, int col, VarDec *v, Exp *h, Exp *b) 
			: Exp(line, col), var(v), hi(h), body(b)
		{}

		const VarDec* getVar( void ) const { return var; }
		const Exp* getHi( void ) const { return hi; }
		const Exp* getBody( void ) const { return body; }

		~ForExp() { delete var; delete hi; delete body;}
	};

	class FunctionDec : public Dec {
	private:
		string			name;
		FieldList		*params;
		NameTy			*result;  /* optional */
		Exp				*body;
		FunctionDec		*next;
	public:
		FunctionDec(int line, int col, string n, FieldList *a, NameTy *r, Exp *b, FunctionDec *x)
			: Dec(line, col), name(n), params(a), result(r), body(b), next(x)
		{}

		string getName( void ) const { return name; }
		const FieldList* getParams( void ) const { return params; }
		const NameTy* getResult( void ) const { return result; }
		const Exp* getBody( void ) const { return body; }
		const FunctionDec* getNext( void ) const { return next; }
		void setNext( FunctionDec* n ) { next = n; }

		~FunctionDec() { delete params; delete result; delete body; delete next; }
	};




}	//end of namespace Absyn
