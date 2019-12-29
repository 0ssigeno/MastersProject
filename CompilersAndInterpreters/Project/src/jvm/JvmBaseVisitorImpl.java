package jvm;

import jvmGrammar.JvmBaseVisitor;
import jvmGrammar.JvmLexer;
import jvmGrammar.JvmParser;

import java.util.HashMap;

public class JvmBaseVisitorImpl extends JvmBaseVisitor {
    public int[] code = new int[JvmVM.CODESIZE];
    private int i = 0;
    private HashMap<String,Integer> labelAdd = new HashMap<String,Integer>();
    private HashMap<Integer,String> labelRef = new HashMap<Integer,String>();

    @Override
    public Void visitAssembly(JvmParser.AssemblyContext ctx) {
        visitChildren(ctx);
        for (Integer refAdd: labelRef.keySet()) {
            code[refAdd]=labelAdd.get(labelRef.get(refAdd));
        }
        return null;
    }

    @Override
    public Void visitInstruction(JvmParser.InstructionContext ctx) {
        switch (ctx.getStart().getType()) {
            case JvmLexer.PUSH:
                if(ctx.r1 != null) {
                    code[i++] = JvmParser.PUSH;
                    code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                }
                break;
            case JvmLexer.POP:
                code[i++] = JvmParser.POP;
                break;
            case JvmLexer.ADD:
                code[i++] = JvmParser.ADD;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r3.getText());
                break;
            case JvmLexer.ADDI:
                code[i++] = JvmParser.ADDI;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                code[i++] = Integer.parseInt(ctx.n.getText());
                break;
            case JvmLexer.SUB:
                code[i++] = JvmParser.SUB;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r3.getText());
                break;
            case JvmLexer.MULT:
                code[i++] = JvmParser.MULT;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r3.getText());
                break;
            case JvmLexer.DIV:
                code[i++] = JvmParser.DIV;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r3.getText());
                break;
            case JvmLexer.STOREW:
                code[i++] = JvmParser.STOREW;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = Integer.parseInt(ctx.n.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                break;
            case JvmLexer.LOADW:
                code[i++] = JvmParser.LOADW;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = Integer.parseInt(ctx.n.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                break;
            case JvmLexer.LOADI :
                code[i++] = JvmParser.LOADI;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = Integer.parseInt(ctx.n.getText());
                break;
            case JvmLexer.LABEL:
                labelAdd.put(ctx.l.getText(), i);
                break;
            case JvmLexer.TOP:
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                break;
            case JvmLexer.BRANCH:
                code[i++] = JvmParser.BRANCH;
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.BRANCHEQ:
                code[i++] = JvmParser.BRANCHEQ;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.BRANCHNE:
                code[i++] = JvmParser.BRANCHNE;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.BRANCHLE:
                code[i++] = JvmParser.BRANCHLE;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.BRANCHLT:
                code[i++] = JvmParser.BRANCHLT;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.BRANCHGT:
                code[i++] = JvmParser.BRANCHGT;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.BRANCHGE:
                code[i++] = JvmParser.BRANCHGE;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.MOVE:
                code[i++] = JvmParser.MOVE;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                code[i++] = JvmEnv.registerToCode(ctx.r2.getText());
                break;
            case JvmLexer.JL:
                code[i++] = JvmParser.JL;
                labelRef.put(i++,(ctx.l!=null? ctx.l.getText():null));
                break;
            case JvmLexer.JR:
                code[i++] = JvmParser.JR;
                code[i++] = JvmEnv.registerToCode(ctx.r1.getText());
                break;
            case JvmLexer.PRINT:
                code[i++] = JvmParser.PRINT;
                break;
            case JvmLexer.HALT:
                code[i++] = JvmParser.HALT;
                break;
            default:
                break;
        }
        return null;
    }



}
