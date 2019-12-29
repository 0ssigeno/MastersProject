package models;

import semanticLib.Environ;
import semanticLib.Ventry;
import errors.SemanticError;
import errors.TypeError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

public class StmtDeclarationVarNode extends StmtDeclarationNode {

    private TypeNode typeNode;
    private String ID;
    private ExpNode expNode;

    public StmtDeclarationVarNode(String type, String ID, ExpNode expNode) {
        this.typeNode = new TypeNode(type);
        this.ID = ID;
        this.expNode = expNode;
    }

    @Override
    public String toPrint(String s) {
        return s + "\nTypeNode " + this.typeNode.toPrint(s) + ", ID " + this.ID + ", ExpNode" + this.expNode.toPrint(s);

    }

    @Override
    public TypeNode typeCheck(Environ env) {
        env.addVariable(this.ID, new Ventry(this.typeNode));
        if (this.typeNode.getType().equals(this.expNode.typeCheck(env).getType())) {
            return new TypeNode("void");
        } else {

            new TypeError(TypeError.printable.TYPE_DECLARATION_VAR_ERROR + ID);
        }
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        //se  c'e' una variabile o una funzione con lo stesso nome
        if (env.containsThisLevelIDvtable(ID) || env.containsThisLevelIDftable(ID)) {
            Ventry ventry = env.getVentry(ID,0);
            //se e' cancellato
            if (ventry.isDeleted()) {
                //lo risetto a true senza farci nulla
                ventry.setDeleted(false);
                expNode.checkSemantics(env);
                //se non e' cancellato ho un errore
            } else {
                new SemanticError(SemanticError.printable.ALREADY_DECLARED_VAR_ERROR + ID);
            }
        //se non c'e' una variabile o una funzione con lo stesso nome
        } else {
            expNode.checkSemantics(env);
            //me lo aggiungo
            env.addVariable(ID, new Ventry(typeNode));
        }
    }


    @Override
    public String getID() {
        return null;
    }


    @Override
    public String codGen() {
        CodeGenEnviron.addVariable(ID,typeNode.getType());

        return expNode.codGen()+
                AssemblyLib.push();

    }
}
