package models;

import semanticLib.Environ;
import errors.TypeError;
import codeGenLib.AssemblyLib;

public class TermNode implements Node {

    private FactorNode leftFactorNode;
    private TermNode rightTermNode;
    private String operator;

    public TermNode(FactorNode leftFactorNode) {
        this.leftFactorNode = leftFactorNode;
    }

    public TermNode(FactorNode leftFactorNode, TermNode rightTermNode, String operator) {
        this.leftFactorNode = leftFactorNode;
        this.rightTermNode = rightTermNode;
        this.operator = operator;
    }

    @Override
    public String toPrint(String s) {
        String left = s + "\n Nodo sinistro: FactorNode " + this.leftFactorNode.toPrint(s);
        if (this.rightTermNode != null) {
            left = left + operator + " Nodo destro TermNode " + this.rightTermNode.toPrint(s);
        }
        return left;
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        if (this.rightTermNode == null) {
            return this.leftFactorNode.typeCheck(env);
        } else {
            if (this.leftFactorNode.typeCheck(env).getType().equals(this.rightTermNode.typeCheck(env).getType())) {
                if (this.leftFactorNode.typeCheck(env).getType().equals("int")) {
                    return new TypeNode("int");
                } else {
                    new TypeError(TypeError.printable.OP_NOT_BOOL_ERROR.toString() + operator);

                }
            } else {
                new TypeError(TypeError.printable.DIFFERENT_OPERANDS_ERROR.toString() + operator);
            }
            return new TypeNode("void");
        }

    }

    @Override
    public void checkSemantics(Environ env) {
        leftFactorNode.checkSemantics(env);
        if (this.rightTermNode != null) {

            rightTermNode.checkSemantics(env);
        }

    }


    @Override
    public String getID() {
        if (rightTermNode != null) {
            return null;
        }
        return leftFactorNode.getID();
    }

    @Override
    public String codGen() {
        if(rightTermNode!=null){
            String result;
            if (operator.equals("*")){
                result="mult";
            }else{
                result="div";
            }
            return leftFactorNode.codGen() +
                    AssemblyLib.push()+
                    rightTermNode.codGen() +
                    AssemblyLib.top()+
                    AssemblyLib.pop()+
                    AssemblyLib.math(result,"$a0","$t1","$a0");
        }
        return  leftFactorNode.codGen();
    }
}
