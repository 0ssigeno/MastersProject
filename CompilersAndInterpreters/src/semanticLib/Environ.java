package semanticLib;

import errors.SemanticError;
import models.*;

import java.util.ArrayList;
import java.util.HashMap;

public class Environ {

    public static Environ typeCheckEnviron;

    private ArrayList<HashMap<String, Ventry>> vTable = new ArrayList<>();
    private ArrayList<HashMap<String, Fentry>> fTable = new ArrayList<>();


    private ArrayList<ParameterNode> parameterToAddInNextScope = new ArrayList<>();
    private ArrayList<String> stackFunctions = new ArrayList<String>() {{
        add(null);
    }};

    //perche' java altrimenti urla
    public Environ() {

    }

    //dummy copia perche' java non mi permette deep copy
    public Environ(Environ dummyEnv) {
        this.vTable = new ArrayList<>();
        for (HashMap hashMap : dummyEnv.vTable) {
            HashMap<String, Ventry> clone = new HashMap<>();
            hashMap.keySet().forEach(key -> {
                clone.put((String) key, new Ventry((Ventry) hashMap.get(key)));
            });
            this.vTable.add(clone);
        }
        this.fTable = new ArrayList<>();
        for (HashMap hashMap : dummyEnv.fTable) {
            HashMap<String, Fentry> clone = new HashMap<>();
            hashMap.keySet().forEach(key -> {
                clone.put((String) key, new Fentry((Fentry) hashMap.get(key)));
            });
            this.fTable.add(clone);
        }
        this.parameterToAddInNextScope = dummyEnv.parameterToAddInNextScope;
        this.stackFunctions = dummyEnv.stackFunctions;
    }

    public void addFunctionToStack(String currentDefinitionFunction) {
        this.stackFunctions.add(0, currentDefinitionFunction);
    }

    public void removeFunctionFromStack() {
        this.stackFunctions.remove(0);
    }

    public String getCurrentFunction() {
        return stackFunctions.get(0);
    }

    @Override
    //Controllo ogni cosa, perch' java non si rimedia
    public boolean equals(Object obj) {
        if (obj == this) {
            return true;
        }

        if (!(obj instanceof Environ)) {
            return false;
        }
        Environ env = (Environ) obj;
        if (this.vTable.size() != env.vTable.size()) {
            return false;
        } else {
            for (int i = 0; i < this.vTable.size(); i++) {
                if (!this.vTable.get(i).keySet().equals(env.vTable.get(i).keySet())) {
                    return false;
                } else {
                    HashMap<String, Ventry> thisHashMap = this.vTable.get(i);
                    HashMap<String, Ventry> envHashMap = env.vTable.get(i);
                    for (String name : thisHashMap.keySet()) {
                        if (thisHashMap.get(name).isDeleted() != envHashMap.get(name).isDeleted()) {
                            return false;
                        }
                    }
                }

            }

        }
        if (this.fTable.size() != env.fTable.size()) {
            return false;
        } else {
            for (int i = 0; i < this.fTable.size(); i++) {
                if (!this.fTable.get(i).keySet().equals(env.fTable.get(i).keySet())) {
                    return false;
                } else {
                    HashMap<String, Fentry> thisHashMap = this.fTable.get(i);
                    HashMap<String, Fentry> envHashMap = env.fTable.get(i);
                    for (String name : thisHashMap.keySet()) {
                        if (thisHashMap.get(name).isDeleted() != envHashMap.get(name).isDeleted()) {
                            return false;
                        }
                    }
                }
            }

        }
        return true;
    }


    public void addVariable(String id, Ventry st) {
        vTable.get(0).put(id, st);

    }

    public void addFunction(String id, Fentry st) {
        fTable.get(0).put(id, st);
    }

    public void setParameterToAddInNextScope(ArrayList<ParameterNode> parameterNodes) {
        this.parameterToAddInNextScope = parameterNodes;
    }

    public TypeNode getVariableType(String id) {

        for (HashMap<String, Ventry> scope : vTable) {
            if (scope.containsKey(id)){
                return scope.get(id).getTypeNode();
            }

        }
        return null;
    }

    public TypeNode getFunctionType(String id) {
        for (HashMap<String, Ventry> scope : vTable) {
            if (scope.containsKey(id))
                return new TypeNode("void");
        }
        return null;
    }

    public int getFunctionlevel(String id) {
        for (int i = 0; i < fTable.size(); i++) {
            HashMap<String, Fentry> scope = fTable.get(i);
            if (scope.containsKey(id))
                return i;
        }
        return -1;
    }

    public int getVariableLevel(String id) {
        for (int i = 0; i < vTable.size(); i++) {
            HashMap<String, Ventry> scope = vTable.get(i);
            if (scope.containsKey(id))
                return i;
        }
        return -1;
    }


    public Fentry getFentry(String id) {
        for (HashMap<String, Fentry> scope : fTable) {
            if (scope.containsKey(id))
                return scope.get(id);
        }
        return null;
    }

    public Ventry getVentry(String id,int level) {
        for(int i=level;i<vTable.size();i++){
            HashMap<String,Ventry>scope=vTable.get(i);
            if (scope.containsKey(id))
                return scope.get(id);
        }
        return null;
    }



    public void openScope() {
        //dato che la mia gestione fa abbastanza schifo, devo aggiornare tutti i livelli di tutte le variabili ogni volta
        //che uno scope viene aperto
        for(HashMap<String,Fentry>hashMap:fTable){
            for(String string:hashMap.keySet()){
                Fentry fentry=hashMap.get(string);
                for(NameAndPosition nameAndPosition :fentry.getGlobalD()){
                    nameAndPosition.level+=1;
                }
                for(NameAndPosition nameAndPosition :fentry.getGlobalRW()){
                    nameAndPosition.level+=1;
                }
            }
        }
        vTable.add(0, new HashMap<>());
        fTable.add(0, new HashMap<>());
        //aggiungo i parametri che mi hanno chiesto di salvarmi
        if (parameterToAddInNextScope.size() > 0) {
            for (ParameterNode parameterNode : parameterToAddInNextScope) {
                this.addVariable(parameterNode.getID(), new Ventry(parameterNode.getTypeNode()));
            }
            parameterToAddInNextScope = new ArrayList<>();
        }

    }


    public void closeScope() {
        //diminuisco di 1, contrario apertura
        for(HashMap<String,Fentry>hashMap:fTable){
            for(String string:hashMap.keySet()){
                Fentry fentry=hashMap.get(string);
                for(NameAndPosition nameAndPosition :fentry.getGlobalD()){
                    nameAndPosition.level-=1;
                }
                for(NameAndPosition nameAndPosition :fentry.getGlobalRW()){
                    nameAndPosition.level-=1;
                }

            }
        }

        vTable.remove(0);
        fTable.remove(0);
    }

    public boolean containsIDvtable(String id) {
        for (HashMap<String, Ventry> scope : vTable) {
            if (scope.containsKey(id)) {

                return true;
            }

        }
        return false;
    }

    public boolean isDeletedIDvtable(String id) {
        for (HashMap<String, Ventry> scope : vTable) {
            if (scope.containsKey(id)) {
                Ventry ventry = scope.get(id);
                return ventry.isDeleted();


            }

        }
        return false;
    }

    public boolean containsIDftable(String id) {
        for (HashMap<String, Fentry> scope : fTable) {
            if (scope.containsKey(id))
                return true;
        }
        return false;
    }

    public boolean checkNumberArgumentsFtable(String id, int lenParameter) {
        for (HashMap<String, Fentry> scope : fTable) {
            if (scope.containsKey(id)) {
                Fentry fentry = scope.get(id);
                return fentry.getFunctionParameterNodes().size() == lenParameter;
            }
        }
        return false;
    }

    //controllo del passaggio espressioni al posto di variabili var
    public boolean checkVarFtable(String id, ArrayList<ExpNode> expNodes) {
        for (HashMap<String, Fentry> scope : fTable) {
            if (scope.containsKey(id)) {
                Fentry fentry = scope.get(id);
                for (int i = 0; i < expNodes.size(); i++) {
                    if (fentry.getFunctionParameterNodes().get(i).getReference()) {
                        if (expNodes.get(i).getID() == null) {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    public boolean containsThisLevelIDvtable(String id) {
        return this.vTable.get(0).containsKey(id);
    }


    public boolean containsThisLevelIDftable(String id) {
        return this.fTable.get(0).containsKey(id);
    }


    public void deleteID(String id,int level) {
        //prendo solo la prima variabile che trovo, perche' lo scope e' statico
        for (int i = level; i < vTable.size(); i++) {
            HashMap<String, Ventry> vTableI = vTable.get(i);
            HashMap<String, Fentry> fTableI = fTable.get(i);

            if (vTableI.containsKey(id)) {
                Ventry ventry = vTableI.get(id);
                if (!ventry.isDeleted()) {
                    //se sono una funzione, mi aggiungo la variabile alla lista
                    if (this.getCurrentFunction() != null) {
                        this.getFentry(this.getCurrentFunction()).addToD(id, i,this); //aggiungo alla lista di delete
                    }
                    ventry.setDeleted(true);
                    return;
                } else {
                    break; //ho errore,

                }
            } else if (fTableI.containsKey(id)) {
                Fentry fentry = fTableI.get(id);
                if (!fentry.isDeleted()) {
                    //mi sto cercando di autocancellare
                    if (stackFunctions.contains(id)) {
                        new SemanticError(SemanticError.printable.AUTODESTRUCTION_ERROR + id);
                        return;

                    } else {
                        if (this.getCurrentFunction() != null) {

                            this.getFentry(this.getCurrentFunction()).addToD(id,i, this); //aggiungo alla lista di delete
                        }

                        fentry.setDeleted(true);
                        return;
                    }

                }

            }
        }
        new SemanticError(SemanticError.printable.DELETE_NOT_DECLARED_ERROR + id);

    }


}
