#pragma once
#include <string>
#include <typeinfo>
#include <vector>

using namespace std;

//defines types assoicated to variables and type definitions used in Tiger language
namespace types
{
	class Type {
	public:
		//returns the actual data type
		virtual const Type* actual() const { return this;}

		//return true if t can be converted to current type, 
		//otherwise return false
    	virtual bool coerceTo(const Type *t) const = 0;
	};

	class ARRAY : public Type {
	private:
		Type*	element;
	public:
		ARRAY(Type *e) : element(e) {}
		const Type* getElement( void ) const { return element; }
		bool coerceTo(const Type *t) const
		{
			return this == t->actual();
		}
	};

	class INT : public Type {
	public:
		INT () {}
		bool coerceTo(const Type *t) const 
		{
			return (dynamic_cast<const INT *>(t->actual()) != NULL);
		}
	};

	class NAME : public Type {
	private:
		string		name;
		Type		*binding;
	public:
		NAME(string n) : name(n), binding(NULL) {}
		
		bool isLoop()  
		{
			Type	*b = binding; 
			bool	any;
			
			binding = NULL;
			if (b == NULL) 
				any = true;
			else if ( dynamic_cast<NAME *>(b) != NULL )
			{
				NAME * n = (NAME *)b;
				if ( n->getBinding() == NULL )
					any = true;
				else
					any = ((NAME *)b)->isLoop();
			}
			else 
				any = false;
			binding = b;
			return any;
		 }
     
		const Type* actual() const {return binding->actual();}
		bool coerceTo(const Type *t) const 	{ return actual()->coerceTo(t); }
		void bind(Type *t) { binding = t; }
		string getName( void ) const { return name; }
		const Type* getBinding( void ) const { return binding; }
	};

	class RECORD : public Type {
	private:
		string		fieldName;
		Type		*fieldType;
		RECORD		*rest;
	public:
		RECORD(string n, Type *t, RECORD *x) 
			: fieldName(n), fieldType(t), rest(x)
		{}

		string getFieldName( void ) const { return fieldName; }
		const Type* getFieldType( void ) const { return fieldType; }
		const RECORD * getRest( void ) const { return rest; }
		void setRest(RECORD *r) { rest = r; }
		bool coerceTo(const Type *t) const { return this == t->actual();  }
	};

	class NIL : public Type {
	public:
		NIL () {}
		bool coerceTo(const Type *t) const 
		{
			const Type *a = t->actual();
			return	(dynamic_cast<const RECORD *>(a) != NULL) || 
					(dynamic_cast<const NIL *>(a) != NULL);
        }
	};

	class STRING : public Type {
	public:
		STRING() {}
		bool coerceTo(const Type *t) const 
		{ return dynamic_cast<const STRING *>(t->actual()) != NULL; }
	};

	class VOID : public Type {
	public:
		VOID () {}
		bool coerceTo(const Type *t) const 
		{ return dynamic_cast<const VOID *>(t->actual()) != NULL;; }
	};

	class FUNCTION : public Type {
	private:
		string					funcName;
		vector<const Type *>	fieldType;
		Type*					result;
	public:
		FUNCTION(string n, vector<const Type *> t, Type* r) 
			: funcName(n), fieldType(t), result(r)
		{}

		string getFuncName( void ) const { return funcName; }
		const vector<const Type*> getFieldType( void ) const { return fieldType; }
		const Type* getResult( void ) const { return result; }
		bool coerceTo(const Type *t) const 
		{ 		
			return this == t->actual();
		}
	};
} //end of namespace types