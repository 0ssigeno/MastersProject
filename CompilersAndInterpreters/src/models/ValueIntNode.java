package models;

import semanticLib.Environ;
import codeGenLib.AssemblyLib;

public class ValueIntNode extends ValueNode {

    private Integer valueInt;

    public ValueIntNode(Integer valueInt) {
        this.valueInt = valueInt;
    }

    @Override
    public String toPrint(String s) {
        return s + "\nValueIntNode " + this.valueInt;
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        return new TypeNode("int");
    }

    @Override
    public void checkSemantics(Environ env) {

    }


    @Override
    public String getID() {
        return null;
    }

    @Override
    public String codGen() {
        return AssemblyLib.loadiA0(valueInt);
    }
}
