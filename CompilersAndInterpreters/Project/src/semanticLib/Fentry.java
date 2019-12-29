package semanticLib;

import models.ParameterNode;
import models.StmtBlockNode;

import java.util.ArrayList;


public class Fentry {

    private ArrayList<ParameterNode> functionParameterNodes;
    private boolean isDeleted;

    private ArrayList<NameAndPosition> parameterRW = new ArrayList<>();
    private ArrayList<NameAndPosition> parameterD = new ArrayList<>();

    private ArrayList<NameAndPosition> globalD = new ArrayList<>();
    private ArrayList<NameAndPosition> globalRW = new ArrayList<>();


    public Fentry(ArrayList<ParameterNode> functionParameterNodes) {
        this.functionParameterNodes = functionParameterNodes;
        this.isDeleted = false;
    }

    public Fentry(Fentry dummyentry) {
        this.functionParameterNodes = dummyentry.functionParameterNodes;
        this.isDeleted = dummyentry.isDeleted;
        this.globalD = dummyentry.globalD;
        this.parameterD = dummyentry.getParameterD();
        this.parameterRW = dummyentry.getParameterRW();
        this.globalRW=dummyentry.globalRW;
    }

    public ArrayList<NameAndPosition> getParameterD() {
        return parameterD;
    }

    public void setParameterD(ArrayList<NameAndPosition> parameterD) {
        this.parameterD = parameterD;
    }

    public ArrayList<NameAndPosition> getParameterRW() {
        return parameterRW;
    }

    public void setParameterRW(ArrayList<NameAndPosition> parameterRW) {
        this.parameterRW = parameterRW;
    }

    public ArrayList<NameAndPosition> getGlobalD() {
        return globalD;
    }

    public void setGlobalD(ArrayList<NameAndPosition> globalD) {
        this.globalD = globalD;
    }

    public void setGlobalRW(ArrayList<NameAndPosition> globalRW) {
        this.globalRW = globalRW;
    }

    public ArrayList<NameAndPosition> getGlobalRW() {
        return globalRW;
    }


    public void addToD(String deleted,int level, Environ env) {
        addToArrayList(deleted, level, env, globalD, parameterD);

    }

    public void addToRW(String readWrite,int level,Environ env) {
        addToArrayList(readWrite, level, env, globalRW, parameterRW);

    }


    private void addToArrayList(String name, int level, Environ env, ArrayList<NameAndPosition> globalS, ArrayList<NameAndPosition> parameterS) {
        if (env.getVariableLevel(name) >= env.getFunctionlevel(env.getCurrentFunction())) { //check se globale
            NameAndPosition nameAndPosition =new NameAndPosition(name,level);
            if (!globalS.contains(nameAndPosition))
                globalS.add(nameAndPosition);
        }
        else{
            for (ParameterNode parameterNode : functionParameterNodes) {
                if (parameterNode.getID().equals(name) && parameterNode.getReference()) {
                    NameAndPosition nameAndPosition =new NameAndPosition(name, StmtBlockNode.numStmt);
                    if (!parameterS.contains(nameAndPosition))
                        parameterS.add(nameAndPosition);

                }
            }
        }
    }

    public boolean isDeleted() {
        return isDeleted;
    }

    public void setDeleted(boolean deleted) {
        isDeleted = deleted;
    }

    public ArrayList<ParameterNode> getFunctionParameterNodes() {
        return functionParameterNodes;
    }


}