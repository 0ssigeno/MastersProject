package models;

import semanticLib.Environ;
import errors.SemanticError;

public class StmtDeletionNode extends StmtNode {

    private String ID;

    public StmtDeletionNode(String ID) {
        this.ID = ID;
    }


    @Override
    public String toPrint(String s) {
        return s + "\n StmtDeletionNode ID" + this.ID;
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        return new TypeNode("void");
    }

    @Override
    public void checkSemantics(Environ env) {
        //se e' gia' cancellata ho un errore
        if (env.isDeletedIDvtable(ID)) {
            new SemanticError(SemanticError.printable.ALREADY_DELETED_VAR_ERROR.toString() + ID);

        }
        //altrimenti la cancello
        else {
            env.deleteID(ID,0);
        }


    }


    @Override
    public String getID() {
        return null;
    }

    @Override
    public String codGen() {
        return "";

    }
}
