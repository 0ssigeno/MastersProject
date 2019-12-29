// Generated from /home/ossigeno/IdeaProjects/ComplexStaticAnalysis/src/parser/ComplexStaticAnalysis.g4 by ANTLR 4.7.2
package parser;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link ComplexStaticAnalysisParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface ComplexStaticAnalysisVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#block}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitBlock(ComplexStaticAnalysisParser.BlockContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#statement}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitStatement(ComplexStaticAnalysisParser.StatementContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#assignment}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAssignment(ComplexStaticAnalysisParser.AssignmentContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#deletion}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDeletion(ComplexStaticAnalysisParser.DeletionContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#print}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitPrint(ComplexStaticAnalysisParser.PrintContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#functioncall}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFunctioncall(ComplexStaticAnalysisParser.FunctioncallContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#ifthenelse}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitIfthenelse(ComplexStaticAnalysisParser.IfthenelseContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#declaration}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDeclaration(ComplexStaticAnalysisParser.DeclarationContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#typeNode}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTypeNode(ComplexStaticAnalysisParser.TypeNodeContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#parameter}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitParameter(ComplexStaticAnalysisParser.ParameterContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#expNode}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitExpNode(ComplexStaticAnalysisParser.ExpNodeContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#term}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitTerm(ComplexStaticAnalysisParser.TermContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#factor}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFactor(ComplexStaticAnalysisParser.FactorContext ctx);
	/**
	 * Visit a parse tree produced by {@link ComplexStaticAnalysisParser#value}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitValue(ComplexStaticAnalysisParser.ValueContext ctx);
}