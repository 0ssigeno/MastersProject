package models;

import semanticLib.Environ;

public class TypeNode implements Node {


    private String type;


    public TypeNode(String type) {
        this.type = type;
    }


    public String getType() {
        return type;
    }

    @Override
    public String toPrint(String s) {
        return s + "\n" + type;


    }

    @Override
    public TypeNode typeCheck(Environ env) {
        return this;
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
        //return type+ "\n";
        return"";
    }
}
