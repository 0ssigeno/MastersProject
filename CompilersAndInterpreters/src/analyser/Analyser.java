package analyser;

import jvm.JvmVM;
import jvmGrammar.JvmLexer;
import jvmGrammar.JvmParser;
import jvm.JvmBaseVisitorImpl;
import semanticLib.Environ;
import errors.CodeGenError;
import errors.SemanticError;
import errors.TypeError;
import models.ComplexStaticAnalysisBaseVisitorImpl;
import models.StmtBlockNode;
import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import parser.ComplexStaticAnalysisLexer;
import parser.ComplexStaticAnalysisParser;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class Analyser {

    public static void main(String[] args) {

        String fileName = "test.spl";
        FileInputStream is = null;
        try {
            is = new FileInputStream(fileName);
        } catch (FileNotFoundException e) {
            System.err.println("File not found");
            System.exit(1);
        }
        ANTLRInputStream input = null;
        try {
            input = new ANTLRInputStream(is);
        } catch (IOException e) {
            System.err.println("Input error");
            System.exit(1);
        }
        ComplexStaticAnalysisLexer lexer = new ComplexStaticAnalysisLexer(input);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        ComplexStaticAnalysisParser parser = new ComplexStaticAnalysisParser(tokens);
        parser.setBuildParseTree(true);
        ComplexStaticAnalysisBaseVisitorImpl visitor = new ComplexStaticAnalysisBaseVisitorImpl();
        StmtBlockNode mainBlock = null;
        try {
            mainBlock = (StmtBlockNode) visitor.visitBlock(parser.block());
        } catch (Exception e) {
            System.err.println("Check syntax FAILED");
            System.exit(1);
        }
        if(parser.getNumberOfSyntaxErrors()==0){
            System.out.println("Check syntax CORRECT");
            mainBlock.checkSemantics(new Environ());
            if (SemanticError.getSemanticErrors().size() > 0) {
                System.err.println("Check semantics FAILED");
                for (SemanticError err : SemanticError.getSemanticErrors())
                    System.err.println(err);
                System.exit(1);
            } else {
                System.out.println("Check semantics CORRECT");
                mainBlock.typeCheck(new Environ());
                if (TypeError.getTypeErrors().size() > 0) {
                    System.err.println("Type check FAILED");
                    for (TypeError typeError : TypeError.getTypeErrors()) {
                        System.err.println(typeError.toString());
                    }
                    System.exit(1);
                } else {
                    System.out.println("Type checking CORRECT");
                    String codegen=mainBlock.codGen();
                    if(CodeGenError.getCodeGenErrors().size()>0){
                        System.err.println("Code gen FAILED");
                        for (CodeGenError codeGenError : CodeGenError.getCodeGenErrors()) {
                            System.err.println(codeGenError.toString());
                        }
                        System.exit(1);
                    }else{
                        System.out.println("Assembly Created:");
                        System.out.println(codegen);
                        System.out.println("Code Execution:");
                        CharStream charStream = new ANTLRInputStream(codegen);
                        JvmLexer lexerCodeGen = new JvmLexer (charStream);
                        CommonTokenStream tokenJvm = new CommonTokenStream(lexerCodeGen);
                        JvmParser parserJvm = new JvmParser(tokenJvm);
                        parserJvm.setBuildParseTree(true);
                        JvmBaseVisitorImpl visitorCodeGen = new JvmBaseVisitorImpl();
                        visitorCodeGen.visitAssembly(parserJvm.assembly());
                        JvmVM vm = new JvmVM(visitorCodeGen.code);
                        vm.CPU();


                    }
                    System.exit(0);
                }

            }

        }else{
            System.err.println("Check syntax FAILED");
            System.exit(1);

        }


    }
}
