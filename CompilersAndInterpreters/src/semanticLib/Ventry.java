package semanticLib;


import models.TypeNode;

public class Ventry {

    private TypeNode typeNode;
    private boolean deleted;

    public Ventry(TypeNode typeNode) {
        this.typeNode = typeNode;
        this.deleted = false;
    }

    public Ventry(Ventry dummyentry) {
        this.typeNode = dummyentry.typeNode;
        this.deleted = dummyentry.deleted;
    }


    public boolean isDeleted() {
        return deleted;
    }

    public void setDeleted(boolean deleted) {
        this.deleted = deleted;
    }

    public TypeNode getTypeNode() {
        return this.typeNode;
    }


}