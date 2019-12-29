package models;

import semanticLib.Environ;
import errors.SemanticError;
import errors.TypeError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

public class StmtIfElseNode extends StmtNode {


    private ExpNode conditionExpNode;
    private StmtBlockNode thenStmtBlockNode;
    private StmtBlockNode elseStmtBlockNode;


    public StmtIfElseNode(ExpNode conditionExpNode, StmtBlockNode thenStmtBlockNode, StmtBlockNode elseStmtBlockNode) {
        this.conditionExpNode = conditionExpNode;
        this.thenStmtBlockNode = thenStmtBlockNode;
        this.elseStmtBlockNode = elseStmtBlockNode;
    }

    @Override
    public String toPrint(String s) {
        return s + "\n Condizione " + this.conditionExpNode.toPrint(s) + ", blocco If " + this.thenStmtBlockNode.toPrint(s) + "| blocco Else" + this.elseStmtBlockNode.toPrint(s);
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        if (this.conditionExpNode.typeCheck(env).getType().equals("bool")) {
            if (this.thenStmtBlockNode.typeCheck(env).getType().equals(this.elseStmtBlockNode.typeCheck(env).getType())) {
                return new TypeNode("void");
            } else {
                new TypeError(TypeError.printable.IF_ELSE_ERROR.toString());
            }
        } else {
            new TypeError(TypeError.printable.IF_EXP_NOT_BOOL_ERROR.toString());
        }
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        conditionExpNode.checkSemantics(env);
        Environ copyEnv = new Environ(env);
        thenStmtBlockNode.checkSemantics(env);
        elseStmtBlockNode.checkSemantics(copyEnv);
        if (!env.equals(copyEnv)) {
            new SemanticError(SemanticError.printable.IF_ELSE_ERROR.toString());
        }

    }


    @Override
    public String getID() {
        return null;
    }

    @Override
    public String codGen() {
        String ifFresh= CodeGenEnviron.freshLabel("then");
        String endFresh= CodeGenEnviron.freshLabel("else");
        return conditionExpNode.codGen()+
                AssemblyLib.loadiT1(1)+
                AssemblyLib.jumpConditional("beq","$a0","$t1",ifFresh)+
                elseStmtBlockNode.codGen() +
                AssemblyLib.jump(endFresh)+
                AssemblyLib.startLabel(ifFresh)+
                thenStmtBlockNode.codGen() +
                AssemblyLib.startLabel(endFresh);
    }
}
