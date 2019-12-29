package models;

import semanticLib.Environ;

public class ParameterNode implements Node {

    private Boolean isReference;
    private TypeNode typeNode;
    private String id;


    public ParameterNode(Boolean isReference, String type, String id) {
        this.isReference = isReference;
        this.typeNode = new TypeNode(type);
        this.id = id;
    }


    public TypeNode getTypeNode() {
        return typeNode;
    }


    public Boolean getReference() {
        return isReference;
    }


    @Override
    public String toPrint(String s) {
        return s + "\n Is reference " + this.isReference + ", Tipo " + this.typeNode.toPrint("") + ",Id " + this.id;
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        return this.typeNode;
    }

    @Override
    public void checkSemantics(Environ env) {
        typeNode.checkSemantics(env);

    }


    @Override
    public String getID() {
        return id;
    }

    @Override
    public String codGen() {
        return "";

    }
}
