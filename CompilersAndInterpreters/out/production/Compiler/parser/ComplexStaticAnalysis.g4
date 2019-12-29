grammar ComplexStaticAnalysis;

// PARSER RULES

block		: '{' statement* '}';

statement	: assignment ';' 
		  	| deletion ';' 
		  	| print ';'
		  	| functioncall ';'
		  	| ifthenelse
		  	| declaration 
		  	| block ;

assignment	: ID '=' exp ;

deletion	: 'delete' ID ;

print		: 'print' exp ;

functioncall : ID '(' (exp (',' exp)* )?  ')' ;

ifthenelse 	: 'if' '(' exp ')' 'then' block 'else' block ;

declaration	: type ID '=' exp ';'
		  	| ID '(' ( parameter ( ',' parameter)* )? ')' block ;

type   		: 'int'
        	  | 'bool'  ;  

parameter  	: ('var')? type ID ;

exp    		:  ('-')? left=term (op=('+' | '-') right=exp)? ;
   
term   		: left=factor (op=('*' | '/') right=term)? ;
   
factor 		: left=value (op=ROP right=value)?
	        |   left=value (op=('&&' | '||') right=value)? ;
   
value  		: INTEGER
     		  | ( 'true' | 'false' )
      		  | '(' exp ')'
		      | ID ;
    
// LEXER RULES

ROP     : '==' | '>' | '<' | '<=' | '>=' | '!=' ;

//Numbers
fragment DIGIT : '0'..'9';    
INTEGER       : DIGIT+;

//IDs
fragment CHAR  : 'a'..'z' |'A'..'Z' ;
ID              : CHAR (CHAR | DIGIT)* ;

//ESCAPE SEQUENCES
WS              : (' '|'\t'|'\n'|'\r')-> skip ;
LINECOMENTS    	: '//' (~('\n'|'\r'))* -> skip ;
BLOCKCOMENTS    : '/*'( ~('/'|'*')|'/'~'*'|'*'~'/'|BLOCKCOMENTS)* '*/' -> skip ;
ERR     	: .  -> channel(HIDDEN) ; 
