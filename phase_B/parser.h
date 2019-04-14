/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     FLOAT = 259,
     IDENTIFIER = 260,
     STRING_DEF = 261,
     COMMENT1 = 262,
     COMMENT2 = 263,
     COMMENT3 = 264,
     IF = 265,
     ELSE = 266,
     WHILE = 267,
     FOR = 268,
     FUNCTION = 269,
     RETURN = 270,
     BREAK = 271,
     CONTINUE = 272,
     AND = 273,
     NOT = 274,
     OR = 275,
     LOCAL = 276,
     TRUE = 277,
     FALSE = 278,
     NIL = 279,
     GREATER_EQ = 280,
     LESSER_EQ = 281,
     DOUBLE_COLON = 282,
     DOUBLE_DOT = 283,
     PLUS_PLUS = 284,
     MINUS_MINUS = 285,
     NOTEQUAL = 286,
     EQUAL = 287,
     UMINUS = 288
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define FLOAT 259
#define IDENTIFIER 260
#define STRING_DEF 261
#define COMMENT1 262
#define COMMENT2 263
#define COMMENT3 264
#define IF 265
#define ELSE 266
#define WHILE 267
#define FOR 268
#define FUNCTION 269
#define RETURN 270
#define BREAK 271
#define CONTINUE 272
#define AND 273
#define NOT 274
#define OR 275
#define LOCAL 276
#define TRUE 277
#define FALSE 278
#define NIL 279
#define GREATER_EQ 280
#define LESSER_EQ 281
#define DOUBLE_COLON 282
#define DOUBLE_DOT 283
#define PLUS_PLUS 284
#define MINUS_MINUS 285
#define NOTEQUAL 286
#define EQUAL 287
#define UMINUS 288




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 30 "parser.y"

	int intValue;
	float floatValue;
	char* strValue;
	void *expression; 



/* Line 2068 of yacc.c  */
#line 125 "parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


