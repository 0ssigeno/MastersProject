package models;

import codeGenLib.InfoClasses;
import semanticLib.Environ;
import errors.SemanticError;
import errors.TypeError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

public class StmtAssignmentNode extends StmtNode {

    private String ID;
    private ExpNode expNode;

    public StmtAssignmentNode(String id, ExpNode expNode) {
        this.ID = id;
        this.expNode = expNode;
    }

    @Override
    public String toPrint(String s) {
        return s + "\n StmtAssignmentNode " + this.ID + ", con ExpNode " + this.expNode.toPrint(s);
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        if (!env.getVariableType(this.ID).getType().equals(this.expNode.typeCheck(env).getType())) {

            new TypeError(TypeError.printable.TYPE_ASSIGNMENT_VAR_ERROR.toString());
        }
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        expNode.checkSemantics(env);
        if (!env.containsIDvtable(ID)) {
            new SemanticError(SemanticError.printable.NOT_DECLARED_VAR_ERROR + ID);
        } else {
            //se sono in una funzione
            if (env.getCurrentFunction() != null) {
                //aggiungo ai R/W della funzione la variabile
                env.getFentry(env.getCurrentFunction()).addToRW(ID,env.getVariableLevel(ID),env);

            }
        }
        //se e' gia' stata cancellata e ci sto accedendo
        if (env.isDeletedIDvtable(ID)) {
            new SemanticError(SemanticError.printable.ALREADY_DELETED_VAR_ERROR.toString() + ID);
        }

    }


    @Override
    public String getID() {
        return null;
    }


    @Override
    public String codGen() {
        InfoClasses.VariableInfo variableInfo= CodeGenEnviron.findVariableInfo(ID);
        if (variableInfo.isParameter() && variableInfo.isVar()){
            return expNode.codGen()+
                    AssemblyLib.loopStatic(variableInfo.getLevel())+
                    AssemblyLib.loadAl(variableInfo.getOffset())+  //carico l'indirizzo  in al
                    AssemblyLib.store(0); //metto in a0 e il valore di quell'indirizzo
        }
        return  expNode.codGen()+
                AssemblyLib.loopStatic(variableInfo.getLevel())+
                AssemblyLib.store(variableInfo.getOffset()); //store normale

    }
}
