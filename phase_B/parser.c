/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "parser.y"


	#include "libfunc.h"
	#include "symtable.h"
	
	int yyerror( char * yaccProvidedMessage);
	int yylex(void);

	extern int yylineno;
	extern char * yytext;
	extern FILE * yyin;
	
	int scope = 0, functionScope = 0;
	SymTable_t * mySymbolTable;
	int numberOfUnnamed = 0;
	
	char *argumentName = NULL;
	struct argNames* list = NULL;
	
	int whileLoop = 0 ;
	bool intoFunction = false;
	
	int maxScope = 0;
	
	


/* Line 268 of yacc.c  */
#line 99 "parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
#line 30 "parser.y"

	int intValue;
	float floatValue;
	char* strValue;
	void *expression; 



/* Line 293 of yacc.c  */
#line 210 "parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 222 "parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  76
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   611

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  120
/* YYNRULES -- Number of states.  */
#define YYNSTATES  202

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    40,     2,     2,
      45,    46,    38,    36,    48,    37,    42,    39,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    50,    47,
      35,    31,    34,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    44,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    49,     2,    51,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    32,    33,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    16,    18,
      20,    22,    25,    28,    30,    32,    34,    36,    38,    40,
      42,    44,    46,    48,    50,    52,    54,    58,    62,    66,
      70,    74,    78,    82,    86,    90,    94,    98,   102,   106,
     110,   113,   116,   119,   122,   125,   128,   130,   132,   134,
     138,   140,   142,   144,   148,   150,   152,   155,   158,   160,
     164,   169,   173,   178,   181,   184,   189,   191,   193,   197,
     203,   206,   208,   209,   213,   216,   218,   221,   224,   228,
     231,   235,   239,   242,   244,   248,   251,   257,   258,   264,
     265,   269,   272,   273,   274,   275,   284,   285,   286,   294,
     296,   298,   300,   302,   304,   306,   308,   311,   312,   314,
     317,   321,   325,   328,   334,   342,   343,   350,   351,   362,
     365
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      53,     0,    -1,    54,    -1,    -1,    55,    54,    -1,    55,
      -1,    57,    47,    -1,    93,    -1,    94,    -1,    96,    -1,
      98,    -1,    16,    47,    -1,    17,    47,    -1,    81,    -1,
      85,    -1,    56,    -1,    47,    -1,     7,    -1,     8,    -1,
       9,    -1,    65,    -1,    62,    -1,    58,    -1,    59,    -1,
      60,    -1,    61,    -1,    57,    36,    57,    -1,    57,    37,
      57,    -1,    57,    38,    57,    -1,    57,    39,    57,    -1,
      57,    40,    57,    -1,    57,    34,    57,    -1,    57,    25,
      57,    -1,    57,    35,    57,    -1,    57,    26,    57,    -1,
      57,    33,    57,    -1,    57,    32,    57,    -1,    57,    18,
      57,    -1,    57,    20,    57,    -1,    45,    57,    46,    -1,
      37,    57,    -1,    19,    57,    -1,    63,    67,    -1,    67,
      63,    -1,    64,    67,    -1,    67,    64,    -1,    66,    -1,
      29,    -1,    30,    -1,    67,    31,    57,    -1,    67,    -1,
      69,    -1,    77,    -1,    45,    85,    46,    -1,    90,    -1,
       5,    -1,    21,     5,    -1,    27,     5,    -1,    68,    -1,
      67,    42,     5,    -1,    67,    43,    57,    44,    -1,    69,
      42,     5,    -1,    69,    43,    57,    44,    -1,    69,    71,
      -1,    67,    70,    -1,    45,    85,    46,    71,    -1,    71,
      -1,    72,    -1,    45,    73,    46,    -1,    28,     5,    45,
      73,    46,    -1,    57,    74,    -1,    57,    -1,    -1,    48,
      57,    74,    -1,    48,    57,    -1,    57,    -1,    57,    76,
      -1,    48,    57,    -1,    48,    57,    76,    -1,    43,    44,
      -1,    43,    75,    44,    -1,    43,    78,    44,    -1,    80,
      79,    -1,    80,    -1,    48,    80,    79,    -1,    48,    80,
      -1,    49,    57,    50,    57,    51,    -1,    -1,    49,    82,
      55,    84,    51,    -1,    -1,    49,    83,    51,    -1,    55,
      84,    -1,    -1,    -1,    -1,    14,     5,    86,    45,    91,
      46,    87,    81,    -1,    -1,    -1,    14,    88,    45,    91,
      46,    89,    81,    -1,     3,    -1,     4,    -1,     6,    -1,
      24,    -1,    22,    -1,    23,    -1,     5,    -1,     5,    92,
      -1,    -1,    56,    -1,     5,    56,    -1,     5,    92,    56,
      -1,    48,     5,    92,    -1,    48,     5,    -1,    10,    45,
      57,    46,    55,    -1,    10,    45,    57,    46,    55,    11,
      55,    -1,    -1,    12,    95,    45,    57,    46,    55,    -1,
      -1,    13,    97,    45,    73,    47,    57,    47,    73,    46,
      55,    -1,    15,    47,    -1,    15,    57,    47,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    62,    62,    63,    67,    68,    72,    73,    74,    75,
      76,    77,    85,    93,    94,    95,    96,   100,   101,   102,
     106,   107,   108,   109,   110,   111,   115,   116,   120,   121,
     122,   126,   127,   128,   129,   130,   131,   135,   136,   140,
     141,   142,   143,   184,   223,   262,   302,   306,   309,   312,
     355,   356,   357,   358,   359,   363,   463,   501,   513,   517,
     535,   551,   552,   556,   557,   558,   562,   563,   567,   571,
     575,   576,   577,   581,   582,   586,   587,   591,   592,   597,
     598,   599,   603,   604,   608,   609,   613,   617,   617,   622,
     622,   630,   631,   635,   643,   635,   701,   707,   701,   752,
     753,   754,   755,   756,   757,   761,   769,   778,   779,   780,
     788,   800,   806,   816,   817,   821,   821,   825,   825,   829,
     837
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "FLOAT", "IDENTIFIER",
  "STRING_DEF", "COMMENT1", "COMMENT2", "COMMENT3", "IF", "ELSE", "WHILE",
  "FOR", "FUNCTION", "RETURN", "BREAK", "CONTINUE", "AND", "NOT", "OR",
  "LOCAL", "TRUE", "FALSE", "NIL", "GREATER_EQ", "LESSER_EQ",
  "DOUBLE_COLON", "DOUBLE_DOT", "PLUS_PLUS", "MINUS_MINUS", "'='",
  "NOTEQUAL", "EQUAL", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "'['", "']'", "'('", "')'", "';'", "','", "'{'", "':'",
  "'}'", "$accept", "program", "help_stmt", "stmt", "comment", "expr",
  "addsubExpr", "muldivmodExpr", "cmpExpr", "booleanExpr", "term", "plus",
  "minus", "assignexpr", "primary", "lvalue", "member", "call",
  "callsuffix", "normcall", "methodcall", "elist", "listed_expr", "eelist",
  "listed_eexpr", "objectdef", "indexed", "indexed_expr", "indexedelem",
  "block", "$@1", "$@2", "morestmt", "funcdef", "$@3", "$@4", "$@5", "$@6",
  "const", "idlist", "helpidlist", "ifstmt", "whilestmt", "$@7", "forstmt",
  "$@8", "returnstmt", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,    61,   286,   287,    62,    60,    43,    45,    42,    47,
      37,   288,    46,    91,    93,    40,    41,    59,    44,   123,
      58,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    53,    54,    54,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    56,    56,    56,
      57,    57,    57,    57,    57,    57,    58,    58,    59,    59,
      59,    60,    60,    60,    60,    60,    60,    61,    61,    62,
      62,    62,    62,    62,    62,    62,    62,    63,    64,    65,
      66,    66,    66,    66,    66,    67,    67,    67,    67,    68,
      68,    68,    68,    69,    69,    69,    70,    70,    71,    72,
      73,    73,    73,    74,    74,    75,    75,    76,    76,    77,
      77,    77,    78,    78,    79,    79,    80,    82,    81,    83,
      81,    84,    84,    86,    87,    85,    88,    89,    85,    90,
      90,    90,    90,    90,    90,    91,    91,    91,    91,    91,
      91,    92,    92,    93,    93,    95,    94,    97,    96,    98,
      98
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     3,
       1,     1,     1,     3,     1,     1,     2,     2,     1,     3,
       4,     3,     4,     2,     2,     4,     1,     1,     3,     5,
       2,     1,     0,     3,     2,     1,     2,     2,     3,     2,
       3,     3,     2,     1,     3,     2,     5,     0,     5,     0,
       3,     2,     0,     0,     0,     8,     0,     0,     7,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     1,     2,
       3,     3,     2,     5,     7,     0,     6,     0,    10,     2,
       3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    99,   100,    55,   101,    17,    18,    19,     0,   115,
     117,    96,     0,     0,     0,     0,     0,   103,   104,   102,
       0,    47,    48,     0,     0,     0,    16,    87,     0,     2,
       5,    15,     0,    22,    23,    24,    25,    21,     0,     0,
      20,    46,    50,    58,    51,    52,    13,    14,    54,     7,
       8,     9,    10,     0,     0,     0,    93,     0,   119,     0,
      11,    12,    41,    56,    57,    40,    79,     0,    75,     0,
       0,    83,     0,     0,     0,     0,     1,     4,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     0,    42,     0,    44,     0,     0,     0,     0,
      72,    43,    45,    64,    66,    67,     0,     0,    63,     0,
       0,    72,     0,   107,   120,     0,     0,    76,    80,    81,
       0,    82,    39,    53,    92,    90,    37,    38,    32,    34,
      36,    35,    31,    33,    26,    27,    28,    29,    30,     0,
       0,    49,    59,     0,    71,     0,    61,     0,     0,     0,
       0,   107,   105,   108,     0,     0,    77,    85,    65,    92,
       0,     0,    72,    60,     0,    70,    68,    62,   113,     0,
       0,     0,     0,   109,   106,    97,     0,    78,    84,    91,
      88,     0,    74,     0,   116,     0,    94,   112,   110,     0,
      86,    69,    73,   114,    72,     0,   111,    98,     0,    95,
       0,   118
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,   103,   104,
     105,   145,   165,    69,   117,    45,    70,   121,    71,    46,
      74,    75,   160,    47,   112,   195,    57,   189,    48,   154,
     174,    49,    50,    54,    51,    55,    52
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -173
static const yytype_int16 yypact[] =
{
     132,  -173,  -173,  -173,  -173,  -173,  -173,  -173,   -40,  -173,
    -173,    32,   196,    -9,    -2,   254,    43,  -173,  -173,  -173,
      45,  -173,  -173,   254,   167,   226,  -173,     0,    53,  -173,
     132,  -173,   339,  -173,  -173,  -173,  -173,  -173,    -1,    -1,
    -173,  -173,   237,  -173,   -18,  -173,  -173,  -173,  -173,  -173,
    -173,  -173,  -173,   254,     9,    12,  -173,    14,  -173,   362,
    -173,  -173,  -173,  -173,  -173,  -173,  -173,   254,   291,    20,
      21,    31,    88,    37,   132,    33,  -173,  -173,   254,   254,
     254,   254,   254,   254,   254,   254,   254,   254,   254,   254,
     254,  -173,    72,    18,   -18,    18,    84,   254,    91,   254,
     254,  -173,  -173,  -173,  -173,  -173,    92,   254,  -173,   408,
     254,   254,    55,   102,  -173,   268,   254,  -173,  -173,  -173,
      56,  -173,  -173,    57,   132,  -173,   539,   523,   571,   571,
     555,   555,   571,   571,    54,    54,  -173,  -173,  -173,    66,
      58,   500,  -173,   454,   315,    83,  -173,   477,   132,   431,
      86,   102,     1,  -173,    85,   254,   291,    31,  -173,   132,
      79,    57,   254,  -173,   254,  -173,  -173,  -173,   146,   132,
     254,   114,   158,  -173,   109,  -173,    -4,  -173,  -173,  -173,
    -173,   118,   315,   132,  -173,   385,  -173,   117,  -173,   119,
    -173,  -173,  -173,  -173,   254,   119,  -173,  -173,   120,  -173,
     132,  -173
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -173,  -173,   137,   -68,  -112,   -12,  -173,  -173,  -173,  -173,
    -173,   134,   136,  -173,  -173,   -20,  -173,     4,  -173,   -42,
    -173,  -104,    -8,  -173,    24,  -173,  -173,    26,    64,  -172,
    -173,  -173,    28,   -10,  -173,  -173,  -173,  -173,  -173,    34,
       5,  -173,  -173,  -173,  -173,  -173,  -173
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -90
static const yytype_int16 yytable[] =
{
      59,   153,   108,    62,     3,    53,   124,   150,     5,     6,
       7,    65,    68,    72,    78,    73,    79,   197,    93,    95,
      16,    80,    81,   199,   106,   107,    20,   100,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    56,    60,   153,
     173,   109,    94,    94,    92,    61,    96,   190,    63,   172,
      64,   -89,   108,    76,   110,   115,   159,   111,   181,   113,
      98,    99,   188,   100,   118,   119,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   120,
     168,   158,   139,   123,   125,   141,    11,   143,   144,   140,
     198,   159,    88,    89,    90,   147,   142,   146,   149,   144,
     151,   184,   100,   162,   156,    67,    78,   152,    79,     5,
       6,     7,   161,    80,    81,   193,     5,     6,     7,   158,
      82,    83,    84,    85,    86,    87,    88,    89,    90,   166,
     180,   175,   201,   170,   122,     1,     2,     3,     4,     5,
       6,     7,     8,   176,     9,    10,    11,    12,    13,    14,
     144,    15,   182,    16,    17,    18,    19,   183,   185,    20,
     186,    21,    22,   187,   191,   172,   200,    77,    27,    23,
       1,     2,     3,     4,   192,    24,   101,    25,   102,    26,
     177,    27,   144,   178,   157,   171,    15,   179,    16,    17,
      18,    19,   196,     0,    20,     0,    21,    22,     0,     1,
       2,     3,     4,     0,    23,     0,     0,     0,     0,     0,
      24,    66,    25,     0,     0,    15,    67,    16,    17,    18,
      19,     0,     0,    20,     0,    21,    22,     0,     0,     1,
       2,     3,     4,    23,     0,     0,     0,     0,     0,    24,
      11,    25,     0,    58,     0,    15,     0,    16,    17,    18,
      19,     0,     0,    20,     0,    21,    22,     1,     2,     3,
       4,     0,     0,    23,     0,    96,    21,    22,    97,    24,
       0,    25,     0,    15,     0,    16,    17,    18,    19,    98,
      99,    20,   100,    21,    22,     0,    78,     0,    79,     0,
       0,    23,     0,    80,    81,     0,     0,    24,     0,    25,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    78,
       0,    79,     0,     0,     0,     0,    80,    81,   155,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,     0,    78,     0,    79,     0,     0,     0,   116,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,     0,    78,     0,    79,
       0,     0,     0,   164,    80,    81,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      78,     0,    79,     0,     0,     0,    91,    80,    81,     0,
       0,     0,     0,     0,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    78,     0,    79,     0,     0,     0,   114,
      80,    81,     0,     0,     0,     0,     0,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    78,     0,    79,     0,
       0,     0,   194,    80,    81,     0,     0,     0,     0,     0,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    78,
       0,    79,     0,     0,   148,     0,    80,    81,     0,     0,
       0,     0,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    78,     0,    79,     0,     0,   169,     0,    80,
      81,     0,     0,     0,     0,     0,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    78,     0,    79,   163,     0,
       0,     0,    80,    81,     0,     0,     0,     0,     0,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    78,     0,
      79,   167,     0,     0,     0,    80,    81,     0,     0,     0,
       0,     0,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    78,     0,     0,     0,     0,     0,     0,    80,    81,
       0,     0,     0,     0,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    80,    81,     0,     0,     0,     0,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      80,    81,     0,     0,     0,     0,     0,   -90,   -90,    84,
      85,    86,    87,    88,    89,    90,   -90,   -90,     0,     0,
       0,     0,     0,     0,     0,   -90,   -90,    86,    87,    88,
      89,    90
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-173))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-90))

static const yytype_int16 yycheck[] =
{
      12,   113,    44,    15,     5,    45,    74,   111,     7,     8,
       9,    23,    24,    25,    18,    25,    20,   189,    38,    39,
      21,    25,    26,   195,    42,    43,    27,    45,    32,    33,
      34,    35,    36,    37,    38,    39,    40,     5,    47,   151,
     152,    53,    38,    39,    45,    47,    28,    51,     5,    48,
       5,    51,    94,     0,    45,    67,   124,    45,   162,    45,
      42,    43,   174,    45,    44,    44,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    48,
     148,   123,    92,    46,    51,    97,    14,    99,   100,     5,
     194,   159,    38,    39,    40,   107,     5,     5,   110,   111,
      45,   169,    45,    45,   116,    49,    18,     5,    20,     7,
       8,     9,    46,    25,    26,   183,     7,     8,     9,   161,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    46,
      51,    46,   200,    47,    46,     3,     4,     5,     6,     7,
       8,     9,    10,   155,    12,    13,    14,    15,    16,    17,
     162,    19,   164,    21,    22,    23,    24,    11,   170,    27,
      46,    29,    30,     5,    46,    48,    46,    30,    49,    37,
       3,     4,     5,     6,   182,    43,    42,    45,    42,    47,
     156,    49,   194,   157,   120,   151,    19,   159,    21,    22,
      23,    24,   187,    -1,    27,    -1,    29,    30,    -1,     3,
       4,     5,     6,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      43,    44,    45,    -1,    -1,    19,    49,    21,    22,    23,
      24,    -1,    -1,    27,    -1,    29,    30,    -1,    -1,     3,
       4,     5,     6,    37,    -1,    -1,    -1,    -1,    -1,    43,
      14,    45,    -1,    47,    -1,    19,    -1,    21,    22,    23,
      24,    -1,    -1,    27,    -1,    29,    30,     3,     4,     5,
       6,    -1,    -1,    37,    -1,    28,    29,    30,    31,    43,
      -1,    45,    -1,    19,    -1,    21,    22,    23,    24,    42,
      43,    27,    45,    29,    30,    -1,    18,    -1,    20,    -1,
      -1,    37,    -1,    25,    26,    -1,    -1,    43,    -1,    45,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    26,    50,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    18,    -1,    20,    -1,    -1,    -1,    48,
      25,    26,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    18,    -1,    20,
      -1,    -1,    -1,    48,    25,    26,    -1,    -1,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      18,    -1,    20,    -1,    -1,    -1,    47,    25,    26,    -1,
      -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    18,    -1,    20,    -1,    -1,    -1,    47,
      25,    26,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    18,    -1,    20,    -1,
      -1,    -1,    47,    25,    26,    -1,    -1,    -1,    -1,    -1,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    18,
      -1,    20,    -1,    -1,    46,    -1,    25,    26,    -1,    -1,
      -1,    -1,    -1,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    18,    -1,    20,    -1,    -1,    46,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    18,    -1,    20,    44,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    18,    -1,
      20,    44,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,
      -1,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    25,    26,    -1,    -1,    -1,    -1,
      -1,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      25,    26,    -1,    -1,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    35,    36,    37,    38,
      39,    40
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    12,
      13,    14,    15,    16,    17,    19,    21,    22,    23,    24,
      27,    29,    30,    37,    43,    45,    47,    49,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    77,    81,    85,    90,    93,
      94,    96,    98,    45,    95,    97,     5,    88,    47,    57,
      47,    47,    57,     5,     5,    57,    44,    49,    57,    75,
      78,    80,    57,    85,    82,    83,     0,    54,    18,    20,
      25,    26,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    47,    45,    67,    69,    67,    28,    31,    42,    43,
      45,    63,    64,    70,    71,    72,    42,    43,    71,    57,
      45,    45,    86,    45,    47,    57,    48,    76,    44,    44,
      48,    79,    46,    46,    55,    51,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    85,
       5,    57,     5,    57,    57,    73,     5,    57,    46,    57,
      73,    45,     5,    56,    91,    50,    57,    80,    71,    55,
      84,    46,    45,    44,    48,    74,    46,    44,    55,    46,
      47,    91,    48,    56,    92,    46,    57,    76,    79,    84,
      51,    73,    57,    11,    55,    57,    46,     5,    56,    89,
      51,    46,    74,    55,    47,    87,    92,    81,    73,    81,
      46,    55
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 62 "parser.y"
    { fprintf(stdout, "Rule used: program -> help_stmt\n"); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 63 "parser.y"
    { fprintf(stdout, "Rule used: program -> e\n"); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 67 "parser.y"
    { fprintf(stdout, "Rule used: help_stmt -> stmt help_stmt\n"); }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 68 "parser.y"
    { fprintf(stdout, "Rule used: help_stmt -> stmt\n"); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 72 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> expr;\n"); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 73 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> ifstmt\n"); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 74 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> whilestmt\n"); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 75 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> forstmt\n");}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 76 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> returnstmt\n"); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 77 "parser.y"
    { 
		if (!whileLoop){
		/* Wrong placement of break */
				fprintf(stdout, "ERROR:: Break not in loop. Program will be terminated. \n");
				exit(EXIT_FAILURE);
		}
		fprintf(stdout, "Rule used: stmt -> BREAK;\n"); 
	}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 85 "parser.y"
    { 
		if (!whileLoop){
		/* Wrong placement of continue */
					fprintf(stdout, "ERROR:: Continue not in loop. Program will be terminated. \n");
					exit(EXIT_FAILURE);
		}
		fprintf(stdout, "Rule used: stmt -> CONTINUE;\n"); 
	}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 93 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> block\n"); }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 94 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> funcdef\n"); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 95 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> comment\n");}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 96 "parser.y"
    { fprintf(stdout, "Rule used: stmt -> ;\n");}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 100 "parser.y"
    { fprintf(stdout, "Rule used: comment -> Single Line Comment\n");}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 101 "parser.y"
    { fprintf(stdout, "Rule used: comment -> Multi Line Comment\n");}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 102 "parser.y"
    { fprintf(stdout, "Rule used: comment -> Nested Comment\n");}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 106 "parser.y"
    { fprintf(stdout, "Rule used: expr -> assignexpr\n"); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 107 "parser.y"
    { fprintf(stdout, "Rule used: expr -> term\n"); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 108 "parser.y"
    { fprintf(stdout, "Rule used: expr -> addsubExpr\n"); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 109 "parser.y"
    { fprintf(stdout, "Rule used: expr -> muldivmodExpr\n"); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 110 "parser.y"
    { fprintf(stdout, "Rule used: expr -> cmpExpr\n"); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 111 "parser.y"
    { fprintf(stdout, "Rule used: expr -> booleanExpr\n");}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 115 "parser.y"
    {  fprintf(stdout, "Rule used: addsubExpr -> expr + expr\n"); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 116 "parser.y"
    {  fprintf(stdout, "Rule used: addsubExpr -> expr - expr\n"); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 120 "parser.y"
    { fprintf(stdout, "Rule used: muldivmodExpr -> expr * expr\n"); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 121 "parser.y"
    { fprintf(stdout, "Rule used: muldivmodExpr -> expr / expr\n"); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 122 "parser.y"
    { fprintf(stdout, "Rule used: muldivmodExpr -> expr % expr\n"); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 126 "parser.y"
    { fprintf(stdout, "Rule used: cmpExpr -> expr > expr\n"); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 127 "parser.y"
    { fprintf(stdout, "Rule used: cmpExpr -> expr >= expr\n"); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 128 "parser.y"
    { fprintf(stdout, "Rule used: cmpExpr -> expr < expr\n"); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 129 "parser.y"
    { fprintf(stdout, "Rule used: cmpExpr -> expr <= expr\n"); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 130 "parser.y"
    { fprintf(stdout, "Rule used: cmpExpr -> expr == expr\n"); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 131 "parser.y"
    { fprintf(stdout, "Rule used: cmpExpr -> expr != expr\n"); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 135 "parser.y"
    { fprintf(stdout, "Rule used: booleanExpr -> expr AND expr\n"); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 136 "parser.y"
    { fprintf(stdout, "Rule used: booleanExpr -> expr OR expr\n"); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 140 "parser.y"
    { fprintf(stdout, "Rule used: term -> (expr) \n");   }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 141 "parser.y"
    { fprintf(stdout, "Rule used: term -> -expr \n"); 	 }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 142 "parser.y"
    { fprintf(stdout, "Rule used: term -> NOT expr \n"); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 143 "parser.y"
    { 
		int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(2) - (2)].expression),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator ++ to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, (yyvsp[(2) - (2)].expression));
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == (yyvsp[(2) - (2)].expression)){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator ++ to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
		
		
		fprintf(stdout, "Rule used: term -> ++lvalue \n"); 
	}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 184 "parser.y"
    { 
		int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(1) - (2)].expression),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator ++ to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, (yyvsp[(1) - (2)].expression));
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == (yyvsp[(1) - (2)].expression)){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator ++ to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
		
	fprintf(stdout, "Rule used: term -> lvalue++ \n"); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 223 "parser.y"
    { 
			int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(2) - (2)].expression),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator -- to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, (yyvsp[(2) - (2)].expression));
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == (yyvsp[(2) - (2)].expression)){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator -- to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
			
			fprintf(stdout, "Rule used: term -> --lvalue \n"); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 262 "parser.y"
    { 
	
	int remainingLibFun = 0, isShadowed = 0, tempScope = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(1) - (2)].expression),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use operator -- to library function\n");	
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, (yyvsp[(1) - (2)].expression));
			if (varExists){
				tempScope = scope;
				while (tempScope >= 0){
					bool userFuncexist = false;
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == (yyvsp[(1) - (2)].expression)){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot use operator -- to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			
			}
		}
		
		
	fprintf(stdout, "Rule used: term -> lvalue-- \n"); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 302 "parser.y"
    { fprintf(stdout, "Rule used: term -> primary \n");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 306 "parser.y"
    { fprintf(stdout, "plus -> ++ \n");  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 309 "parser.y"
    { fprintf(stdout, "minus -> -- \n");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 312 "parser.y"
    { 
		/* We cannot assign in lib function and user function -- throw error*/
		int remainingLibFun = 0, isShadowed = 0;
		bool varExists = false;
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(1) - (3)].expression),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot assign to library function");
			
		}else{
			varExists = lookup_in_all_scope(mySymbolTable, (yyvsp[(1) - (3)].expression));
			if (varExists){
				int tempScope = scope;
				bool userFuncexist = false;
				while (tempScope >= 0){
					SymbolTableEntry_t* firstSymbol = lookup_firstSymbolInScope(mySymbolTable, tempScope);
					while(firstSymbol != NULL){
						if(firstSymbol->name == (yyvsp[(1) - (3)].expression)){
							if (firstSymbol->type == USERFUNC){
								userFuncexist = true;
								fprintf(stdout, "ERROR:: Cannot assign to user function\n");
								break;
							}
						}
						firstSymbol = firstSymbol->nextinScope;
					}
					tempScope--;
				}
			}else{}
		}
		
		fprintf(stdout, "Rule used: assignexpr -> lvalue = expr; \n"); 
	}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 355 "parser.y"
    { fprintf(stdout, "Rule used: primary -> lvalue \n"); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 356 "parser.y"
    { fprintf(stdout, "Rule used: primary -> call \n"); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 357 "parser.y"
    { fprintf(stdout, "Rule used: primary -> objectdef \n"); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 358 "parser.y"
    { fprintf(stdout, "Rule used: primary -> (funcdef) \n"); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 359 "parser.y"
    { fprintf(stdout, "Rule used: primary -> const \n"); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 363 "parser.y"
    { 
		(yyval.expression) = (yyvsp[(1) - (1)].strValue);
		bool varExists;
		SymbolTableEntry_t* newEntry, * existVar;
		bool existsInSameScope = false;
		bool existsDiff = false;
		
		int i = 0;
		/* Look up in symbol table if this variable exists */
		varExists = lookup_in_all_scope(mySymbolTable, (yyvsp[(1) - (1)].strValue));
		int remainingLibFun = 0, isShadowed = 0;
		
		
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(1) - (1)].strValue),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
	
		/*CASE that variable is shadowing lib function */	
		if (isShadowed && scope != 0){
			fprintf (stdout, "ERROR: Variable %s is shadowing library function.\n", (yyvsp[(1) - (1)].strValue));
		}else{
			
			if (varExists){
				
				/* if there is a id in same scope we refer to it */
				existVar = lookup_firstSymbolInScope(mySymbolTable, scope);
				while (existVar!=NULL){
					if (!strcmp(existVar->name, (yyvsp[(1) - (1)].strValue))){
						break;
					}
					existVar = existVar->nextinScope;
				}
					
				if (existVar != NULL){
					existsInSameScope = true;
					if (existVar->type != USERFUNC){
						fprintf(stdout, "ok:: Variable %s refers to an active variable in same scope.\n", (yyvsp[(1) - (1)].strValue));
					}else{
						fprintf(stdout, "ERROR : Cannot change the use of symbol %s from function to variable.\n", (yyvsp[(1) - (1)].strValue));
					}
				}
				
				/* if there is an id in different scope */
				if (existsInSameScope == false){
					/* Find in what scope the variable exists */
					for (i = 0; i < scope; i++){
						existsDiff = lookup_specific_scope (mySymbolTable, (yyvsp[(1) - (1)].strValue), i);
						
						if (existsDiff) break;
					}
					
					if (existsDiff){
						existVar = lookup_firstSymbolInScope(mySymbolTable, i);
						while (existVar!=NULL){
							if (!strcmp(existVar->name, (yyvsp[(1) - (1)].strValue))){
								break;
							}
							existVar = existVar->nextinScope;
						}
						
						if (existVar->scope < scope ){
							if (intoFunction == 1){
								fprintf(stdout, "ERROR : Cannot use symbol %s into this function.\n", (yyvsp[(1) - (1)].strValue));
							}else{
								fprintf(stdout, "ok:: Symbol %s has access to this variable.\n", (yyvsp[(1) - (1)].strValue));
							
							}
						}
						else{
						/* Different scope */
							if (scope == 0){
								newEntry = createSymbolTableEntry ((yyvsp[(1) - (1)].strValue), yylineno, scope, GLOBALVAR);
							}else{
								newEntry = createSymbolTableEntry ((yyvsp[(1) - (1)].strValue), yylineno, scope, LOCALVAR);
							}			
							insertSymbol (mySymbolTable, newEntry);
						
						}
					}
				}
				
			
			}else{
			/* if the variable doesn't exist insert it in symbol table. */
				if (scope == 0){
					newEntry = createSymbolTableEntry ((yyvsp[(1) - (1)].strValue), yylineno, scope, GLOBALVAR);
				}else{
					newEntry = createSymbolTableEntry ((yyvsp[(1) - (1)].strValue), yylineno, scope, LOCALVAR);
				}			
				insertSymbol (mySymbolTable, newEntry);
			
			}
		}
		
		fprintf(stdout, "Rule used: lvalue  -> IDENTIFIER\n"); 
	}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 463 "parser.y"
    { 
		(yyval.expression) = (yyvsp[(2) - (2)].strValue);
		bool varExists = false;
		int remainingLibFun = 0, isShadowed = 0;
		SymbolTableEntry_t* newEntry;
		
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(2) - (2)].strValue),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		/*CASE that variable is shadowing lib function */	
		if (isShadowed && scope != 0){
			fprintf (stdout, "ERROR: Variable %s is shadowing library function\n", (yyvsp[(2) - (2)].strValue));
		}else if(isShadowed && scope == 0){
			fprintf (stdout, "ok:: %s refers to library function\n", (yyvsp[(2) - (2)].strValue));
		}else{
			varExists = lookup_specific_scope (mySymbolTable, (yyvsp[(2) - (2)].strValue), scope);
			if (varExists){
				fprintf(stdout,"ok:: Variable %s refers to a local variable in same scope\n", (yyvsp[(2) - (2)].strValue));
			
			}else{
				if (scope == 0){
					newEntry = createSymbolTableEntry ((yyvsp[(2) - (2)].strValue), yylineno, scope, GLOBALVAR);
				}else{
					newEntry = createSymbolTableEntry ((yyvsp[(2) - (2)].strValue), yylineno, scope, LOCALVAR);
				}			
				insertSymbol (mySymbolTable, newEntry);
			}
			
		}
	
		fprintf(stdout, "Rule used: lvalue  -> LOCAL IDENTIFIER\n"); 
	
	}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 501 "parser.y"
    { 
		bool varExists = false;
		(yyval.expression) = (yyvsp[(2) - (2)].strValue);
		varExists = lookup_specific_scope (mySymbolTable, (yyvsp[(2) - (2)].strValue), 0);
		if (varExists){
			fprintf(stdout, "Variable %s refers to global variable\n",(yyvsp[(2) - (2)].strValue));
		}else{
			fprintf(stdout, "ERROR: Variable %s doesn't exist in global scope\n",(yyvsp[(2) - (2)].strValue));
		}
	
		fprintf(stdout, "Rule used: lvalue  -> DOUBLE_COLON IDENTIFIER\n"); 
	}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 513 "parser.y"
    { fprintf(stdout, "Rule used: lvalue  -> member\n"); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 517 "parser.y"
    { 
		int remainingLibFun = 0, isShadowed = 0;
		
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(1) - (3)].expression),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use library function as lvalue.ID");
		}
		
		fprintf(stdout, "Rule used: member -> lvalue.IDENTIFIER\n"); 
	}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 535 "parser.y"
    { 
		int remainingLibFun = 0, isShadowed = 0;
		
			/* Look up if the IDENTIFIER is shadowing library functions. */
		while(remainingLibFun < MAXLIBFUN){
			if (strcmp((yyvsp[(1) - (4)].expression),libraryFunctions[remainingLibFun]) == 0){
				isShadowed = 1; 
				break;
			}
			remainingLibFun++;
		}
		
		if (isShadowed){
			fprintf(stdout,"ERROR:: Cannot use library function as lvalue[ID]");
		}
		fprintf(stdout, "Rule used: member -> lvalue [expr]\n"); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 551 "parser.y"
    { fprintf(stdout, "Rule used: member -> call.IDENTIFIER\n"); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 552 "parser.y"
    { fprintf(stdout, "Rule used: member -> call [expr] \n"); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 556 "parser.y"
    { fprintf(stdout, "Rule used: call  -> call normcall  \n"); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 557 "parser.y"
    { fprintf(stdout, "Rule used: call  -> lvalue callsuffix \n"); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 558 "parser.y"
    { fprintf(stdout, "Rule used: call  -> '(' funcdef ')' normcall  \n"); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 562 "parser.y"
    { fprintf(stdout, "Rule used: callsuffix  -> normcall \n"); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 563 "parser.y"
    { fprintf(stdout, "Rule used: callsuffix  -> methodcall \n"); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 567 "parser.y"
    { fprintf(stdout, "Rule used: normcall  -> (elist) \n");}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 571 "parser.y"
    { fprintf(stdout, "Rule used: methodcall  -> .. IDENTIFIER(elist) \n");}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 575 "parser.y"
    { fprintf(stdout, "Rule used: elist -> expr listed_eexpr\n"); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 576 "parser.y"
    { fprintf(stdout, "Rule used: elist -> expr\n"); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 581 "parser.y"
    { fprintf(stdout, "Rule used: elist -> ',' expr listed_eexpr \n"); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 582 "parser.y"
    { fprintf(stdout, "Rule used: elist -> ',' expr\n"); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 586 "parser.y"
    { fprintf(stdout, "Rule used: elist -> expr\n"); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 587 "parser.y"
    { fprintf(stdout, "Rule used: elist -> listed_expr\n"); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 591 "parser.y"
    { fprintf(stdout, "Rule used: elist -> elist ',' expr\n"); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 597 "parser.y"
    { fprintf(stdout, "Rule used: objectdef -> [] \n"); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 598 "parser.y"
    { fprintf(stdout, "Rule used: objectdef -> [elist] \n"); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 599 "parser.y"
    { fprintf(stdout, "Rule used: objectdef -> [indexed] \n"); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 603 "parser.y"
    { fprintf(stdout, "Rule used: indexed -> indexedelem\n"); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 604 "parser.y"
    { fprintf(stdout, "Rule used: indexed -> indexed indexed_expr \n"); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 608 "parser.y"
    { fprintf(stdout, "Rule used: indexed -> ,indexedelem  \n"); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 609 "parser.y"
    { fprintf(stdout, "Rule used: indexed -> ,indexedelem  \n"); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 613 "parser.y"
    { fprintf(stdout, "Rule used: indexedelem -> '{' expr ':' expr '}'	 \n"); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 617 "parser.y"
    {++scope; if (maxScope < scope) {maxScope = scope;}}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 617 "parser.y"
    {
		Hide(mySymbolTable, scope); 
		scope--; 
		fprintf(stdout, "Rule used: block -> '{' stmt '}' \n"); 
	}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 622 "parser.y"
    {++scope;}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 622 "parser.y"
    {
		Hide(mySymbolTable, scope); 
		scope--; 
		fprintf(stdout, "Rule used: block -> '{'  '}' \n"); 
	}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 630 "parser.y"
    { fprintf(stdout, "Rule used: morestmt -> stmt \n"); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 631 "parser.y"
    { fprintf(stdout, "Rule used: morestmt -> e \n"); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 635 "parser.y"
    {
		if (scope == 0) {
			++functionScope;
		}else{
			functionScope = scope + 1;
		}		
		intoFunction = true; 
		printf("Func:%s %d", (yyvsp[(2) - (2)].strValue), functionScope);
		}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 643 "parser.y"
    { 
			
			int remainingLibFun = 0, isShadowed = 0;
			/* Look up if the IDENTIFIER is shadowing library functions. */
			while(remainingLibFun < MAXLIBFUN){
				if (strcmp((yyvsp[(2) - (6)].strValue),libraryFunctions[remainingLibFun]) == 0){
					isShadowed = 1; 
					break;
				}
				remainingLibFun++;
			}
			/* If is shadowed print error and continue the syntax analysis. */
			if (isShadowed == 1){
				fprintf(stdout, "Error: User function %s tries to shadow library function.\n", (yyvsp[(2) - (6)].strValue)); 
			}else{
				bool functionExists = lookup_specific_scope (mySymbolTable, (yyvsp[(2) - (6)].strValue), scope);
				if (functionExists){
					fprintf(stdout, "ERROR: User function name exists in scope.\n");
				} else {
					Argument * newArgument;	// If function has arguments insert in argument list
					SymbolTableEntry_t* newEntry = createSymbolTableEntry ((yyvsp[(2) - (6)].strValue), yylineno, scope, USERFUNC);
					
					while (list != NULL){
						bool formalExists = false;
						int remaining = 0, isShadowed = 0;
						/* Look up if the formal is shadowing library functions. */
						while(remaining < MAXLIBFUN){
							if (strcmp(list->name,libraryFunctions[remaining]) == 0){
								isShadowed = 1; 			
								break;
							}
							remaining++;
						}
						/* If formal shadows a libfunc print error*/
						if (isShadowed == 1){
							fprintf(stdout, "ERROR: lif func: %s is shadowed by variable %s:\n", libraryFunctions[remaining], list->name);
						}else{
								/*look if a formal exists in symbol table for this scope */
								formalExists = lookup_specific_scope (mySymbolTable, list->name, functionScope);
								
								if (formalExists == true){
									fprintf(stdout, "ERROR: Formal name exists in scope.\n");
								}else{
									
									newArgument=createArgument(list->name,FORMAL, functionScope, yylineno);
									insertArgument(mySymbolTable, newEntry, newArgument);
								}
						}
						list = list->next;
					}
					
					deleteList(&list);				
					insertSymbol(mySymbolTable, newEntry);
				}
				
	}

}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 700 "parser.y"
    {functionScope--; intoFunction = false; fprintf(stdout, "Rule used: funcdef -> FUNCTION IDENTIFIER (idlist) block \n"); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 701 "parser.y"
    { 
		if (scope == 0) {
			++functionScope;
		}else{
			functionScope = scope + 1;
		}
		intoFunction = true;}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 707 "parser.y"
    {
					char * buf;
					numberOfUnnamed++;
					
					buf = counterUnnamed(numberOfUnnamed);
				
					Argument * newArgument;	// If function has arguments insert in argument list
					SymbolTableEntry_t* newEntry = createSymbolTableEntry (buf, yylineno, scope, USERFUNC);
					
					while (list != NULL){
						bool formalExists = false;
						int remaining = 0, isShadowed = 0;
						/* Look up if the formal is shadowing library functions. */
						while(remaining < MAXLIBFUN){
							if (strcmp(list->name,libraryFunctions[remaining]) == 0){
								isShadowed = 1; 			
								break;
							}
							remaining++;
						}
						/* If formal shadows a libfunc print error*/
						if (isShadowed == 1){
							fprintf(stdout, "ERROR: lif func: %s is shadowed by variable %s:\n", libraryFunctions[remaining], list->name);
						}else{
								/*look if a formal exists in symbol table for this scope */
								formalExists = lookup_specific_scope (mySymbolTable, list->name, functionScope);
								
								if (formalExists == true){
									fprintf(stdout, "ERROR: Formal name exists in scope.\n");
								}else{
									newArgument=createArgument(list->name,FORMAL, functionScope, yylineno);
									insertArgument(mySymbolTable, newEntry, newArgument); /* Insert args in argument list AND in symbol table */
								}
						}
						list = list->next;
					}
					
					deleteList(&list);				
					insertSymbol(mySymbolTable, newEntry);
							
				
	}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 748 "parser.y"
    { functionScope--; intoFunction = false; fprintf(stdout, "Rule used: funcdef -> FUNCTION (idlist) block \n"); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 752 "parser.y"
    { fprintf(stdout, "Rule used: const -> INTEGER \n"); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 753 "parser.y"
    { fprintf(stdout, "Rule used: const -> FLOAT \n"); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 754 "parser.y"
    { fprintf(stdout, "Rule used: const -> STRING_DEF \n"); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 755 "parser.y"
    { fprintf(stdout, "Rule used: const -> NIL\n"); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 756 "parser.y"
    { fprintf(stdout, "Rule used: const -> TRUE\n"); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 757 "parser.y"
    { fprintf(stdout, "Rule used: const -> FALSE\n"); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 761 "parser.y"
    { 
	/* Single arg:: Save argument name, insert it into a list and print the */
		argumentName = malloc(strlen((yyvsp[(1) - (1)].strValue)) * sizeof(char));
		strcpy(argumentName, (yyvsp[(1) - (1)].strValue));
		insert(&list, argumentName);
		//print(list);
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER \n"); 
	}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 769 "parser.y"
    { 
		
		argumentName = malloc(strlen((yyvsp[(1) - (2)].strValue)) * sizeof(char));
		strcpy(argumentName, (yyvsp[(1) - (2)].strValue));
		insert(& list, argumentName);	
		//print(list);
	
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER  helpidlist \n"); 
	}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 778 "parser.y"
    { fprintf(stdout, "Rule used: idlist -> e\n"); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 779 "parser.y"
    { fprintf(stdout, "Rule used: idlist -> comment\n"); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 780 "parser.y"
    { 
	/* Single arg:: Save argument name, insert it into a list and print the */
		argumentName = malloc(strlen((yyvsp[(1) - (2)].strValue)) * sizeof(char));
		strcpy(argumentName, (yyvsp[(1) - (2)].strValue));
		insert(&list, argumentName);
		//print(list);
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER comment \n"); 
	}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 788 "parser.y"
    { 
		
		argumentName = malloc(strlen((yyvsp[(1) - (3)].strValue)) * sizeof(char));
		strcpy(argumentName, (yyvsp[(1) - (3)].strValue));
		insert(& list, argumentName);	
		//print(list);
	
		fprintf(stdout, "Rule used: idlist -> IDENTIFIER  helpidlist comment \n"); 
	}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 800 "parser.y"
    { 
		argumentName = malloc(strlen((yyvsp[(2) - (3)].strValue)) * sizeof(char));
		strcpy(argumentName, (yyvsp[(2) - (3)].strValue));
		insert(& list, argumentName);
		//print(list);		
		fprintf(stdout, "Rule used: helpidlist -> ',' IDENTIFIER helpidlist\n"); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 806 "parser.y"
    { 
		argumentName = malloc(strlen((yyvsp[(2) - (2)].strValue)) * sizeof(char));
		strcpy(argumentName, (yyvsp[(2) - (2)].strValue));
		insert(& list, argumentName);	
		//print(list);
		fprintf(stdout, "Rule used: helpidlist -> ',' IDENTIFIER	\n"); 
		}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 816 "parser.y"
    { fprintf(stdout, "Rule used: ifstmt -> IF \'(\' expr \')\' stmt\n"); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 817 "parser.y"
    { fprintf(stdout, "Rule used: ifstmt -> IF \'(\' expr \')\' stmt ELSE stmt\n"); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 821 "parser.y"
    {++whileLoop;}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 821 "parser.y"
    { whileLoop--; fprintf(stdout, "Rule used: whilestmt -> WHILE '(' expr ')' stmt\n"); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 825 "parser.y"
    {++whileLoop;}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 825 "parser.y"
    { whileLoop--; fprintf(stdout, "Rule used: forstmt -> FOR \n"); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 829 "parser.y"
    { 
	if (intoFunction == false){
		fprintf(stdout, "ERROR: Return not in function. \n");
		exit(EXIT_FAILURE);
	}
	
	fprintf(stdout, "Rule used: returnstmt -> RETURN;\n"); 
	}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 837 "parser.y"
    { 
		if (functionScope == 0){
			fprintf(stdout, "ERROR: Return not in function. Program will be terminated. \n");
			exit(EXIT_FAILURE);
		}else {
			/*bool varExists = false;
			SymbolTableEntry_t* entry = lookup_firstSymbolInScope(mySymbolTable, functionScope);
			varExists = lookup_specific_scope(mySymbolTable,  $2, functionScope);
			if (varExists){
				while(entry != NULL){
					if (!strcmp(entry->name,  $2)){
						break;
					}
					entry = entry->nextinScope;
				}
			}
			if (!varExists || (varExists && entry->scope != 0)){
				fprintf(stdout, "ERROR: Function returns invalid expression. \n");
			}
		*/
		}
		fprintf(stdout, "Rule used: returnstmt -> RETURN expr ;\n"); 
	}
    break;



/* Line 1806 of yacc.c  */
#line 3122 "parser.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 863 "parser.y"


/* Function for recognizing errors*/
int yyerror(char * yaccProvidedMessage){
	fprintf(stderr, "%s: at line %d, before token: %s\nINPUT NOT VALID\n", yaccProvidedMessage, yylineno, yytext);
}

int main (int argc, char ** argv){
	
	int remainingLibFun = 0;
	
	mySymbolTable = createSymbolTable();
	/* Insert in symbol table all library functions with scope 0 and type LIBFUNC*/
	while(remainingLibFun < MAXLIBFUN){
	
		SymbolTableEntry_t* newEntry = createSymbolTableEntry (libraryFunctions[remainingLibFun], 0, 0, LIBFUNC);
		insertSymbol(mySymbolTable, newEntry);
		remainingLibFun++;
		
	}
	
	if (argc > 1){
		if (!(yyin = fopen(argv[1], "r"))){
			fprintf(stderr, "Cannot read file: %s\n", argv[1]);
			return 1;
		}

	}
	else{
		yyin = stdin;
	}
	
	yyparse();
	
	printSymbolTable(mySymbolTable);
	//printSymbolTableperscope(mySymbolTable, maxScope);
	freeSymbolTable(mySymbolTable);
	
	return 0;
}
