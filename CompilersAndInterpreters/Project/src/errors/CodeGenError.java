package errors;

import java.util.ArrayList;

public class CodeGenError {
    public static ArrayList<CodeGenError> codeGenErrors= new ArrayList<>();
    public final String msg;

    public CodeGenError(String msg) {
        this.msg = msg;
        codeGenErrors.add(this);
    }

    public static ArrayList<CodeGenError> getCodeGenErrors() {
        return codeGenErrors;
    }

    @Override
    public String toString() {
        return msg;
    }

    public enum printable {
        LEVEL_FUNCTION_ERROR("Impossibile trovare il livello della funzione: "),
        LABEL_FUNCTION_ERROR("Impossibile trovare la label della funzione: "),
        REFERENCE_FUNCTION_ERROR("Impossibile trovare le reference dei parametri della funzione: "),

        OFFSET_VARIABLE_ERROR("Impossibile trovare l'offset della variabile: "),
        LEVEL_VARIABLE_ERROR("Impossibile trovare il livello della variabile: ");
        private final String text;

        printable(final String s) {
            text = s;
        }

        @Override
        public String toString() {
            return text;
        }


    }

}
