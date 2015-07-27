/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         HSQL_STYPE
#define YYLTYPE         HSQL_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         hsql_parse
#define yylex           hsql_lex
#define yyerror         hsql_error
#define yydebug         hsql_debug
#define yynerrs         hsql_nerrs


/* Copy the first part of user declarations.  */
#line 1 "bison_parser.y" /* yacc.c:339  */

/**
 * bison_parser.y
 * defines bison_parser.h
 * outputs bison_parser.c
 * 
 * Grammar File Spec: http://dinosaur.compilertools.net/bison/bison_6.html
 *
 */
/*********************************
 ** Section 1: C Declarations
 *********************************/

#include "sqllib.h"
#include "bison_parser.h"
#include "flex_lexer.h"

#include <stdio.h>
#include <iostream>

using namespace hsql;

int yyerror(YYLTYPE* llocp, SQLStatementList** result, yyscan_t scanner, const char *msg) {
	SQLStatementList* list = new SQLStatementList();
	list->isValid = false;
	list->SetParserMsg(msg);
	list->error_line = llocp->first_line;
	list->error_col = llocp->first_column;

	*result = list;
	return 0;
}




#line 111 "bison_parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "bison_parser.h".  */
#ifndef YY_HSQL_BISON_PARSER_H_INCLUDED
# define YY_HSQL_BISON_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef HSQL_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define HSQL_DEBUG 1
#  else
#   define HSQL_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define HSQL_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined HSQL_DEBUG */
#if HSQL_DEBUG
extern int hsql_debug;
#endif
/* "%code requires" blocks.  */
#line 43 "bison_parser.y" /* yacc.c:355  */

// %code requires block	
#include "parser_typedef.h"

// Auto update column and line number
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    for(int i = 0; yytext[i] != '\0'; i++) { \
    	yylloc->total_column++; \
        if(yytext[i] == '\n') { \
            yylloc->last_line++; \
            yylloc->last_column = 0; \
        } \
        else { \
            yylloc->last_column++; \
        } \
    }

#line 169 "bison_parser.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef HSQL_TOKENTYPE
# define HSQL_TOKENTYPE
  enum hsql_tokentype
  {
    SQL_IDENTIFIER = 258,
    SQL_STRING = 259,
    SQL_SUBSTRING_INDEX = 260,
    SQL_SUBSTRING = 261,
    SQL_FLOATVAL = 262,
    SQL_INTVAL = 263,
    SQL_NOTEQUALS = 264,
    SQL_LESSEQ = 265,
    SQL_GREATEREQ = 266,
    SQL_DEALLOCATE = 267,
    SQL_PARAMETERS = 268,
    SQL_INTERSECT = 269,
    SQL_TEMPORARY = 270,
    SQL_TIMESTAMP = 271,
    SQL_DISTINCT = 272,
    SQL_NVARCHAR = 273,
    SQL_RESTRICT = 274,
    SQL_TRUNCATE = 275,
    SQL_ANALYZE = 276,
    SQL_BETWEEN = 277,
    SQL_CASCADE = 278,
    SQL_COLUMNS = 279,
    SQL_CONTROL = 280,
    SQL_DEFAULT = 281,
    SQL_EXECUTE = 282,
    SQL_EXPLAIN = 283,
    SQL_HISTORY = 284,
    SQL_INTEGER = 285,
    SQL_NATURAL = 286,
    SQL_PREPARE = 287,
    SQL_PRIMARY = 288,
    SQL_SCHEMAS = 289,
    SQL_SPATIAL = 290,
    SQL_VIRTUAL = 291,
    SQL_BEFORE = 292,
    SQL_COLUMN = 293,
    SQL_CREATE = 294,
    SQL_DELETE = 295,
    SQL_DIRECT = 296,
    SQL_DOUBLE = 297,
    SQL_ESCAPE = 298,
    SQL_EXCEPT = 299,
    SQL_EXISTS = 300,
    SQL_GLOBAL = 301,
    SQL_HAVING = 302,
    SQL_IMPORT = 303,
    SQL_INSERT = 304,
    SQL_ISNULL = 305,
    SQL_OFFSET = 306,
    SQL_RENAME = 307,
    SQL_SCHEMA = 308,
    SQL_SELECT = 309,
    SQL_SORTED = 310,
    SQL_TABLES = 311,
    SQL_UNIQUE = 312,
    SQL_UNLOAD = 313,
    SQL_UPDATE = 314,
    SQL_VALUES = 315,
    SQL_AFTER = 316,
    SQL_ALTER = 317,
    SQL_CROSS = 318,
    SQL_DELTA = 319,
    SQL_GROUP = 320,
    SQL_INDEX = 321,
    SQL_INNER = 322,
    SQL_LIMIT = 323,
    SQL_LOCAL = 324,
    SQL_MERGE = 325,
    SQL_MINUS = 326,
    SQL_ORDER = 327,
    SQL_OUTER = 328,
    SQL_RIGHT = 329,
    SQL_TABLE = 330,
    SQL_UNION = 331,
    SQL_USING = 332,
    SQL_WHERE = 333,
    SQL_CALL = 334,
    SQL_DATE = 335,
    SQL_DESC = 336,
    SQL_DROP = 337,
    SQL_FILE = 338,
    SQL_FROM = 339,
    SQL_FULL = 340,
    SQL_HASH = 341,
    SQL_HINT = 342,
    SQL_INTO = 343,
    SQL_JOIN = 344,
    SQL_LEFT = 345,
    SQL_LIKE = 346,
    SQL_LOAD = 347,
    SQL_NULL = 348,
    SQL_PART = 349,
    SQL_PLAN = 350,
    SQL_SHOW = 351,
    SQL_TEXT = 352,
    SQL_TIME = 353,
    SQL_VIEW = 354,
    SQL_WITH = 355,
    SQL_ADD = 356,
    SQL_ALL = 357,
    SQL_AND = 358,
    SQL_ASC = 359,
    SQL_CSV = 360,
    SQL_FOR = 361,
    SQL_INT = 362,
    SQL_KEY = 363,
    SQL_NOT = 364,
    SQL_OFF = 365,
    SQL_SET = 366,
    SQL_TBL = 367,
    SQL_TOP = 368,
    SQL_AS = 369,
    SQL_BY = 370,
    SQL_IF = 371,
    SQL_IN = 372,
    SQL_IS = 373,
    SQL_OF = 374,
    SQL_ON = 375,
    SQL_OR = 376,
    SQL_TO = 377,
    SQL_EQUALS = 378,
    SQL_LESS = 379,
    SQL_GREATER = 380,
    SQL_NOTNULL = 381,
    SQL_UMINUS = 382
  };
#endif

/* Value type.  */
#if ! defined HSQL_STYPE && ! defined HSQL_STYPE_IS_DECLARED
typedef union HSQL_STYPE HSQL_STYPE;
union HSQL_STYPE
{
#line 99 "bison_parser.y" /* yacc.c:355  */

	double fval;
	int64_t ival;
	char* sval;
	uint uval;
	bool bval;

	hsql::SQLStatement* statement;
	hsql::SelectStatement* 	select_stmt;
	hsql::ImportStatement* 	import_stmt;
	hsql::CreateStatement* 	create_stmt;
	hsql::InsertStatement* 	insert_stmt;
	hsql::DeleteStatement* 	delete_stmt;
	hsql::UpdateStatement* 	update_stmt;
	hsql::DropStatement*   	drop_stmt;
	hsql::PrepareStatement* prep_stmt;
	hsql::ExecuteStatement* exec_stmt;

	hsql::TableRef* table;
	hsql::TableName* tbl_name;
	hsql::Expr* expr;
	hsql::OrderDescription* order;
	hsql::OrderType order_type;
	hsql::LimitDescription* limit;
	hsql::ColumnDefinition* column_t;
	hsql::GroupByDescription* group_t;
	hsql::UpdateClause* update_t;

	hsql::SQLStatementList* stmt_list;

	std::vector<std::string>* str_vec;
	std::vector<hsql::TableRef*>* table_vec;
	std::vector<hsql::ColumnDefinition*>* column_vec;
	std::vector<hsql::UpdateClause*>* update_vec;
	std::vector<hsql::Expr*>* expr_vec;

#line 346 "bison_parser.cpp" /* yacc.c:355  */
};
# define HSQL_STYPE_IS_TRIVIAL 1
# define HSQL_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined HSQL_LTYPE && ! defined HSQL_LTYPE_IS_DECLARED
typedef struct HSQL_LTYPE HSQL_LTYPE;
struct HSQL_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define HSQL_LTYPE_IS_DECLARED 1
# define HSQL_LTYPE_IS_TRIVIAL 1
#endif



int hsql_parse (hsql::SQLStatementList** result, yyscan_t scanner);

#endif /* !YY_HSQL_BISON_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 374 "bison_parser.cpp" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL \
             && defined HSQL_STYPE_IS_TRIVIAL && HSQL_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  46
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   848

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  149
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  69
/* YYNRULES -- Number of rules.  */
#define YYNRULES  160
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  305

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   382

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   134,   147,     2,
     139,   140,   132,   130,   146,   131,   141,   133,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   143,   142,
     125,   123,   126,   148,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   137,     2,   138,   135,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   144,     2,   145,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   124,   127,
     128,   129,   136
};

#if HSQL_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   232,   232,   239,   240,   244,   249,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   270,   275,   283,   287,
     299,   307,   311,   321,   327,   336,   337,   341,   342,   346,
     347,   351,   354,   355,   356,   361,   362,   363,   364,   374,
     378,   390,   398,   410,   416,   426,   427,   437,   446,   447,
     451,   463,   464,   468,   469,   473,   478,   490,   491,   492,
     496,   507,   508,   512,   517,   524,   525,   529,   534,   538,
     539,   542,   543,   547,   548,   549,   554,   555,   556,   563,
     564,   568,   569,   573,   580,   581,   582,   583,   584,   585,
     589,   590,   591,   595,   596,   597,   598,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   617,
     622,   623,   624,   625,   626,   627,   631,   632,   636,   637,
     638,   642,   643,   644,   648,   653,   654,   658,   662,   667,
     678,   679,   689,   690,   696,   701,   702,   707,   717,   725,
     726,   731,   732,   736,   737,   746,   756,   766,   774,   785,
     786,   787,   788,   789,   795,   801,   805,   814,   815,   820,
     821
};
#endif

#if HSQL_DEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "STRING",
  "SUBSTRING_INDEX", "SUBSTRING", "FLOATVAL", "INTVAL", "NOTEQUALS",
  "LESSEQ", "GREATEREQ", "DEALLOCATE", "PARAMETERS", "INTERSECT",
  "TEMPORARY", "TIMESTAMP", "DISTINCT", "NVARCHAR", "RESTRICT", "TRUNCATE",
  "ANALYZE", "BETWEEN", "CASCADE", "COLUMNS", "CONTROL", "DEFAULT",
  "EXECUTE", "EXPLAIN", "HISTORY", "INTEGER", "NATURAL", "PREPARE",
  "PRIMARY", "SCHEMAS", "SPATIAL", "VIRTUAL", "BEFORE", "COLUMN", "CREATE",
  "DELETE", "DIRECT", "DOUBLE", "ESCAPE", "EXCEPT", "EXISTS", "GLOBAL",
  "HAVING", "IMPORT", "INSERT", "ISNULL", "OFFSET", "RENAME", "SCHEMA",
  "SELECT", "SORTED", "TABLES", "UNIQUE", "UNLOAD", "UPDATE", "VALUES",
  "AFTER", "ALTER", "CROSS", "DELTA", "GROUP", "INDEX", "INNER", "LIMIT",
  "LOCAL", "MERGE", "MINUS", "ORDER", "OUTER", "RIGHT", "TABLE", "UNION",
  "USING", "WHERE", "CALL", "DATE", "DESC", "DROP", "FILE", "FROM", "FULL",
  "HASH", "HINT", "INTO", "JOIN", "LEFT", "LIKE", "LOAD", "NULL", "PART",
  "PLAN", "SHOW", "TEXT", "TIME", "VIEW", "WITH", "ADD", "ALL", "AND",
  "ASC", "CSV", "FOR", "INT", "KEY", "NOT", "OFF", "SET", "TBL", "TOP",
  "AS", "BY", "IF", "IN", "IS", "OF", "ON", "OR", "TO", "'='", "EQUALS",
  "'<'", "'>'", "LESS", "GREATER", "NOTNULL", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'^'", "UMINUS", "'['", "']'", "'('", "')'", "'.'", "';'", "':'",
  "'{'", "'}'", "','", "'&'", "'?'", "$accept", "input", "statement_list",
  "statement", "preparable_statement", "prepare_statement",
  "execute_statement", "import_statement", "import_file_type", "file_path",
  "create_statement", "opt_not_exists", "column_def_commalist",
  "column_commalist", "column_def", "column_type", "drop_statement",
  "delete_statement", "truncate_statement", "insert_statement",
  "opt_column_list", "update_statement", "update_clause_commalist",
  "update_clause", "select_statement", "select_with_paren",
  "select_no_paren", "set_operator", "select_clause", "opt_distinct",
  "select_list", "from_clause", "opt_where", "opt_group", "opt_having",
  "opt_order", "opt_order_type", "opt_limit", "expr_list", "literal_list",
  "expr_alias", "expr", "scalar_expr", "unary_expr", "binary_expr",
  "ternary_expr", "comp_expr", "function_expr", "column_name", "literal",
  "string_literal", "num_literal", "int_literal", "star_expr",
  "placeholder_expr", "table_ref", "table_ref_atomic",
  "table_ref_commalist", "table_ref_name", "table_ref_name_no_alias",
  "table_name", "alias", "opt_alias", "join_clause", "opt_join_type",
  "join_table", "join_condition", "opt_semicolon", "ident_commalist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,    61,   378,    60,    62,   379,   380,   381,
      43,    45,    42,    47,    37,    94,   382,    91,    93,    40,
      41,    46,    59,    58,   123,   125,    44,    38,    63
};
# endif

#define YYPACT_NINF -225

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-225)))

#define YYTABLE_NINF -156

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-156)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     693,    -3,    28,    85,    89,    27,    29,    31,    34,   113,
      28,    62,   -31,   106,     6,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,    18,
     147,    15,  -225,    25,  -110,    41,    28,    54,    28,  -225,
       7,    77,  -225,    28,    51,    53,  -225,   693,  -225,  -225,
    -225,    80,  -225,   142,   129,  -225,   195,    12,   709,   693,
      90,    28,   124,  -225,   121,    66,    65,  -225,    68,    69,
    -225,  -225,     7,    75,     7,  -225,     7,  -225,   131,    70,
    -225,    40,  -225,  -225,  -225,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,  -225,  -225,   219,  -225,  -225,  -225,  -225,
       7,   151,   216,  -225,  -225,  -102,  -225,  -225,     6,   180,
     -58,     7,  -225,   222,   225,    20,   226,     7,     7,   559,
       7,    47,   167,     2,   124,     7,  -225,     7,     7,     7,
       7,     7,   140,   230,    -9,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     7,  -225,  -225,   114,   -53,
    -225,   109,   129,   185,  -225,    12,    93,  -225,   136,    21,
     489,   161,  -225,  -225,   -61,   111,  -225,   110,   200,   236,
     269,  -225,   -31,  -225,   107,     9,    14,    36,    22,   189,
    -225,   584,   134,   134,   584,   559,     7,  -225,  -225,   162,
     526,   584,   134,   134,    47,    47,   -94,   -94,   -94,   115,
     489,    12,   219,  -225,  -225,  -225,  -225,  -225,   216,  -225,
    -225,   174,    10,   153,   132,   133,   -49,  -225,    28,  -225,
     260,    12,   270,     7,     7,     7,   135,     2,  -225,  -225,
    -225,  -225,  -225,   188,   193,   159,  -225,   584,  -225,  -225,
    -225,  -225,   222,  -225,  -225,  -225,  -225,  -225,   144,   281,
     281,  -225,    21,  -225,  -225,   -11,  -225,   302,   354,   387,
      14,  -225,   141,     3,     3,     7,  -225,   281,  -225,    -6,
      43,  -225,  -225,     7,  -225,     7,    95,     2,   -31,  -225,
     166,   169,   -25,    46,  -225,   291,  -225,   420,   456,  -225,
     155,     7,     7,     7,  -225,  -225,  -225,  -225,  -225,    14,
     489,  -225,  -225,   489,  -225
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
       0,     0,     0,     0,   158,     3,     6,     5,    15,     8,
       9,    14,    11,    12,    10,    13,     7,    51,    52,    72,
       0,   139,    42,    18,     0,    26,     0,     0,     0,    61,
       0,     0,   138,     0,     0,     0,     1,   157,     2,    58,
      59,     0,    57,     0,    78,    40,     0,     0,     0,     0,
       0,     0,    66,    21,     0,    46,   118,   124,     0,     0,
     125,   127,     0,     0,     0,   128,     0,   129,     0,    63,
      79,   144,    85,    86,    87,    88,    97,    89,    90,    92,
     121,   122,   126,    91,   123,     0,    39,    54,    53,     4,
       0,    72,     0,    55,   140,     0,    81,    16,   158,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,    96,
       0,    93,     0,     0,    66,     0,   142,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   143,    83,     0,    66,
      48,    75,    78,    76,    19,     0,     0,    25,     0,     0,
      65,     0,    22,   159,     0,     0,    44,   119,     0,     0,
       0,    84,     0,    64,   130,   132,   144,   134,   153,    68,
      80,   111,   114,   115,   107,   105,     0,   141,    94,     0,
     106,   110,   112,   113,    99,    98,   101,   100,   102,   103,
     104,     0,     0,    47,    74,    73,    71,    56,     0,    82,
      17,     0,     0,     0,     0,     0,     0,    27,     0,    45,
       0,     0,     0,     0,     0,     0,     0,     0,   137,   149,
     150,   152,   151,     0,     0,     0,    60,   108,    95,    50,
      49,    77,     0,    36,    37,    38,    35,    31,     0,     0,
       0,    24,     0,    20,   160,     0,   120,     0,     0,     0,
       0,   135,   131,     0,     0,     0,    23,     0,    29,     0,
       0,    28,    43,     0,   116,     0,   133,     0,     0,   155,
     145,   147,    70,     0,    33,     0,    34,     0,     0,   136,
       0,     0,     0,     0,    67,    32,    30,   117,   109,     0,
     156,   146,   148,    69,   154
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -225,  -225,   237,   257,   247,  -225,  -225,  -225,  -225,    64,
    -225,  -225,  -225,  -220,    56,  -225,  -225,  -225,  -225,  -225,
    -225,  -225,  -225,   108,  -154,   303,    -4,  -225,   263,  -225,
    -225,  -225,   -63,  -225,  -225,   218,  -225,   165,    55,   101,
     199,   -72,  -225,  -225,  -225,  -225,  -225,  -225,  -225,   -54,
    -106,  -225,   -81,  -225,  -225,  -225,  -200,  -225,  -159,  -225,
      -1,  -224,   152,  -225,   158,  -131,    37,   229,  -225
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    16,    17,    18,    19,    64,   161,
      20,    61,   216,   269,   217,   247,    21,    22,    23,    24,
     115,    25,   149,   150,    26,    27,    28,    53,    29,    40,
      78,   124,   112,   236,   294,    54,   206,   103,    79,   105,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,   173,   174,   262,   175,    41,
     176,   146,   147,   177,   233,   178,   301,    48,   164
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     119,    32,   121,   106,   122,    31,    31,   162,    45,    42,
      66,    67,    68,    69,    70,    71,    67,   126,   226,    70,
      71,   153,   293,     9,   212,   111,   158,   261,   151,    30,
     270,    31,    49,    58,    59,    62,   276,    65,   154,   160,
     243,   144,    96,   126,   155,   168,   169,   283,   170,   127,
     128,   129,   244,   145,   213,   181,   182,   183,   184,   185,
     110,   179,    50,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,     9,   304,  -155,   289,   214,   219,
     165,   159,  -155,  -155,   188,   220,   203,   215,    33,   229,
      51,   251,    34,   202,    52,   230,   231,   252,  -155,  -155,
     189,   209,    35,   229,   279,   279,    46,   245,    12,   230,
     231,   166,   232,    36,   237,    37,    72,   246,   127,   128,
     129,   125,    38,    73,   290,  -153,   232,   241,   133,   272,
      39,   130,   280,   281,   284,   155,   162,    43,    74,    75,
     285,   172,   278,   131,  -156,  -156,    76,   239,    47,   132,
      55,   257,   258,   259,   133,    77,    56,    60,   134,    63,
      77,   135,  -154,   136,    57,   137,   138,   106,  -154,  -154,
     139,   140,   141,   142,   143,   144,   127,   128,   129,   141,
     142,   143,   144,   286,  -154,  -154,   295,   145,    95,   285,
     204,    97,   285,    98,   145,   100,     9,   102,   104,   109,
     130,   287,   111,   288,   113,   114,   116,   117,   118,   127,
     128,   129,   131,   205,   120,   123,   125,   253,   132,   300,
     300,   303,   148,    51,    71,   157,    67,   134,   163,   167,
     135,   186,   136,   187,   137,   138,   208,   201,   210,   139,
     140,   141,   142,   143,   144,   127,   128,   129,   211,   218,
     221,   222,   134,   227,   235,   238,   145,   242,   130,  -156,
    -156,   248,   145,   254,   139,   140,   141,   142,   143,   144,
     131,   249,   250,   256,   265,   260,   132,   263,   127,   128,
     129,   145,   264,   267,   268,   134,   291,   277,   135,   292,
     136,   130,   137,   138,   296,   299,   108,   139,   140,   141,
     142,   143,   144,   131,    99,   107,   266,   171,   271,   132,
     240,   127,   128,   129,   145,    44,   101,   207,   134,   152,
     282,   135,   255,   136,   180,   137,   138,   130,   228,   302,
     139,   140,   141,   142,   143,   144,   234,   156,     0,   131,
       0,     0,     0,     0,     0,   132,   223,   145,     0,     0,
       0,     0,     0,     0,   134,     0,     0,   135,     0,   136,
     130,   137,   138,   127,   128,   129,   139,   140,   141,   142,
     143,   144,   131,     0,     0,     0,     0,     0,   132,     0,
       0,     0,   224,   145,     0,     0,     0,   134,     0,     0,
     135,     0,   136,   130,   137,   138,   127,   128,   129,   139,
     140,   141,   142,   143,   144,   131,     0,     0,     0,     0,
       0,   132,     0,     0,     0,   225,   145,     0,     0,     0,
     134,     0,     0,   135,     0,   136,     0,   137,   138,   127,
     128,   129,   139,   140,   141,   142,   143,   144,     0,     0,
       0,     0,     0,     0,     0,   130,     0,     0,   273,   145,
       0,     0,     0,     0,     0,     0,     0,   131,     0,     0,
       0,     0,     0,   132,     0,   127,   128,   129,     0,     0,
       0,     0,   134,     0,     0,   135,     0,   136,   130,   137,
     138,     0,     0,     0,   139,   140,   141,   142,   143,   144,
     131,     0,     0,     0,   274,     0,   132,     0,   127,   128,
     129,   145,     0,     0,     0,   134,     0,     0,   135,     0,
     136,   130,   137,   138,     0,     0,     0,   139,   140,   141,
     142,   143,   144,   131,     0,     0,     0,     0,     0,   132,
       0,     0,     0,   275,   145,   127,   128,   129,   134,     0,
       0,   135,     0,   136,     0,   137,   138,   130,     0,     0,
     139,   140,   141,   142,   143,   144,     0,     0,     0,   131,
     297,     0,     0,     0,     0,   132,     0,   145,   127,   128,
     129,     0,     0,     0,   134,     0,     0,   135,     0,   136,
     130,   137,   138,     0,     0,     0,   139,   140,   141,   142,
     143,   144,   131,   127,   128,   129,   298,     0,   132,     0,
       0,     0,     0,   145,     0,     0,     0,   134,     0,     0,
     135,     0,   136,     0,   137,   138,     0,   130,     0,   139,
     140,   141,   142,   143,   144,     0,     0,     0,     0,   131,
       0,     0,     0,     0,     0,   132,   145,     0,     0,     0,
       0,     0,     0,     0,   134,     0,     0,     0,     0,   136,
     130,   137,   138,     0,     0,     0,   139,   140,   141,   142,
     143,   144,     0,     0,     0,     0,     0,     0,   132,     0,
       0,     0,     0,   145,     0,   130,     0,   134,     0,     0,
       0,     0,   136,     0,   137,   138,     0,     0,     0,   139,
     140,   141,   142,   143,   144,     0,     0,     0,     0,     0,
       0,     0,   134,     0,     0,     1,   145,   136,     0,   137,
     138,     0,     0,     2,   139,   140,   141,   142,   143,   144,
       3,     1,     0,     0,     0,     4,     0,     0,     0,     2,
       0,   145,     5,     6,     0,     0,     3,     0,     0,     0,
       0,     7,     8,     0,     0,     0,     0,     9,     5,     6,
       0,     0,    10,     0,     0,     0,     0,     7,     8,     0,
       0,     0,     0,     9,     0,     0,     0,     0,    10,     0,
       0,     0,     0,     0,     0,    11,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    11,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    12,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    12
};

static const yytype_int16 yycheck[] =
{
      72,     2,    74,    57,    76,     3,     3,   113,    12,    10,
       3,     4,     5,     6,     7,     8,     4,     3,   172,     7,
       8,   102,    47,    54,     3,    78,    84,   227,   100,    32,
     250,     3,    14,   143,   144,    36,   260,    38,   140,   111,
      30,   135,    43,     3,   146,   117,   118,   267,   120,     9,
      10,    11,    42,   147,    33,   127,   128,   129,   130,   131,
      61,   124,    44,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,    54,   299,    67,   277,    57,   140,
      60,   139,    73,    74,    93,   146,   149,    66,     3,    67,
      72,   140,     3,   146,    76,    73,    74,   146,    89,    90,
     109,   155,    75,    67,   263,   264,     0,    97,   139,    73,
      74,   115,    90,    84,   186,    84,   109,   107,     9,    10,
      11,   146,    88,   116,   278,    89,    90,   208,   114,   140,
      17,    91,   263,   264,   140,   146,   242,    75,   131,   132,
     146,   139,   139,   103,    10,    11,   139,   201,   142,   109,
       3,   223,   224,   225,   114,   148,   141,   116,   118,   105,
     148,   121,    67,   123,   139,   125,   126,   221,    73,    74,
     130,   131,   132,   133,   134,   135,     9,    10,    11,   132,
     133,   134,   135,   140,    89,    90,   140,   147,   111,   146,
      81,   140,   146,   140,   147,   115,    54,    68,     3,   109,
      91,   273,    78,   275,    83,   139,   141,   139,   139,     9,
      10,    11,   103,   104,   139,    84,   146,   218,   109,   291,
     292,   293,     3,    72,     8,    45,     4,   118,     3,     3,
     121,    91,   123,     3,   125,   126,    51,   123,   145,   130,
     131,   132,   133,   134,   135,     9,    10,    11,   112,    88,
     139,   141,   118,   146,    65,    93,   147,    83,    91,   125,
     126,   108,   147,     3,   130,   131,   132,   133,   134,   135,
     103,   139,   139,     3,   115,   140,   109,    89,     9,    10,
      11,   147,    89,   139,     3,   118,   120,   146,   121,   120,
     123,    91,   125,   126,     3,   140,    59,   130,   131,   132,
     133,   134,   135,   103,    47,    58,   242,   140,   252,   109,
     202,     9,    10,    11,   147,    12,    53,   152,   118,   101,
     265,   121,   221,   123,   125,   125,   126,    91,   176,   292,
     130,   131,   132,   133,   134,   135,   178,   108,    -1,   103,
      -1,    -1,    -1,    -1,    -1,   109,   146,   147,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,   123,
      91,   125,   126,     9,    10,    11,   130,   131,   132,   133,
     134,   135,   103,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,   146,   147,    -1,    -1,    -1,   118,    -1,    -1,
     121,    -1,   123,    91,   125,   126,     9,    10,    11,   130,
     131,   132,   133,   134,   135,   103,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    -1,   146,   147,    -1,    -1,    -1,
     118,    -1,    -1,   121,    -1,   123,    -1,   125,   126,     9,
      10,    11,   130,   131,   132,   133,   134,   135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,   146,   147,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    -1,   109,    -1,     9,    10,    11,    -1,    -1,
      -1,    -1,   118,    -1,    -1,   121,    -1,   123,    91,   125,
     126,    -1,    -1,    -1,   130,   131,   132,   133,   134,   135,
     103,    -1,    -1,    -1,   140,    -1,   109,    -1,     9,    10,
      11,   147,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,
     123,    91,   125,   126,    -1,    -1,    -1,   130,   131,   132,
     133,   134,   135,   103,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,   146,   147,     9,    10,    11,   118,    -1,
      -1,   121,    -1,   123,    -1,   125,   126,    91,    -1,    -1,
     130,   131,   132,   133,   134,   135,    -1,    -1,    -1,   103,
     140,    -1,    -1,    -1,    -1,   109,    -1,   147,     9,    10,
      11,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,   123,
      91,   125,   126,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,   103,     9,    10,    11,   140,    -1,   109,    -1,
      -1,    -1,    -1,   147,    -1,    -1,    -1,   118,    -1,    -1,
     121,    -1,   123,    -1,   125,   126,    -1,    91,    -1,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,   109,   147,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,   123,
      91,   125,   126,    -1,    -1,    -1,   130,   131,   132,   133,
     134,   135,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,   147,    -1,    91,    -1,   118,    -1,    -1,
      -1,    -1,   123,    -1,   125,   126,    -1,    -1,    -1,   130,
     131,   132,   133,   134,   135,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    12,   147,   123,    -1,   125,
     126,    -1,    -1,    20,   130,   131,   132,   133,   134,   135,
      27,    12,    -1,    -1,    -1,    32,    -1,    -1,    -1,    20,
      -1,   147,    39,    40,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    48,    49,    -1,    -1,    -1,    -1,    54,    39,    40,
      -1,    -1,    59,    -1,    -1,    -1,    -1,    48,    49,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    12,    20,    27,    32,    39,    40,    48,    49,    54,
      59,    82,   139,   150,   151,   152,   153,   154,   155,   156,
     159,   165,   166,   167,   168,   170,   173,   174,   175,   177,
      32,     3,   209,     3,     3,    75,    84,    84,    88,    17,
     178,   208,   209,    75,   174,   175,     0,   142,   216,    14,
      44,    72,    76,   176,   184,     3,   141,   139,   143,   144,
     116,   160,   209,   105,   157,   209,     3,     4,     5,     6,
       7,     8,   109,   116,   131,   132,   139,   148,   179,   187,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   111,   209,   140,   140,   152,
     115,   177,    68,   186,     3,   188,   198,   153,   151,   109,
     209,    78,   181,    83,   139,   169,   141,   139,   139,   190,
     139,   190,   190,    84,   180,   146,     3,     9,    10,    11,
      91,   103,   109,   114,   118,   121,   123,   125,   126,   130,
     131,   132,   133,   134,   135,   147,   210,   211,     3,   171,
     172,   190,   184,   201,   140,   146,   216,    45,    84,   139,
     190,   158,   199,     3,   217,    60,   175,     3,   190,   190,
     190,   140,   139,   204,   205,   207,   209,   212,   214,   181,
     189,   190,   190,   190,   190,   190,    91,     3,    93,   109,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   123,   146,   181,    81,   104,   185,   186,    51,   198,
     145,   112,     3,    33,    57,    66,   161,   163,    88,   140,
     146,   139,   141,   146,   146,   146,   173,   146,   211,    67,
      73,    74,    90,   213,   213,    65,   182,   190,    93,   198,
     172,   201,    83,    30,    42,    97,   107,   164,   108,   139,
     139,   140,   146,   209,     3,   188,     3,   190,   190,   190,
     140,   205,   206,    89,    89,   115,   158,   139,     3,   162,
     162,   163,   140,   146,   140,   146,   210,   146,   139,   207,
     214,   214,   187,   162,   140,   146,   140,   190,   190,   205,
     173,   120,   120,    47,   183,   140,     3,   140,   140,   140,
     190,   215,   215,   190,   210
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   149,   150,   151,   151,   152,   152,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   154,   154,   155,   155,
     156,   157,   158,   159,   159,   160,   160,   161,   161,   162,
     162,   163,   163,   163,   163,   164,   164,   164,   164,   165,
     165,   166,   167,   168,   168,   169,   169,   170,   171,   171,
     172,   173,   173,   174,   174,   175,   175,   176,   176,   176,
     177,   178,   178,   179,   180,   181,   181,   182,   182,   183,
     183,   184,   184,   185,   185,   185,   186,   186,   186,   187,
     187,   188,   188,   189,   190,   190,   190,   190,   190,   190,
     191,   191,   191,   192,   192,   192,   192,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   194,
     195,   195,   195,   195,   195,   195,   196,   196,   197,   197,
     197,   198,   198,   198,   199,   200,   200,   201,   202,   203,
     204,   204,   205,   205,   205,   206,   206,   207,   208,   209,
     209,   210,   210,   211,   211,   212,   212,   212,   212,   213,
     213,   213,   213,   213,   214,   214,   215,   216,   216,   217,
     217
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     6,     2,     5,
       7,     1,     1,     8,     7,     3,     0,     1,     3,     1,
       3,     2,     5,     4,     4,     1,     1,     1,     1,     3,
       3,     4,     2,     8,     5,     3,     0,     5,     1,     3,
       3,     1,     1,     3,     3,     3,     5,     1,     1,     1,
       6,     1,     0,     1,     2,     2,     0,     4,     0,     2,
       0,     4,     0,     1,     1,     0,     2,     4,     0,     1,
       3,     1,     3,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     3,     4,     2,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     8,
       3,     3,     3,     3,     3,     3,     6,     8,     1,     3,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     4,     1,     1,     3,     2,     1,     1,
       3,     2,     1,     1,     0,     4,     6,     4,     6,     1,
       1,     1,     1,     0,     4,     1,     1,     1,     0,     1,
       3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (&yylloc, result, scanner, YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if HSQL_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, hsql::SQLStatementList** result, yyscan_t scanner)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, hsql::SQLStatementList** result, yyscan_t scanner)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, result, scanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, hsql::SQLStatementList** result, yyscan_t scanner)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !HSQL_DEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !HSQL_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
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

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, hsql::SQLStatementList** result, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (result);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (hsql::SQLStatementList** result, yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined HSQL_LTYPE_IS_TRIVIAL && HSQL_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 77 "bison_parser.y" /* yacc.c:1429  */
{
	// Initialize
	yylloc.first_column = 0;
	yylloc.last_column = 0;
	yylloc.first_line = 0;
	yylloc.last_line = 0;
	yylloc.total_column = 0;
	yylloc.placeholder_id = 0;
}

#line 1749 "bison_parser.cpp" /* yacc.c:1429  */
  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 232 "bison_parser.y" /* yacc.c:1646  */
    {
			*result = (yyvsp[-1].stmt_list);
		}
#line 1940 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 239 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.stmt_list) = new SQLStatementList((yyvsp[0].statement)); }
#line 1946 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 240 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].stmt_list)->addStatement((yyvsp[0].statement)); (yyval.stmt_list) = (yyvsp[-2].stmt_list); }
#line 1952 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 244 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyvsp[0].prep_stmt)->setPlaceholders(yyloc.placeholder_list);
			yyloc.placeholder_list.clear();
			(yyval.statement) = (yyvsp[0].prep_stmt);
		}
#line 1962 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 254 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].select_stmt); }
#line 1968 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 255 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].import_stmt); }
#line 1974 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 256 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].create_stmt); }
#line 1980 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 257 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].insert_stmt); }
#line 1986 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 258 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 1992 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 259 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].delete_stmt); }
#line 1998 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 260 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].update_stmt); }
#line 2004 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 261 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].drop_stmt); }
#line 2010 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 262 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.statement) = (yyvsp[0].exec_stmt); }
#line 2016 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 270 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.prep_stmt) = new PrepareStatement();
			(yyval.prep_stmt)->SetName((yyvsp[-2].sval));
			(yyval.prep_stmt)->query = new SQLStatementList((yyvsp[0].statement));
		}
#line 2026 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 275 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.prep_stmt) = new PrepareStatement();
			(yyval.prep_stmt)->SetName((yyvsp[-4].sval));
			(yyval.prep_stmt)->query = (yyvsp[-2].stmt_list);
		}
#line 2036 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 283 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.exec_stmt) = new ExecuteStatement();
			(yyval.exec_stmt)->SetName((yyvsp[0].sval));
		}
#line 2045 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 287 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.exec_stmt) = new ExecuteStatement();
			(yyval.exec_stmt)->SetName((yyvsp[-3].sval));
			(yyval.exec_stmt)->parameters = (yyvsp[-1].expr_vec);
		}
#line 2055 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 299 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.import_stmt) = new ImportStatement((ImportStatement::ImportType) (yyvsp[-4].uval));
			(yyval.import_stmt)->SetFilePath((yyvsp[-2].sval));
			(yyval.import_stmt)->table_name = (yyvsp[0].tbl_name);
		}
#line 2065 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 307 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = ImportStatement::kImportCSV; }
#line 2071 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 311 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.sval) = strdup((yyvsp[0].expr)->name.c_str()); }
#line 2077 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 321 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.create_stmt) = new CreateStatement(CreateStatement::kTableFromTbl);
			(yyval.create_stmt)->if_not_exists = (yyvsp[-5].bval);
			(yyval.create_stmt)->table_name = (yyvsp[-4].tbl_name);
			(yyval.create_stmt)->SetFilePath((yyvsp[0].sval));
		}
#line 2088 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 327 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.create_stmt) = new CreateStatement(CreateStatement::kTable);
			(yyval.create_stmt)->if_not_exists = (yyvsp[-4].bval);
			(yyval.create_stmt)->table_name = (yyvsp[-3].tbl_name);
			(yyval.create_stmt)->columns = (yyvsp[-1].column_vec);
		}
#line 2099 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 336 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 2105 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 337 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 2111 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 341 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_vec) = new std::vector<ColumnDefinition*>(); (yyval.column_vec)->push_back((yyvsp[0].column_t)); }
#line 2117 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 342 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].column_vec)->push_back((yyvsp[0].column_t)); (yyval.column_vec) = (yyvsp[-2].column_vec); }
#line 2123 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 346 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.str_vec) = new std::vector<std::string>(); (yyval.str_vec)->push_back((yyvsp[0].sval)); }
#line 2129 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 347 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].str_vec)->push_back((yyvsp[0].sval)); (yyval.str_vec) = (yyvsp[-2].str_vec); }
#line 2135 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 351 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.column_t) = new ColumnDefinition((yyvsp[-1].sval), (ColumnDefinition::DataType) (yyvsp[0].uval));
		}
#line 2143 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 354 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_t) = new ColumnDefinition(*((yyvsp[-1].str_vec)), ColumnDefinition::DataType::PRIMARY); }
#line 2149 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 355 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_t) = new ColumnDefinition(*((yyvsp[-1].str_vec)), ColumnDefinition::DataType::UNIQUE); }
#line 2155 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 356 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.column_t) = new ColumnDefinition(*((yyvsp[-1].str_vec)), ColumnDefinition::DataType::INDEX); }
#line 2161 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 361 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = ColumnDefinition::INT; }
#line 2167 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 362 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = ColumnDefinition::INT; }
#line 2173 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 363 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = ColumnDefinition::DOUBLE; }
#line 2179 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 364 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = ColumnDefinition::TEXT; }
#line 2185 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 374 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.drop_stmt) = new DropStatement(DropStatement::kTable);
			(yyval.drop_stmt)->name = (yyvsp[0].tbl_name);
		}
#line 2194 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 378 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.drop_stmt) = new DropStatement(DropStatement::kPreparedStatement);
			(yyval.drop_stmt)->name = new TableName((yyvsp[0].sval));
		}
#line 2203 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 390 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.delete_stmt) = new DeleteStatement();
			(yyval.delete_stmt)->table_name = (yyvsp[-1].tbl_name);
			(yyval.delete_stmt)->expr = (yyvsp[0].expr);
		}
#line 2213 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 398 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.delete_stmt) = new DeleteStatement();
			(yyval.delete_stmt)->table_name = (yyvsp[0].tbl_name);
		}
#line 2222 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 410 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.insert_stmt) = new InsertStatement(InsertStatement::kInsertValues);
			(yyval.insert_stmt)->table_name = (yyvsp[-5].tbl_name);
			(yyval.insert_stmt)->columns = (yyvsp[-4].str_vec);
			(yyval.insert_stmt)->values = (yyvsp[-1].expr_vec);
		}
#line 2233 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 416 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.insert_stmt) = new InsertStatement(InsertStatement::kInsertSelect);
			(yyval.insert_stmt)->table_name = (yyvsp[-2].tbl_name);
			(yyval.insert_stmt)->columns = (yyvsp[-1].str_vec);
			(yyval.insert_stmt)->select = (yyvsp[0].select_stmt);
		}
#line 2244 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 426 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.str_vec) = (yyvsp[-1].str_vec); }
#line 2250 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 427 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.str_vec) = NULL; }
#line 2256 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 437 "bison_parser.y" /* yacc.c:1646  */
    {
		(yyval.update_stmt) = new UpdateStatement();
		(yyval.update_stmt)->table = (yyvsp[-3].table);
		(yyval.update_stmt)->updates = (yyvsp[-1].update_vec);
		(yyval.update_stmt)->where = (yyvsp[0].expr);
	}
#line 2267 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 446 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.update_vec) = new std::vector<UpdateClause*>(); (yyval.update_vec)->push_back((yyvsp[0].update_t)); }
#line 2273 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 447 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].update_vec)->push_back((yyvsp[0].update_t)); (yyval.update_vec) = (yyvsp[-2].update_vec); }
#line 2279 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 451 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.update_t) = new UpdateClause();
			(yyval.update_t)->column = (yyvsp[-2].sval);
			(yyval.update_t)->value = (yyvsp[0].expr);
		}
#line 2289 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 468 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 2295 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 469 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.select_stmt) = (yyvsp[-1].select_stmt); }
#line 2301 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 473 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.select_stmt) = (yyvsp[-2].select_stmt);
			(yyval.select_stmt)->order = (yyvsp[-1].order);
			(yyval.select_stmt)->limit = (yyvsp[0].limit);
		}
#line 2311 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 478 "bison_parser.y" /* yacc.c:1646  */
    {
			// TODO: allow multiple unions (through linked list)
			// TODO: capture type of set_operator
			// TODO: might overwrite order and limit of first select here
			(yyval.select_stmt) = (yyvsp[-4].select_stmt);
			(yyval.select_stmt)->union_select = (yyvsp[-2].select_stmt);
			(yyval.select_stmt)->order = (yyvsp[-1].order);
			(yyval.select_stmt)->limit = (yyvsp[0].limit);
		}
#line 2325 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 496 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.select_stmt) = new SelectStatement();
			(yyval.select_stmt)->select_distinct = (yyvsp[-4].bval);
			(yyval.select_stmt)->select_list = (yyvsp[-3].expr_vec);
			(yyval.select_stmt)->from_table = (yyvsp[-2].table);
			(yyval.select_stmt)->where_clause = (yyvsp[-1].expr);
			(yyval.select_stmt)->group_by = (yyvsp[0].group_t);
		}
#line 2338 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 507 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 2344 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 508 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 2350 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 512 "bison_parser.y" /* yacc.c:1646  */
    {(yyval.expr_vec) = (yyvsp[0].expr_vec); }
#line 2356 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 517 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.table) = (yyvsp[0].table); 
		}
#line 2364 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 524 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2370 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 525 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = NULL; }
#line 2376 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 529 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.group_t) = new GroupByDescription();
			(yyval.group_t)->columns = (yyvsp[-1].expr_vec);
			(yyval.group_t)->having = (yyvsp[0].expr);
		}
#line 2386 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 534 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.group_t) = NULL; }
#line 2392 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 538 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 2398 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 539 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = NULL; }
#line 2404 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 542 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order) = new OrderDescription((yyvsp[0].order_type), (yyvsp[-1].expr)); }
#line 2410 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 543 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order) = NULL; }
#line 2416 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 547 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_type) = kOrderAsc; }
#line 2422 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 548 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_type) = kOrderDesc; }
#line 2428 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 549 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.order_type) = kOrderAsc; }
#line 2434 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 554 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription((yyvsp[0].expr)->ival, kNoOffset); delete (yyvsp[0].expr); }
#line 2440 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 555 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = new LimitDescription((yyvsp[-2].expr)->ival, (yyvsp[0].expr)->ival); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 2446 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 556 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.limit) = NULL; }
#line 2452 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 563 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = new std::vector<Expr*>(); (yyval.expr_vec)->push_back((yyvsp[0].expr)); }
#line 2458 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 564 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr)); (yyval.expr_vec) = (yyvsp[-2].expr_vec); }
#line 2464 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 568 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr_vec) = new std::vector<Expr*>(); (yyval.expr_vec)->push_back((yyvsp[0].expr)); }
#line 2470 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 569 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].expr_vec)->push_back((yyvsp[0].expr)); (yyval.expr_vec) = (yyvsp[-2].expr_vec); }
#line 2476 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 573 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.expr) = (yyvsp[-1].expr);
			(yyval.expr)->SetAlias((yyvsp[0].sval));
		}
#line 2485 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 580 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 2491 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 595 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(Expr::UMINUS, (yyvsp[0].expr));}
#line 2497 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 596 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(Expr::OperatorType::ISNULL, (yyvsp[-2].expr)); }
#line 2503 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 597 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(Expr::OperatorType::ISNOTNULL, (yyvsp[-3].expr)); }
#line 2509 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 598 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpUnary(Expr::NOT, (yyvsp[0].expr));}
#line 2515 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 603 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '-', (yyvsp[0].expr)); }
#line 2521 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 604 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '+', (yyvsp[0].expr)); }
#line 2527 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 605 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '/', (yyvsp[0].expr)); }
#line 2533 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 606 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '*', (yyvsp[0].expr)); }
#line 2539 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 607 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '%', (yyvsp[0].expr)); }
#line 2545 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 608 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '^', (yyvsp[0].expr)); }
#line 2551 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 609 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '&', (yyvsp[0].expr)); }
#line 2557 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 610 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::AND, (yyvsp[0].expr)); }
#line 2563 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 611 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::OR, (yyvsp[0].expr)); }
#line 2569 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 612 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::LIKE, (yyvsp[0].expr)); }
#line 2575 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 613 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-3].expr), Expr::NOT_LIKE, (yyvsp[0].expr)); }
#line 2581 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 617 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpTernary((yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr), Expr::OperatorType::IF); }
#line 2587 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 622 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '=', (yyvsp[0].expr)); }
#line 2593 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 623 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::NOT_EQUALS, (yyvsp[0].expr)); }
#line 2599 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 624 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '<', (yyvsp[0].expr)); }
#line 2605 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 625 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), '>', (yyvsp[0].expr)); }
#line 2611 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 626 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::LESS_EQ, (yyvsp[0].expr)); }
#line 2617 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 627 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeOpBinary((yyvsp[-2].expr), Expr::GREATER_EQ, (yyvsp[0].expr)); }
#line 2623 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 631 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeFunctionRef(Expr::SUBSTRING, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 2629 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 632 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeFunctionRef(Expr::SUBSTRING_INDEX, (yyvsp[-5].expr), (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 2635 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 636 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeColumnRef((yyvsp[0].sval)); }
#line 2641 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 637 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeColumnRef((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 2647 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 638 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeColumnRef((yyvsp[-4].sval), (yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 2653 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 648 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].sval)); }
#line 2659 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 653 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].fval)); }
#line 2665 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 658 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = Expr::makeLiteral((yyvsp[0].ival)); }
#line 2671 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 662 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new Expr(kExprStar); }
#line 2677 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 667 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.expr) = Expr::makePlaceholder(yylloc.total_column);
			yyloc.placeholder_list.push_back((yyval.expr));
		}
#line 2686 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 678 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table) = (yyvsp[0].table); }
#line 2692 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 679 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyvsp[0].table_vec)->push_back((yyvsp[-2].table));
			auto tbl = new TableRef(kTableCrossProduct);
			tbl->list = (yyvsp[0].table_vec);
			(yyval.table) = tbl;
		}
#line 2703 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 690 "bison_parser.y" /* yacc.c:1646  */
    {
			auto tbl = new TableRef(kTableSelect);
			tbl->select = (yyvsp[-2].select_stmt);
			tbl->alias = (yyvsp[0].sval);
			(yyval.table) = tbl;
		}
#line 2714 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 701 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.table_vec) = new std::vector<TableRef*>(); (yyval.table_vec)->push_back((yyvsp[0].table)); }
#line 2720 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 702 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].table_vec)->push_back((yyvsp[0].table)); (yyval.table_vec) = (yyvsp[-2].table_vec); }
#line 2726 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 707 "bison_parser.y" /* yacc.c:1646  */
    {
			TableRef *tbl1 = new TableRef(kTableName);
			tbl1->name = (yyvsp[-1].tbl_name);
			tbl1->SetAlias((yyvsp[0].sval));
			(yyval.table) = tbl1;
		}
#line 2737 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 717 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.table) = new TableRef(kTableName);
			(yyval.table)->name = (yyvsp[0].tbl_name);
		}
#line 2746 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 725 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.tbl_name) = new TableName((yyvsp[0].sval)); }
#line 2752 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 726 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.tbl_name) = new TableName((yyvsp[-2].sval), (yyvsp[0].sval)); }
#line 2758 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 731 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.sval) = (yyvsp[0].sval); }
#line 2764 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 737 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.sval) = NULL; }
#line 2770 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 746 "bison_parser.y" /* yacc.c:1646  */
    { //( 2 )
			(yyval.table) = new TableRef(kTableJoin);
			(yyval.table)->join = new JoinDefinition();
			(yyval.table)->join->type = (JoinType) (yyvsp[-2].uval);
			(yyval.table)->join->right = (yyvsp[0].table);
			(yyval.table)->join->condition = NULL;

			(yyval.table)->prev_join = (yyvsp[-3].table);
			(yyvsp[-3].table)->next_join = (yyval.table);
		}
#line 2785 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 756 "bison_parser.y" /* yacc.c:1646  */
    { //( 2 )
			(yyval.table) = new TableRef(kTableJoin);
			(yyval.table)->join = new JoinDefinition();
			(yyval.table)->join->type = (JoinType) (yyvsp[-4].uval);
			(yyval.table)->join->right = (yyvsp[-2].table);
			(yyval.table)->join->condition = (yyvsp[0].expr);

			(yyval.table)->prev_join = (yyvsp[-5].table);
			(yyvsp[-5].table)->next_join = (yyval.table);
		}
#line 2800 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 766 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.table) = new TableRef(kTableJoin);
			(yyval.table)->join = new JoinDefinition();
			(yyval.table)->join->type = (JoinType) (yyvsp[-2].uval);
			(yyval.table)->join->left = (yyvsp[-3].table);
			(yyval.table)->join->right = (yyvsp[0].table);
			(yyval.table)->join->condition = NULL;
		}
#line 2813 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 774 "bison_parser.y" /* yacc.c:1646  */
    {
			(yyval.table) = new TableRef(kTableJoin);
			(yyval.table)->join = new JoinDefinition();
			(yyval.table)->join->type = (JoinType) (yyvsp[-4].uval);
			(yyval.table)->join->left = (yyvsp[-5].table);
			(yyval.table)->join->right = (yyvsp[-2].table);
			(yyval.table)->join->condition = (yyvsp[0].expr);
		}
#line 2826 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 785 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = kJoinInner; }
#line 2832 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 786 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = kJoinOuter; }
#line 2838 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 787 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = kJoinLeft; }
#line 2844 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 788 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = kJoinRight; }
#line 2850 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 789 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.uval) = kJoinInner; }
#line 2856 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 795 "bison_parser.y" /* yacc.c:1646  */
    {
			auto tbl = new TableRef(kTableSelect);
			tbl->select = (yyvsp[-2].select_stmt);
			tbl->alias = (yyvsp[0].sval);
			(yyval.table) = tbl;
		}
#line 2867 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 820 "bison_parser.y" /* yacc.c:1646  */
    { (yyval.str_vec) = new std::vector<std::string>(); (yyval.str_vec)->push_back(std::string((yyvsp[0].sval))); }
#line 2873 "bison_parser.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 821 "bison_parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].str_vec)->push_back(std::string((yyvsp[0].sval))); (yyval.str_vec) = (yyvsp[-2].str_vec); }
#line 2879 "bison_parser.cpp" /* yacc.c:1646  */
    break;


#line 2883 "bison_parser.cpp" /* yacc.c:1646  */
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
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, result, scanner, YY_("syntax error"));
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
        yyerror (&yylloc, result, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc, result, scanner);
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

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, result, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, result, scanner);
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
  return yyresult;
}
#line 824 "bison_parser.y" /* yacc.c:1906  */

/*********************************
 ** Section 4: Additional C code
 *********************************/

/* empty */

