package models;

import semanticLib.Environ;
import errors.TypeError;
import codeGenLib.AssemblyLib;

public class ExpNode implements Node {

    private TermNode leftTermNode;
    private ExpNode rightExpNode;
    private String operator;
    private Boolean isNegative;

    public ExpNode(TermNode leftTermNode, Boolean isNegative) {
        this.leftTermNode = leftTermNode;
        this.isNegative=isNegative;
    }

    public ExpNode(TermNode leftTermNode, ExpNode rightExpNode, String operator, Boolean isNegative) {
        this.leftTermNode = leftTermNode;
        this.rightExpNode = rightExpNode;
        this.operator = operator;
        this.isNegative=isNegative;
    }

    @Override
    public String toPrint(String s) {
        String temp = s + "\nLeft Term Node " + this.leftTermNode.toPrint(s);
        if (this.rightExpNode != null) {
            temp = temp + ", Right Exp Node" + this.rightExpNode.toPrint(s);
        }
        return temp;

    }

    @Override
    public TypeNode typeCheck(Environ env) {
        if (this.rightExpNode == null) {
            return this.leftTermNode.typeCheck(env);
        } else {
            if (this.leftTermNode.typeCheck(env).getType().equals(this.rightExpNode.typeCheck(env).getType())) {
                if (this.leftTermNode.typeCheck(env).getType().equals("int")) {
                    return new TypeNode("int");
                } else {
                    new TypeError(TypeError.printable.OP_NOT_BOOL_ERROR + operator);
                }
            } else {
                new TypeError(TypeError.printable.DIFFERENT_OPERANDS_ERROR + operator);
            }
        }

        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        leftTermNode.checkSemantics(env);
        if (this.rightExpNode != null) {
            rightExpNode.checkSemantics(env);
        }
    }


    public String getID() {
        if (rightExpNode != null) {
            return null;
        }
        return leftTermNode.getID();
    }

    @Override
    public String codGen() {
        String isneg="";
        if(isNegative){
            isneg+=AssemblyLib.loadiT1(0);
            isneg+="subi $a0 0 $a0 \n";
        }

        if(rightExpNode!=null){
            String result="";
            if (operator.equals("+")){
                result="add";
            }else{
                result="sub";
            }

            return leftTermNode.codGen()+
                    isneg +
                    AssemblyLib.push()+
                    rightExpNode.codGen()+
                    AssemblyLib.top()+
                    AssemblyLib.pop()+
                    AssemblyLib.math(result,"$a0","$t1","$a0");

        }

        return leftTermNode.codGen()+isneg ;
    }
}
