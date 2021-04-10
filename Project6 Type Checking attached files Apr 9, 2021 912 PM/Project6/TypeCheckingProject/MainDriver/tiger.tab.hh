/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C

Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
part or all of the Bison parser skeleton and distribute that work
under terms of your choice, so long as that work isn't itself a
parser generator using the skeleton or a modified version thereof
as a parser skeleton.  Alternatively, if you modify or redistribute
the parser skeleton itself, you may (at your option) remove this
special exception, which will cause the skeleton and the resulting
Bison output files to be licensed under the GNU General Public
License without this special exception.

This special exception was added by the Free Software Foundation in
version 2.2 of Bison.  */

#ifndef YY_YY_TIGER_TAB_HH_INCLUDED
# define YY_YY_TIGER_TAB_HH_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
/* Line 2058 of yacc.c  */
#line 6 "tiger.yy"

#include <iostream>
#include "ErrorMsg.h"

#include "Absyn.h"

int yylex(void); /* function prototype */
void yyerror(char *s);	//called by the parser whenever an eror occurs
void yyerror(int, int, char *s);	//called by the parser whenever an eror occurs



									/* Line 2058 of yacc.c  */
#line 59 "tiger.tab.hh"

									/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
									/* Put the tokens into the symbol table, so that GDB and other debuggers
									know about them.  */
enum yytokentype {
	ID = 258,
	STRING = 259,
	INT = 260,
	COMMA = 261,
	COLON = 262,
	SEMICOLON = 263,
	LPAREN = 264,
	RPAREN = 265,
	LBRACK = 266,
	RBRACK = 267,
	LBRACE = 268,
	RBRACE = 269,
	DOT = 270,
	ARRAY = 271,
	IF = 272,
	THEN = 273,
	ELSE = 274,
	WHILE = 275,
	FOR = 276,
	TO = 277,
	DO = 278,
	LET = 279,
	IN = 280,
	END = 281,
	OF = 282,
	BREAK = 283,
	NIL = 284,
	FUNCTION = 285,
	VAR = 286,
	TYPE = 287,
	ASSIGN = 288,
	OR = 289,
	AND = 290,
	GE = 291,
	GT = 292,
	LE = 293,
	LT = 294,
	NEQ = 295,
	EQ = 296,
	MINUS = 297,
	PLUS = 298,
	DIVIDE = 299,
	TIMES = 300,
	UMINUS = 301
};
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
	/* Line 2058 of yacc.c  */
#line 18 "tiger.yy"

	int			ival;	//integer value of INT token
	string*		sval;	//pointer to name of IDENTIFIER or value of STRING	
						//I have to use pointers since C++ does not support 
						//string object as the union member
	absyn::Exp*				exp;
	absyn::Var*				var;
	absyn::Ty*				ty;
	absyn::Dec*				dec;
	absyn::DecList*			declist;
	absyn::ExpList*			explist;


	/* Line 2058 of yacc.c  */
#line 134 "tiger.tab.hh"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse(void *YYPARSE_PARAM);
#else
int yyparse();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse(void);
#else
int yyparse();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_TIGER_TAB_HH_INCLUDED  */
