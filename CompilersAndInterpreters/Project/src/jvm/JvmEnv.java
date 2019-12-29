package jvm;

import java.util.HashMap;
import java.util.Map;


public class JvmEnv {
    private static HashMap<Integer, String> mapRegister =  new HashMap<Integer, String>(){{
        put(100, "$a0");
        put(101, "$t1");
        put(102, "$fp");
        put(103, "$sp");
        put(104, "$al");
        put(105, "$ra");
    }};


    public static String codeToRegister(Integer code){
        return mapRegister.get(code);
    }

    public static int registerToCode(String register){
        for(Map.Entry<Integer,String> entry: mapRegister.entrySet()){
            if(entry.getValue().equals(register)){
                return entry.getKey();
            }
        }
        return -1;
    }

}
