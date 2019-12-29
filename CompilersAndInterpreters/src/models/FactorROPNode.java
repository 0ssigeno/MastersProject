package models;

import semanticLib.Environ;
import errors.TypeError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

public class FactorROPNode extends FactorNode {

    private ValueNode leftValueNode;
    private ValueNode rightValueNode;
    private String ropOperator;

    public FactorROPNode(ValueNode leftValueNode) {
        this.leftValueNode = leftValueNode;
    }

    public FactorROPNode(ValueNode leftValueNode, ValueNode rightValueNode, String ropOperator) {
        this.leftValueNode = leftValueNode;
        this.rightValueNode = rightValueNode;
        this.ropOperator = ropOperator;
    }

    @Override
    public String toPrint(String s) {
        String temp = s + "\n Nodo sinistro:" + this.leftValueNode.toPrint(s);
        if (this.rightValueNode != null) {
            temp = temp + ", Operatore " + this.ropOperator + ", ValueNode destro " + this.rightValueNode.toPrint(s);
        }
        return temp;
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        if (this.rightValueNode == null) {
            return this.leftValueNode.typeCheck(env);
        } else {
            if (this.leftValueNode.typeCheck(env).getType().equals(this.rightValueNode.typeCheck(env).getType())) {
                return new TypeNode("bool");
            } else {
                new TypeError(TypeError.printable.DIFFERENT_OPERANDS_ERROR + ropOperator);
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
        if (rightValueNode != null) {
            return null;
        }
        return leftValueNode.getID();
    }


    @Override
    public String codGen() {
        if(rightValueNode!=null){
            String trueFresh= CodeGenEnviron.freshLabel("true");
            String endFresh= CodeGenEnviron.freshLabel("end");
            String result="b";
            switch (ropOperator){
                case "==":
                    result+="eq";
                    break;
                case ">=":
                    result+="ge";
                    break;
                case ">":
                    result+="gt";
                    break;
                case"<=":
                    result+="le";
                    break;
                case "<":
                    result+="lt";
                    break;
                case "!=":
                    result+="ne";
                    break;
                default:
                    break;
            }
            return leftValueNode.codGen()+
                    AssemblyLib.push()+
                    rightValueNode.codGen()+
                    AssemblyLib.top()+
                    AssemblyLib.pop()+
                    AssemblyLib.jumpConditional(result ,"$t1","$a0",trueFresh)+
                    AssemblyLib.loadiA0(0)+
                    AssemblyLib.jump(endFresh)+
                    AssemblyLib.startLabel(trueFresh)+
                    AssemblyLib.loadiA0(1)+
                    AssemblyLib.startLabel(endFresh);

        }
        return leftValueNode.codGen();
    }
}
