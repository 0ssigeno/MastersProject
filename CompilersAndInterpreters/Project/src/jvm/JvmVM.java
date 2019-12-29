package jvm;

import jvmGrammar.JvmParser;

import java.util.HashMap;

public class JvmVM {
    public static final int CODESIZE = 10000;
    public static final int MEMSIZE = 10000;
    private int[] code;
    public static int countExecution=0;
    private int[] memory = new int[MEMSIZE];
    private int ip = 0;
    private HashMap<String,Integer> registers = new HashMap<String,Integer>(){{
        put("$a0", 0);
        put("$t1", 0);
        put("$fp", MEMSIZE);
        put("$sp", MEMSIZE);
        put("$al", MEMSIZE);
        put("$ra", 0);

    }};
    public JvmVM(int[] code) {
        this.code = code;
    }

    public void CPU() {
        while (true) {
            countExecution++;
            if (registers.get("$sp") > MEMSIZE) {
                System.err.println("Error: Out of memory "+ countExecution);
                return;
            } else {
                int bytecode = code[ip++]; // fetch
                int v1, v2;
                int address;
                String result;
                int offset;
                switch (bytecode) {
                    case 100:
                        registers.put(JvmEnv.codeToRegister(100), memory[registers.get("$sp")]);
                        break;
                    case 101:
                        registers.put(JvmEnv.codeToRegister(101), memory[registers.get("$sp")]);
                        break;
                    case 102:
                        registers.put(JvmEnv.codeToRegister(102), memory[registers.get("$sp")]);
                        break;
                    case 103:
                        registers.put(JvmEnv.codeToRegister(103), memory[registers.get("$sp")]);
                        break;
                    case 104:
                        registers.put(JvmEnv.codeToRegister(104), memory[registers.get("$sp")]);
                        break;
                    case 105:
                        registers.put(JvmEnv.codeToRegister(105), memory[registers.get("$sp")]);
                        break;
                    case JvmParser.PUSH:
                        push(code[ip++]);
                        break;
                    case JvmParser.POP:
                        pop();
                        break;
                    case JvmParser.STOREW:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        offset = code[ip++];
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        memory[v1+offset] = registers.get(result);
                        break;
                    case JvmParser.LOADW:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        offset = code[ip++];
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        registers.put(result, memory[v1+offset]);
                        break;

                    case JvmParser.LOADI:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        v1 = code[ip++];
                        registers.put(result, v1);
                        break;
                    case JvmParser.ADD:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        registers.put(result, v1 + v2);
                        break;
                    case JvmParser.ADDI:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = code[ip++];
                        registers.put(result, v1 + v2);
                        break;
                    case JvmParser.MULT:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        registers.put(result, v1 * v2);
                        break;
                    case JvmParser.DIV:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        registers.put(result, v1 / v2);
                        break;
                    case JvmParser.SUB:
                        result = JvmEnv.codeToRegister((code[ip++]));
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        registers.put(result, v1 - v2);
                        break;
                    case JvmParser.BRANCH:
                        address = code[ip];
                        ip = address;
                        break;
                    case JvmParser.BRANCHEQ:
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        address = code[ip++];
                        if (v2 == v1) ip = address;
                        break;
                    case JvmParser.BRANCHNE:
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        address = code[ip++];
                        if (v2 != v1) ip = address;
                        break;
                    case JvmParser.BRANCHLE:
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        address = code[ip++];
                        if (v1 <= v2) ip = address;
                        break;
                    case JvmParser.BRANCHLT:
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        address = code[ip++];
                        if (v1 < v2) ip = address;
                        break;
                    case JvmParser.BRANCHGT:
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        address = code[ip++];
                        if (v1 > v2) ip = address;
                        break;
                    case JvmParser.BRANCHGE:
                        v1 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        v2 = registers.get(JvmEnv.codeToRegister((code[ip++])));
                        address = code[ip++];
                        if (v1 >= v2) ip = address;
                        break;
                    case JvmParser.MOVE:
                        result = JvmEnv.codeToRegister(code[ip++]);
                        registers.put(result, registers.get(JvmEnv.codeToRegister(code[ip++])));
                        break;
                    case JvmParser.JL:
                        registers.put("$ra",ip+1);
                        ip = code[ip];
                        break;
                    case JvmParser.JR:
                        ip = registers.get("$ra");
                        break;
                    case JvmParser.PRINT:
                        System.out.println("Print: "+registers.get("$a0"));
                        break;
                    case JvmParser.HALT:
                        System.out.println("Halt");
                        return;
                }
            }
        }
    }

    private int pop() {
        int temp = registers.get("$sp");
        if((registers.get("$sp") +4 ) <= MEMSIZE) {
            registers.put("$sp", registers.get("$sp") + 4);
        }
        else{
            System.err.println("Out of memory");
            System.exit(1);
        }
        return memory[temp];

    }

    private void push(int v) {
        if((registers.get("$sp") - 4) > 0) {
            registers.put("$sp", registers.get("$sp")-4);
            memory[registers.get("$sp")] = registers.get(JvmEnv.codeToRegister(v));
        } else {
            System.err.println("Out of memory");
            System.exit(1);
        }

    }

}
