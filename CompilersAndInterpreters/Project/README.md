#Membri
Simone Berni 0000753869 

Marco Negrini 0000908784

# Premesse
- Il compilatore non supporta funzioni mutuamente ricorsive
- Lo scope adottato e' di tipo statico
- Il controllo delle funzioni e' fatto staticamente, non a runtime
# Controllo semantico

- Il controllo e' formato da 3 fasi, Controllo sintattico, Controllo semantico e Controllo dei tipi, seguendo la falsa 
riga del compilatore di Foo
- Il controllo dei branch dell' If-Else e' effettuato nel seguente modo: se i risultati globali dei due branch sono diversi,
allora i branch non sono ben tipati e viene restituito un errore.
In questo modo il testo {if(true)then{delete x;}else{delete y;}} provoca errore.
Il test
 ~~~~
 {if(true)then{a=b+2;}else{b=a+2;} 
 ~~~~
 non provoca errore.
- Il controllo delle funzioni e' effettuato nel seguente modo: ogni funzione ha 4 liste, i parametri passati per riferimento
che sono stati acceduti in lettura o scrittura, RW, i parametri passati per riferimento passati acceduti in delete, D, 
i parametri globali che sono stati cancellati, globalD e i parametri globali che sono stati acceduti in lettura o scrittura, globalRW.
 Il tipo di una funzione e' percio' ottenuto utilizzando questi 4 array, e i controlli sono effettuati su questi.
-Le funzioni possono cancellare anche variabili globali.
Ad esempio nel test
~~~~{int x=3;
    {
        h(){ delete x;}
        int x=1;
        g(){
            h();
            delete x;
        }
        {
            int x=2;
            f(){
                g();
                delete x;
                print x;
            }
            f();
            print x;
        }
        print x;
    }
   print x;
}
~~~~
vengono rilevati 3 errori semantici nelle 3 print.
- Durante il controllo dei tipi delle funzioni e dei branch dell'if-else, e' stato pure checkato in che posizione
le variabili siano cancellate.
Se non fosse stato fatto questo controllo il seguente codice avrebbe dovuto restituire un errore, poiche' si sta accedendo
in lettura e in delete alla stessa variabile. 
~~~~
{f(var int x){ x=x+1; del x;} int a=2; f(a)} 
~~~~

#Generazione di codice

Per facilitare tutto il lavoro e' stato deciso di creare una seconda grammatica, per facilitare
la costruzione dell'interprete. In questo modo, partendo da un codice assembly si generano i token e la VM non fa' altro
che fare operazioni sulla lista di token. Per questa ragione le scelte progettuali sono tutte sulla creazione del codice 
assembly, e non sull'interprete.
- Inizializzazione: i registri fp, al e sp sono stati inizializzati a MEMSIZE, i rimanenti a 0
- Preappending delle funzioni: per evitare che l'interprete entri nelle definizioni di funzioni senza che esse siano state invocate
si e' utilizato il trick di inserire tutto il bytecode di tutte le funzioni all'inizio del codice e di circondare cio' da un jump e dalla label "start"
- Pila all'apertura di un nuovo scope: uno scope, che non sia quello di una funzione, si aggiungono allo stack i seguenti registri:
si pusha *fp*,*al* e un registro a caso per avere lo stesso numero di push rispetto alla dichiarazione di funzione
- Pila per la dichiarazione di funzione: si e' deciso di seguire le specifiche viste a lezione.L'unica differenza e' l'offset
in cui si troveranno le variabili, che e' diverso, poiche' l'invocazione di funzione fara' piu' push rispetto a quelle viste a
lezione
- Pila per la invocazione di funzione: dato che vi era il problema di sapere dove si trovassero le variabili passate per riferimento,
allo stack presentato a lezione sono stati aggiunti anche gli indirizzi dei parametri. In sintesi allo stack vengono pushati i seguenti valori:
(valore del parametro, indirizzo del parametro) *n parametri, *fp*, *al*, e infine *ra*
In questo modo le variabili passate per riferimento vengono accedute e modificate nel loro spazio di memoria 


{int x = 56;

 f(var int x){

 	if (x == 0) then { print  x; }

 	else { int y = x-1 ; f(y) ;  }

 	delete x ;
}
 f(x) ;
}