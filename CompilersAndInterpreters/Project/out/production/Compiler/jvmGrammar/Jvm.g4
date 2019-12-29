grammar Jvm;


//parser
assembly: (instruction)* ;

instruction:
    ( PUSH r1= REGISTER
	  | POP
	  | ADD r1 = REGISTER r2 = REGISTER r3 =REGISTER
	  | ADDI r1 = REGISTER r2 = REGISTER n = NUMBER
	  | SUB r1 = REGISTER r2 = REGISTER r3 =REGISTER
	  | MULT r1 = REGISTER r2 = REGISTER r3 =REGISTER
	  | DIV r1 = REGISTER r2 = REGISTER r3 =REGISTER
	  | STOREW r1 = REGISTER n = NUMBER '(' r2 = REGISTER ')'
	  | LOADW r1 =  REGISTER n = NUMBER '(' r2 = REGISTER ')'
	  | LOADI r1 =  REGISTER n = NUMBER
	  | TOP r1=REGISTER
      | BRANCH l=LABEL
	  | BRANCHNE r1 = REGISTER r2 =REGISTER l=LABEL
      | BRANCHEQ r1 = REGISTER r2 =REGISTER l=LABEL
	  | BRANCHGT r1 = REGISTER r2 =REGISTER  l=LABEL
	  | BRANCHGE r1 = REGISTER r2 =REGISTER  l=LABEL
	  | BRANCHLT r1 = REGISTER r2 =REGISTER  l=LABEL
	  | BRANCHLE r1 = REGISTER r2 =REGISTER  l=LABEL
	  | MOVE r1 = REGISTER r2 =REGISTER
	  | JR r1=REGISTER
	  | JL l=LABEL
	  | l=LABEL  COL
      | PRINT
	  | HALT
	  ) ;

//Lexer
PUSH            : 'push' ;
POP             : 'pop' ;
MOVE            : 'mv';
ADD             : 'add';
ADDI            : 'addi';
TOP             : 'top';
SUB             : 'sub' ;
MULT            : 'mult' ;
DIV             : 'div' ;
STOREW	        : 'sw' ;
LOADW	        : 'lw' ;
LOADI           : 'li';
BRANCH	        : 'b' ;
BRANCHEQ        : 'beq' ;
BRANCHNE        : 'bne' ;
BRANCHLT        : 'blt' ;
BRANCHLE        : 'ble' ;
BRANCHGT        : 'bgt';
BRANCHGE        : 'bge';
JL	            : 'jl' ;
JR              : 'jr';
PRINT	        : 'print' ;
HALT	        : 'halt' ;
COL	            : ':' ;
REGISTER: '$a0' | '$t1' | '$al' | '$ra'| '$fp' | '$sp';
LABEL	 : ('a'..'z'|'A'..'Z')('a'..'z' | 'A'..'Z' | '0'..'9')* ;
NUMBER	 : '0' | ('-')?(('1'..'9')('0'..'9')*) ;

WHITESP  : ( '\t' | ' ' | '\r' | '\n' )+   -> skip;

ERR     	: .  -> channel(HIDDEN) ;
