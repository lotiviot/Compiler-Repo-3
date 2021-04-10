#include "Print.h"

namespace absyn {

	void Print::indent(int d) {
		for(int i=0; i<d; i++) 
			*out << ' ';
	}

	void Print::say(string s) {
		*out << s;
	}

	void Print::say(char* s) {
		*out << s;
	}

	void Print::say(int i) {
		*out << i;
	}

	void Print::say(bool b) {
		*out << (b?"true":"false");
	}

	void Print::sayln(string s) {
		*out << s << endl;
	}

	void Print::prVar(const SimpleVar *v, int d) {
		say("SimpleVar("); say(v->getName()); say(")");
	}
  
	void Print::prVar(const FieldVar *v, int d) {
		sayln("FieldVar(");
		prVar(v->getVar(), d+1); sayln(",");
		indent(d+1); say(v->getField()); say(")");
	}

	void Print::prVar(const SubscriptVar *v, int d) {
		sayln("SubscriptVar(");
		prVar(v->getVar(), d+1); sayln(",");
		prExp(v->getIndex(), d+1); say(")");
	}

	/* Print A_var types. Indent d spaces. */
	void Print::prVar(const Var *v, int d) {
		indent(d);
		if (dynamic_cast<const SimpleVar *>(v) != NULL) prVar((const SimpleVar *) v, d);
		else if (dynamic_cast<const FieldVar *>(v) != NULL) prVar((const FieldVar *) v, d);
		else if (dynamic_cast<const SubscriptVar *>(v) != NULL) prVar((const SubscriptVar *) v, d);
		else throw new runtime_error("Print.prVar");
	}
  
	void Print::prExp(const OpExp *e, int d) {
		sayln("OpExp(");
		indent(d+1); 
		switch(e->getOper()) {
		case OpExp::PLUS: say(string("PLUS")); break;
		case OpExp::MINUS: say(string("MINUS")); break;
		case OpExp::MUL: say(string("MUL")); break;
		case OpExp::DIV: say(string("DIV")); break;
		case OpExp::EQ: say(string("EQ")); break;
		case OpExp::NE: say(string("NE")); break;
		case OpExp::LT: say(string("LT")); break;
		case OpExp::LE: say(string("LE")); break;
		case OpExp::GT: say(string("GT")); break;
		case OpExp::GE: say(string("GE")); break;
		default:
			throw new runtime_error("Print.prExp.OpExp");
		}
		sayln(",");
		prExp(e->getLeft(), d+1); sayln(",");
		prExp(e->getRight(), d+1); say(")");
	}

	void Print::prExp(const VarExp *e, int d) {
		sayln("varExp("); prVar(e->getVar(), d+1);
		say(")");
	}

	void Print::prExp(const NilExp *e, int d) {
		say("NilExp()");
	}

	void Print::prExp(const IntExp *e, int d) {
		say("IntExp("); say(e->getValue()); say(")");
	}

	void Print::prExp(const StringExp *e, int d) {
		say("StringExp("); say(e->getValue()); say(")");
	}

	void Print::prExp(const CallExp *e, int d) {
		say("CallExp("); say(e->getFunc()); sayln(",");
		prExplist(e->getArgs(), d+1); say(")");
	}

	void Print::prExp(const RecordExp *e, int d) {
		say("RecordExp("); say(e->getType());  sayln(",");
		prFieldExpList(e->getFields(), d+1); say(")");
	}

	void Print::prExp(const SeqExp *e, int d) {
		sayln("SeqExp(");
		prExplist(e->getList(), d+1); say(")");
	}

	void Print::prExp(const AssignExp *e, int d) {
		sayln("AssignExp(");
		prVar(e->getVar(), d+1); sayln(",");
		prExp(e->getExp(), d+1); say(")");
	}
  
	void Print::prExp(const IfExp *e, int d) {
		sayln("IfExp(");
		prExp(e->getTest(), d+1); sayln(",");
		prExp(e->getThenClause(), d+1);
		if (e->getElseClause()!=NULL) { /* else is optional */
			sayln(",");
			prExp(e->getElseClause(), d+1);
		}
		say(")");
	}

	void Print::prExp(const WhileExp *e, int d) {
		sayln("WhileExp(");
		prExp(e->getTest(), d+1); sayln(",");
		prExp(e->getBody(), d+1); sayln(")");
	}

	void Print::prExp(const ForExp *e, int d) {
		sayln("ForExp("); 
		indent(d+1); prDec(e->getVar(), d+1); sayln(",");
		prExp(e->getHi(), d+1); sayln(",");
		prExp(e->getBody(), d+1); say(")");
	}

	void Print::prExp(const BreakExp *e, int d) {
		say("BreakExp()");
	}

	void Print::prExp(const LetExp *e, int d) {
		say("LetExp("); sayln("");
		prDecList(e->getDecs(), d+1); sayln(",");
		prExp(e->getBody(), d+1); say(")");
	}

	void Print::prExp(const ArrayExp *e, int d) {
		say("ArrayExp("); say(e->getType()); sayln(",");
		prExp(e->getSize(), d+1); sayln(",");
		prExp(e->getInit(), d+1); say(")");
	}

  /* Print Exp class types. Indent d spaces. */
	void Print::prExp(const Exp *e, int d) {
		indent(d);
		if (dynamic_cast<const OpExp*>(e) != NULL) prExp((const OpExp*)e, d);
		else if (dynamic_cast<const VarExp*>(e) != NULL) prExp((const VarExp*)e, d);
		else if (dynamic_cast<const NilExp*>(e) != NULL) prExp((const NilExp*)e, d);
		else if (dynamic_cast<const IntExp*>(e) != NULL) prExp((const IntExp*)e, d);
		else if (dynamic_cast<const StringExp*>(e) != NULL) prExp((const StringExp*)e, d);
		else if (dynamic_cast<const CallExp*>(e) != NULL) prExp((const CallExp*)e, d);
		else if (dynamic_cast<const RecordExp*>(e) != NULL) prExp((const RecordExp*)e, d);
		else if (dynamic_cast<const SeqExp*>(e) != NULL) prExp((const SeqExp*)e, d);
		else if (dynamic_cast<const AssignExp*>(e) != NULL) prExp((const AssignExp*)e, d);
		else if (dynamic_cast<const IfExp*>(e) != NULL) prExp((const IfExp*)e, d);
		else if (dynamic_cast<const WhileExp*>(e) != NULL) prExp((const WhileExp*)e, d);
		else if (dynamic_cast<const ForExp*>(e) != NULL) prExp((const ForExp*)e, d);
		else if (dynamic_cast<const BreakExp*>(e) != NULL) prExp((const BreakExp*)e, d);
		else if (dynamic_cast<const LetExp*>(e) != NULL) prExp((const LetExp*)e, d);
		else if (dynamic_cast<const ArrayExp*>(e) != NULL) prExp((const ArrayExp*)e, d);
		else throw new runtime_error("Print.prExp");
	}

	void Print::prDec(const FunctionDec *d, int i) {
		say("FunctionDec(");
		if (d!=NULL) {
			sayln(d->getName());
			prFieldlist(d->getParams(), i+1); sayln(",");
			if (d->getResult()!=NULL) {
				indent(i+1); sayln(d->getResult()->getName());
			}
			prExp(d->getBody(), i+1); sayln(",");
			indent(i+1); prDec(d->getNext(), i+1);
		}
		say(")");
	}

	void Print::prDec(const VarDec *d, int i) {
		say("VarDec("); say(d->getName()); sayln(",");
		if (d->getType()!=NULL) {
			indent(i+1); say(d->getType()->getName());  sayln(",");
		}
		prExp(d->getInit(), i+1); sayln(",");
		indent(i+1); say(d->getEscape()); say(")"); 
	}

	void Print::prDec(const TypeDec *d, int i) {
		if (d!=NULL) {
			say("TypeDec("); say(d->getName()); sayln(",");
			prTy(d->getTy(), i+1); 
			if (d->getNext()!=NULL) {
				say(","); prDec(d->getNext(), i+1); 
			}
			say(")");
		}
	}

	void Print::prDec(const Dec *d, int i) {
		indent(i);
		if (dynamic_cast<const FunctionDec*>(d) != NULL) prDec((const FunctionDec*) d, i);
		else if (dynamic_cast<const VarDec*>(d) != NULL) prDec((const VarDec*) d, i);
		else if (dynamic_cast<const TypeDec*>(d) != NULL) prDec((const TypeDec*) d, i);
		else throw runtime_error("Print.prDec");
	}

	void Print::prTy(const NameTy *t, int i) {
		say("NameTy("); say(t->getName()); say(")");
	}

	void Print::prTy(const RecordTy *t, int i) {
		sayln("RecordTy(");
		prFieldlist(t->getFields(), i+1); say(")");
	}

	void Print::prTy(const ArrayTy *t, int i) {
		say("ArrayTy("); say(t->getName()); say(")");
	}

	void Print::prTy(const Ty *t, int i) {
		if (t!=NULL) {
			indent(i);
			if (dynamic_cast<const NameTy*>(t) != NULL) prTy((const NameTy*) t, i);
			else if (dynamic_cast<const RecordTy*>(t) != NULL) prTy((const RecordTy*) t, i);
			else if (dynamic_cast<const ArrayTy*>(t) != NULL) prTy((const ArrayTy*) t, i);
			else throw runtime_error("Print.prTy");
		}
	}

	void Print::prFieldlist(const FieldList *f, int d) {
		indent(d);
		say("Fieldlist("); 
		if (f!=NULL) {
			sayln("");
			indent(d+1); say(f->getName()); sayln("");
			indent(d+1); say(f->getType()); sayln(",");
			indent(d+1); 
			say(f->getEscape()); 
			sayln(",");
			prFieldlist(f->getRest(), d+1);
		}
		say(")");
	}

	void Print::prExplist(const ExpList *e, int d) {
		indent(d);
		say("ExpList("); 
		if (e!=NULL) {
			sayln("");
			prExp(e->getHead(), d+1); 
			if (e->getRest() != NULL) {
				sayln(","); prExplist(e->getRest(), d+1);
			}
		}
		say(")");
	}

	void Print::prDecList(const DecList *v, int d) {
		indent(d);
		say("DecList("); 
		if (v!=NULL) {
			sayln("");
			prDec(v->getHead(), d+1); 
			if (v->getRest() != NULL) {
				sayln(",");
				prDecList(v->getRest(), d+1);
			}
		}
		say(")");
	}

	void Print::prFieldExpList(const FieldExpList *f, int d) {
		indent(d);
		say("FieldExpList("); 
		if (f!=NULL) {
			sayln(""); 		indent(d+1);
			say(f->getName()); sayln(",");
			prExp(f->getInit(), d+1); 
			if (f->getRest() != NULL) {
				sayln(",");
				prFieldExpList(f->getRest(), d+1);
			}
		}
		say(")");
	}

} // end of namespace Absyn