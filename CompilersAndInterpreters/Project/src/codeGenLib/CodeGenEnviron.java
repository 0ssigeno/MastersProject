package codeGenLib;

import errors.CodeGenError;
import models.ExpNode;
import models.ParameterNode;
import models.TypeNode;

import java.util.ArrayList;

public class CodeGenEnviron {




    private static int labCount = 0;
    private static int funLabCount = 0;
    public static boolean amIinsideFunction = false;

    public static Level.level preparedNextLevel= new Level.level();

    public static ArrayList<Level.level> levels=new ArrayList<>();
    public static String freshLabel(String name) {
        return name + (labCount++);
    }

    public static String freshFunLabel(String name) {
        return "fun"+name + (funLabCount++);
    }

    public static String functionDefinitions="";
    public static Boolean mainBlock=true;


    public static int getVars(){
        return levels.get(0).getVars();
    }

    public static InfoClasses.VariableInfo findVariableInfo(String id){
        for(Level.level level: levels){

            InfoClasses.VariableInfo variableInfo=level.findVariable(id);
            if(variableInfo!=null){
                return variableInfo;
            }
        }
        new CodeGenError(CodeGenError.printable.OFFSET_VARIABLE_ERROR + id);
        return null;

    }
    public static InfoClasses.FunctionInfo findFunctionInfo(String id, ArrayList<ExpNode>parameters){
        for(Level.level level: levels){
            InfoClasses.FunctionInfo functionInfo=level.findFunction(id,parameters);
            if(functionInfo!=null){

                return functionInfo;
            }
        }
        new CodeGenError(CodeGenError.printable.LEVEL_FUNCTION_ERROR + id);

        return null;

    }

    public static void addVariable(String id,String type ){
        levels.get(0).addVariableInfo(id,levels.size()-1,type);
    }

    public static void addParameter(String id,String type,boolean isVar, int len ){
        levels.get(0).addParameterInfo(id,levels.size(),type,isVar,len);
    }



    public static void addFunction(String id, ArrayList<ParameterNode> parameter, String label){
        ArrayList<TypeNode> types = new ArrayList<>();
        ArrayList<Boolean> referenced = new ArrayList<>();
        ArrayList<String> ids = new ArrayList<>();
        for(ParameterNode parameterNode: parameter){
            types.add(parameterNode.getTypeNode());
            referenced.add(parameterNode.getReference());
            ids.add(parameterNode.getID());
        }
        levels.get(0).addFunctionInfo(new InfoClasses.FunctionInfo(id,types,referenced,ids,label,levels.size()-1));
    }
    public static void nextLevel(){
        levels.add(0,preparedNextLevel);
        preparedNextLevel=new Level.level();


    }
    public static void prevLevel(){
        levels.remove(0);
    }


}
