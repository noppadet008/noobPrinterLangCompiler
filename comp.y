
%{
  #include <math.h>
  #include <stdio.h>
  #include <string.h>
  #include "stack.h"
  #include "syntax.h"
  #include "asmX86emiter.h"
  
  void yyerror (char const *);
  int yylex();
  char* reply;
  extern int size;

  extern FILE *yyin;
  Stack *syntax_stack;
  Stack *string_stack;
  int countstr = 0;
  Syntax *top;
%}

%union
{
	char* str;
	long long num;
}

%token NUM HEX IDEN WHEN LOOP PRINT PRINT16
%token OB CB LP RP 
%token STRING SEMICOLON 

%nonassoc ASSIGN
%nonassoc MORETHAN LESSTHAN EQ MORETHAN_EQ LESSTHAN_EQ
%left ADD MINUS
%left MUL DIV MOD
%right NEG

//for identity type in yylval
%type<str> IDEN STRING
%type<num> NUM HEX expression

%%
block:
		/*empty*/	
		| statement block
			{
				/* Append to the current block, or start a new block. */
				Syntax *block_syntax;
				 if (stack_empty(syntax_stack)) {
					block_syntax = block_new(list_new());
					top = block_syntax;
				} else if (((Syntax *)stack_peek(syntax_stack))->type != BLOCK) {
					block_syntax = block_new(list_new());
				} else {
					block_syntax = stack_pop(syntax_stack);
				}

				list_push(block_syntax->block->statements, stack_pop(syntax_stack));
				stack_push(syntax_stack, block_syntax);
			}
;

statement: 
		WHEN condition OB block CB								
			{
				Syntax *then = stack_pop(syntax_stack);
            	Syntax *condition = stack_pop(syntax_stack);
            	stack_push(syntax_stack, if_new(condition, then));
				
			}
		|  LOOP condition OB block CB
			{
				Syntax *body = stack_pop(syntax_stack);
            	Syntax *condition = stack_pop(syntax_stack);
            	stack_push(syntax_stack, while_new(condition, body));
			}
		|  PRINT expression SEMICOLON  
			{
				Syntax *exp = stack_pop(syntax_stack);
				stack_push(syntax_stack, printint_new(exp,PRINTDEC_STATEMENT));
			}
		|  PRINT STRING SEMICOLON
			{
				Syntax* printstmt =  print_new(countstr,$2);
				stack_push(string_stack,printstmt);
				stack_push(syntax_stack,printstmt);
				countstr++;
			}
		|  PRINT16 expression SEMICOLON
			{
				Syntax *exp = stack_pop(syntax_stack);
				stack_push(syntax_stack, printint_new(exp,PRINTHEX_STATEMENT));
			}
		|  IDEN ASSIGN expression SEMICOLON
			{
				Syntax *exp = stack_pop(syntax_stack);
				stack_push(syntax_stack,assignment_new($1, exp));
			}
		|  IDEN SEMICOLON
			{
				stack_push(syntax_stack,variable_new($1));
			}		
;


expression: NUM													
				{
					stack_push(syntax_stack, immediate_new($1));
				}
			|  MINUS NUM %prec NEG								
				{
					stack_push(syntax_stack, immediate_new($2*(-1)));
				}
			|  HEX 												
				{
					stack_push(syntax_stack, immediate_new($1));	
				}
			|  IDEN												
				{
					stack_push(syntax_stack,variable_new($1));
				}
			|  expression ADD expression						
				{
					Syntax *right = stack_pop(syntax_stack);
					Syntax *left = stack_pop(syntax_stack);
					if(right->type == IMMEDIATE && left->type == IMMEDIATE){
						stack_push(syntax_stack,immediate_new(left->immediate->value + right->immediate->value));
						syntax_free(left);
						syntax_free(right);
					}else{
						stack_push(syntax_stack, addition_new(left, right));
					}
				}
			|  expression MINUS expression						
				{
					Syntax *right = stack_pop(syntax_stack);
					Syntax *left = stack_pop(syntax_stack);
					if(right->type == IMMEDIATE && left->type == IMMEDIATE){
						stack_push(syntax_stack,immediate_new(left->immediate->value - right->immediate->value));
						syntax_free(left);
						syntax_free(right);
					}else{
						stack_push(syntax_stack, subtraction_new(left, right));
					}
				}
			|  expression MUL expression						
				{
					Syntax *right = stack_pop(syntax_stack);
					Syntax *left = stack_pop(syntax_stack);
					if(right->type == IMMEDIATE && left->type == IMMEDIATE){
						stack_push(syntax_stack,immediate_new(left->immediate->value * right->immediate->value));
						syntax_free(left);
						syntax_free(right);
					}else{
						stack_push(syntax_stack, multiplication_new(left, right));
					}
				}
			|  expression DIV expression
				{
					Syntax *right = stack_pop(syntax_stack);
					Syntax *left = stack_pop(syntax_stack);
					if(right->type == IMMEDIATE && left->type == IMMEDIATE){
						stack_push(syntax_stack,immediate_new(left->immediate->value / right->immediate->value));
						syntax_free(left);
						syntax_free(right);
					}else{
						stack_push(syntax_stack, div_new(left, right));
					}
				}
			|  expression MOD expression
				{
					Syntax *right = stack_pop(syntax_stack);
					Syntax *left = stack_pop(syntax_stack);
					if(right->type == IMMEDIATE && left->type == IMMEDIATE){
						stack_push(syntax_stack,immediate_new(left->immediate->value % right->immediate->value));
						syntax_free(left);
						syntax_free(right);
					}else{
						stack_push(syntax_stack, mod_new(left, right));
					}
				}
			|  LP expression RP									
				{
					/*do nothing*/
				}
;

condition: expression EQ expression							
				{
					Syntax *right = stack_pop(syntax_stack);
            		Syntax *left = stack_pop(syntax_stack);
            		stack_push(syntax_stack, eq_new(left, right));
				}
		    |  expression MORETHAN expression
				{	//switch opetand of lessthan
					Syntax *left = stack_pop(syntax_stack);
					Syntax *right = stack_pop(syntax_stack);
            		stack_push(syntax_stack, less_than_new(left, right));
				}
		    |  expression LESSTHAN expression
				{
					Syntax *right = stack_pop(syntax_stack); 
					Syntax *left = stack_pop(syntax_stack);
            		stack_push(syntax_stack, less_than_new(left, right));
				}
			|  expression LESSTHAN_EQ expression
				{
					Syntax *right = stack_pop(syntax_stack); 
					Syntax *left = stack_pop(syntax_stack);
            		stack_push(syntax_stack, less_or_equal_new(left, right));
				}
			|  expression MORETHAN_EQ expression
				{
					Syntax *left = stack_pop(syntax_stack); 
					Syntax *right = stack_pop(syntax_stack);
            		stack_push(syntax_stack, less_or_equal_new(left, right));
				}	
			|  expression
				{
					Syntax *var = stack_pop(syntax_stack);
					stack_push(syntax_stack,noteq_new(var,immediate_new(0)));
				}
;

%%

void yyerror (char const *s)
{
    fprintf (stderr, "%s\n", s);
}


int main(int argc, char *argv[])
{
	int result;
	syntax_stack = stack_new();
	string_stack = stack_new();
	result = yyparse();
	if (result != 0) {
        printf("\n");
        goto cleanup_syntax;
    }
	printf("parse complete\n\n");
    Syntax *complete_syntax = stack_pop(syntax_stack);
    if (syntax_stack->size > 0) {
        printf("remaining");
        while(syntax_stack->size > 0) {
            fprintf(stderr, "%s", syntax_type_name(stack_pop(syntax_stack)));
        }
    }
	print_syntax(complete_syntax);
	//write asm
	write_asm(complete_syntax,string_stack);
cleanup_syntax:
	stack_free(syntax_stack);
	stack_free(string_stack);
	syntax_free(complete_syntax);


}
