package codeGenLib;

import semanticLib.Environ;
import models.ExpNode;

import java.util.ArrayList;

class Level {
    static class level {
        public ArrayList<InfoClasses.VariableInfo> variables= new ArrayList<>();
        public  ArrayList<InfoClasses.FunctionInfo> functions = new ArrayList<>();
        public int countParams=0;
        public int currentOffset=-8;
        public int currentOffsetParam=0;

        level(){

        }
        void addVariableInfo(String id,int level,String type ){
            variables.add(0,new InfoClasses.VariableInfo(id,currentOffset,level,type,false,false));
            currentOffset-=4;
        }
        void addParameterInfo(String id,int level,String type,boolean isVar,int numvar ){
            //sistemo per il prossimo blocco
            int offsetParam=CodeGenEnviron.preparedNextLevel.currentOffsetParam;
            if(isVar){
                CodeGenEnviron.preparedNextLevel.variables.add(0,new InfoClasses.VariableInfo(id,numvar*8+offsetParam,level,type,true,isVar));
            }else{
                //CodeGenEnviron.preparedNextLevel.variables.add(0,new InfoClasses.VariableInfo(id,numvar*8+offsetParam,level,type,true,isVar));
                CodeGenEnviron.preparedNextLevel.variables.add(0,new InfoClasses.VariableInfo(id,4+numvar*8+offsetParam,level,type,true,isVar));
            }
            CodeGenEnviron.preparedNextLevel.currentOffsetParam-=8;
            CodeGenEnviron.preparedNextLevel.countParams+=1;

        }

        int getVars(){
            return variables.size()-countParams;
        }

        void addFunctionInfo(InfoClasses.FunctionInfo functionInfo){
            functions.add(functionInfo);
        }


        InfoClasses.VariableInfo findVariable(String id){
            for(InfoClasses.VariableInfo variableInfo: variables){
                if(id.equals(variableInfo.getId())){
                    return variableInfo;
                }
            }
            return null;
        }

        public ArrayList<InfoClasses.FunctionInfo> getFunctions() {
            return functions;
        }

        public ArrayList<InfoClasses.VariableInfo> getVariables() {
            return variables;
        }

        InfoClasses.FunctionInfo findFunction(String id, ArrayList<ExpNode> expNodes){
            for(InfoClasses.FunctionInfo functionInfo: functions){
                if(id.equals(functionInfo.getId()) && expNodes.size()==functionInfo.getParameters().size()){
                    for(int i=0;i<expNodes.size();i++){
                        if(expNodes.get(i).getID()!=null){
                            InfoClasses.VariableInfo variablePar = CodeGenEnviron.findVariableInfo(expNodes.get(i).getID());
                            if(!variablePar.getType()
                                    .equals(functionInfo.getParameters().get(i).getType())){
                                return null;

                            }
                        }else{
                            if(!expNodes.get(i).typeCheck(Environ.typeCheckEnviron).getType()
                                    .equals(functionInfo.getParameters().get(i).getType())){
                                return null;

                            }
                        }

                    }
                    return functionInfo;

                }
            }
            return null;
        }

    }
}
