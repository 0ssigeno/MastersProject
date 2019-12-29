package codeGenLib;

public class AssemblyLib {

    public static int countPush=0;
    public static int countPop=0;
    public static String jumpConditional(String type, String arg1, String arg2, String label){
        return type+" "+ arg1 +" "+ arg2 + " " + label + "\n";
    }
    public static String jump(String label){
        return "b "+ label + "\n";
    }
    public static String jumpRet(){
        return "jr $ra"+ "\n";
    }
    public static String jumpLabel(String label){
        return "jl "+ label+ "\n";
    }


    public static String math(String op, String where, String arg1, String arg2){
        return op+" "+ where +" "+ arg1 + " " + arg2 + "\n";
    }
    public static String halt(){
        return "halt \n";
    }
    public static String loadiT1(int val){
        return "li $t1 "+val+" \n";
    }
    public static String addi(String reg,int val  ){
        countPop+=val/4;
        return "addi "+reg+" "+reg+" "+val+" \n";
    }

    public static String push(){
        countPush+=1;
        return "push $a0 \n";
    }

    public static String pushReg(String reg){
        countPush+=1;
        return "push "+reg+ "\n";
    }
    public static String pop(){
        countPop+=1;
        return "pop \n";
    }
    public static String top(){
        return "top $t1 \n";
    }
    public static String topReg(String reg){
        return "top "+reg+"\n";
    }
    public static String startLabel(String label){
        return label+": \n";
    }
    public static String loadiA0(int value){
        return "li $a0 "+value+"\n";
    }
    public static String store(int offset){
        return "sw $a0 " +offset+"($al)"+"\n";
    }
    public static String load(int offset){

        return "lw $a0 " +offset+"($al)"+"\n";
    }
    public static String loadAl(int offset){

        return "lw $al " +offset+"($al)"+"\n";
    }

    public static String loopStatic(int times){
        times= CodeGenEnviron.levels.size()-times-1; //PERCHE' LA LISTA E' INVERTITA
        StringBuilder howManyTimes= new StringBuilder();
        for(int i=0;i<times;i++){
            howManyTimes.append("lw $al 0($al) \n");
        }
        return "mv $al $fp \n" + howManyTimes;
    }
    public static String print(){
        return "print \n";
    }
    public static String mv(String src, String dst){
        return "mv "+src + " "+ dst +"\n";
    }

}
