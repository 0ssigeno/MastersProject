package models;

import semanticLib.Environ;
import errors.TypeError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

public class FactorBOOLNode extends FactorNode {

    private ValueNode leftValueNode;
    private ValueNode rightValueNode;
    private String boolOperator;


    public FactorBOOLNode(ValueNode leftValueNode, ValueNode rightValueNode, String boolOperator) {
        this.leftValueNode = leftValueNode;
        this.rightValueNode = rightValueNode;
        this.boolOperator = boolOperator;
    }


    @Override
    public String toPrint(String s) {
        return s + "\nLeft Value Node " + this.leftValueNode.toPrint(s) + ", Operator " + this.boolOperator + ", Right Value Node " + this.rightValueNode.toPrint(s);
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        if (this.rightValueNode == null) {
            return this.leftValueNode.typeCheck(env);
        } else {
            if (this.leftValueNode.typeCheck(env).getType().equals(this.rightValueNode.typeCheck(env).getType())) {
                if (this.leftValueNode.typeCheck(env).getType().equals("bool")) {
                    return new TypeNode("bool");
                } else {
                    new TypeError(TypeError.printable.OP_NOT_INT_ERROR + boolOperator);
                }
            } else {
                new TypeError(TypeError.printable.DIFFERENT_OPERANDS_ERROR + boolOperator);
            }
        }
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        leftValueNode.checkSemantics(env);
        if (this.rightValueNode != null) {
            rightValueNode.checkSemantics(env);
        }
    }


    @Override
    public String getID() {
        return null;
    }

    @Override
    public String codGen() {
        if(rightValueNode!=null){
            String endFresh= CodeGenEnviron.freshLabel("end");
            int result = -1;
            switch (boolOperator){
                case "&&":
                    result=0;
                    break;
                case "||":
                    result=1;
                    break;
                default:
                    break;
            }
            return leftValueNode.codGen()+
                    AssemblyLib.loadiT1(result)+
                    AssemblyLib.jumpConditional("beq","$a0","$t1",endFresh)+
                    rightValueNode.codGen()+
                    AssemblyLib.startLabel(endFresh);
        }
        return leftValueNode.codGen();

    }
}
