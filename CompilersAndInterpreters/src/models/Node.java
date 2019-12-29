package models;

import semanticLib.Environ;

public interface Node {

    String toPrint(String indent);

    TypeNode typeCheck(Environ env);

    void checkSemantics(Environ env);


    String getID();

    String codGen();
}  