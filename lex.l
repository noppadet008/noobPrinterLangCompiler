
D		[0-9]
L		[a-zA-Z_]

%{
    /* enum for test flex
    enum{
    NUM = 1,
    HEX,
    STRING,
    IDEN,
    LP,
    RP,
    OB,
    CB,
    MINUS,
    ADD,
    MUL,
    DIVIDE,
    MOD,
    NEWLINE,
    ASSIGN,
    PRINT,
    PRINT16,
    EQUAL,
    MORETHAN,
    LESSTHAN,
    WHEN,
    LOOP,
    SEMICOLON};
    */
    #include "comp.tab.h"
	#include <stdio.h>
	#include <math.h>
    #include <string.h>
	int binary_decimal(char *);
	int hex_decimal(char *);
    void comment();
    char* normstr(char* str);
%}



%option noyywrap
%%
"^_^"                    { comment(); }
[ \t\n]+                 { /* Ignore whitespace */ }
"^^"[^\n]*               { /* one line comments. */ }

{D}+          	  	     { yylval.num = atoll(yytext); return NUM; }
[x][0-9A-Fa-f]+  		 { yylval.num = hex_decimal(yytext); return HEX; }
[$]{L}({L}|{D})*         { yylval.str = strdup(yytext); return IDEN;}
"when"                   { return WHEN;}
"loop"                   { return LOOP;}
"printer"          	 	 { return PRINT; }
"printer16"              { return PRINT16;}

"("						 { return LP; }
")"						 { return RP; }
"{"                      { return OB; }
"}"                      { return CB; }
"-"						 { return MINUS; }
"+"						 { return ADD; }
"*"						 { return MUL; }
"/"						 { return DIV; }
"%"						 { return MOD; }
"="						 { return ASSIGN; }

"#"						 { return EQ; }
">"                      { return MORETHAN;}
"<"                      { return LESSTHAN;}
["]({L}|[^\n])*["]        { yylval.str = normstr(yytext); return STRING;}
";"                      { return SEMICOLON;}
.						 { }


%%
/* converter is bug */
int binary_decimal(char *input)
{
	int i = strlen(input)-1;
	int total = 0;
	for(i ; i>0 ; i--)
	{
		total += (input[i-1]-'0') * pow(2,strlen(input)-1-i);
	}
	
	return total;
}

int hex_decimal(char *input)
{
	long long result = 0;
    char temp;
    if(input+1) {//skip x such as xA3
            temp = *input;
    }
    while(temp != 0){// '/0' is end
        result = result << 4;
        if(temp >= '0' && temp <= '9' ) result += temp-'0';
        else if(temp >= 'A' && temp <= 'F') result += temp-'A'+10;
        else if(temp >= 'a' && temp <= 'f') result += temp-'a'+10;
        input++;
        temp = *input;
    }
    return result;
}

#define INPUT_EOF 0

void comment(void) {
    /* Consume characters up to the closing comment marker. */
    char c, prev,prev2 = 0;
  
    while ((c = input()) != INPUT_EOF) {
        if (c == '^' && prev == '_' && prev2 == '^')
            return;
        prev2 = prev;
        prev = c;
        
        //printf("%c%c%c\n",prev2,prev,c);
    }
    //yyerror("unterminated comment");
}

char* normstr(char* str){
    int i = 0;
    char* newstr = (char*)malloc(sizeof(str-2));
    for(;i<strlen(str)-2;i++){//index 0-(length-1) -1 head  -1 tail  +1 /0
        *(newstr+i) = *(str+i+1);
    }
    return newstr;
}

/* test flex */
/*commented 
int main(){
    int tokenNumber;
	while(tokenNumber = yylex()){
        printf("%d ",tokenNumber);
		if(tokenNumber == 260)
			printf("%s ",yylval.str);
		if(tokenNumber == 258 || tokenNumber == 259)
			printf("%d ",yylval.num);
		printf("\n");
        }
	
    return 0;
}

*/

