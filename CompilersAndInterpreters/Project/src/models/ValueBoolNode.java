package models;

import semanticLib.Environ;
import codeGenLib.AssemblyLib;

public class ValueBoolNode extends ValueNode {

    private Boolean valueBool;

    public ValueBoolNode(Boolean boolVal) {
        this.valueBool = boolVal;
    }

    @Override
    public String toPrint(String s) {
        return s + "\nValueBoolNode" + this.valueBool;
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        return new TypeNode("bool");
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
        if(valueBool){
            return AssemblyLib.loadiA0(1);
        }
        else{
            return AssemblyLib.loadiA0(0);
        }

    }
}
