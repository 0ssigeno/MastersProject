package models;

import semanticLib.Environ;

public class ValueExpNode extends ValueNode {
    private ExpNode exp;

    public ValueExpNode(ExpNode exp) {
        this.exp = exp;
    }

    @Override
    public String toPrint(String s) {
        return s + "\nExpNode" + this.exp.toPrint(s);
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        return this.exp.typeCheck(env);
    }

    @Override
    public void checkSemantics(Environ env) {
        exp.checkSemantics(env);

    }


    @Override
    public String getID() {
        return this.exp.getID();
    }

    @Override
    public String codGen() {
        return exp.codGen();
    }
}
