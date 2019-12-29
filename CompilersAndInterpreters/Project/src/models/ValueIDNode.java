package models;

import codeGenLib.InfoClasses;
import semanticLib.Environ;
import errors.SemanticError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

public class ValueIDNode extends ValueNode {

    private String ID;

    public ValueIDNode(String ID) {
        this.ID = ID;
    }

    @Override
    public String toPrint(String s) {
        return s + "\nValueIDNode " + this.ID;

    }


    @Override
    public TypeNode typeCheck(Environ env) {
        if (env.getVariableType(this.ID) != null) {
            return env.getVariableType(this.ID);
        }
        if (env.getFunctionType(this.ID) != null) {
            return new TypeNode("void");
        }
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        if (!env.containsIDvtable(ID)) {
            new SemanticError(SemanticError.printable.NOT_DECLARED_VAR_ERROR.toString() + ID);

        } else {
            if (env.getCurrentFunction() != null) {
                env.getFentry(env.getCurrentFunction()).addToRW(ID,env.getVariableLevel(ID),env);

            }
        }
        if (env.isDeletedIDvtable(ID)) {
            new SemanticError(SemanticError.printable.ALREADY_DELETED_VAR_ERROR.toString() + ID);
        }
    }


    @Override
    public String getID() {
        return this.ID;
    }

    @Override
    public String codGen() {
        InfoClasses.VariableInfo variableInfo= CodeGenEnviron.findVariableInfo(ID);
        if (variableInfo.isParameter() && variableInfo.isVar()){
            return AssemblyLib.loopStatic(variableInfo.getLevel())+
                    AssemblyLib.loadAl(variableInfo.getOffset())+ ///carico l'indirizzo
                    AssemblyLib.load(0); //metto il contenuto dell'indirizzo in a0
        }
        return  AssemblyLib.loopStatic(variableInfo.getLevel())+
                AssemblyLib.load(variableInfo.getOffset());

    }
}
