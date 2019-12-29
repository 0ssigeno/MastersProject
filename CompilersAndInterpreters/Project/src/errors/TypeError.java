package errors;

import java.util.ArrayList;

public class TypeError {
    public static ArrayList<TypeError> typeErrors = new ArrayList<>();
    public final String msg;

    public TypeError(String msg) {
        this.msg = msg;
        typeErrors.add(this);
    }

    public static ArrayList<TypeError> getTypeErrors() {
        return typeErrors;
    }

    @Override
    public String toString() {
        return msg;
    }

    public enum printable {
        OP_NOT_INT_ERROR("Operazione non consentita tra interi: "),
        OP_NOT_BOOL_ERROR("Operazione non consentita tra booleani: "),

        DIFFERENT_OPERANDS_ERROR("Operazione non consentita tra operatori di tipo diverso: "),
        IF_ELSE_ERROR("I blocchi Then e Else hanno tipo diversi "),
        IF_EXP_NOT_BOOL_ERROR("Il tipo dell'espressione nell'if non e' booleana"),
        FUNC_CALL_PARAMS_ERROR("Scorretto tipo del parametro attuale: "),
        TYPE_DECLARATION_VAR_ERROR("Variabile dichiarata con tipo errato: "),
        TYPE_ASSIGNMENT_VAR_ERROR("Variabile assegnata con tipo errato: "),

        ;

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
