#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** DEFINIZIONE DELLE STRUTTURE DATI  */
typedef struct stazione {
    int distanza;
    int numero_auto;
    int parco_veicoli[512];
    int max_auto; //auto con autonomia massima
    char c; //colore
    struct stazione * p;
    struct stazione * left;
    struct stazione * right;
} stazione;

enum color{
    RED = 'r',
    BLACK = 'b'
};

typedef struct nodo_percorso{
    int distanza;
    int max_auto;
    struct nodo_percorso * next;
}nodo_percorso;

char end_of_file = '0';
int nodo_minimo;
int minimo_stazione;
int minimo_auto = 0;
int nodi_raggiunti = 0;

/** DICHIARAZIONE DEI METODI */
void aggiungi_stazione(struct stazione ** root, struct stazione * NIL);
void demolisci_stazione(struct stazione ** root, struct stazione * NIL);
void aggiungi_auto(struct stazione ** root, struct stazione * NIL);
void rottama_auto(struct stazione ** root, struct stazione * NIL);
void pianifica_percorso(struct stazione * root, struct stazione * NIL);
int cerca_percorso(int start, int end, struct stazione * x, struct stazione * NIL);
int cerca_percorso_inverso(int end,  nodo_percorso * partenza, struct stazione * root, struct stazione * x, struct stazione * NIL);
int cerca_minimo_raggiungibile(int distanza, int last, struct stazione * x, struct stazione * NIL);
void controllo_finale(int end, int num_stazioni, struct nodo_percorso *first, struct stazione *root, struct stazione * NIL);
nodo_percorso * controlla_sequenza(struct nodo_percorso * prec, struct nodo_percorso *last, struct nodo_percorso * succ, struct stazione * x, struct stazione * NIL);
stazione* crea_stazione();
void rb_insert(struct stazione ** T, struct stazione * z, struct stazione * NIL);
void rb_insert_fixup(struct stazione ** T, struct stazione * z, struct stazione * NIL);
void right_rotate(struct stazione ** T, struct stazione * x, struct stazione * NIL);
void left_rotate(struct stazione ** T, struct stazione * x, struct stazione * NIL);
stazione* inserimento_auto(struct stazione * x, int distanza, struct stazione * NIL);
stazione * tree_search(struct stazione * x, int distanza, struct stazione * NIL);
stazione * rb_delete(struct stazione ** T, struct stazione * z, struct stazione * NIL);
stazione * tree_minimum(struct stazione * x, struct stazione * NIL);
void rb_delete_fixup(struct stazione ** T, struct stazione * x, struct stazione * y, struct stazione * NIL);
void rb_transplant(struct stazione ** T, struct stazione * u, struct stazione * v, struct stazione * NIL);


int main() {
    char * input_string;
    input_string = (char*)malloc(20);

    struct stazione * NIL = malloc(sizeof(struct stazione));
    NIL->c = BLACK;
    NIL->distanza = -1;
    struct stazione * root;
    root = malloc(sizeof(struct stazione));
    root = NIL;

    do{
        if(scanf("%s", input_string)!=1){
            return 0;
        }

        //5 possibili input
        if(strcmp(input_string,"aggiungi-stazione") == 0){
            aggiungi_stazione(&root, NIL);

        } else if(strcmp(input_string,"demolisci-stazione") == 0){
            demolisci_stazione(&root, NIL);

        } else if(strcmp(input_string,"aggiungi-auto") == 0){
            aggiungi_auto(&root, NIL);

        } else if(strcmp(input_string,"rottama-auto") == 0){
            rottama_auto(&root, NIL);

        } else if(strcmp(input_string,"pianifica-percorso") == 0){
            pianifica_percorso(root, NIL);

        } else{
            end_of_file = '1';
        }
    } while (end_of_file != '1');
    free(input_string);
}

stazione* crea_stazione(){
    stazione* nuova_stazione = (stazione*)malloc(sizeof(stazione));
    if(nuova_stazione!=NULL){
        if(scanf("%d", &(nuova_stazione->distanza))!=1){
            return 0;
        }
        if(scanf("%d", &(nuova_stazione->numero_auto))!=1){
            return 0;
        }
        nuova_stazione->max_auto = 0;
        for(int i=0; i<nuova_stazione->numero_auto; i++){
            if(scanf("%d", &(nuova_stazione->parco_veicoli[i]))!=1){
                return 0;
            }
            if(nuova_stazione->parco_veicoli[i] > nuova_stazione->max_auto){
                nuova_stazione->max_auto = nuova_stazione->parco_veicoli[i];
            }
        }
        nuova_stazione->p = NULL;
        nuova_stazione->left = NULL;
        nuova_stazione->right = NULL;
    }
    return nuova_stazione;
}

void aggiungi_stazione(stazione** root, stazione * NIL){
    stazione * nuova_stazione = crea_stazione();
    rb_insert(root, nuova_stazione, NIL);
}

void rb_insert(struct stazione ** T, struct stazione * z, struct stazione * NIL){
    struct stazione * y = NIL; // padre del nodo considerato
    struct stazione * x = *T; // nodo considerato(radice)

    while(x!=NIL){
        y=x;
        if(z->distanza < x->distanza){
            x = x->left;
        } else if(z->distanza > x->distanza){
            x = x->right;
        } else if(z->distanza == x->distanza){
            free(z);
            printf("non aggiunta\n");
            return;
        }
    }
    z->p = y;
    if(y==NIL){
        *T = z;
    } else if(z->distanza < y->distanza){
        y->left = z;
    } else{
        y->right = z;
    }
    z->left = NIL;
    z->right = NIL;
    z->c = RED;
    rb_insert_fixup(T, z, NIL);
    printf("aggiunta\n");
}

void rb_insert_fixup(struct stazione ** T, struct stazione * z, struct stazione * NIL){
    struct stazione * y;
    while(z->p->c == RED){
        if(z->p == z->p->p->left){
            y = z->p->p->right;
            if(y->c == RED){
                z->p->c = BLACK;
                y->c = BLACK;
                z->p->p->c = RED;
                z = z->p->p;
            } else{
                if(z == z->p->right){
                    z = z->p;
                    left_rotate(T, z, NIL);
                }
                z->p->c = BLACK;
                z->p->p->c = RED;
                right_rotate(T, z->p->p, NIL);
            }
        } else if(z->p == z->p->p->right){
            y = z->p->p->left;
            if(y->c == RED){
                z->p->c = BLACK;
                y->c = BLACK;
                z->p->p->c = RED;
                z = z->p->p;
            } else{
                if(z == z->p->left){
                    z = z->p;
                    right_rotate(T, z, NIL);
                }
                z->p->c = BLACK;
                z->p->p->c = RED;
                left_rotate(T, z->p->p, NIL);
            }
        }
    }
    (*T)->c = BLACK;
}

void right_rotate(struct stazione ** T, struct stazione * x, struct stazione * NIL){
    struct stazione * y = x->left;
    x->left = y->right;
    if(y->right != NIL)
        y->right->p = x;
    y->p = x->p;
    if(x->p == NIL) {
        *T = y;
    } else if(x == x->p->right){
        x->p->right = y;
    } else {
        x->p->left = y;
    }
    y->right = x;
    x->p = y;
}

void left_rotate(struct stazione ** T, struct stazione * x, struct stazione * NIL){
    struct stazione * y = x->right;
    x->right = y->left;
    if(y->left != NIL)
        y->left->p = x;
    y->p = x->p;
    if(x->p == NIL) {
        *T = y;
    } else if(x == x->p->left){
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;
}


void demolisci_stazione(struct stazione ** root, struct stazione * NIL){
    struct stazione * z;
    int distanza;
    if(scanf("%d", &distanza)!=1){
        return;
    }
    z = tree_search(*root, distanza, NIL);
    if(z == NIL){
        printf("non demolita\n");
    } else {
        rb_delete(root, z, NIL);
        printf("demolita\n");
    }
}

stazione * tree_search(struct stazione * x, int distanza, struct stazione * NIL){
    if(x == NIL || x->distanza == distanza){
        return x;
    }
    if(distanza < x->distanza){
        return tree_search(x->left, distanza, NIL);
    } else{
        return tree_search(x->right, distanza, NIL);
    }
}

stazione * rb_delete(struct stazione ** T, struct stazione * z, struct stazione * NIL){
    struct stazione * y = z;
    struct stazione * x;
    char y_c = y->c;

    if(z->left == NIL){
        x = z->right;
        rb_transplant(T, z, z->right, NIL);
    } else if(z->right == NIL){
        x = z->left;
        rb_transplant(T, z, z->left, NIL);
    } else{
        y = tree_minimum(z->right, NIL);
        y_c = y->c;
        x = y->right;
        if(y->p == z){
            x->p = y;
        } else{
            rb_transplant(T, y, y->right, NIL);
            y->right = z->right;
            y->right->p = y;
        }
        rb_transplant(T, z, y, NIL);
        y->left = z->left;
        y->left->p = y;
        y->c = z->c;
    }
    if(y_c == BLACK){
        rb_delete_fixup(T, x, y, NIL);
    }
    (*T)->p = NIL;
    return y;
}

stazione * tree_minimum(struct stazione * x, struct stazione * NIL){
    while(x->left != NIL){
        x = x->left;
    }
    return x;
}

void rb_delete_fixup(struct stazione ** T, struct stazione * x, struct stazione * y, struct stazione * NIL){
    while(x != *T && x->c == BLACK){
        if(x == x->p->left){
            struct stazione * w = x->p->right;
            if(w->c == RED){
                w->c = BLACK;
                x->p->c = RED;
                left_rotate(T, x->p, NIL);
                w = x->p->right;
            }
            if(w->left->c == BLACK && w->right->c == BLACK){
                w->c = RED;
                x = x->p;
            } else {
                if(w->right->c == BLACK){
                    w->left->c = BLACK;
                    w->c = RED;
                    right_rotate(T, w, NIL);
                    w = x->p->right;
                }
                w->c = x->p->c;
                x->p->c = BLACK;
                w->right->c = BLACK;
                left_rotate(T, x->p, NIL);
                x = *T;
            }
        } else if(x == x->p->right){
            struct stazione * w = x->p->left;
            if(w->c == RED){
                w->c = BLACK;
                x->p->c = RED;
                right_rotate(T, x->p, NIL);
                w = x->p->left;
            }
            if((w->right->c == BLACK) && (w->left->c == BLACK)){
                w->c = RED;
                x = x->p;
            } else {
                if(w->left->c == BLACK){
                    w->right->c = BLACK;
                    w->c = RED;
                    left_rotate(T, w, NIL);
                    w = x->p->left;
                }
                w->c = x->p->c;
                x->p->c = BLACK;
                w->left->c = BLACK;
                right_rotate(T, x->p, NIL);
                x = *T;
            }
        }
    }
    x->c = BLACK;
}

void rb_transplant(struct stazione ** T, struct stazione * u, struct stazione * v, struct stazione * NIL){
    if(u->p == NIL){
        *T  = v;
    } else if(u == u->p->left){
        u->p->left = v;
    } else
        u->p->right = v;
    v->p = u->p;
}

void aggiungi_auto(struct stazione ** root, struct stazione * NIL){
    int distanza;
    if(scanf("%d", &distanza)!=1){
        return;
    }

    struct stazione * stazione1 = inserimento_auto(*root, distanza, NIL);
    if (stazione1 == NIL){
        printf("non aggiunta\n");
        return;
    } else if (stazione1->distanza == distanza){
        printf("aggiunta\n");
    }
}

stazione* inserimento_auto(struct stazione * x, int distanza, struct stazione * NIL){
    if(x == NIL){
        int autonomia;
        if(scanf("%d", &autonomia)!=1)
            return NIL;
        return NIL;
    }else if(distanza == x->distanza){
        int autonomia;
        if(scanf("%d", &autonomia)!=1)
            return NIL;
        if(autonomia > x->max_auto){
            x->max_auto = autonomia;
        }
        x->parco_veicoli[x->numero_auto] = autonomia;
        x->numero_auto ++;
        return x;
    }
    if(distanza < x->distanza){
        return inserimento_auto(x->left, distanza, NIL);
    } else{
        return inserimento_auto(x->right, distanza, NIL);
    }
}

void rottama_auto(struct stazione ** root, struct stazione * NIL){
    int distanza, automobile, i;
    if(scanf("%d", &distanza)!=1)
        return;
    if(scanf("%d", &automobile)!=1)
        return;
    struct stazione *z;
    z = tree_search(*root, distanza, NIL);

    if(z == NIL){
        printf("non rottamata\n");
    } else {
        for(i=0; i<z->numero_auto; i++){
            if(z->parco_veicoli[i] == automobile){
                z->parco_veicoli[i] = z->parco_veicoli[z->numero_auto-1];
                z->parco_veicoli[z->numero_auto-1] = -1;
                z->numero_auto--;
                if(automobile == z->max_auto){
                    z->max_auto = 0;
                    for(int j=0; j<z->numero_auto; j++){
                        if(z->parco_veicoli[j] > z->max_auto){
                            z->max_auto = z->parco_veicoli[j];
                        }
                    }
                }
                i=520;
                printf("rottamata\n");
            }
        }
        if(i==z->numero_auto){
            printf("non rottamata\n");
        }
    }
}

void pianifica_percorso(struct stazione * root, struct stazione * NIL) {
    int start, end;
    if (scanf("%d", &start) != 1)
        return;
    if (scanf("%d", &end) != 1)
        return;

    if (start == end) {
        printf("%d\n", start);
    } else if (start < end) {
        int ultimo = end;
        struct nodo_percorso *first;
        struct nodo_percorso *temp = malloc(sizeof(nodo_percorso));
        temp->next = NULL;
        temp->distanza = end;
        do {
            ultimo = cerca_percorso(start, ultimo, root, NIL);
            if (ultimo != -1) {
                struct nodo_percorso *nodo = malloc(sizeof(nodo_percorso));
                nodo->distanza = ultimo;
                nodo->next = temp;
                temp = nodo;
            }
        } while (ultimo != -1 && ultimo != start);

        if (ultimo == -1) {
            printf("nessun percorso\n");
        } else if (ultimo == start) {
            while (temp != NULL && temp->distanza != -1) {
                if (temp->next != NULL) {
                    printf("%d ", temp->distanza);
                } else {
                    printf("%d\n", temp->distanza);
                }
                first = temp;
                temp = temp->next;
                free(first);
            }
        }

    } else if (start > end) {
        int res;
        struct nodo_percorso *temp, *nodo;
        struct nodo_percorso *first = malloc(sizeof(nodo_percorso));
        first->next = NULL;
        first->distanza = start;
        first->max_auto = tree_search(root, start, NIL)->max_auto;
        nodo_minimo = start;
        minimo_stazione = start;
        temp = first;
        int num_stazioni=1;
        do{
            nodi_raggiunti = 0;
            res = cerca_percorso_inverso(end, temp, root, root, NIL);
            if(res!=-1 || minimo_stazione == end) {
                num_stazioni++;
                nodo = malloc(sizeof(nodo_percorso));
                nodo->distanza = minimo_stazione;
                nodo->max_auto = minimo_auto;
                nodo->next = NULL;
                temp->next = nodo;
                temp = nodo;
            }
        } while(res!=-1 && temp->distanza != end);


        if (res == -1  && minimo_stazione != end) {
            printf("nessun percorso\n");
        } else {
            controllo_finale(end, num_stazioni, first, root, NIL);
        }
    }
}

int cerca_percorso(int start, int end, struct stazione * x, struct stazione * NIL){
    if(x!=NIL) {
        if(x->distanza > start) {
            int risultato_sx = cerca_percorso(start, end, x->left, NIL);
            if (risultato_sx != -1) {
                return risultato_sx;
            }
        }

        if(x->distanza >= end){
            return -1;
        } else if(x->distanza + x->max_auto >= end && x->distanza >= start && x->distanza < end){
            return x->distanza;
        }

        if(x->distanza < end)
            return cerca_percorso(start, end, x->right, NIL);

    }
    return -1;
}

int cerca_percorso_inverso(int end, struct nodo_percorso * partenza, struct stazione * root, struct stazione * x, struct stazione * NIL){
    if(x!=NIL) {
        if(x->distanza < partenza->distanza) {
            int risultato_dx = cerca_percorso_inverso(end, partenza, root, x->right, NIL);
            if (risultato_dx != -1) {
                return risultato_dx;
            }
        }

        if(x->distanza <= partenza->distanza && (partenza->distanza - partenza->max_auto) > x->distanza) {
            if (nodi_raggiunti == 0) {
                return -1;
            } else {
                return 1;
            }
        } else if(x->distanza == end){
            minimo_stazione = x->distanza;
            minimo_auto = x->max_auto;
            return 1;
        } else if(x->distanza < partenza->distanza && (partenza->distanza - partenza->max_auto) <= x->distanza){
            if(x->distanza - x->max_auto <= end){
                minimo_stazione = x->distanza;
                minimo_auto = x->max_auto;
                nodo_minimo = end;
            } else if(x->distanza - x->max_auto <= nodo_minimo) {
                nodo_minimo = cerca_minimo_raggiungibile(x->distanza - x->max_auto, x->distanza, root, NIL);
                minimo_stazione = x->distanza;
                minimo_auto = x->max_auto;
            }
            nodi_raggiunti ++;
        }

        if(x->distanza > end) {
            return cerca_percorso_inverso(end, partenza, root, x->left, NIL);
        }
    }
    return -1;
}

int cerca_minimo_raggiungibile(int distanza, int last, struct stazione * x, struct stazione * NIL) {
    if (x == NIL) {
        return last;
    }

    if (x->distanza < last && distanza <= x->distanza) {
        last = x->distanza;
    }

    if(distanza < x->distanza){
        return cerca_minimo_raggiungibile(distanza, last, x->left, NIL);
    } else{
        return cerca_minimo_raggiungibile(distanza, last, x->right, NIL);
    }
}

void controllo_finale(int end, int num_stazioni, struct nodo_percorso *first, struct stazione *root, struct stazione * NIL){
    struct nodo_percorso * nodo = first, *min, *temp;
    int j = num_stazioni-1;
    struct nodo_percorso *res[num_stazioni];
    temp = malloc(sizeof(nodo_percorso));

    if(nodo != NULL && j>=0) {
        do {
            res[j] = nodo;
            j--;
            nodo = nodo->next;
        } while (nodo != NULL && j >= 0);
    }

    j = 1;
    while(j<num_stazioni-1){
        temp->distanza = res[j+1]->distanza;
        temp->max_auto = res[j+1]->max_auto;
        min = controlla_sequenza(res[j+1], temp, res[j-1], root, NIL);
        if(min != NULL && min->distanza < res[j]->distanza){
            res[j]->distanza = min->distanza;
            res[j]->max_auto = min->max_auto;
        }
        j++;
    }

    for(int i = (num_stazioni-1); i>=0; i--){
        if(i==0)
            printf("%d\n", res[i]->distanza);
        else
            printf("%d ", res[i]->distanza);
    }
}


nodo_percorso * controlla_sequenza(struct nodo_percorso * prec, struct nodo_percorso *last, struct nodo_percorso * succ, struct stazione * x, struct stazione * NIL) {
    if (x != NIL) {
        if (x->distanza > (prec->distanza - prec->max_auto)) {
            nodo_percorso *risultato_sx = controlla_sequenza(prec, last, succ, x->left, NIL);
            if (risultato_sx != NULL) {
                return risultato_sx;
            }
        }

        if (x->distanza >= (prec->distanza - prec->max_auto) && (x->distanza - x->max_auto) <= succ->distanza && x->distanza > succ->distanza && x->distanza < last->distanza) {
            last->distanza = x->distanza;
            last->max_auto = x->max_auto;
        }

        if(x->distanza > prec->distanza){
            return last;
        }

        if (x->distanza < prec->distanza)
            return controlla_sequenza(prec, last, succ, x->right, NIL);
    }

    return NULL;
}