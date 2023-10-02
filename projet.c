# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <assert.h>
# include <math.h>



// exercice 1

// renommage et déclaration des type etat, transition et automate
typedef struct etat_s etat;
typedef struct transition_s transition;
typedef struct automate_s automate;

/*
etat : 
    - indentifiant : nom de l'état
    - final : dis si l'état est final
    - transi_l : liste des transitions 
    - nb_transi : nb de transition de l'état

*/    
struct etat_s {
    int identifiant;
    bool final;
    transition ** transi_l;
    int nb_transi;
};

/*
transition :
    - etiq : étiquette de la transition
    - dest : état d'arriver de la transition

*/
struct transition_s {
    char etiq;
    etat * dest;
};

/*
automate :
    - nb_etat : nombre d'etat dans l'automate
    - etat_init : pointeur vers l'état initial de l'automate
    - etat_l : liste des états de l'automate
*/
struct automate_s {
    int nb_etat;
    etat * etat_init;
    etat ** etat_l;
};

// pile pour parcours en profondeur; stocke des entiers dans un tableau
typedef struct int_arr_s pile;
// struct utiliser comme buffer simple
typedef pile int_arr;

/*
pile :
    - cont : contenu, tableau des entiers
    - cap : capacité maximum du tableau
    - len : longueur du tableau remplis
*/
struct int_arr_s {
    int * cont;
    int len;
    int cap;
};

// cree une pile vide de capacite cap
pile * make_pile(int cap){
    // créer une pile sur le tas
    pile * ret = malloc(sizeof(*ret));
    // vérifie que malloc n'a pas échouer
    assert(ret != NULL);

    // initialisation : longueur à 0; capacité à cap ou 2; tableau de taille cap vide 
    ret->len = 0;
    // éviter erreur si cap <= 0;
    if (cap <= 0)
        cap = 2;
    ret->cap = cap;
    ret->cont = malloc(sizeof(ret->cont) * ret->cap);
    // vérifie que malloc n'a pas échouer
    assert(ret->cont != NULL);
    return ret;
}

// créer une liste vide
int_arr * make_int_arr(int cap){
    return make_pile(cap);
}

// empile x à p
void pile_push(pile * p, int x){
    // extension du buffer s'il n'y a plus assez de place dans le tableau
    if (p->len + 1 == p->cap){
        // on double la capacité
        p->cap = 2 * p->cap;
        p->cont = realloc(p->cont, sizeof(int) * p->cap);
        // vérifie que realloc n'a pas échouer
        assert(p->cont != NULL);
    }
    // place le nouvelle élément en dernière position
    p->cont[p->len] = x;
    // la longueur a augmenté de 1 élément
    p->len++;
}

// empile x à p, alias de pile_push
void empiler(pile * p, int x){
    pile_push(p, x);
}

// ajoute l'entier x à la fin, alias de empiler car même fonction et même type
void int_arr_append(int_arr * tab, int x){
    empiler(tab, x);
}

// renvoie si une pile est vide, ie ça longueur est égal à 0
bool is_empty_pile(pile * p){
    return p->len == 0;
}

// depile le sommet de la pile p et le renvoie
int pile_pop(pile * p){
    // vérifie que la pile existe et est non vide
    assert(p != NULL); assert(!is_empty_pile(p));
    // réduction de la longueuer
    p->len--;
    // extraction du sommet
    int ret = p->cont[p->len];
    // réduction de la capacité de la pile en cas de non nécessité
    if (p->len <= (1/4 * p->cap + 1)){
        // capacité / 2 + 1
        p->cap = p->cap / 2 + 1;
        // recopiage du contenu
        p->cont = realloc(p->cont, sizeof(int) * p->cap);
        // vérifie que malloc n'a pas échouer
        assert(p->cont != NULL);
    }
    // retourne le sommet
    return ret;
}

// depile p, alias de pile_pop
int depiler(pile *p){
    return pile_pop(p);
}

// supprime et renvoie le dernier entier de la liste, alias de pile_pop car même type et même fonction
int int_arr_pop(int_arr * tab){
    return pile_pop(tab);
}

// renvoie l'entier sur la pile sans le depiler
int on_top(pile * p){
    return p->cont[p->len - 1];
}

// renvoie le dernier entier d'une liste d'entier
int int_arr_last(int_arr * tab){
    return tab->cont[tab->len - 1];
}

// renvoie si l'entier x est dans la liste d'entier lis
bool is_in_int(int x, int * lis, int n){
    // parcours tous les indices 
    for (int i = 0; i < n; i++)
        // si x est égal à l'element i, retourne vrai
        if (lis[i] == x)
            return true;
    //  tous les indices sont parcourus, x n'est pas dedans, retourne faux
    return false;
}

// renvoie si l'entier x est dans la pile
bool is_in_pile(int x, pile * p){
    // si la pile est vide retourne faux
    if (p == NULL)
        return false;
    else
        // recherche si x est dans la pile
        return is_in_int(x, p->cont, p->len);
}

// renvoie si l'entier x est dans la liste
bool is_in_int_arr(int x, int_arr * tab){
    return is_in_pile(x, tab);
}

// étend la capacite de p
void new_cap_pile(pile * p, int n_cap){
    if (p != NULL)
    {
        p->cap = n_cap;
        p->cont = realloc(p, sizeof(int) * n_cap);
        assert(p->cont != NULL);
    }
}

// retourne l'indice du max d'un tableau d'entier, prend un tableau et sa longueur en argument
int get_i_max_tab(int * tab, int n){
    int ret = 0;
    for (int i = 1; i < n; i++)
        if (tab[i] > tab[ret]) 
            ret = i;
    return ret;
}

// échange l'élément i et j de tab
void switch_int(int * tab, int i, int j){
    int tmp = tab[i];
    tab[i] = tab[j];
    tab[j] = tmp;
}

// tri une liste d'entier dans l'ordre croissant en O(n²)
void tri_selection(int * tab, int n){
    for (int i = n - 1; i >= 0; i--){
        int maxq = get_i_max_tab(tab, i + 1);
        switch_int(tab, i, maxq);
    }
}


// tri un tableau stocké dans une structure de buffer dans l'ordre croissant
void tri_int_arr(int_arr * tab){
    tri_selection(tab->cont, tab->len);
}

// libere une pile p ou un int_arr p
void free_pile(pile * p){
    if (p != NULL){
        // s'il y a un tableau stocké dans la pile
        if (p->cont != NULL)
            free(p->cont);
        free(p);
        p = NULL;
    }
}

// libère un int_arr tab
void free_int_arr(int_arr * tab){
    free_pile(tab);
}

// tabint pour stocker une liste de liste de d'entier extensible
typedef struct tabint_s tabint;
// même structure que int_arr, le type stocké change en liste de tableau
struct tabint_s {
    int_arr ** cont;
    int len, cap;
};

// crée une liste de liste d'entier vide de capacite cap
tabint * make_tabint(int cap){
    // alloue le tableau
    tabint * ret = malloc(sizeof(*ret));
    // vérifie que malloc n'a pas échouer
    assert(ret != NULL);
    // initialisation : len : 0; cap : cap si cap > 0 , 2 sinon ; une liste vide
    ret->len = 0;
    if (cap <= 0)
        cap = 2;
    ret->cap = cap;
    ret->cont = malloc(sizeof(ret->cont) * ret->cap);
    // vérifie que malloc n'a pas échouer
    assert(ret->cont != NULL);
    return ret;
}


// empile la liste d'entier etl à la liste de liste tab
void append_iia(tabint * tab, int_arr * etl){
    // double la capacité s'il n'y a plus de place pour le nouvel élément
    if (tab->len + 1 == tab->cap){
        tab->cap = 2 * tab->cap;
        tab->cont = realloc(tab->cont, sizeof(int_arr *) * tab->cap);
        // vérifie que malloc n'a pas échouer
        assert(tab->cont != NULL);
    }
    // ajoute l'élément trié à la fin
    tri_int_arr(etl);
    tab->cont[tab->len] = etl;
    tab->len++;
}

// renvoie si une liste de liste d'entier tab est vide
bool tabint_is_empty(tabint * tab){
    return tab->len == 0;
}

// supprime et renvoie le dernier élément de tab
int_arr * tabint_pop(tabint * tab){
    // vérifie que la pile éxiste et est non vide
    assert(tab != NULL); assert(!tabint_is_empty(tab));
    tab->len--;
    // extraction de la liste à retourner
    int_arr * ret = tab->cont[tab->len];
    // réadapte la capacité de la liste
    if (tab->len <= (1/4 * tab->cap)){
        tab->cap = tab->cap / 2;
        tab->cont = realloc(tab->cont, sizeof(pile *) * tab->cap);
        // vérifie que malloc n'a pas échouer
        assert(tab->cont != NULL);
    }
    return ret;
}


// renvoie le dernier élément de tab sans le supprimer
int_arr * tabint_last(tabint * tab){
    return tab->cont[tab->len - 1];
}


// renvoie si arr est dans la liste de liste
bool is_in_tabint(int_arr * arr, tabint * tab){
    if (tab == NULL)
        return false;
    else
        for (int i = 0; i < tab->len; i++)
            if (arr == tab->cont[i])
                return true;
    return false;
}

// libere une liste de liste de d'entier
void free_tabint(tabint * tab){
    if (tab != NULL)
    {
        // libère tout les élément de la liste
        for (int i = 0; i < tab->len; i++)
            free_int_arr(tab->cont[i]);
        // libère la liste d'élément
        free(tab->cont);
        // libère la liste
        free(tab);
        tab = NULL;
    }
}


// renvoie l'élément i du tableau
int_arr * get_i_arr(tabint * arr, int i){
    if (i < 0 || i >= arr->len)
        return NULL;
    return arr->cont[i];
}



// renvoie l'indice de la liste tab dans le tableau arr
int get_index_tabint(tabint * arr, int_arr * tab){
    // considère que -1 signifie qu'elle n'est pas dedans
    int index = -1;
    // on trie la liste car arr ne contient que des liste trié
    tri_int_arr(tab);
    // si le programme trouve la liste alors pas besoin de continuer à chercher
    bool same = false;
    // on parcours la liste de liste
    for (int i = 0; (!same) && i < arr->len; i++){
        // élément de la liste
        int_arr * li = arr->cont[i];
        // si les deux liste ont la même longueur
        if (tab->len == li->len){
            // le programme considére que les liste sont égales tant qu'il n'y pas de différence
            same = true;
            // test de tout les éléments des listes
            for (int j = 0; j < tab->len; j++){
                if (tab->cont[j] != li->cont[j])
                    same = false;
            }
            // si elle sont les mêmes
            if (same) {
                index = i;
            }
        }
    }
    return index;
}



// liste des fonctions


pile * make_pile(int cap);
int_arr * make_int_arr(int cap);
void empiler(pile * p, int x);
void int_arr_append(int_arr * tab, int x);
bool is_empty_pile(pile * p);
int pile_pop(pile * p);
int depiler(pile *p);
int int_arr_pop(int_arr * tab);
int on_top(pile * p);
int int_arr_last(int_arr * tab);
bool is_in_int(int x, int * lis, int n);
bool is_in_pile(int x, pile * p);
bool is_in_int_arr(int x, int_arr * tab);
void new_cap_pile(pile * p, int n_cap);
char * str_of_bool(bool a);
int get_i_max_tab(int * tab, int n);
void switch_int(int * tab, int i, int j);
void tri_selection(int * tab, int n);
void tri_int_arr(int_arr * tab);
void free_pile(pile * p);
void free_int_arr(int_arr * p);
tabint * make_tabint(int cap);
void append_iia(tabint * tab, int_arr * et);
bool tabint_is_empty(tabint * p);
int_arr * tabint_pop(tabint * p);
int_arr * tabint_last(tabint * tab);
bool is_in_tabint(int_arr * x, tabint * p);
void free_tabint(tabint * tab);
int_arr * get_i_arr(tabint * arr, int i);
int get_index_tabint(tabint * arr, int_arr * tab);
int max(int a, int b);
char * str_of_bool(bool a);

// exercice 1 les structs sont en premières lignes

etat * make_etat(int identifiant);
transition * make_transi2(char val, etat * dest);
transition * make_transi(char val, etat * dest);
automate * make_automate();

// exercice 2

etat * find_sommet(automate * autom, int sommet);
int * get_list_etat(automate * autom);
void fprint_list(FILE * stream, int *tab, int n);
void print_list(int *tab, int n);
int get_i_indix1(int_arr * tab, int i);
void print_pile(pile * p);
bool is_in_trans(transition * t1, transition ** tl, int n);
bool is_in_etat(etat * e1, etat ** el, int n);
void copy_trans_l(transition ** tl, transition ** new, int len);
void copy_etat_l(etat ** el, etat ** new, int len);
void free_trans(transition * t1);
void free_etat(etat * e1);
void print_trans(transition * t);
void free_trans_l(transition ** tl, int n);
void free_etat_l(etat ** el, int n);
void free_automate(automate * autom);
void switch_tr(etat * et, int i, int j);
int get_ind_trans(etat * et, transition * tr);
void del_transi(automate * autom, int sommet, transition * trans);

// exercice 3

void add_etat(automate * autom, etat * etat1);
void add_e_som(automate * autom, int sommet);
void set_final(etat * e);
void set_nfinal(etat * e);
void set_final_id(automate * ac, int id);
void set_nfinal_id(automate * ac, int id);

// exercice 4

void copy_etat(automate * autom, etat * e);
automate * copy_auto(automate * autom);
void add_transi_a(automate * autom, int depart, char etiquette, int arrive);

// exercice 5

void add_etat_final(automate * autom, etat * eta);
void print_finaux(automate * ac);
automate * read_automate(char * filename);

// exercice 6

void fprint_automate(FILE * stream, automate * autom);
void print_automate(automate * autom);

// exercice 7

bool is_deter(automate * ac);
int nb_arete(automate * ac);
bool is_in_char(char c, char * mot, int len);
int get_max_trans(automate * ac);
char * get_alpha(automate *ac);
int get_alpha_len(automate *ac);
bool is_complet(automate * ac);
bool is_asynchrone(automate * ac);

// exercice 8

bool is_recon(automate * ac, char * mot);

// exercice 9

void rename1(automate * ac);
void del_etat(automate * ac, etat * eta);
int_arr * parcours(automate * ac, etat * start);
int_arr * etats_acessibles(automate * ac);
bool is_accessible(automate * ac, etat * eta);
bool is_coaccessible(automate * ac, etat * eta);
int_arr * etats_coacessibles(automate * ac);
bool is_emonde(automate * ac);
void set_emonde(automate * ac);

// exercice 10

automate * inter_automate(automate * aca, automate * acb);

// exercice 11

int_arr * parcours_epsilon(automate * ac, etat * start);
etat * get_start(automate * ac);
transition * find_e_transi(etat * et);
void del_e(automate * ac, int sommet);
void del_e_transi(automate * ac);
void set_n_async(automate * ac);

// exercice 12

void free_str(char * str);
etat ** get_etat(automate * ac, int_arr * letat);
int get_index_char(char * mot, char c);
int_arr * get_neight(etat * et, char etiq);
int_arr * merge(int_arr ** ps, int n);
int_arr * get_neight_l(automate * ac, etat * et, char etiq, tabint * arr);
void print_arr(tabint * arr);
bool has_final(automate * ac, pile * p);
void print_boolarray(bool * tab, int len);
void set_deter(automate * ac);



// programme principale
int main(void);



// fonction qui renvoie le max de 2 nombres
int max(int a, int b){
    if (a < b) return b; else return a;
}

// renvoie le texte associe au booleen
char * str_of_bool(bool a){
    if (a) return "true"; else return "false";
}

// cree un etat
etat * make_etat(int identifiant){
    // alloue de l'espace
    etat * ret = malloc(sizeof(*ret));
    // vérifie que le malloc n'a pas échoué
    assert(ret != NULL);
    // initialisation des variables
    ret->identifiant = identifiant;
    ret->final = false;
    ret->transi_l = NULL;
    ret->nb_transi = 0;
    return ret;
}

// cree une transition
transition * make_transi2(char val, etat * dest){
    if (dest != NULL){
        // alloue de l'espace
        printf("rrrrrrrrrrrrrr1\n");
        transition * ret = NULL;
        ret = malloc(sizeof(transition));
        printf("rrrrrrrrrrrrrr2\n");
        // vérifie que le malloc n'a pas échoué
        assert(ret != NULL);
        // initialisation des variables
        ret->etiq = val;
        ret->dest = dest;
        return ret;
    }
    else
    {
        return NULL;
    }
    
}

// cree une transition
transition * make_transi(char val, etat * dest){
    if (dest != NULL){
        printf("make_transi %c %d\n", val, dest->identifiant);
        // alloue de l'espace, stratégie pour que ça marche
        // make_transi2 marche pas je ne sais pas pourquoi
        transition ** ret = NULL;
        ret = malloc(sizeof(transition*));
        ret[0] = malloc(sizeof(transition));
        // vérifie que le malloc n'a pas échoué
        assert(ret != NULL);
        // initialisation des variables
        ret[0]->etiq = val;
        ret[0]->dest = dest;
        transition * retval = ret[0];
        free(ret);
        return retval;
    }
    else
    {
        return NULL;
    }
    
}

// cree un automate
automate * make_automate(){
    // alloue de l'espace
    automate * ret = malloc(sizeof(* ret));
    // vérifie que le malloc n'a pas échoué
    assert(ret != NULL);
    // initialisation des variables
    ret->nb_etat = 0;
    ret->etat_l = NULL;
    ret->etat_init = NULL;
    return ret;
}


// exercice 2

// renvoie l'etait associe à l'entier sommet s'il éxiste NULL sinon
etat * find_sommet(automate * autom, int sommet){
    // parcours chaque état de l'automate 
    for (int i = 0; i < autom->nb_etat; i++){
        if (autom->etat_l[i]->identifiant == sommet)
            return autom->etat_l[i];
    }
    return NULL;
}

// renvoie la liste des identifiant des etats
int * get_list_etat(automate * autom){
    // alloue de l'espace
    int * ret = malloc(sizeof(* ret) * autom->nb_etat);
    // vérifie que le malloc n'a pas échoué
    assert(ret != NULL);
    // ajoute chaque identifiant
    for (int i = 0; i < autom->nb_etat; i ++){
        ret[i] = autom->etat_l[i]->identifiant;
    }
    return ret;
}

// affiche une liste sur le stream
void fprint_list(FILE * stream, int * tab, int n){
    // affiche pour chaque valeurs la valeur du tableau
    for (int i = 0; i < n; i++)
        fprintf(stream, "%d ", tab[i]);
    fprintf(stream, "\n");
}

// affiche une liste sur la sortie standard
void print_list(int * tab, int n){
    fprint_list(stdout, tab, n);
}

// donne le ieme élémentt de tab
int get_i_indix1(int_arr * tab, int i){
    // vérifie que le tableau éxiste et que l'émplacement aussi
    assert(tab != NULL); assert(0 <= i && i < tab->len);
    return tab->cont[i];
}

// affiche une pile
void print_pile(pile * p){
    /// vérifie que p éxiste 
    assert(p != NULL);
    // affichage d'un pile vide
    for (int i = 0; i < 3; i++){
        // printf("\t|   |\n");
    }
    // affichage de chaque valeur dans la pile
    for (int i = p->len - 1; i >= 0; i--){
        printf("\t| %d |\n", get_i_indix1(p, i));
    }
    // affichage fond de la pile
    // printf("\t|___|\n\n");
}

bool is_trans_egal(transition * t1, transition * t2){
    return t2->etiq == t1->etiq && t2->dest->identifiant == t1->dest->identifiant;
}

// renvoie si t1 est dans tl
bool is_in_trans(transition * t1, transition ** tl, int n){
    // parcours chaque transition et regarde si elle est dedans
    for (int i = 0; i < n; i++)
        if (is_trans_egal(tl[i], t1))
            return true;
    // il n'est pas dedans
    return false;
}


// renvoie true si etat1 est dans etat_l
bool is_in_etat(etat * etat1, etat ** etat_l, int n){
    if (etat1 != NULL)
    {
        // parcours chaque etat
        for (int i = 0; i < n; i++){
            if (etat_l[i]->identifiant == etat1->identifiant){
                return true;
            }
        }
    }
    // n'est pas dedans
    return false;
}

// copie les len élémentt de tl dans new
void copy_trans_l(transition ** tl, transition ** new, int len){
    for (int i = 0; i < len; i++){
        new[i] = tl[i];
    }
}

// copie les len élément de el dans new
void copy_etat_l(etat ** el, etat ** new, int len){
    for (int i = 0; i < len; i++){
        new[i] = el[i];
    }
}

// libère une transition
void free_trans(transition * t1){
    if(t1 != NULL)
        free(t1);
    t1 = NULL;
}

// libère un etat
void free_etat(etat * e1){
    if (e1 != NULL){
        // libère les transitions de l'état
        free_trans_l(e1->transi_l, e1->nb_transi);
        free(e1);
    }
    e1 = NULL;
}

// affiche une transition
void print_trans(transition * t){
    printf("%c %d", t->etiq, t->dest->identifiant);
}

// libère une liste de transition tl de longueur n
void free_trans_l(transition ** tl, int n) {
    // pour chaque transition, la libéreé
    for (int i = 0; i < n; i++) {
        print_trans(tl[i]);
        free_trans(tl[i]);
    }
    free(tl);
    tl = NULL;
}

// libère une liste d'etat el de longueur n
void free_etat_l(etat ** el, int n) {
    if (el != NULL){
        for (int i = 0; i < n; i++)
        {
            printf("el[%d] = %d\n", i, el[i]->identifiant);
            free_etat(el[i]);
        }
        free(el);
    }
    el = NULL;
}


// libère un automate
void free_automate(automate * autom){
    // s'il existe
    if(autom != NULL){
        // libère chaque état
        free_etat_l(autom->etat_l, autom->nb_etat);
        free(autom);
    }
    autom = NULL;
}


// ajoute trans à autom si elle n'existe pas déjà
void add_transi(automate * autom, int sommet, transition * trans){
    // on recherche l'état associé au sommet de départ
    etat * start = find_sommet(autom, sommet);
    // si le sommet de départ n'éxiste pas
    // ajoute le sommet
    if (start == NULL)
        add_e_som(autom, sommet);
    if (!is_in_trans(trans, start->transi_l, start->nb_transi)) {
        // adapte le nombre de transition de l'état
        start->nb_transi++;
        // recopie dans une liste plus grande
        start->transi_l = realloc(start->transi_l, sizeof(transition *) * start->nb_transi);
        // vérifie que le malloc n'échoue pas
        assert(start->transi_l != NULL);
        // ajoute le nouveau
        start->transi_l[start->nb_transi - 1] = trans;
        // libère le pointeur vers l'ancienne liste
    }
    else
        // elle éxiste déjà donc ne l'ajoute pas
        // la libère
        free_trans(trans);
}


// échange la transition i et j dans et->trans_l
void switch_tr(etat * et, int i, int j){
    transition * tmp = et->transi_l[i];
    et->transi_l[i] = et->transi_l[j];
    et->transi_l[j] = tmp;
}

// renvoie l'indice de la transition dans l'état
int get_ind_trans(etat * et, transition * tr){
    assert(et != NULL);
    for (int i = 0; i < et->nb_transi; i++)
        if (is_trans_egal(tr, et->transi_l[i]))
            return i;
    return -1;
}

// supprime une transi
void del_transi(automate * autom, int sommet, transition * trans){
    // récupère l'état de départ
    etat * start = find_sommet(autom, sommet);
    if (start == NULL || !is_in_trans(trans, start->transi_l, start->nb_transi))
    {
        printf(" error\n");
        free_trans(trans);
        return;
    }
    // place la transition en dernière position 
    switch_tr(start, get_ind_trans(start, trans), start->nb_transi - 1);
    // libère la transition donné en argument
    if (trans != start->transi_l[start->nb_transi - 1])
        free_trans(trans);
    free_trans(start->transi_l[start->nb_transi - 1]);
    // le nb de transi diminue de 1
    start->nb_transi--;
    // adapte la liste de transition
    start->transi_l = realloc(start->transi_l, sizeof(transition *) * (start->nb_transi + 1));
    // vérifie si le malloc échoué
    if (start->transi_l == NULL)
        start->transi_l = malloc(sizeof(transition *) * (start->nb_transi + 1));
}

// exercice 3


// ajoute un etat à l'automate
void add_etat(automate * autom, etat * etat1){
    // regarde s'il éxiste dans le l'automate
    bool iie = is_in_etat(etat1, autom->etat_l, autom->nb_etat);
    // s'il n'est pas encore mais qu'il éxiste
    if (!iie && etat1 != NULL){
        if (autom->etat_init == NULL)
            autom->etat_init = etat1;
        // adapte la taille de la liste d'automate
        autom->nb_etat++;
        autom->etat_l = realloc(autom->etat_l, sizeof(*autom->etat_l) * autom->nb_etat);
        // vérifie si le malloc échoue
        assert(autom->etat_l != NULL);
        // ajoute l'état
        autom->etat_l[autom->nb_etat - 1] = etat1;
    }
    else
        free_etat(etat1);
}

// ajoute un etat dans l'automate à partir de l'identifiant
void add_e_som(automate * autom, int sommet){
    // créeer un état vide ayant pour identifiant sommet
    etat * ne = make_etat(sommet);
    // ajoute cet état à l'automate
    add_etat(autom, ne);
}

// rend un etat terminal
void set_final(etat * e){
    if (e != NULL)
        e->final = true;
}

// rend un état non terminal
void set_nfinal(etat * e){
    if (e != NULL)
        e->final = false;
}

// rend l'état associé à id dans l'automate ac final
void set_final_id(automate * ac, int id){
    set_final(find_sommet(ac, id));
}

// rend l'état associé à id dans l'automate ac non final
void set_nfinal_id(automate * ac, int id){
    set_final(find_sommet(ac, id));
}


// exercice 4


// copie un etat
void copy_etat(automate * autom, etat * e){
    // recherche s'il n'est pas déjà dans l'automate
    etat * new_et = find_sommet(autom, e->identifiant);
    if (new_et == NULL){
        // ajoute l'état avec le même identifiant
        add_e_som(autom, e->identifiant);
        new_et = find_sommet(autom, e->identifiant);
    }
    // copie le
    if (e->final)
        set_final(new_et);
    // ajoute les transition de l'un dans l'automate
    for (int i = 0; i < e->nb_transi; i++){
        transition * ti = e->transi_l[i];
        // ajoute la transition au même propriété dans l'automate
        add_transi_a(autom, e->identifiant, ti->etiq, ti->dest->identifiant);
    }
}


// copie un automate
automate * copy_auto(automate * autom){
    // créer un nouvelle automate
    automate * ret = make_automate();
    // vérifie que le malloc n'échoue pas
    assert(ret != NULL);
    // on copie chaque état
    for (int i = 0; i < autom->nb_etat; i++){
        copy_etat(ret, autom->etat_l[i]);
    }
    // copiage du sommet initiale
    ret->etat_init = find_sommet(ret, autom->etat_init->identifiant);
    return ret;
}


// ajoute une transition en fonction de ses caractéristiques
void add_transi_a(automate * autom, int depart, char etiquette, int arrive){
    // cherche le sommet de destination
    etat * dest = find_sommet(autom, arrive);
    // crée l'état de destination
    if (dest == NULL)
    {
        add_e_som(autom, arrive);
        dest = find_sommet(autom, arrive);
    }
    // crée la transition
    transition * trans = make_transi(etiquette, dest);
    // l'ajoute à l'automate
    add_transi(autom, depart, trans);
    free(trans);
}

// exercice 5


// ajoute un état eta final à l'automate autom
void add_etat_final(automate * autom, etat * eta){
    // vérifie l'existence d'eta
    if (eta != NULL){
        // ajoute l'état s'il n'est pas dedans
        add_etat(autom, eta);
        // recherche l'état à mettre final
        etat * eta2 = find_sommet(autom, eta->identifiant);
        // le rend final
        set_final(eta2);
    }
}


// affiche les etats finaux
void print_finaux(automate * ac){
    // printf("finaux : ");
    for (int i = 0; i < ac->nb_etat; i++)
        if (ac->etat_l[i]->final)
            printf("%d\t", ac->etat_l[i]->identifiant);
    // printf("\n");
}

// renvoie un automate en lisant un fichier
automate * read_automate(char * filename){
    // crée un automate
    automate * ret = make_automate();
    // ouvre le fichier filename
    FILE * fichier = fopen(filename, "r");
    // récupère le nombre d'état
    int nb_eta;
    fscanf(fichier, "%d", &nb_eta);

    // ajoute le même nombre d'état dans l'automate
    for (int i = 0; i < nb_eta; i++)
        add_e_som(ret, i);

    
    // stocke le nombre d'état finaux
    int nb_fin;
    fscanf(fichier, "%d", &nb_fin);
    // récupère les états finaux et rend les états associé finaux
    for (int i = 0; i < nb_fin; i++){
        int eta_f;
        fscanf(fichier, "%d", &eta_f);
        // ajoute l'état finaux
        add_etat_final(ret, find_sommet(ret, eta_f));
    }

    // récupère l'identifiant du sommet initiale
    int et_init;
    fscanf(fichier, "%d", &et_init);
    
    // définit l'état initiale
    ret->etat_init = find_sommet(ret, et_init);

    // récupère le nombre de transition
    int nb_tran;
    fscanf(fichier, "%d", &nb_tran);
    // recupère chaque transition
    for (int i = 0; i < nb_tran; i++){
        int start, dest;
        char etiq;
        fscanf(fichier, "%d %c %d", &start, &etiq, &dest);
        // ajoute la transition dans l'automate
        add_transi_a(ret, start, etiq, dest);
    }
    return ret;
}


// exercice 6


// affiche un automate sur le stream selon le format predefinit
void fprint_automate(FILE * stream, automate * autom) {
    // écris le nombre d'état de l'automate
    fprintf(stream, "%d\n", autom->nb_etat);
    // initialisation des compteur à 0
    int nb_etat_f = 0;
    int nb_transi = 0;
    int etat_f_l[autom->nb_etat];
    // pour chaque état on regarde s'il est final et ses transitions
    for (int i = 0; i < autom->nb_etat; i++) {
        // ajoute le nombre de transition
        nb_transi += autom->etat_l[i]->nb_transi;
        // ajoute l'identifiant à la liste d'état final s'il l'est
        if (autom->etat_l[i]->final) {
            etat_f_l[nb_etat_f] = i;
            nb_etat_f++;
        }
    }
    // écris le nombre d'état final
    fprintf(stream, "%d\n", nb_etat_f);
    // écris la liste des états finals
    fprint_list(stream, etat_f_l, nb_etat_f);
    // écris l'identifiant de l'état 
    fprintf(stream, "%d\n", autom->etat_init->identifiant);
    // affiche le nombre de transition
    fprintf(stream, "%d\n", nb_transi);
    
    // écris chaque transition
    for (int i = 0; i < autom->nb_etat; i++) {
        etat * eta = autom->etat_l[i];
        for (int j = 0; j < eta->nb_transi; j++) {
            transition *transi = eta->transi_l[j];
            fprintf(stream, "%d %c %d\n", eta->identifiant, transi->etiq, transi->dest->identifiant);
        }
    }
}

// affiche un automate sur la sortie standard
void print_automate(automate * ac){
    fprint_automate(stdout, ac);
}


// exercice 7

// revoie si ac est deterministe
bool is_deter(automate * ac){
    // un automate déterministe n'est pas asynchrone
    if (!is_asynchrone(ac)){
        int n = ac->nb_etat;
        // pour chaque état
        for (int i = 0; i < n; i++){
            etat * eta = ac->etat_l[i];
            int m = eta->nb_transi;
            // pour chaque transition
            for (int j = 0; j < m; j++){
                char et = eta->transi_l[j]->etiq;
                // regarde si une autre transition en partance de l'état a la même étiquette
                for (int k = j + 1; k < m; k++){
                    char et2 = eta->transi_l[k]->etiq;
                    if (et2 == et)
                        return false;
                }
            }
        }
        /* ici, pour tout état, pour tout lettre dans l'alphabet de l'automate
        le nombre d'état accessible avec cette lettre est inférieur ou égal
        à 1, l'automate est donc déterministe */
        return true;
    }
    // ici, l'automate est non déterministe
    return false;
}

// renvoie le nombre d'arête de l'automate ac
int nb_arete(automate * ac){
    int count = 0, n = ac->nb_etat;
    for (int i = 0; i < n; i++){
        etat * eta = ac->etat_l[i];
        count += eta->nb_transi;
    }
    return count;
}


// renvoie si c est dans mot
bool is_in_char(char c, char * mot, int len){
    for (int i = 0; i < len; i++)
        if (c == mot[i])
            return true;
    return false;
}


// renvoie le nombre max des nombres de transition d'ac
int get_max_trans(automate * ac){
    int ret = 0;
    for (int i = 0; i < ac->nb_etat; i++){
        ret = max(ac->etat_l[i]->nb_transi, ret);
    }
    return ret;
}



// renvoie l'alphabet de ac
char * get_alpha(automate * ac){
    if (ac == NULL)
        return NULL;
    int n = ac->nb_etat, len = 0, cap = 4;
    // alloue de l'espace pour la liste de char
    char * ret = malloc(sizeof(* ret) * cap);
    // vérifie que le malloc n'a pas échoué
    assert(ret != NULL);
    // initialise le tableu à des caractère de fin de mot
    for (int j = 0; j < 4; j++)
        ret[j] = '\0';
    // pour chaque état
    for (int i = 0; i < n; i++){
        etat * eta = ac->etat_l[i];
        // pour chaque transi
        for (int j = 0; j < eta->nb_transi; j++){
            transition * tr = eta->transi_l[j];
            // si l'étiquette n'est pas dans la liste on l'ajoute
            if (!is_in_char(tr->etiq, ret, len)){
                // si la capacité n'est pas suffisante, on la double
                if (len + 1 == cap){
                    cap = cap * 2;
                    ret = realloc(ret, sizeof(* ret) * cap);
                    assert(ret != NULL);
                    for (int k = len; k < cap; k++)
                        ret[k] = '\0';
                }
                // on l'ajoute à la liste
                ret[len] = tr->etiq;
                len++;
            }
        }
    }
    return ret;
}


// renvoie la taille de l'alphabet
int get_alpha_len(automate *ac){
    if (ac == NULL)
        return 0;
    // récupère l'alphabet de l'automate
    char * ret = get_alpha(ac);
    // mesure sa taille
    int len = strlen(ret);
    // libère la chaine de caractère
    free(ret);
    return len;
}

// renvoie si un automate est complet
bool is_complet(automate * ac){
    int len_alpha = get_alpha_len(ac);
    /* un automate complet est déterministe et chaque état 
    a exactement le nombre de lettre de l'alphabet */
    if (is_deter(ac)){
        int n = ac->nb_etat;
        // pour chauqe état on vérifie que le nombre de transition 
        // en partance de l'état est éxactement la taille de l'alphabet
        for (int i = 0; i < n; i++){
            etat * eta = ac->etat_l[i];
            if (eta->nb_transi != len_alpha)
                // la condition n'est pas respecté, l'automate n'est pas complet
                return false;
        }
        // la propriété est vraie donc l'automate est complet
        return true;
    }
    // Ici, l'automate n'est pas complet
    return false;
}



// renvoie si ac est asynschrone
bool is_asynchrone(automate * ac){
    // récupère l'alphabet de l'automate
    char * alpha = get_alpha(ac);
    // récupère la longueur de l'alphabet
    int len = strlen(alpha);
    // regarde si une transition porte l'étiquette epsilon
    bool ret = is_in_char('1', alpha, len);
    // libère l'alphabet
    free(alpha);
    return ret;
}


// exercice 8

// renvoie si mot est reconnue par ac deterministe
bool is_recon(automate * ac, char * mot){
    // vérifie si l'automate éxiste
    if (ac == NULL)
        return false;
    // s'il n'est pas déterministe, le rend déterministe
    if(!is_deter(ac))
        set_deter(ac);
    // état actuelle dans l'automate
    etat * cursor = ac->etat_init;
    // pour chaque lettre du mot
    int  n = strlen(mot);
    for (int i = 0; i < n; i++)
    {
        // stocke si changement d'état
        bool next = false;
        // regarde les transitions en partance du curseur
        for (int j = 0; j < cursor->nb_transi && !next; j++){
            transition * trj = cursor->transi_l[j];
            // si on peut enprunter une transition en partance de l'état 
            next = trj->etiq == mot[i];
            // on l'emprunte 
            if (next)
            {
                cursor = trj->dest;
            }
        }
        // si aucune transition ne peut pas être emprunté, le mot n'est pas
        // reconnu
        if (!next)
        {
            return false;
        }
    }
    // toutes les lettres ont été parcourus
    return cursor->final;
}



// exercice 9

/* un automate est emonde si pourt tout ses etats, il existe
un chemin de l'etat initial à l'etat considerer (accessible)
et pour tout ses etats il existe un chemin de l'etat considere
à un etat final (coaccessible)
*/ 

// renomme les etats de ac de 0 à n-1
void rename1(automate * ac){
    for (int i = 0; i < ac->nb_etat; i++){
        ac->etat_l[i]->identifiant = i;
    }
}


// supprime eta de ac
void del_etat(automate * ac, etat * eta){
    // nomme les états en fonction de leur placement dans la liste d'état 
    rename1(ac);
    // pour chaque état
    for (int i = 0; i < ac->nb_etat; i++){
        etat * eti = ac->etat_l[i];
        // pour chaque transition
        for (int j = 0; j < eti->nb_transi; j++){
            // si la destination est l'état qui sera supprimer on la supprime
            transition * trj = eti->transi_l[j];
            if (trj->dest == eta){
                del_transi(ac, i, trj);
            }
        }
    }
    int id = eta->identifiant;
    // libère l'état à supprmier
    free_etat(eta);
    // créer une liste avec un emplacement en moins
    etat ** new = malloc(sizeof(* new) * (ac->nb_etat));
    // vérifie que le malloc n'a pas échoué
    assert(new != NULL);
    // réduction du nombre d'état
    ac->nb_etat--;
    // pour chaque état
    for (int i = 0; i < ac->nb_etat; i++)
    {
        // si leur identifiant est inférieur à celui qui sera supprimer
        //  leur place est inchangé sinon leur place est réduite de 1
        if (i < id)
            new[i] = ac->etat_l[i];
        else if (i >= id)
            new[i] = ac->etat_l[i + 1];
    }
    //  on change la liste
    free(ac->etat_l);
    ac->etat_l = new;
    free(new);
    // redistribut les id
    rename1(ac);
}


// renvoie le parcours en profondeur de l'automate à partir de start
int_arr * parcours(automate * ac, etat * start){
    if (start == NULL){
        return NULL;
    }
    rename1(ac);
    // crée une pile qui stocke les états accessibles à partir de l'état de départ
    int_arr * etats_access = make_int_arr(ac->nb_etat);
    // stocke si une état à été parcouru
    bool marked[ac->nb_etat];
    for (int i = 0; i < ac->nb_etat; i++)
    {
        marked[i] = false;
    }
    // stocke la pile de parcours
    pile * p = make_pile(10);
    // ajoute le départ à la pile
    pile_push(p, start->identifiant);
    // tant que la pile n'est pas vide
    while (!is_empty_pile(p)){
        // identifiant de l'état actuelle
        int id = pile_pop(p);
        // si l'état n'est pas marqué
        if (!marked[id]){
            // marqué l'état
            marked[id] = true;
            // ajoute l'état dans les états accessibles
            int_arr_append(etats_access, id);
            // récupère l'état dans l'automate
            etat * u = find_sommet(ac, id);
            // pour chaque voisins
            for (int j = 0; j < u->nb_transi; j++){
                transition * trj = u->transi_l[j];
                // l'empile à la pile
                pile_push(p, trj->dest->identifiant);
            }
        }
    }
    // libère la pile temporaire
    free_pile(p);
    return etats_access;
}


// renvoie les etats accessibles à partir de l'etat initiale
int_arr * etats_acessibles(automate * ac){
    return parcours(ac, ac->etat_init);
}

// renvoie si eta est coaccessible dans ac
bool is_accessible(automate * ac, etat * eta){
    //  récupère les états accessibles de ac
    int_arr * parc = etats_acessibles(ac);
    // récuprère si l'état est dans les états accessibles
    bool ret = is_in_int_arr(eta->identifiant, parc);
    // libère la liste des états accessibles
    free_int_arr(parc);
    return ret;
}

// renvoie si un etat est coaccessible
bool is_coaccessible(automate * ac, etat * eta){
    // récupère les états accessibles à partir de l'état
    int_arr * parc = parcours(ac, eta);
    // pour chaque état accessible
    for (int j = 0; j < parc->len; j++){
        int etaj = int_arr_pop(parc);
        // s'il est final alors l'état est coaaccessible
        if (find_sommet(ac, etaj)->final){
            // libère le tableau des états accessibles
            free_int_arr(parc);
            return true;
        }
    }
    // libère le tableau des états accessibles
    free_int_arr(parc);
    return false;
}


// renvoie la liste des etats coaccessibles
int_arr * etats_coacessibles(automate * ac){
    // alloue de l'espace pour les états accessible
    int_arr * ret = make_int_arr(ac->nb_etat);
    // pour chaque état ont l'ajoute s'il est coaccessible
    for (int i = 0; i < ac->nb_etat; i++){
        etat * eta = ac->etat_l[i];
        if (is_coaccessible(ac, eta))
            int_arr_append(ret, eta->identifiant);
    }
    return ret;
}


// renvoie si un automate est émondé
bool is_emonde(automate * ac){
    // récupère la liste des états accesssibles car fixe
    int_arr * etats_acess = etats_acessibles(ac);
    // pour chaque états
    for (int i = 0; i < ac->nb_etat; i++){
        etat * eta = ac->etat_l[i];
        // si cet état est soit non coaccessible et soit non accessible, l'automate est non émondé
        if ((!is_in_pile(eta->identifiant, etats_acess)) || (!is_coaccessible(ac, eta))){
            // libère la liste des états accessibles
            free_pile(etats_acess);
            return false;
        }
    }
    // l'automte est émondé
    // libère la liste des états accessibles
    free_pile(etats_acess);
    return true;
}

// rend ac emondé
void set_emonde(automate * ac){
    // récupère les états accessibles
    int_arr * etats_acess = etats_acessibles(ac);
    //  pour chaque états s'il n'est pas accessible ou qu'il n'est pas
    // coaccessible on le supprime
    for (int i = 0; i < ac->nb_etat; i++){
        etat * eta = ac->etat_l[i];
        // si l'état n'est pas accessible
        bool c1 = !is_in_int_arr(eta->identifiant, etats_acess);
        // si l'état n'est pas coaccessible
        bool c2 = !is_coaccessible(ac, eta);
        // si l'état ne satisfait ces condition on le supprime
        if (c1 || c2){
            del_etat(ac, eta);
        }
    }
    // libère la liste des états accessibles
    free_pile(etats_acess);
}


// exercice 10

// renvoie l'automate reconnaissant l'intersection des deux languages 
automate * inter_automate(automate * aca, automate * acb){
    // crée l'automate résultat
    automate * ret = make_automate();
    // on ajoute tous les états du produit cartésien des ensembles d'états
    for (int i = 0; i < aca->nb_etat; i++)
        for (int j = 0; j < acb->nb_etat; j++)
            add_e_som(ret, i * acb->nb_etat + j);

    // pour chaque etat de l'automate a
    for (int i = 0; i < aca->nb_etat; i++){
        etat * etai = aca->etat_l[i];
        //  pour chaque transition dans cet état
        for (int j = 0; j < etai->nb_transi; j++){
            transition * trj = etai->transi_l[j];
            // pour état de l'automate b
            for (int k = 0; k < acb->nb_etat; k++){
                etat * etak = acb->etat_l[k];
                // pour chaque transition de cet état
                for (int l = 0; l < etak->nb_transi; l++){
                    transition * trl = etak->transi_l[l];
                    /* si les transitions ont la même étiquette
                    ajoute une transition dans l'automate résultat
                    qui relie l'état produit des départs à l'état produit 
                    des arrivées */ 
                    if (trj->etiq == trl->etiq){
                        add_transi_a(ret, i * acb->nb_etat + j, trj->etiq, trj->dest->identifiant * acb->nb_etat + trl->dest->identifiant);
                    }
                }
            }
        }
    }
    // le rend émondé
    set_emonde(ret);
    // redistribue les identifiants 
    rename1(ret);
    return ret;
}

// exercice 11

// renvoie un parcours en profondeurs qui suit seulement des epsilon-transitions
int_arr * parcours_epsilon(automate * ac, etat * start){
    // crée une liste des états accessibles
    int_arr * etats_access = make_pile(ac->nb_etat);
    // crée une pile
    pile * p = make_pile(10);
    // liste des états
    bool marked[ac->nb_etat];
    for (int i = 0; i < ac->nb_etat; i++)
    {
        marked[i] = false;
    }
    
    // empile l'état de départ
    pile_push(p, start->identifiant);

    while (!is_empty_pile(p)){
        // récupère l'identifiant au sommet de la pile
        int id = pile_pop(p);
        // s'il n'est pas marqué
        if (!marked[id]){
            // on ajoute à la liste des états accessibles par epsilon transition
            int_arr_append(etats_access, id);
            marked[id] = true;
            // récupère l'état asssocié
            etat * u = find_sommet(ac, id);
            // empiler les états voisins accessibles par une epsilon transition
            for (int j = 0; j < u->nb_transi; j++){
                transition * trj = u->transi_l[j];
                if (trj->etiq == '1')
                    pile_push(p, trj->dest->identifiant);
            }
        }
    }
    // libère la pile
    free_pile(p);
    return etats_access;
}

// renvoie une origine d'un chemin d'epsilon transition
etat * get_start(automate * ac){
    // initialise à aucun
    etat * start = NULL;
    bool end = false;
    // pour chaque état
    for (int i = 0; !end && i < ac->nb_etat; i++)
    {
        etat * etai = ac->etat_l[i];
        // pour chaque transition de cet état
        for (int j = 0;!end && j < etai->nb_transi; j++)
        {
            transition * trj = etai->transi_l[j];
            // si l'étiquette est epsilon
            end = trj->etiq == '1';
        }
        if (end){
            // le départ du parcours est fixé à cet état
            start = ac->etat_l[i];
            // identifiant pour éviter de boucler
            // boolléen de fin de boucle
        }
    }
    if (start == NULL){
        return NULL;
    }
    // stocke l'identifiant de l'état de départ pour vérifier s'il n'existe pas de boucle d'epsilon transitions
    int stop = start->identifiant;
    // start est un état avec une epsilon transition
    // on cherche l'état qui n'est pas destination d'epsilon transition
    // suppose que l'etat est dest d'epsilon transition
    end = false;
    while (!end){
        // suppose que le start actuelle n'est pas destination d'une epsilon transition
        end = true;
        // pour chaque état
        for (int k = 0; k < ac->nb_etat; k++){
            etat * eta = ac->etat_l[k];
            // pour chaque transition
            for (int l = 0; l < eta->nb_transi; l++){
                transition * trl = eta->transi_l[l];
                // si la destination de la epsilon transition est l'état start
                if (trl->dest == start && trl->etiq == '1'){
                    // changement de start
                    start = eta;
                    end = false;
                    // empèche les boucles
                    assert(start->identifiant != stop);
                }
            }            
        }
    }
    return start;
}


// renvoie une transition de et etiquete par un epsilon
transition * find_e_transi(etat * et){
    // si l'etat exite
    if (et != NULL)
        //  pour chaque transition
        for (int i = 0; i < et->nb_transi; i++)
            // si l'étiquette est un epsilon
            if (et->transi_l[i]->etiq == '1')
                return et->transi_l[i];
    return NULL;
}

// supprime une E-transition de sommet si elle existe
void del_e(automate * ac, int sommet){
    // récupère l'état associé à sommet
    etat * et = find_sommet(ac, sommet);
    if (et == NULL)
        return;
    // récupère une epsilon transition
    transition * tre = find_e_transi(et);
    // tant qu'il éxiste une epsilon transition
    while (tre != NULL){
        // récupère l'état destination
        etat * dest = tre->dest;
        // si l'état destination est final, alors on rend l'état final
        if (dest->final)
            set_final(et);
        // pour chaque transition de l'état destination
        for (int i = 0; i < dest->nb_transi; i++){
            transition * tri = dest->transi_l[i];
            /*on ajoute une transition entre l'état départ est tous les états destinations
            des transitions en partance de l'état destination de la
            epsilon transition */
            add_transi_a(ac, sommet, tri->etiq, tri->dest->identifiant);
        }
        del_transi(ac, sommet, tre);
        tre = find_e_transi(et);
    }
}

// supprime les epsilon-transitions avec une fermeture arrière
void del_e_transi(automate * ac){
    // tant que l'automate est asynchrone
    while (is_asynchrone(ac)){
        // récupère le premier état d'un chemin d'epsilon transition
        etat * start = get_start(ac);
        // récupère la liste des états accessibles par epsilon transition
        pile * etats_access = parcours_epsilon(ac, start);
        // pour chaque état du chemin d'epsilon transition 
        for (int j = 0; j < etats_access->len; j++){
            int etaj = pile_pop(etats_access);
            // supprimer les transitions
            del_e(ac, etaj);
        }
        // libère la pile d'état transition
        free_pile(etats_access);
    }
}

// rend non asynchrone ac
void set_n_async(automate * ac){
    del_e_transi(ac);
}


// exercice 12

// libere une chaine de caractère
void free_str(char * str){
    if (str != NULL){
        free(str);
    }
}

// donne les etats associes aux entiers de la liste letat
etat ** get_etat(automate * ac, int_arr * letat){
    int n = letat->len;
    // alloue de l'espace pour stocker la liste de pointeur vers les états
    etat ** ret = malloc(sizeof(* ret) * n);
    // vérifie que le malloc n'a pas échoué
    assert(ret != NULL);
    // pour chaque entier de la liste d'entier
    for (int i = 0; i < n; i++)
        // récupère et stocke l'état associé à l'entier
        ret[i] = find_sommet(ac, pile_pop(letat));
    return ret;
}


// renvoie l'indice de la première occurence d'un caractère, -1 s'il n'est pas dedans
int get_index_char(char * mot, char c){
    int n = strlen(mot);
    for (int i = 0; i < n; i++){
        if (c == mot[i])
            return i;
    }
    return -1;
}


// renvoie toutes les destinations à partir d'un etat simple et d'une etiquette
int_arr * get_neight(etat * et, char etiq){
    // crée la liste des voisins
    int_arr * ret = make_int_arr(et->nb_transi);
    assert(ret != NULL);
    // pour chaque transition
    for (int i = 0; i < et->nb_transi; i++){
        transition * tri = et->transi_l[i];
        // si l'étiquette est identique 
        if (tri->etiq == etiq){
            int_arr_append(ret, tri->dest->identifiant);
        }
    }
    return ret;
}


// rassemble les piles en 1 et free les anciennes piles
int_arr * merge(int_arr ** ps, int n){
    // crée la liste des voisins généralisés
    pile * ret = make_pile(n);
    assert(ret != NULL);
    // pour chaque liste
    for (int i = 0; i < n; i++){
        // on récupère la pile
        int_arr * pi = ps[i];
        // pour chaque élément dans la liste
        for (int j = 0; j < pi->len; j++){
            int d = int_arr_pop(pi);
            /* s'il n'est pas dans la liste 
            on l'ajoute */
            if (!is_in_int_arr(d, ret))
                int_arr_append(ret, d);
        }
        // libère la liste ainsi vidé
        free_pile(pi);
    }
    return ret;
}

// renvoie la liste des destinations à partir d'un etat compose
pile * get_neight_l(automate * ac, etat * et, char etiq, tabint * arr){
    // crée la liste de liste de voisin
    tabint * tmp = make_tabint(ac->nb_etat);
    /* récupère la liste d'état qui compose l'état composé et 
    exemple : l'état 5 du nouvelle automate peut être composé
    de l'ensemble d'état {1; 3} de l'automate source
    aetat stocke donc {1; 3} dans ce cas */
    int_arr * aetat = arr->cont[et->identifiant];
    // pour chaque état qui compose l'état
    for (int i = 0; i < aetat->len; i++){
        // récupère l'état dans l'automate source
        etat * et = find_sommet(ac, aetat->cont[i]);
        // ajoute la liste des voisins de l'état par cette étiquette
        // à la liste de liste
        append_iia(tmp, get_neight(et, etiq));
    }
    // rassemble les listes en une seule
    int_arr * ret = merge(tmp->cont, tmp->len);
    // libère le tableau temporaire
    free_tabint(tmp);
    // tri la liste final dans l'ordre croissant
    tri_int_arr(ret);
    return ret;
}


// affiche un tableau de pile
void print_arr(tabint * arr){
    for (int i = 0; i < arr->len; i++){
        print_pile(arr->cont[i]);
    }
}

// revoie si list a un etat final
bool has_final(automate * ac, pile * p){
    // pour chaque état
    for (int i = 0; i < p->len; i++){
        // récupère l'état associé
        etat * et = find_sommet(ac, p->cont[i]);
        // si l'état est final alors la liste contient un final
        if (et->final)
            return true;
    }
    // la liste n'en contient pas
    return false;
}


// affiche une liste de bool
void print_boolarray(bool * tab, int len){
    printf("[");
    for (int i = 0; i < len; i++)
        printf("%s, ", str_of_bool(tab[i]));
    printf("]\n");
}


// rend un automate deterministe
void set_deter(automate * ac){
    if (!is_deter(ac)){
        // travaille avec un automate emonde non asynchrone
        set_n_async(ac);
        set_emonde(ac);

        // copie l'automate pour avoir en mémoire l'automate intiale
        automate * source = copy_auto(ac);
        // libère l'automate pour le remplacer par l'automate déterministe équivalent
        free_automate(ac);
        // crée un automate vide
        automate * ret = make_automate();


        // alphabet de l'automate source
        char * alpha = get_alpha(source);
        int lalpha = strlen(alpha);

        // nombre etat à l'origine, utile pour avoir les correspondances entre de l'etat liste des etats originelles represente
        int nb_som = source->nb_etat;

        // 2 puissance nb_som
        int deuxpn = pow(2, nb_som);

        // stocke la liste des associations d'états compris à l'id donné
        tabint * tab_id_ass = make_tabint(ret->nb_etat);
        // initialisation avec les états composés d'un simple état
        for (int i = 0; i < nb_som; i++){
            // crée une liste de capacité 1
            pile * tp = make_int_arr(1);
            // met l'identifiant dans la liste
            int_arr_append(tp, source->etat_l[i]->identifiant);
            // l'ajoute à la fin de la liste de liste des compositions
            append_iia(tab_id_ass, tp);
        }

        // crée une pile qui stocke les états à voir
        pile * tosee = make_pile(source->nb_etat);
        // empiler l'état initial
        empiler(tosee, source->etat_init->identifiant);
        // crée un tableau de boolléen pour tous les états de
        // l'automate déterminisé
        bool marked[deuxpn];
        // initialisé à false
        for (int i = 0; i < deuxpn; i++){
            marked[i] = false;
        }
        // tant que la liste n'est pas vide 
        while (!is_empty_pile(tosee)){
            // dépile un état de la pile
            int id = pile_pop(tosee);
            // s'il n'est pas marqué
            if (!marked[id]){
                // marque l'état
                marked[id] = true;
                // l'ajoute à l'automate
                add_e_som(ret, id);
                // on trouve l'automate associé à cet id
                etat * et = find_sommet(ret, id);
                // pour chaque lettre de l'alphabet
                for (int i = 0; i < lalpha; i++){
                    // récupère la lettre dans l'alphabet
                    char lettre = alpha[i];
                    // récupère la liste des voisins par cet lettre de l'état composé
                    int_arr * dest = get_neight_l(source, et, lettre, tab_id_ass);
                    // si la liste n'est pas dans la liste des associations d'états
                    if (!is_in_tabint(dest, tab_id_ass))
                        // l'ajoute
                        append_iia(tab_id_ass, dest);
                    
                    // récupère l'identifiant de la destination, soit l'indice de
                    // la liste des voisins grâce au tableau des associations
                    int dest_ass = get_index_tabint(tab_id_ass, dest);
                    // ajoute l'état dans l'automate
                    add_e_som(ret, dest_ass);
                    // si l'état est composé d'au moins 1 état final
                    if (has_final(source, dest))
                        // il devient final
                        set_final(find_sommet(ret, dest_ass));
                    // ajoute la transition de l'etat depilé de la pile
                    // vers l'état composé de la liste des voisins par cette lettre 
                    // destination
                    add_transi_a(ret, id, lettre, dest_ass);
                    // l'empile dans la liste des états à voir
                    empiler(tosee, dest_ass);
                }
            }
        }
        // libère la liste d'association
        free_tabint(tab_id_ass);
        // libère la pile d'état à regardé
        free_pile(tosee);
        // initialistion de l'état initial possède le même id car étant
        // un état simple l'id est inchangé
        // récupère l'état et indique qu'il s'agit de l'état initial
        ret->etat_init = find_sommet(ret, source->etat_init->identifiant);
        // rend l'automate émondé
        set_emonde(ret);
        // redistribue les identifiants
        rename1(ret);
        // libère la source
        free_automate(source);
        // change le contenue du pointeur donné en argument
        // par l'automate déterminisé équivalent
        *ac = *ret;
        // libère le pointeur vers l'automate déterminisé
        free(ret);
    }
}



// programme principale
int main(void){
    printf("cree un automate vide\n");
    automate * a1 = make_automate();
    printf("cree 1 etat avec l'identifiant 0\n");
    etat * e0 = make_etat(0);
    printf("cree un etat avec l'identifiant 1\n");
    etat * e1 = make_etat(1);
    printf("cree une transition de l'etat 0 a l'etat 1\n");
    transition * t1 = make_transi('a', e1);
    printf("ajoute les deux etats a l'automate\n");
    printf("etat 0 initial\n");
    add_etat(a1, e0);
    printf("etat 1 final\n");
    add_etat_final(a1, e1);
    printf("ajoute la transition a l'automate\n"); 
    add_transi(a1, 0, t1);
    printf("affiche l'automate\n");
    print_automate(a1);
    printf("supprime la transition\n");
    del_transi(a1, 0, t1);
    printf("affiche l'automate\n");
    print_automate(a1);
    printf("libere l'automate 1\n");
    free_automate(a1);
    printf("lis l'automate aut1.txt\n");
    automate * a2 = read_automate("aut1.txt");
    printf("affiche l'automate 2\n");
    print_automate(a2);
    printf("copie de l'automate 2 dans l'automate 3\n");
    automate * a3 = copy_auto(a2);
    printf("libere l'automate 2\n");
    free_automate(a2);
    printf("affiche l'automate 3\n");
    print_automate(a3);
    printf("\nl'automate 3 est deter : %s\n", str_of_bool(is_deter(a3)));
    printf("l'automate 3 est complet : %s\n", str_of_bool(is_complet(a3)));
    printf("l'automate 3 est asynchrone : %s\n", str_of_bool(is_asynchrone(a3)));
    printf("libere l'automate 3\n");
    free_automate(a3);
    printf("lis l'automate auto_assync.txt\n");
    automate * a4 = read_automate("auto_assync.txt");
    printf("affiche l'automate 4\n");
    print_automate(a4);
    printf("\nl'automate 4 est deter : %s\n", str_of_bool(is_deter(a4)));
    printf("l'automate 4 est complet : %s\n", str_of_bool(is_complet(a4)));
    printf("l'automate 4 est asynchrone : %s\n", str_of_bool(is_complet(a4)));
    printf("rend l'automate 4 non assynchrone\n");
    set_n_async(a4);
    printf("affiche l'automate 4\n");
    print_automate(a4);
    printf("rend l'automate 4 emonde\n");
    set_emonde(a4);
    printf("affiche l'automate 4\n");
    print_automate(a4);
    printf("l'automate 5 est l'automate1.txt\n");
    automate * a5 = read_automate("automate1.txt");
    printf("affiche l'automate 5\n");
    print_automate(a5);
    printf("l'automate 6 est l'intersection de l'automate 4 et 5\n");
    automate * a6 = inter_automate(a4, a5);
    printf("affiche l'automate 6\n");
    print_automate(a6);
    printf("rend l'automate 4 déterministe\n");
    set_deter(a4);
    printf("affiche l'automate 4\n");
    print_automate(a4);
    printf("l'automate 4 reconnait le mot abba : %s\n", str_of_bool(is_recon(a4, "abba")));
    printf("libere l'automate 4\n");
    free_automate(a4);
    printf("libere l'automate 5\n");
    free_automate(a5);
    printf("libere l'automate 6\n");
    free_automate(a6);
    return 0;
}