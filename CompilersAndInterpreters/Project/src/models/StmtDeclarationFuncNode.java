package models;

import semanticLib.Environ;
import semanticLib.Fentry;
import errors.SemanticError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

import java.util.ArrayList;

public class StmtDeclarationFuncNode extends StmtDeclarationNode {

    private String ID;
    private ArrayList<ParameterNode> listParameterNodes;
    private StmtBlockNode stmtBlockNode;


    public StmtDeclarationFuncNode(String ID, ArrayList<ParameterNode> listParameterNodes, StmtBlockNode stmtBlockNode) {
        this.ID = ID;
        this.listParameterNodes = listParameterNodes;
        this.stmtBlockNode = stmtBlockNode;
    }

    @Override
    public String toPrint(String s) {
        String temp = s;
        for (ParameterNode par : listParameterNodes) {
            temp = temp + par.toPrint(s);
        }
        temp = temp + this.stmtBlockNode.toPrint(s);
        return temp;
    }

    @Override
    public TypeNode typeCheck(Environ env) {
        Fentry fentry = new Fentry(this.listParameterNodes);
        env.addFunction(ID, fentry);
        env.setParameterToAddInNextScope(this.listParameterNodes);
        this.stmtBlockNode.typeCheck(env);
        return new TypeNode("void");

    }

    @Override
    public void checkSemantics(Environ env) {
        //se non esiste una variabile o una funzione con lo stesso nome
        if ((!env.containsThisLevelIDftable(ID)) && (!env.containsThisLevelIDvtable(ID))) {
            //check dei parametri
            for (ParameterNode parameterNode : listParameterNodes) {
                parameterNode.checkSemantics(env);
            }
            Fentry fentry = new Fentry(this.listParameterNodes);
            //aggiungo la funzione con i parametri
            env.addFunction(ID, fentry);
            //aggiungo la funzione allo stack, aka sono dentro questa funzione
            env.addFunctionToStack(ID);
            //controllo il corpo con aventi i parametri nello scope
            Environ newEnv = new Environ(env);
            //ancora teoricamente lo scope non esiste, verranno aggiunti appena si apre un blocco
            newEnv.setParameterToAddInNextScope(this.listParameterNodes);
            stmtBlockNode.checkSemantics(newEnv);
            //mi copio le variabili dall'environ copia e le metto nel env default
            env.getFentry(ID).setParameterD(newEnv.getFentry(ID).getParameterD());
            env.getFentry(ID).setParameterRW(newEnv.getFentry(ID).getParameterRW());
            env.getFentry(ID).setGlobalD(newEnv.getFentry(ID).getGlobalD());
            env.getFentry(ID).setGlobalRW(newEnv.getFentry(ID).getGlobalRW());
            //sono uscito dalla funzione
            env.removeFunctionFromStack();
        } else {
            new SemanticError(SemanticError.printable.ALREADY_DECLARED_FUNC_ERROR + ID);
        }
    }


    @Override
    public String getID() {
        return null;
    }


    @Override
    public String codGen() {
        String funcLabel= CodeGenEnviron.freshFunLabel(ID);
        CodeGenEnviron.addFunction(ID,listParameterNodes,funcLabel);

        for (ParameterNode parameterNode : listParameterNodes) {
            CodeGenEnviron.addParameter(parameterNode.getID(),parameterNode.getTypeNode().getType(),parameterNode.getReference(),listParameterNodes.size()); //livello -1
        }


        CodeGenEnviron.amIinsideFunction=true;
        String myDef=  AssemblyLib.startLabel(funcLabel)+
                AssemblyLib.mv("$fp","$sp")+
                AssemblyLib.pushReg("$ra")+
                stmtBlockNode.codGen()+
                AssemblyLib.topReg("$ra")+
                AssemblyLib.pop()+
                AssemblyLib.pop()+
                AssemblyLib.topReg("$fp")+
                AssemblyLib.pop()+
                AssemblyLib.jumpRet();
        CodeGenEnviron.functionDefinitions+=myDef;
        return "";
    }
}
