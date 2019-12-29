package errors;

import java.util.ArrayList;

public class SemanticError {

    public static ArrayList<SemanticError> semanticErrors = new ArrayList<>();
    public final String msg;

    public SemanticError(String msg) {
        this.msg = msg;
        semanticErrors.add(this);
    }

    public static ArrayList<SemanticError> getSemanticErrors() {
        return semanticErrors;
    }

    @Override
    public String toString() {
        return msg;
    }

    public enum printable {
        NOT_DECLARED_VAR_ERROR("Variabile non dichiarata: "),
        NOT_DECLARED_FUNC_ERROR("Funzione non dichiarata: "),
        DELETE_NOT_DECLARED_ERROR("Impossibile cancellare la variabile non dichiarata: "),

        ALREADY_DECLARED_VAR_ERROR("Identificativo gia dichiarato nello scope, impossibile dichiarare la variabile: "),
        ALREADY_DECLARED_FUNC_ERROR("Identificativo gia dichiarato nello scope, impossibile dichiarare la funzione: "),
        ALREADY_DELETED_VAR_ERROR("Identificativo eliminato nello scope: "),

        IF_ELSE_ERROR("I blocchi Then e Else hanno comportamento diversi "),

        NUM_PARAMS_ERROR("Numero di parametri passati non corretto nella funzione: "),
        EXP_TO_VAR_ERROR("Passaggio di espressione a parametro per riferimento nella funzione: "),
        FUNCTION_CALL_ERROR("Il passaggio di parametri provoca un errore semantico nella funzione: "),
        FUNCTION_DELETED_ERROR("Impossibile invocare una funzione cancellata: "),
        READ_GLOBAL_ERROR("Lettura di una variabile globale cancellata: "),
        AUTODESTRUCTION_ERROR("Impossibile cancellare una funzione al cui si e' all'interno: ");
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
