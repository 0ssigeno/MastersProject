package models;

import parser.ComplexStaticAnalysisBaseVisitor;
import parser.ComplexStaticAnalysisParser.*;

import java.util.ArrayList;


public class ComplexStaticAnalysisBaseVisitorImpl extends ComplexStaticAnalysisBaseVisitor<Node> {


    @Override
    public Node visitBlock(BlockContext ctx) {
        ArrayList<StmtNode> listaStatement = new ArrayList<>();
        for (StatementContext stmCtx : ctx.statement()) {
            listaStatement.add((StmtNode) visit(stmCtx));
        }
        return new StmtBlockNode(listaStatement);
    }


    @Override
    public Node visitStatement(StatementContext ctx) {
        return visit(ctx.getChild(0));
    }


    @Override
    public Node visitAssignment(AssignmentContext ctx) {
        return new StmtAssignmentNode(ctx.ID().getText(), (ExpNode) visitExp(ctx.exp()));
    }


    @Override
    public Node visitDeletion(DeletionContext ctx) {
        return new StmtDeletionNode(ctx.ID().getText());
    }


    @Override
    public Node visitPrint(PrintContext ctx) {
        return new StmtPrintNode((ExpNode) visitExp(ctx.exp()));
    }


    @Override
    public Node visitFunctioncall(FunctioncallContext ctx) {
        ArrayList<ExpNode> listaExp = new ArrayList<>();
        for (ExpContext exp : ctx.exp()) {
            listaExp.add((ExpNode) visitExp(exp));
        }
        return new StmtFunctionCallNode(ctx.ID().getText(), listaExp);
    }


    @Override
    public Node visitIfthenelse(IfthenelseContext ctx) {
        return new StmtIfElseNode((ExpNode) visitExp(ctx.exp()), (StmtBlockNode) visitBlock(ctx.block(0)), (StmtBlockNode) visitBlock(ctx.block(1)));
    }


    @Override
    public Node visitDeclaration(DeclarationContext ctx) {
        if (ctx.type() != null) {
            //variable declaration
            return new StmtDeclarationVarNode(ctx.type().getText(), ctx.ID().getText(), (ExpNode) visitExp(ctx.exp()));
        } else {
            //function declaration
            ArrayList<ParameterNode> listaParam = new ArrayList<>();
            for (ParameterContext par : ctx.parameter()) {
                listaParam.add((ParameterNode) visitParameter(par));
            }
            return new StmtDeclarationFuncNode(ctx.ID().getText(), listaParam, (StmtBlockNode) visitBlock(ctx.block()));
        }
    }


    @Override
    public Node visitType(TypeContext ctx) {
        return new TypeNode(ctx.getText());
    }


    @Override
    public Node visitParameter(ParameterContext ctx) {
        if (ctx.getText().startsWith("var")) {
            return new ParameterNode(true, ctx.type().getText(), ctx.ID().getText());
        } else {
            return new ParameterNode(false, ctx.type().getText(), ctx.ID().getText());
        }
    }


    @Override
    public Node visitExp(ExpContext ctx) {
        Boolean isNegative= ctx.getText().startsWith("-");
        if (ctx.right == null) {
            return new ExpNode((TermNode) visitTerm(ctx.left),isNegative);
        } else {
            return new ExpNode((TermNode) visitTerm(ctx.left), (ExpNode) visitExp(ctx.right), ctx.op.getText(),isNegative);
        }
    }


    @Override
    public Node visitTerm(TermContext ctx) {
        if (ctx.right == null) {
            return new TermNode((FactorNode) visitFactor(ctx.left));
        } else {
            return new TermNode((FactorNode) visitFactor(ctx.left), (TermNode) visitTerm(ctx.right), ctx.op.getText());
        }
    }


    @Override
    public Node visitFactor(FactorContext ctx) {
        if (ctx.op != null) {
            if (ctx.ROP() != null) {
                return new FactorROPNode((ValueNode) visitValue(ctx.left), (ValueNode) visitValue(ctx.right), ctx.op.getText());
            } else {
                return new FactorBOOLNode((ValueNode) visitValue(ctx.left), (ValueNode) visitValue(ctx.right), ctx.op.getText());
            }
        } else {
            return new FactorROPNode((ValueNode) visitValue(ctx.left));
        }

    }


    @Override
    public Node visitValue(ValueContext ctx) {
        if (ctx.ID() != null) {
            return new ValueIDNode(ctx.ID().getText());
        } else if (ctx.INTEGER() != null) {
            return new ValueIntNode(Integer.parseInt(ctx.INTEGER().getText()));
        } else if (ctx.getText().equals("true") || ctx.getText().equals("false")) {
            return new ValueBoolNode(Boolean.parseBoolean(ctx.getText()));
        } else if (ctx.exp() != null) {
            return new ValueExpNode((ExpNode) visitExp(ctx.exp()));
        }
        return null;
    }

}
