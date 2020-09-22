%option noyywrap

%{
#include "Parser.h"
int line = 1;
%}


arOpAdd_Token				"+"
arOpMul_Token				"*"
assignOp_Token				"="
relOpSmallerThan_Token		"<"
relOpSmallerEqualThan_Token	"<="
relOpBiggerThan_Token		">"
relOpBiggerEqualThan_Token	">="
relOpEqual_Token			"=="
relOpNotEqual_Token			"!="
intKW_Token					int
floatKW_Token				float
voidKW_Token				void
ifKW_Token					if
returnKW_Token				return
ID_Token					[a-z]([A-Za-z]|[0-9]|_[A-Za-z]|_[0-9])*
intNum_Token				0|[1-9][0-9]*
floatNum_Token				{intNum_Token}"."[0-9]+("E"|"e")("+"|"-")?{intNum_Token}
comma_Token					","
colon_Token					":"
semiColon_Token				";"
leftParentheses_Token		"("
rightParentheses_Token		")"
leftBrackets_Token			"["
rightBrackets_Token			"]"
leftCurlyBraces_Token		"{"
rightCurlyBraces_Token		"}"

startComment_Token	"/*"
%x commentCondition


%%

" "			{}
\t			{}
\r			{}
\n			{line++;}

{arOpAdd_Token}	{ create_and_store_token(arOpAdd_Token,  yytext, line);	fprintf(yyout,"Token of type arOpAdd_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{arOpMul_Token}	{ create_and_store_token(arOpMul_Token,  yytext, line);	fprintf(yyout,"Token of type arOpMul_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{assignOp_Token}	{ create_and_store_token(assignOp_Token,  yytext, line);	fprintf(yyout,"Token of type assignOp_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{relOpSmallerThan_Token}	{ create_and_store_token(relOpSmallerThan_Token,  yytext, line);	fprintf(yyout,"Token of type relOpSmallerThan_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{relOpSmallerEqualThan_Token}	{ create_and_store_token(relOpSmallerEqualThan_Token,  yytext, line);	fprintf(yyout,"Token of type relOpSmallerEqualThan_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{relOpBiggerThan_Token}	{ create_and_store_token(relOpBiggerThan_Token,  yytext, line);	fprintf(yyout,"Token of type relOpBiggerThan_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{relOpBiggerEqualThan_Token}	{ create_and_store_token(relOpBiggerEqualThan_Token,  yytext, line);	fprintf(yyout,"Token of type relOpBiggerEqualThan_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{relOpEqual_Token}	{ create_and_store_token(relOpEqual_Token,  yytext, line);	fprintf(yyout,"Token of type relOpEqual_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{relOpNotEqual_Token}	{ create_and_store_token(relOpNotEqual_Token,  yytext, line);	fprintf(yyout,"Token of type relOpNotEqual_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{intKW_Token}	{ create_and_store_token(intKW_Token,  yytext, line);	fprintf(yyout,"Token of type intKW_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{floatKW_Token}	{ create_and_store_token(floatKW_Token,  yytext, line);	fprintf(yyout,"Token of type floatKW_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{voidKW_Token}	{ create_and_store_token(voidKW_Token,  yytext, line);	fprintf(yyout,"Token of type voidKW_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{ifKW_Token}	{ create_and_store_token(ifKW_Token,  yytext, line);	fprintf(yyout,"Token of type ifKW_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{returnKW_Token}	{ create_and_store_token(returnKW_Token,  yytext, line);	fprintf(yyout,"Token of type returnKW_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{ID_Token}       { create_and_store_token(ID_Token,  yytext, line);	fprintf(yyout,"Token of type ID_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{intNum_Token}       { create_and_store_token(intNum_Token,  yytext, line);	fprintf(yyout,"Token of type intNum_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{floatNum_Token}       { create_and_store_token(floatNum_Token,  yytext, line);	fprintf(yyout,"Token of type floatNum_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{comma_Token}       { create_and_store_token(comma_Token,  yytext, line);	fprintf(yyout,"Token of type comma_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{colon_Token}       { create_and_store_token(colon_Token,  yytext, line);	fprintf(yyout,"Token of type colon_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{semiColon_Token}       { create_and_store_token(semiColon_Token,  yytext, line);	fprintf(yyout,"Token of type semiColon_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{leftParentheses_Token}       { create_and_store_token(leftParentheses_Token,  yytext, line);	fprintf(yyout,"Token of type leftParentheses_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{rightParentheses_Token}       { create_and_store_token(rightParentheses_Token,  yytext, line);	fprintf(yyout,"Token of type rightParentheses_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{leftBrackets_Token}       { create_and_store_token(leftBrackets_Token,  yytext, line);	fprintf(yyout,"Token of type leftBrackets_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{rightBrackets_Token}       { create_and_store_token(rightBrackets_Token,  yytext, line);	fprintf(yyout,"Token of type rightBrackets_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{leftCurlyBraces_Token}       { create_and_store_token(leftCurlyBraces_Token,  yytext, line);	fprintf(yyout,"Token of type leftCurlyBraces_Token, lexeme: %s, found in line: %d.\n", yytext, line);}
{rightCurlyBraces_Token}       { create_and_store_token(rightCurlyBraces_Token,  yytext, line);	fprintf(yyout,"Token of type rightCurlyBraces_Token, lexeme: %s, found in line: %d.\n", yytext, line);}

<<EOF>>	{ create_and_store_token(EOF_Token,  "", line); fprintf(yyout,"Token of type EOF_Token, lexeme: <<EOF>>, found in line: %d.\n", line); BEGIN(0); return 0;}

{startComment_Token}  BEGIN(commentCondition);
<commentCondition>[^*/\n \t/*/*]*
<commentCondition>"\n"	line++;
<commentCondition>" "
<commentCondition>"\t"
<commentCondition>"/*"
<commentCondition>"/"
<commentCondition>"*"
<commentCondition>"*/"  BEGIN(0);
.       			fprintf(yyout, "Character %s in line: %d does not begin any legal token in the language.\n", yytext, line);


%%

void main(int argc, char* argv[])
{
	yyin = fopen("C:\\temp\\test1.txt", "r");
	yyout = fopen("C:\\temp\\test1_206081697_208498444_305191041_lex.txt", "w");
	yyoutParser = fopen("C:\\temp\\test1_206081697_208498444_305191041_syntactic.txt", "w");
	yySemanticOut = fopen("C:\\temp\\test1_206081697_208498444_305191041_semantic.txt", "w");
	yylex();
	repoint_to_flag();
	parse();
	yyrestart(yyin);
	repoint_to_flag();
	fclose(yyin);
	fclose(yyout);
	fclose(yyoutParser);
	fclose(yySemanticOut);


	yyin = fopen("C:\\temp\\test2.txt", "r");
	yyout = fopen("C:\\temp\\test2_206081697_208498444_305191041_lex.txt", "w");
	yyoutParser = fopen("C:\\temp\\test2_206081697_208498444_305191041_syntactic.txt", "w");
	yySemanticOut = fopen("C:\\temp\\test2_206081697_208498444_305191041_semantic.txt", "w");
	yylex();
	repoint_to_flag();
	parse();
	yyrestart(yyin);
	repoint_to_flag();
	fclose(yyin);
	fclose(yyout);
	fclose(yyoutParser);
	fclose(yySemanticOut);
}
