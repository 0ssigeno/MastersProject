package models;

import codeGenLib.AssemblyLib;
import semanticLib.Environ;
import codeGenLib.CodeGenEnviron;

import java.util.ArrayList;

public class StmtBlockNode extends StmtNode {

    private ArrayList<StmtNode> statement_list;
    public static int numStmt=0;
    public StmtBlockNode(ArrayList<StmtNode> statement_list) {
        this.statement_list = statement_list;
    }


    @Override
    public String getID() {
        return null;
    }


    @Override
    public String toPrint(String s) {
        StringBuilder temp = new StringBuilder(s + "\nStart Block Node:");
        for (StmtNode stm : statement_list) {
            temp.append(stm.toPrint(s));
        }
        return temp.toString();
    }


    @Override
    public TypeNode typeCheck(Environ env) {
        env.openScope();
        Environ.typeCheckEnviron=new Environ(env);
        for (StmtNode stmt : this.statement_list) {
            stmt.typeCheck(env);
        }
        env.closeScope();
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        env.openScope();
        for (StmtNode stm : statement_list) {
            //conto gli statmt per sapere se accedo ad una variabile prima in lettura o scrittura
            StmtBlockNode.numStmt+=1;
            stm.checkSemantics(env);
        }
        env.closeScope();
    }

    @Override
    public String codGen() {
        CodeGenEnviron.nextLevel();
        StringBuilder result= new StringBuilder();
        boolean amIMain= false;
        boolean amIinsideFunction= CodeGenEnviron.amIinsideFunction;
        if(CodeGenEnviron.mainBlock){
            CodeGenEnviron.mainBlock=false;
            amIMain=true;
        }
        if(!amIinsideFunction){ //non sono dentro
            result.append(AssemblyLib.pushReg("$fp"))
                    .append(AssemblyLib.mv("$al","$fp"))
                    .append(AssemblyLib.pushReg("$al"))
                    .append(AssemblyLib.mv("$fp","$sp"))
                    .append(AssemblyLib.pushReg("$a0"));//thrash, per evitare casini con apertura funzioni

        }
        CodeGenEnviron.amIinsideFunction=false;
        for(StmtNode node: statement_list){
            result.append(node.codGen());

        }
        int countVar=CodeGenEnviron.getVars();
        result.append(AssemblyLib.addi("$sp",countVar*4)); //tolgo l'allocazione delle variabili
        if(!amIinsideFunction){ //non ci sono dentro
                    result.append(AssemblyLib.pop())
                    .append(AssemblyLib.pop())
                    .append(AssemblyLib.topReg("$fp"))
                    .append(AssemblyLib.pop());

        }

        CodeGenEnviron.prevLevel();
        if(amIMain){

            return AssemblyLib.jump("start")+CodeGenEnviron.functionDefinitions+AssemblyLib.startLabel("start")+result.toString()+ AssemblyLib.halt();
        }else{
            return result.toString();
        }
    }

}
