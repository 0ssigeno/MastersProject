package models;

import semanticLib.Environ;
import codeGenLib.AssemblyLib;

public class StmtPrintNode extends StmtNode {


    private ExpNode expNode;

    public StmtPrintNode(ExpNode expNode) {
        this.expNode = expNode;
    }

    @Override
    public String toPrint(String s) {
        return s + "\n Expnode" + this.expNode.toPrint(s);

    }

    @Override
    public TypeNode typeCheck(Environ env) {
        expNode.typeCheck(env);
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        expNode.checkSemantics(env);
    }


    @Override
    public String getID() {
        return null;
    }

    @Override
    public String codGen() {
        return expNode.codGen()+
                AssemblyLib.print();
    }
}
