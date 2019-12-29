package codeGenLib;

import models.TypeNode;

import java.util.ArrayList;

public class InfoClasses {
    public static class VariableInfo {
        private String id;
        private int offset;
        private int level;
        private String type;
        private boolean parameter;
        private boolean var;
        VariableInfo(String id, int offset, int level,String type,boolean parameter,boolean var){
            this.id=id;
            this.offset=offset;
            this.level=level;
            this.type=type;
            this.parameter=parameter;
            this.var=var;
        }

        public String getId() {
            return id;
        }

        public int getLevel() {
            return level;
        }

        public int getOffset() {
            return offset;
        }
        public String getType(){
            return type;
        }
        public Boolean isParameter(){
            return parameter;
        }

        public boolean isVar() {
            return var;
        }
    }

    public static class FunctionInfo {
        private String id;
        private int level;
        private ArrayList<TypeNode> parameters;
        private ArrayList<Boolean> referenced;
        private ArrayList<String> ids;
        private String label;

        FunctionInfo(String id, ArrayList<TypeNode> parameters, ArrayList<Boolean> referenced,ArrayList<String> ids, String label, int level){
            this.id=id;
            this.level=level;
            this.parameters=parameters;
            this.label=label;
            this.referenced=referenced;
            this.ids=ids;
        }

        public ArrayList<TypeNode> getParameters() {
            return parameters;
        }

        public int getLevel() {
            return level;
        }

        public String getId() {
            return id;
        }

        public String getLabel() {
            return label;
        }

        public ArrayList<Boolean> getReferenced() {
            return referenced;
        }

        public ArrayList<String> getIds() {
            return ids;
        }

    }
}
