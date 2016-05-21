#include "list.h"

#ifndef BABYC_SYNTAX_HEADER
#define BABYC_SYNTAX_HEADER

typedef enum {
    IMMEDIATE,
    VARIABLE,
    UNARY_OPERATOR,
    BINARY_OPERATOR,
    // We already use 'RETURN' and 'IF' as token names.
    BLOCK,
    IF_STATEMENT,
    PRINT_STATEMENT,
    PRINTDEC_STATEMENT,
	PRINTHEX_STATEMENT,
    DEFINE_VAR,
    ASSIGNMENT,
    WHILE_SYNTAX,
    TOP_LEVEL
} SyntaxType;
typedef enum { BITWISE_NEGATION, LOGICAL_NEGATION } UnaryExpressionType;
typedef enum {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
	DIVIDE,
	MODULO,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
	EQUAL,
    NOT_EQ
} BinaryExpressionType;

struct Syntax;
typedef struct Syntax Syntax;

typedef struct Immediate { long long value; } Immediate;

typedef struct Variable {
    // TODO: once we have other types, we will need to store type here.
    char *var_name;
    int regindex;
} Variable;

typedef struct UnaryExpression {
    UnaryExpressionType unary_type;
    Syntax *expression;
} UnaryExpression;

typedef struct BinaryExpression {
    BinaryExpressionType binary_type;
    Syntax *left;
    Syntax *right;
} BinaryExpression;


typedef struct Assignment {
    char *var_name;
    Syntax *expression;
} Assignment;

typedef struct IfStatement {
    Syntax *condition;
    Syntax *then;
} IfStatement;

typedef struct DefineVarStatement {
    char *var_name;
    Syntax *init_value;
} DefineVarStatement;

typedef struct WhileStatement {
    Syntax *condition;
    Syntax *body;
} WhileStatement;

typedef struct PrintStatement {
	char *value;
	char tag[6];
} PrintStatement;

typedef struct PrintExp {
	Syntax *exp;
} PrintExp;



typedef struct Block { List *statements; } Block;


typedef struct Parameter {
    // TODO: once we have other types, we will need to store type here.
    char *name;
} Parameter;

typedef struct TopLevel { List *declarations; } TopLevel;

struct Syntax {
    SyntaxType type;
    union {
        Immediate *immediate;

        Variable *variable;

        UnaryExpression *unary_expression;

        BinaryExpression *binary_expression;

        Assignment *assignment;

        IfStatement *if_statement;

        DefineVarStatement *define_var_statement;

        WhileStatement *while_statement;

        Block *block;

        TopLevel *top_level;
		
		PrintStatement *printstmt;
		
		PrintExp *printexp;
        
    };
};

Syntax *immediate_new(int value);

Syntax *variable_new(char *var_name);

Syntax *bitwise_negation_new(Syntax *expression);

Syntax *logical_negation_new(Syntax *expression);

Syntax *addition_new(Syntax *left, Syntax *right);

Syntax *subtraction_new(Syntax *left, Syntax *right);

Syntax *multiplication_new(Syntax *left, Syntax *right);
//yook edit
Syntax *div_new(Syntax *left, Syntax *right);
Syntax *mod_new(Syntax *left, Syntax *right);
Syntax *eq_new(Syntax *left, Syntax *right);
Syntax *noteq_new(Syntax *left, Syntax *right);

Syntax *less_than_new(Syntax *left, Syntax *right);

Syntax *less_or_equal_new(Syntax *left, Syntax *right);

Syntax *assignment_new(char *var_name, Syntax *expression);

Syntax *block_new(List *statements);

Syntax *if_new(Syntax *condition, Syntax *then);

Syntax *define_var_new(char *var_name, Syntax *init_value);

Syntax *while_new(Syntax *condition, Syntax *body);

Syntax *top_level_new();


//yook edit
Syntax *printint_new(Syntax *expression,SyntaxType type);
Syntax *print_new(int index,char* value);

void syntax_free(Syntax *syntax);

char *syntax_type_name(Syntax *syntax);
void print_syntax_indented(Syntax *syntax, int indent);
void print_syntax(Syntax *syntax);
void syntax_list_free(List *syntaxes);
#endif
