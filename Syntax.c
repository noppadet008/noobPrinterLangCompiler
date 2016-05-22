#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syntax.h"
#include "list.h"

Syntax *immediate_new(int value) {
    Immediate *immediate = malloc(sizeof(Immediate));
    immediate->value = value;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = IMMEDIATE;
    syntax->immediate = immediate;

    return syntax;
}

Syntax *variable_new(char *var_name) {
    Variable *variable = malloc(sizeof(Variable));
    variable->var_name = var_name;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = VARIABLE;
    syntax->variable = variable;

    return syntax;
}

Syntax *bitwise_negation_new(Syntax *expression) {
    UnaryExpression *unary_syntax = malloc(sizeof(UnaryExpression));
    unary_syntax->unary_type = BITWISE_NEGATION;
    unary_syntax->expression = expression;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = UNARY_OPERATOR;
    syntax->unary_expression = unary_syntax;

    return syntax;
}

Syntax *logical_negation_new(Syntax *expression) {
    UnaryExpression *unary_syntax = malloc(sizeof(UnaryExpression));
    unary_syntax->unary_type = LOGICAL_NEGATION;
    unary_syntax->expression = expression;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = UNARY_OPERATOR;
    syntax->unary_expression = unary_syntax;

    return syntax;
}

Syntax *addition_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = ADDITION;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *subtraction_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = SUBTRACTION;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *multiplication_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = MULTIPLICATION;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}
//nodet modified
Syntax *div_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = DIVIDE;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *mod_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = MODULO;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *less_than_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = LESS_THAN;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *less_or_equal_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = LESS_THAN_OR_EQUAL;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *eq_new(Syntax *left, Syntax *right) {
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = EQUAL;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *noteq_new(Syntax *left, Syntax *right){
    BinaryExpression *binary_syntax = malloc(sizeof(BinaryExpression));
    binary_syntax->binary_type = NOT_EQ;
    binary_syntax->left = left;
    binary_syntax->right = right;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BINARY_OPERATOR;
    syntax->binary_expression = binary_syntax;

    return syntax;
}

Syntax *assignment_new(char *var_name, Syntax *expression) {
    Assignment *assignment = malloc(sizeof(Assignment));
    assignment->var_name = var_name;
    assignment->expression = expression;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = ASSIGNMENT;
    syntax->assignment = assignment;

    return syntax;
}
/*evalute for use expression value here*/
Syntax *printint_new(Syntax *expression,SyntaxType type) {
    PrintExp *printexp = malloc(sizeof(PrintExp));
	printexp->exp = expression;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = type;
    syntax->printexp = printexp;

    return syntax;
}
/*print stting*/
Syntax *print_new(int index,char* value) {
    PrintStatement *printstmt = malloc(sizeof(PrintStatement));
	printstmt->value = value;
	sprintf(printstmt->tag,".STR%d",index);//.STR0 for reference string
    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = PRINT_STATEMENT;
    syntax->printstmt = printstmt;

    return syntax;
}

Syntax *block_new(List *statements) {
    Block *block = malloc(sizeof(Block));
    block->statements = statements;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = BLOCK;
    syntax->block = block;

    return syntax;
}

Syntax *if_new(Syntax *condition, Syntax *then) {
    BinaryExpression *temp = malloc(sizeof(BinaryExpression));
    IfStatement *if_statement = malloc(sizeof(IfStatement));
    temp->binary_type = 
        invertCondition(condition->binary_expression->binary_type);
    temp->left = condition->binary_expression->right;
    temp->right = condition->binary_expression->left;
    free(condition->binary_expression); // delete old.
    condition->binary_expression = temp; //assign new.
    if_statement->condition = condition;
    if_statement->then = then;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = IF_STATEMENT;
    syntax->if_statement = if_statement;

    return syntax;
}

BinaryExpressionType invertCondition(BinaryExpressionType type){
    if(type == LESS_THAN)
        return LESS_THAN_OR_EQUAL;
    else if(type == LESS_THAN_OR_EQUAL){
        return LESS_THAN;
    }else if(type == EQUAL){
        return NOT_EQ;
    }else if(type == NOT_EQ){
        return EQUAL;
    }
}

Syntax *define_var_new(char *var_name, Syntax *init_value) {
    DefineVarStatement *define_var_statement =
        malloc(sizeof(DefineVarStatement));
    define_var_statement->var_name = var_name;
    define_var_statement->init_value = init_value;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = DEFINE_VAR;
    syntax->define_var_statement = define_var_statement;

    return syntax;
}

Syntax *while_new(Syntax *condition, Syntax *body) {
    WhileStatement *while_statement = malloc(sizeof(WhileStatement));
    while_statement->condition = condition;
    while_statement->body = body;

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = WHILE_SYNTAX;
    syntax->while_statement = while_statement;

    return syntax;
}


Syntax *top_level_new() {
    TopLevel *top_level = malloc(sizeof(TopLevel));
    top_level->declarations = list_new();

    Syntax *syntax = malloc(sizeof(Syntax));
    syntax->type = TOP_LEVEL;
    syntax->top_level = top_level;

    return syntax;
}

void syntax_list_free(List *syntaxes) {
    if (syntaxes == NULL) {
        return;
    }

    for (int i = 0; i < list_length(syntaxes); i++) {
        syntax_free(list_get(syntaxes, i));
    }

    list_free(syntaxes);
}

void syntax_free(Syntax *syntax) {
    if (syntax->type == IMMEDIATE) {
        free(syntax->immediate);

    } else if (syntax->type == VARIABLE) {
        free(syntax->variable->var_name);
        free(syntax->variable);

    } else if (syntax->type == UNARY_OPERATOR) {
        syntax_free(syntax->unary_expression->expression);
        free(syntax->unary_expression);

    } else if (syntax->type == BINARY_OPERATOR) {
        syntax_free(syntax->binary_expression->left);
        syntax_free(syntax->binary_expression->right);
        free(syntax->binary_expression);

    } else if (syntax->type == IF_STATEMENT) {
        syntax_free(syntax->if_statement->condition);
        syntax_free(syntax->if_statement->then);

    } else if (syntax->type == DEFINE_VAR) {
        free(syntax->define_var_statement->var_name);
        syntax_free(syntax->define_var_statement->init_value);
        free(syntax->define_var_statement);

    } else if (syntax->type == BLOCK) {
        syntax_list_free(syntax->block->statements);
        free(syntax->block);

    } else if (syntax->type == ASSIGNMENT) {
        free(syntax->assignment->var_name);
        syntax_free(syntax->assignment->expression);

        free(syntax->assignment);

    } else if (syntax->type == WHILE_SYNTAX) {
        syntax_free(syntax->while_statement->condition);
        syntax_free(syntax->while_statement->body);

    } else if (syntax->type == TOP_LEVEL) {
        syntax_list_free(syntax->top_level->declarations);
        free(syntax->top_level);
    } else if (syntax->type == PRINTHEX_STATEMENT ||
        syntax->type == PRINTDEC_STATEMENT) {
        syntax_free(syntax->printexp->exp);
        free(syntax->printexp);
    } else if (syntax->type == PRINT_STATEMENT){
        free(syntax->printstmt);
    }
    else {
        printf("Could not free syntax tree with type:%s \n"
            ,syntax_type_name(syntax));
    }

    free(syntax);
}

char *syntax_type_name(Syntax *syntax) {
    if (syntax->type == IMMEDIATE) {
        return "IMMEDIATE";
    } else if (syntax->type == VARIABLE) {
        return "VARIABLE";
    } else if (syntax->type == UNARY_OPERATOR) {
        if (syntax->unary_expression->unary_type == BITWISE_NEGATION) {
            return "UNARY BITWISE_NEGATION";
        } else if (syntax->unary_expression->unary_type == LOGICAL_NEGATION) {
            return "UNARY BITWISE_NEGATION";
        }
    } else if (syntax->type == BINARY_OPERATOR) {
        if (syntax->binary_expression->binary_type == ADDITION) {
            return "ADDITION";
        } else if (syntax->binary_expression->binary_type == SUBTRACTION) {
            return "SUBTRACTION";
        } else if (syntax->binary_expression->binary_type == MULTIPLICATION) {
            return "MULTIPLICATION";
        } else if (syntax->binary_expression->binary_type == DIVIDE) {
            return "DIVIDE";
        } else if (syntax->binary_expression->binary_type == MODULO) {
            return "MODULO";
        } else if (syntax->binary_expression->binary_type == LESS_THAN) {
            return "LESS THAN";
        } else if (syntax->binary_expression->binary_type ==
                   LESS_THAN_OR_EQUAL) {
            return "LESS THAN OR EQUAL";
        } else if (syntax->binary_expression->binary_type == EQUAL) {
            return "EQUAL";
        } else if (syntax->binary_expression->binary_type == NOT_EQ) {
            return "NOT_EQUAL";
        }
    } else if (syntax->type == PRINT_STATEMENT) {
        return "PRINT STRING";
    } else if (syntax->type == PRINTDEC_STATEMENT){
        return "PRINT DEC";
    } else if (syntax->type == PRINTHEX_STATEMENT){
        return "PRINT HEX";
    } else if (syntax->type == IF_STATEMENT) {
        return "IF";
    } else if (syntax->type == DEFINE_VAR) {
        return "DEFINE VARIABLE";
    } else if (syntax->type == BLOCK) {
        return "BLOCK";
    } else if (syntax->type == ASSIGNMENT) {
        return "ASSIGNMENT";
    } else if (syntax->type == WHILE_SYNTAX) {
        return "WHILE";
    } else if (syntax->type == TOP_LEVEL) {
        return "TOP LEVEL";
    }

    // Should never be reached.
    return "??? UNKNOWN SYNTAX";
}
//for show syntax tree.
void print_syntax_indented(Syntax *syntax, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }

    char *syntax_type_string = syntax_type_name(syntax);

    if (syntax->type == IMMEDIATE) {
        printf("%s %d\n", syntax_type_string, syntax->immediate->value);
    } else if (syntax->type == VARIABLE) {
        printf("%s '%s'\n", syntax_type_string, syntax->variable->var_name);
    } else if (syntax->type == UNARY_OPERATOR) {
        printf("%s\n", syntax_type_string);
        print_syntax_indented(syntax->unary_expression->expression, indent + 4);

    } else if (syntax->type == BINARY_OPERATOR) {
        printf("%s LEFT\n", syntax_type_string);
        print_syntax_indented(syntax->binary_expression->left, indent + 4);

        for (int i = 0; i < indent; i++) {
            printf(" ");
        }

        printf("%s RIGHT\n", syntax_type_string);
        print_syntax_indented(syntax->binary_expression->right, indent + 4);


    } else if (syntax->type == IF_STATEMENT) {
        printf("%s CONDITION\n", syntax_type_string);
        print_syntax_indented(syntax->if_statement->condition, indent + 4);

        for (int i = 0; i < indent; i++) {
            printf(" ");
        }

        printf("%s THEN\n", syntax_type_string);
        print_syntax_indented(syntax->if_statement->then, indent + 4);
    } else if (syntax->type == PRINT_STATEMENT){
        printf("PRINT %s : %s\n",syntax->printstmt->tag,syntax->printstmt->value);
    } else if (syntax->type == PRINTDEC_STATEMENT){
        printf("PRINTDEC ");
        print_syntax_indented(syntax->printexp->exp,0);
    } else if (syntax->type == PRINTHEX_STATEMENT){
        printf("PRINTHEX ");
        print_syntax_indented(syntax->printexp->exp,0);
    } else if (syntax->type == DEFINE_VAR) {
        printf("%s '%s'\n", syntax_type_string,
               syntax->define_var_statement->var_name);

        for (int i = 0; i < indent; i++) {
            printf(" ");
        }

        printf("'%s' INITIAL VALUE\n", syntax->define_var_statement->var_name);
        print_syntax_indented(syntax->define_var_statement->init_value,
                              indent + 4);

    } else if (syntax->type == BLOCK) {
        printf("%s\n", syntax_type_string);

        List *statements = syntax->block->statements;
        for (int i = 0; i < list_length(statements); i++) {
            print_syntax_indented(list_get(statements, i), indent + 4);
        }

    } else if (syntax->type == ASSIGNMENT) {
        printf("%s '%s'\n", syntax_type_string, syntax->assignment->var_name);
        print_syntax_indented(syntax->assignment->expression, indent + 4);

    } else if (syntax->type == WHILE_SYNTAX) {
        printf("%s CONDITION\n", syntax_type_string);
        print_syntax_indented(syntax->while_statement->condition, indent + 4);

        for (int i = 0; i < indent; i++) {
            printf(" ");
        }

        printf("%s BODY\n", syntax_type_string);
        print_syntax_indented(syntax->while_statement->body, indent + 4);

    } else if (syntax->type == TOP_LEVEL) {
        printf("%s\n", syntax_type_string);

        List *declarations = syntax->top_level->declarations;
        for (int i = 0; i < list_length(declarations); i++) {
            print_syntax_indented(list_get(declarations, i), indent + 4);
        }

    } else {
        printf("??? UNKNOWN SYNTAX TYPE\n");
    }
}

void print_syntax(Syntax *syntax) { print_syntax_indented(syntax, 0); }