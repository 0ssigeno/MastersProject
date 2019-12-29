package models;

import codeGenLib.InfoClasses;
import semanticLib.Environ;
import semanticLib.Fentry;
import semanticLib.NameAndPosition;
import errors.SemanticError;
import errors.TypeError;
import codeGenLib.AssemblyLib;
import codeGenLib.CodeGenEnviron;

import java.util.ArrayList;

public class StmtFunctionCallNode extends StmtNode {

    private String ID;
    private ArrayList<ExpNode> listOfExpNode;


    public StmtFunctionCallNode(String ID, ArrayList<ExpNode> listOfExpNode) {
        this.ID = ID;
        this.listOfExpNode = listOfExpNode;
    }


    @Override
    public String toPrint(String s) {
        StringBuilder temp = new StringBuilder(s);
        temp.append("\n FunctionCallnode");
        for (ExpNode en : listOfExpNode) {
            temp.append(en.toPrint(s));
        }
        return temp.toString();
    }


    @Override
    public TypeNode typeCheck(Environ env) {
        Fentry entryFunction = env.getFentry(this.ID);
        for (int i = 0; i < entryFunction.getFunctionParameterNodes().size(); i++) {
            String typeExp = this.listOfExpNode.get(i).typeCheck(env).getType();
            String typeParameter = entryFunction.getFunctionParameterNodes().get(i).typeCheck(env).getType();
            if (!(typeExp.equals(typeParameter))) {
                new TypeError(TypeError.printable.FUNC_CALL_PARAMS_ERROR + ID);
            }
        }
        return new TypeNode("void");

    }

    @Override
    public void checkSemantics(Environ env) {
        //se la funzione invocata e' stata dichiarata
        if (env.containsIDftable(ID)) {
            //se non e' stata cancellata
            if (!env.getFentry(ID).isDeleted()) {
                //se hanno lo stesso numero di argomenti
                if (env.checkNumberArgumentsFtable(ID, listOfExpNode.size())) {
                    //analizzo i miei parametri
                    for (ExpNode expNode : listOfExpNode) {
                        expNode.checkSemantics(env);
                    }
                    //se sto passando delle espressioni come var, ritorno errore
                    if (!env.checkVarFtable(ID, listOfExpNode)) {
                        new SemanticError(SemanticError.printable.EXP_TO_VAR_ERROR + ID);
                    } else {
                        //se ho trovato degli errori in precedenza esco perche' ho gia un errore semantico
                        if (SemanticError.getSemanticErrors().size() > 0) return;

                        Fentry fentry = env.getFentry(ID);
                        //se sono una funzione mi sposto le variabili globali di lettura e delete

                        if (env.getCurrentFunction() != null) {
                            if(!env.getCurrentFunction().equals(ID)){
                                for(NameAndPosition nameAndPosition : fentry.getGlobalRW()){
                                    env.getFentry(env.getCurrentFunction()).addToRW(nameAndPosition.name, nameAndPosition.level, env);

                                }
                                for(NameAndPosition nameAndPosition : fentry.getGlobalD()){
                                    env.getFentry(env.getCurrentFunction()).addToRW(nameAndPosition.name, nameAndPosition.level, env);


                                }
                            }

                        }


                        ArrayList<ParameterNode> declarationParams = env.getFentry(ID).getFunctionParameterNodes();
                        // i parametri dell'invocato
                        ArrayList<NameAndPosition> parameterD = (ArrayList<NameAndPosition>) fentry.getParameterD().clone();
                        ArrayList<NameAndPosition> parameterRW = (ArrayList<NameAndPosition>) fentry.getParameterRW().clone();
                        //per ogni parametro che passo
                        for (int i = 0; i < this.listOfExpNode.size(); i++) {
                            //se in quella posiziojne c'e' un var
                            if (declarationParams.get(i).getReference()) {
                                String exValue = declarationParams.get(i).getID();
                                String newValue = listOfExpNode.get(i).getID();
                                //gli modifico il nome con quello che gli sto passando per questa esecuzione
                                for (int j = 0; j < parameterRW.size(); j++) { //riscrittura
                                    if (parameterRW.get(j).name.equals(exValue)) {
                                        parameterRW.set(j,new NameAndPosition(newValue,parameterRW.get(j).level));

                                    }


                                }
                                //gli modifico il nome con quello che gli sto passando
                                for (int j = 0; j < parameterD.size(); j++) { //riscrittura
                                    if (parameterD.get(j).name.equals(exValue)) {
                                        parameterD.set(j,new NameAndPosition(newValue,parameterD.get(j).level));
                                        //se sono una funzione mi aggiungo anche io le delete e le read della funzione chiamata
                                        if (env.getCurrentFunction() != null) {
                                           //if(!env.getCurrentFunction().equals(ID)) {
                                                env.getFentry(env.getCurrentFunction()).addToD(newValue, env.getVariableLevel(newValue), env);
                                            //}
                                        }
                                        //per ogni variabile cancellata, provo a vedere se era gia' cancellata
                                        if(env.getVentry(newValue,0).isDeleted()){
                                            new SemanticError(SemanticError.printable.ALREADY_DELETED_VAR_ERROR + newValue);
                                        }else{
                                            env.getVentry(newValue,0).setDeleted(true);
                                        }


                                    }
                                    //finalmente controllo se ho conflitti tra parametri acceduti in lettura e scrittura
                                    for(NameAndPosition nameAndPosition : parameterRW){
                                        if(nameAndPosition.name.equals(parameterD.get(j).name  )&& nameAndPosition.level>parameterD.get(j).level ){
                                            new SemanticError(SemanticError.printable.FUNCTION_CALL_ERROR + ID + " con l'attributo " +newValue);
                                        }


                                    }

                                }


                            }
                        }
                        //se ogni variabile globale acceduta in lettura, controllo che non sia stata cancellata prima
                        for (NameAndPosition nameAndPosition : fentry.getGlobalRW()) {
                            if(env.getVentry(nameAndPosition.name, nameAndPosition.level).isDeleted()){
                                new SemanticError(SemanticError.printable.READ_GLOBAL_ERROR+ nameAndPosition.name+ nameAndPosition.level);
                            }
                        }
                        //cancello le variabili nel mio scope
                        for (NameAndPosition nameAndPosition : fentry.getGlobalD()) {
                            env.deleteID(nameAndPosition.name, nameAndPosition.level);

                        }



                    }


                } else {
                    new SemanticError(SemanticError.printable.NUM_PARAMS_ERROR + ID);
                }
            } else {
                new SemanticError(SemanticError.printable.FUNCTION_DELETED_ERROR + ID);

            }


        } else {
            new SemanticError(SemanticError.printable.NOT_DECLARED_FUNC_ERROR + ID);
        }
    }


    @Override
    public String getID() {
        return null;
    }

    @Override
    public String codGen() {
        InfoClasses.FunctionInfo functionInfo = CodeGenEnviron.findFunctionInfo(ID,listOfExpNode);

        String label= functionInfo.getLabel();
        ArrayList<Boolean> referencesParams= functionInfo.getReferenced();
        int level= functionInfo.getLevel();
        assert referencesParams != null;
        assert referencesParams.size()==listOfExpNode.size();


        StringBuilder params= new StringBuilder();
        for(ExpNode exp: listOfExpNode){
            if(exp.getID()!=null){
                InfoClasses.VariableInfo variableInfo=CodeGenEnviron.findVariableInfo(exp.getID());
                params.append(AssemblyLib.loopStatic(variableInfo.getLevel()));
                //se quello che sto passando e' un parametro

                if(variableInfo.isParameter() && variableInfo.isVar()){
                    params.append(AssemblyLib.loadAl(variableInfo.getOffset())) //prendo l'indirizzo
                            .append(AssemblyLib.load(0)) //ne carico il valore
                            .append(AssemblyLib.push()) //pusho il valore
                            .append(AssemblyLib.pushReg("$al")); //pusho indirizzo
                }else{

                    params.append(AssemblyLib.load(variableInfo.getOffset())) //prendo il valore perche ho +4
                            .append(AssemblyLib.push()) //pusho valore
                            .append(AssemblyLib.mv("$a0","$al"))  //metto la base al
                            .append(AssemblyLib.addi("$a0",variableInfo.getOffset())) //ci sommo l'offset
                            .append(AssemblyLib.push()); //pusho l'indirizzo

                }
            }else{
                params.append(exp.codGen())
                        .append(AssemblyLib.push()) //pushi valore
                        .append(AssemblyLib.loadiA0(0))
                        .append(AssemblyLib.push()); //pushi indirizzo aka merda
            }
        }
        String function = AssemblyLib.pushReg("$fp") +
                AssemblyLib.loopStatic(level) +
                AssemblyLib.pushReg("$al") +
                AssemblyLib.jumpLabel(label);

        if(listOfExpNode.size()>0){
            function+=AssemblyLib.addi("$sp", listOfExpNode.size() * 8); //perche' ho pushato sia indirizzo che valore sempre
        }
        return  params.toString()+ function;
    }

}
