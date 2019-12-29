// Generated from /home/ossigeno/IdeaProjects/ComplexStaticAnalysis/src/jvmGrammar/Jvm.g4 by ANTLR 4.7.2
package jvmGrammar;
import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link JvmParser}.
 */
public interface JvmListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link JvmParser#assembly}.
	 * @param ctx the parse tree
	 */
	void enterAssembly(JvmParser.AssemblyContext ctx);
	/**
	 * Exit a parse tree produced by {@link JvmParser#assembly}.
	 * @param ctx the parse tree
	 */
	void exitAssembly(JvmParser.AssemblyContext ctx);
	/**
	 * Enter a parse tree produced by {@link JvmParser#instruction}.
	 * @param ctx the parse tree
	 */
	void enterInstruction(JvmParser.InstructionContext ctx);
	/**
	 * Exit a parse tree produced by {@link JvmParser#instruction}.
	 * @param ctx the parse tree
	 */
	void exitInstruction(JvmParser.InstructionContext ctx);
}