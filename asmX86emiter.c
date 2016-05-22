#define MAX_MNEMONIC_LENGTH 7
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <stdarg.h>
#include"asmX86emiter.h"


typedef struct OffsetVarmap {
	char* varname;
	int offset;
} OffsetVarmap;
void openf(char* filename);
void closef();

void function_declare(char* functionName);
void printtab(int tab);
void emit_header(void);
void emit_init(void);
void emit_label(char* label);
void emit_footer(void);
void emit_ins(char* instr,char* oparand);
void emit_format(void);
void emit_ascii(char* string);

//function emit token.
void emitblock(Syntax* syntax);
void emitstmt(void* stmt);
void emit_expression(Syntax* exp);
void emit_str(Stack* string_stack);
void emit_condition(Syntax* condition,int label);

FILE *fp;
int offset = 8;
int labelconditionid = 0;
List* tablevar = NULL;

 void write_asm(Syntax* complete_syntax,Stack* string_stack){
	 tablevar = list_new();
	openf("output.s"); //open file.
	emit_format();
	emit_str(string_stack);
	emit_header();	//print .text
	function_declare("main"); //declare .globl main
	emit_init(); //alloc for main.
	if(complete_syntax->type == BLOCK){
		emitblock(complete_syntax);
	}else
		printf("invalid syntax");
	emit_footer(); //ralloc for main
	closef(fp); //close file.
	free(tablevar);
}

int findOffsetVar(char* varname){
	int offset = -1;//not found is -1
	for (int i = 0; i < list_length(tablevar); i++) {
		//strcmp 0 is equal string.
		OffsetVarmap* temp;
		int notfound;
		temp = (OffsetVarmap*)list_get(tablevar,i);
		notfound = strcmp(varname,temp->varname);
		if(notfound == 0){//found
			offset = temp->offset;
			break;
		}
	}
	return offset;
}

void emit_header(){
	fprintf(fp,"    .text\n");
}
void emit_format(){
	emit_label(".DEC");//emit format of integer.
	emit_ascii("%d\\n");
	
	//fprintf(fp,"    .ascii \"%%d\\n\\0\"\n");
	emit_label(".HEX");
	emit_ascii("0x%X\\n");
	//fprintf(fp,"    .ascii \"%%X\\n\\0\"\n");
}

void emit_str(Stack* string_stack){
	while(!stack_empty(string_stack)){
		Syntax *syntax = (Syntax*)stack_pop(string_stack);
		emit_label(syntax->printstmt->tag);
		emit_ascii(syntax->printstmt->value);
	}
}

void emit_ascii(char* string){
	fprintf(fp,"    .ascii \"%s\\0\"\n",string);
}
void emit_footer(){
	emit_ins("movl","$0, %%eax");
	emit_ins("addq","$48, %%rsp");
	emit_ins("popq","%%rbp");
	emit_ins("ret",NULL);
}

void function_declare(char* functionName){
	fprintf(fp,"	.globl	%s\n",functionName);
	fprintf(fp,"%s:\n",functionName);
}
void printtab(int tab){
	//tab for indentation
	int temp;
	for(temp = 0;temp<tab;temp++){
		fprintf(fp,"    ");
	}
}
void emit_label(char* labelname){
	fprintf(fp,"%s:\n",labelname);
}

void emit_ins(char* instr,char* oparand){
	
	// The assembler requires at least 4 spaces for indentation.
    fprintf(fp,"    %s", instr);

    // Ensure our argument are aligned, regardless of the assembly
    // mnemonic length.
    int argument_offset = MAX_MNEMONIC_LENGTH - strlen(instr) + 1;
    for (;argument_offset > 0;argument_offset--) {
        fprintf(fp, " ");
    }
	if(oparand)
		fprintf(fp,oparand);
    fputs("\n", fp);
}

void emit_init(){
	emit_ins("pushq","%%rbp");
	emit_ins("movq","%%rsp, %%rbp");
	emit_ins("subq","$48, %%rsp");
	emit_ins("call","__main");
}

/* Write instruction INSTR with formatted operands OPERANDS_FORMAT to
 * OUT.
 *
 * Example:
 * emit_instr_format(out, "MOV", "%%eax, %s", 5);
 */
void emit_instr_format( char *instr, char *operands_format, ...) {
    // The assembler requires at least 4 spaces for indentation.
    fprintf(fp, "    %s", instr);

    // Ensure our argument are aligned, regardless of the assembly
    // mnemonic length.
    int argument_offset = MAX_MNEMONIC_LENGTH - strlen(instr) + 1;
    while (argument_offset > 0) {
        fprintf(fp, " ");
        argument_offset--;
    }

    va_list argptr;
    va_start(argptr, operands_format);
    vfprintf(fp, operands_format, argptr);
    va_end(argptr);

    fputs("\n", fp);
}

void openf(char* filename){
	errno_t err;

	err = fopen_s(&fp, filename, "w+");// file open function fix here
	if (err == 0)
	{
		printf("The file 'crt_fopen_s.c' was opened\n");
	}
	else
	{
		printf("The file 'crt_fopen_s.c' was not opened\n");
	}
}

void closef(){
	errno_t err;
	if (fp)
	{
		err = fclose(fp);
		if (err == 0)
		{
			printf("The file 'crt_fopen_s.c' was closed\n");
		}
		else
		{
			printf("The file 'crt_fopen_s.c' was not closed\n");
		}
	}
}


/*emit token*/
void emitblock(Syntax* syntax){
	/*
	List* temp;
	//no reset offset because no use function
	if(tablevar)//for store old table.
		temp = tablevar;
		*/
	List *stmt = syntax->block->statements;
	for (int i = 0; i < list_length(stmt); i++) {
    	emitstmt(list_get(stmt,i));//for each stmt.
    }
	//free(tablevar);
	//tablevar = temp;
}

void emitstmt(void* stmt){
	Syntax *st = (Syntax*) stmt;
	if(st->type == IF_STATEMENT){
		int label = labelconditionid++;
		char c[10];
		emit_condition(st->if_statement->condition,label);
		emitblock(st->if_statement->then);
		sprintf(c,".LC%d",label);
		emit_label(c);
	}else if(st->type == WHILE_SYNTAX){
		int labelcondition,labelblock;
		labelcondition = labelconditionid++;
		labelblock = labelconditionid++;
		char c[10];
		emit_instr_format("jmp",".LC%d",labelcondition);//to check condition
		sprintf(c,".LC%d",labelblock);
		emit_label(c);
		emitblock(st->while_statement->body);
		sprintf(c,".LC%d",labelcondition);
		emit_label(c);
		emit_condition(st->while_statement->condition,labelblock);
	}else if(st->type == PRINT_STATEMENT){
		emit_instr_format("leaq","%s(%%rip), %%rcx",st->printstmt->tag);
		emit_ins("call","printf");
	}else if(st->type == PRINTDEC_STATEMENT){
		emit_expression(st->printexp->exp);
		emit_ins("movq","%%rax, %%rdx");
		emit_ins("leaq",".DEC(%%rip), %%rcx");//.DEC print dec format
		emit_ins("call","printf");
	}else if(st->type == PRINTHEX_STATEMENT){
		emit_expression(st->printexp->exp);
		emit_ins("movq","%%rax, %%rdx");
		emit_ins("leaq",".HEX(%%rip), %%rcx");//.HEX print hex format
		emit_ins("call","printf");
	}else if(st->type == ASSIGNMENT){
		int tempoffset = findOffsetVar(st->assignment->var_name);//not found is -1
		if(tempoffset == -1){
			OffsetVarmap* temp = (OffsetVarmap*)malloc(sizeof(OffsetVarmap));
			temp->varname = st->assignment->var_name;
			temp->offset = offset;
			tempoffset = offset;
			//printf("offset %d \n",offset);
			offset = offset+8;
			list_push(tablevar,temp);
		}
		emit_expression(st->assignment->expression);
		emit_instr_format("movq","%%rax, -%d(%%rbp)",tempoffset);//result to variable.
	}else if(st->type == VARIABLE){
		OffsetVarmap *temp = (OffsetVarmap*)malloc(sizeof(OffsetVarmap));
		temp->varname = st->variable->var_name;
		temp->offset = offset;
		offset = offset+8;
		list_push(tablevar,temp);
	}else{
		fprintf(fp,"statement error");
	}
}

void emit_expression(Syntax *exp){
	if (exp->type == UNARY_OPERATOR) {
        UnaryExpression *unary_syntax = exp->unary_expression;
		emit_expression(unary_syntax->expression);
        
        if (unary_syntax->unary_type == BITWISE_NEGATION) {
            emit_ins("notq", "%%rax");
        } else {
            emit_ins("testq", "$0xFFFFFFFF, %%rax");
            emit_ins("setz", "%%al");
        }
    } else if (exp->type == IMMEDIATE) {
		//may be fix 64 bit
        emit_instr_format("movq", "$%d, %%rax", exp->immediate->value);

    } else if (exp->type == VARIABLE) {
		int tempoffset = findOffsetVar(exp->variable->var_name);
        emit_instr_format("movq", "-%d(%%rbp), %%rax",tempoffset);

    } else if (exp->type == BINARY_OPERATOR) {
        BinaryExpression *binary_syntax = exp->binary_expression;
		char l[20];
		char r[20];//digit no exceed 20 digit.
		if(binary_syntax->left->type == VARIABLE){
			int os = findOffsetVar(binary_syntax->left->variable->var_name);
			if(os == -1)
				printf("%s not initialize",binary_syntax->left->variable->var_name);
			sprintf(l,"-%d(%%rbp)",os);
		}else if(binary_syntax->left->type == IMMEDIATE){
			sprintf(l,"$%d",binary_syntax->left->immediate->value);
		}else{
			emit_expression(binary_syntax->left);
		}
		
		if(binary_syntax->right->type == VARIABLE){
			int os = findOffsetVar(binary_syntax->right->variable->var_name);
			if(os == -1)
				printf("%s not initialize",binary_syntax->right->variable->var_name);
			sprintf(r,"-%d(%%rbp)",os);
		}else if(binary_syntax->right->type == IMMEDIATE){
			sprintf(r,"$%d",binary_syntax->right->immediate->value);
		}else{
			emit_expression(binary_syntax->right);
		}
		/*
        int stack_offset = ctx->stack_offset;
        ctx->stack_offset -= WORD_SIZE;

        emit_instr(out, "sub", "$4, %esp");
        write_syntax(out, binary_syntax->left, ctx);
        emit_instr_format(out, "mov", "%%eax, %d(%%ebp)", stack_offset);

        write_syntax(out, binary_syntax->right, ctx);
		*/
		emit_instr_format("movq", "%%rax ,%%r8");//bug
		emit_instr_format("movq","%s ,%%rax",l);
        if (binary_syntax->binary_type == MULTIPLICATION) {
            emit_instr_format("imulq", "%s ,%%rax",r);

        } else if (binary_syntax->binary_type == ADDITION) {
            emit_instr_format("addq", "%s ,%%rax",r);

        } else if (binary_syntax->binary_type == SUBTRACTION) {
			emit_instr_format("movq", "%s, %%r10", r);
            emit_ins("subq", "%%r10, %%rax");
            //emit_instr_format("movq", "%s, %%rax", r);

        } else if (binary_syntax->binary_type == DIVIDE) {
			emit_ins("cltd",NULL);
			emit_instr_format("idivl","%s",r);
		} else if (binary_syntax->binary_type == MODULO){
			emit_ins("cltd",NULL);
			emit_instr_format("idivl","%s",r);
			emit_ins("movq","%rdx, %rax");
		} else{
			printf("expression error");
		}

    } 
}

void emit_condition(Syntax* condition,int label){
	if(condition->type == BINARY_OPERATOR){
		BinaryExpression *binary_syntax = condition->binary_expression;
		char l[20];
		char r[20];//digit no exceed 20 digit.
		if(binary_syntax->left->type == VARIABLE){
			int os = findOffsetVar(binary_syntax->left->variable->var_name);
			if(os == -1)
				printf("%s not initialize",binary_syntax->left->variable->var_name);
			sprintf(l,"-%d(%%rbp)",os);
		}else if(binary_syntax->left->type == IMMEDIATE){
			sprintf(l,"$%d",binary_syntax->left->immediate->value);
		}else{
			printf("condition accept only variable or immediate : left operand");
			exit(0);
		}
		emit_instr_format("movq","%s, %%rax",l);
		if(binary_syntax->right->type == VARIABLE){
			int os = findOffsetVar(binary_syntax->right->variable->var_name);
			if(os == -1)
				printf("%s not initialize",binary_syntax->right->variable->var_name);
			sprintf(r,"-%d(%%rbp)",os);
		}else if(binary_syntax->right->type == IMMEDIATE){
			sprintf(r,"$%d",binary_syntax->right->immediate->value);
		}else{
			printf("condition accept only variable or immediate : right operand");
			exit(0);
		}
		
		emit_instr_format("movq","%s, %%r8",r);
		if(binary_syntax->binary_type == LESS_THAN){// l = arg1 r= arg2
			emit_instr_format("cmpq","%%r8, %%rax"); //cmpq arg2, arg1 gas syntax.
			emit_instr_format("jl",".LC%d",label);
		}else if(binary_syntax->binary_type == LESS_THAN_OR_EQUAL){
			emit_instr_format("cmpq","%%r8, %%rax");
			emit_instr_format("jle",".LC%d",label);
		}else if(binary_syntax->binary_type == EQUAL){
			emit_instr_format("cmpq","%%rax, %%r8");
			emit_instr_format("je",".LC%d",label);
		}else if(binary_syntax->binary_type == NOT_EQ){
			emit_instr_format("cmpq","%%rax ,%%r8");
			emit_instr_format("jne",".LC%d",label);
		}else{
			printf("unknown condition");
		}
	}else
		printf("invalid condition");
}

