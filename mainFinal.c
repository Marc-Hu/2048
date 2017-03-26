
/**
 * \mainpage 
 * \section Titre1 Contexte :
 * A but pédagogique nous devons réaliser un jeu 2048 en langage C.Nous avons donc procédé à la rédaction
 * d’un Cahier des Charges Fonctionnel (CdCF) qui est un document formulant le
 * besoin, au moyen de fonctions détaillant les services rendus par un produit et
 */

/**
 * \file Jeu 2048
 * \brief Jeu du 2048
 * \author HU Marc
 * \author SAGER Damien
 * \version 10.2
 * \date 24 Fevrier 2017
 *
 * Programme regroupant les fonctions du jeu 2048
 */

#include"saisieM.c"
#include"couleurs_terminal.c"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

/*
* Structure d'un menu
* Contient une flèche '>'
* Un menu avec différente proposition
* Le nombre de choix disponible
*/
typedef struct{
	char *fleche;
	int *menu;
	int nbChoix;
	char **choix;
} menu;

typedef struct {
    int n; 
    int valMax; 
    int nbCasesLibres; 
    int *grille;
    int score;
} jeu;


/*Alloue la grille de la variable passée par adresse
* Initialise les cases de la grille avc des cases vides
* Initialise n et valMax avec les valeurs en paramètre via le pointeur p
*/

void initialiseJeu (jeu *p, int n, int valMax) {
    (*p).grille=malloc((n*n)*sizeof(int)); 
    int i=0;
    (*p).n=n; //initialisation de n
    (*p).valMax=valMax; //initialisation de valMax
    (*p).nbCasesLibres=n*n;
    (*p).score=0;

/*Allocation dynamique de la grille
* n*n cases contenant un int donc n*n*taille_mémoire_int
* Boucle "for" initialisant tout le contenu du tableau pointé par grille à 0 
*/
    for (i=0; i<(n*n); i++)
        (*p).grille[i]=0;
}

/*
* Fonction indiveValide qui retourne 1 si l'indice rentré dans les paramètres est valide et 0 sinon
* \param p (entrée): pointeur vers 
* \param i (entrée): numéro de ligne
* \param j (entrée): numéro de colonne
*/

int indiceValide(jeu *p, int i, int j) {
    int indice=i*(*p).n+j; //Calcul de l'indice entré dans la fonction
    if (indice<((*p).n*(*p).n) && indice>=0)  // Vérifie si l'indice calculé est bien compris entre 0 et l'indice max
        return 1;  // Retourne 1 si l'indice est valide
    return 0;  // Retourne 0 sinon.
}

int getVal(jeu *p, int ligne, int colonne) {
    if (indiceValide(p, ligne, colonne)) { // Vérifie si le numéro de ligne et que le numéro de colonne est valide
        int i=ligne*((*p).n)+colonne;     // Dans le cas ou il est valide on initialise l'indice 
        return (*p).grille[i];          // Et on retourne la valeur avec l'indice i précédemment calculé
    }
    return -1;                          // Retourne -1 si l'indice n'est pas valide
}

/*
    Fonction setVal qui permet de remplacer une valeur de la case (ligne, colonne) par une valeur rentré en paramètre
*/

int setVal (jeu *p, int ligne, int colonne, int val){
    if (indiceValide(p, ligne, colonne)){    //Vérifie si la ligne et la colonne rentré en paramètre est valide
        int i=ligne*(*p).n+colonne;             // Si c'est valide alors on initialise l'indice avec les paramètres ligne, colonne et n du pointeur
        (*p).grille[i]=val;
        return 1;                     // On écrase la valeur qu'il y avait dans la grille par la valeur entré en paramètre
    }
    return 0;
}

// Affichage de la grille

/** Numéro de couleur
0 : Noir
1 : Rouge
2 : Vert foncé
3 : Orange
4 : Bleu foncé
5 : Violet
6 : turquoise
7 : Blanc
*/
void affichage(jeu *p){
    clear_terminal();
    /*
        Variables : i pour les indices du tableau de la ligne 2; k pour les indices du tableau de la ligne 1;
        l pour les indices du tableau de la ligne 3; m valeur à incrémenter pour savoir quelles couleurs pour le fond de la case;
        compteur pour savoir sur quelles lignes du 2048 on est; total pour éviter d'effectuer la même opération;
        cellule évite de réécrire (*p).grille[i] à chaque fois; ligne pour savoir si nous sommes sur la ligne 1, 2 ou 3
        de l'affichage; couleur1 pour la couleur de la police (blanche pour toutes les cases); couleur 2 pour la couleur de fond des cases.
    */
    int i=0, k=0, l=0, m=0, compteur=1, total=((*p).n)*((*p).n), cellule, ligne=1, couleur1=7, couleur2;
    // pointeur *g qui va pointer sur quelles lignes on est et donc pointer les variables i, k ou l;
    int *g;
    // Taille des cases pour le color_printf; val qui contiendra la valeur de la case en chaîne de caractère
    // Point pour remplacer les 0 par les point; espace qui va remplacer les valeurs de la case par un espace pour les lignes 1 et 3;
    char tailleCase1[6]="%3s  ", tailleCase2[6]="%4s ", tailleCase3[6]="%5s", val[6], point[2]=".", espace[2]=" ";
    // *j pointeur du troisième paramètre de color_printf, *y et *z sont des pointeurs du 4 ème paramètre du color_printf réglé selon la ligne et la valeur de la case
    char *j, *y, *z;
    do{         // boucle qui s'arrête lorsqu'on a fini d'afficher toutes les cases de la troisième ligne donc indice l;
        switch (ligne){     // Selon la ligne on pointe l'indice de cette ligne
            case 1 : g=&k; break;
            case 2 : g=&i; break;
            default : g=&l; break;
        }
        for (; (*g)<((*p).n)*compteur; (*g)++){   // Boucle qui va afficher une ligne en incrémentent les indices des lignes
            cellule=(*p).grille[*g];
            j=tailleCase1;
            if(ligne==1 || ligne==3){       // On va pointer les chaînes de caractères qu'on veut pour la ligne 1 et 3
                z=espace;
                y=espace;
            }
            else{           // Sinon on pointe pour la ligne 2
                z=val;
                y=point;
            }
            if (cellule>0){     // Si la cellule est supérieur à 0
                for(m=0; cellule<128; m++)      // Boucle qui va choisir une couleur entre 1 et 6 pour les valeurs 2 à 64
                    cellule*=2;
                if (cellule>=128 && cellule<8192 && m==0){
                    for(m=0; cellule<8192; m++)             // Boucle qui va choisir une couleur entre 1 et 6 pour les valeurs 128 à 4096
                        cellule*=2;                        
                }
                else if (cellule>=8192 && cellule<262144 && m==0){      // Boucle qui va choisir une couleur entre 1 et 6 pour les valeurs 8192 à 131072
                    for(m=0; cellule<262144; m++)
                        cellule*=2;                        
                }
                couleur2=m;         // On affecte la couleur à couleur2
                // Selon la taille du nombre dans la case on change la taille de la case
                if ((*p).grille[*g]<100 && ligne==2)
                    j=tailleCase1;
                else if ((*p).grille[*g]<1000 && ligne==2)
                    j=tailleCase2;
                else if ((*p).grille[*g]<100000 && ligne==2)
                    j=tailleCase3;
                if (ligne==2)       // Dans la ligne 2 ou on affiche les nombres, on va affecter la valeur du tableau dans la chaîne de caractère val 
                    sprintf(val, "%d", (*p).grille[*g]);        // val contiendra (*p).grille[*g] qui est en format %d dans val et deviendra donc une chaîne de caractère
                color_printf(couleur1, couleur2, j, z);     // affiche la case selon la couleur de la police, la couleur de fond, le format du la case et la valeur de celle-ci
                printf(" ");    // On espace les valeurs car sinon on pourrait confondre la case 2 et 256 par exemple
            }
            else{               // Sinon si la cellule est égale à 0
                color_printf(WHITE, BLACK, j, y);   // on affiche la case avec un point
                printf(" ");        // On espace les valeurs comme pour celui au dessus
            }
        }
        printf("\n");       // Dès qu'on arrive à la fin de la ligne (1, 2 ou 3) on fait un retour chariot
        ligne++;        // On change de ligne (1, 2 ou 3)
        if (ligne>3){   // Lorsqu'on a dépassé la ligne 3 on remet la ligne à 1, on incrémente le compteur pour dire qu'on va aller sur la ligne 2 du 2048. Et on fait un retour chariot
            ligne=1;
            compteur++;
            printf("\n");
        }
    }while(l<total);
}

// Vérification que la case est vide

int caseVide(jeu *p, int i, int j){                                
    if (indiceValide(p, i, j) && getVal(p, i, j)==0)    // Condition : Si l'indice est valide et que la case de la grille est égale à 0
        return 1;                                           // On retourne 1 si la conditon est validé
    return 0;                                               // On retourne 0 si la condition n'est pas valide et donc si la case est différent de 0 ou que l'indice n'est pas valide
}

// Si le jeu contient au moins une case de libre alors la fonction ajoute aléatoirement une case (de valeur 2 ou 4) dans la grille

void ajouteValAlea(jeu *p){
    srand(time(NULL)); 
    int i, j=0, diviseur=(*p).n, ligne, colonne;                                                 // i pour l'indice et j pour un compteur
    if ((*p).nbCasesLibres>0){                                                   // Condition : si il y a une case vide au moins                                 
        j=0;                                                    // On remet j à 0
        while (j==0){
                i=rand()%(diviseur*diviseur);                            // On choisi un indice aléatoire entre 0 et nbEff de grille            
                ligne = i/diviseur;
                colonne = i%diviseur;
                if (caseVide(p, ligne, colonne)){ 
                        int val=(rand()%2+1)*2;                              // Condition : si la case est égale à zero
                        setVal(p, ligne, colonne, val);
                        j++;                                            // On incrémente j pour dire qu'on sort de la boucle et qu'on à ajouté une valeur (2 ou 4)
                    }
            }
        }
    }

// Fonction qui renvoie 1 si la partie est gagné et 0 sinon

int gagne (jeu *p){
    int i;
    for (i=0; i<((*p).n)*((*p).n); i++){               // Boucle : Tant qu'on a pas parcouru tout le tableau
        if ((*p).grille[i]==(*p).valMax)      // Condition : Si la valeur dans la grille est égale à la valeur max 
            return 1;                   // On retourne 1 si les deux valeurs sont égaux
    }
    return 0;                           // On retourne 0 sinon
}

// Fonction qui retourne 1 si la partie est perdue et 0 sinon

int perdu(jeu *p){
    int i, comparaison=0, nbCase=(*p).n*(*p).n;  

    /*
        Dans la boucle for nous comparons deux cases horizontalement; si il y en a au moins 2 qui sont égaux alors on retourne 0

    */
    for (i=0; i<nbCase; i++){
        if ((*p).grille[i]==0)
            return 0;
    }
    for (i=0; (i+1)<nbCase; i++){
        if ((*p).grille[i]==(*p).grille[i+1])             // On sort de la fonction si il y a au moins une égalité horizontale  
            return 0;
        if ((*p).grille[i]!=(*p).grille[i+1])
            comparaison++;                           // On incrémente la variable comparaison car sur une ligne on doit comparer au maximum (*p).n-1
        if (comparaison==((*p).n)-1){               // Si on atteint cette valeur alors on incrémente i et j pour passer à la ligne suivante et ainsi de suite
            i++;
            comparaison=0;                              // On remet bien sur comparaison à zéro 
        }
    }                                           // On va maintenant comparer les cases verticalement donc on met l'indice j à (*p).n car c'est la deuxième ligne et première colonne
    for (i=0; (i+(*p).n)<nbCase; i++){
        if ((*p).grille[i]==(*p).grille[i+(*p).n])             // On sort de la fonction si il y a au moins une égalité vertical
            return 0;
    }
    return 1;                                           // On sort de la fonction et on retourne 1 si il n'y a aucune égalité entre les cases que sa soit horzontalement et verticalement
}

// Fonction qui retourne 1 si la partie est terminé et 0 sinon

int finPartie (jeu *p){
    if (perdu(p) || gagne(p))          // Si c'est perdu ou si c'est gagné alors la partie est fini et on retourne 1
        return 1;
    return 0;                            // Sinon on retourne 0 si aucune condition n'est valide
}

// Effectue les mouvements à gauche ou à droite des cases d'une ligne et cela renvoie 1 si on a déplacé au moins une case et 0 sinon

int mouvementLigne (jeu *p, int ligne, int direction){
    int i, j, mouvement=0, nbLigne=(*p).n, compteur;
    int nb, nb1;
    if (direction == 1){
        nb=nbLigne*ligne+nbLigne-1; 
        nb1=ligne*nbLigne;
    }
    else{
        nb=ligne*nbLigne;
        nb1=nbLigne*ligne+nbLigne-1;
    }
    for (compteur=0; compteur<nbLigne; compteur++){
            for (j=nb1 ; j<nb || j>nb ; j+=direction){          // Boucle : cette boucle va tasser la ligne
                    if ((*p).grille[j]==0 && (*p).grille[j+direction]!=0){
                        (*p).grille[j]=(*p).grille[j+direction];
                        (*p).grille[j+direction]=0;
                        mouvement++;
                    }
                }
        }
    for (i=nb1 ; i<nb || i>nb; i+=direction){            // Boucle : cette boucle va additionner les mêmes valeurs
            if ((*p).grille[i]==(*p).grille[i+direction] && (*p).grille[i]!=0){
                (*p).grille[i]*=2;
                (*p).score+=(*p).grille[i];
                (*p).grille[i+direction]=0;
                mouvement++;
                (*p).nbCasesLibres++;           // Lorsqu'on fusionne on incrémente le nb de case vide
                for (j=nb1 ; j<nb || j>nb; j+=direction){  // Boucle : cela va tasser la ligne
                    if ((*p).grille[j]==0 && (*p).grille[j+direction]!=0)  {
                        (*p).grille[j]=(*p).grille[j+direction];
                        (*p).grille[j+direction]=0;
                    }
                }
            }
        }
    if (mouvement!=0)   // Retourne 1 si il y a au moins un mouvement
        return 1;
    return 0;
}

// Fonction mouvementLignes qui va faire bouger toutes les lignes vers la droite ou la gauche et retourne 1 si il y au moins une ligne qui bouge et 0 sinon

int mouvementLignes (jeu *p, int direction){
    int i, mouvement=0;
    for (i=0; i<(*p).n; i++){                    // Boucle : Executer la fonction mouvementLigne autant de fois qu'il y a de ligne initialisé
        if(mouvementLigne(p, i, direction))  // Condition : Si il y a un mouvement effectué alors on incrémente mouvement
            mouvement++;    
    }
    if (mouvement!=0)                           // Condition : Si il y a au moins un mouvement effectué alors on retourne 1 sinon 0
        return 1;
    return 0;
}

// Fonction mouvementColonne qui va faire bouger vers le haut ou vers le bas une colonne de la grille
int mouvementColonne (jeu *p, int colonne, int direction){
    int i, j, mouvement=0, nbColonne=(*p).n, compteur;
    int nb=nbColonne*nbColonne-nbColonne+colonne;
    if(direction==(-1)){
        int a=colonne;
        colonne=nb;
        nb=a;
        nbColonne=(-nbColonne);
    }
    for (compteur=0; compteur<nbColonne || compteur>nbColonne; compteur+=direction){        // Boucle qui tasse
        for (j=colonne ; j<nb || j>nb; j+=nbColonne){
                if ((*p).grille[j]==0 && (*p).grille[j+nbColonne]!=0){
                    (*p).grille[j]=(*p).grille[j+nbColonne];
                    (*p).grille[j+nbColonne]=0;
                    mouvement++;
                }
            }
        }
        for (i=colonne ; i<nb || i>nb; i+=nbColonne){                                       // Boucle qui additione et qui tasse si on a fusionné des cases
            if ((*p).grille[i]==(*p).grille[i+nbColonne] && (*p).grille[i]!=0){
                (*p).grille[i]*=2;
                (*p).score+=(*p).grille[i];
                (*p).grille[i+nbColonne]=0;
                mouvement++;
                (*p).nbCasesLibres++;           // On incrémente le nb des cases libres dès qu'on fusionne deux cases
                for (j=colonne ; j<nb || j>nb; j+=nbColonne){
                    if ((*p).grille[j]==0 && (*p).grille[j+nbColonne]!=0){
                        (*p).grille[j]=(*p).grille[j+nbColonne];
                        (*p).grille[j+nbColonne]=0;
                    }
                }
            }
        }
    if (mouvement!=0)
        return 1;       // Retourne 1 si on a au moins fait un mouvement (tasser ou fusionner)
    return 0;           // Retourne 0 si on a rien bougé
}

int mouvementColonnes (jeu *p, int direction){
    int i, mouvement=0;
    for (i=0; i<(*p).n; i++){                       // Pour chaque ligne on effectue mouvement selon la direction entré en paramètre
        if(mouvementColonne(p, i, direction))
            mouvement++;
    }
    if (mouvement!=0)
        return 1;       // Retourne 1 si on a au moins fait un mouvement sur une colonne au moins et 0 sinon.
    return 0;
}

int mouvement (jeu *p, int direction){
    int mouv=0;
    switch (direction){         // Selon la direction on va effectuer les différents mouvements
        case 0 :
            if(mouvementColonnes(p, -1))
                mouv++;
            break;
        case 1 :
            if(mouvementLignes(p, -1))
                mouv++;
            break;
        case 2 :
            if(mouvementColonnes(p, 1))
                mouv++;
            break;
        case 3 :
            if(mouvementLignes(p, 1))
                mouv++;
            break;
    }
    if (mouv!=0)        // Retourne 1 si on a au moins effectué un mouvement et 0 sinon.
        return 1;
    return 0;
}

int saisieD(){
    debutTerminalSansR();               // On débute le terminal sans retour chariot 
    Key saisieDir = NO_KEY;             // On définie une structure saisiDir qui sera initié à NO_KEY 
    while (saisieDir==NO_KEY){          // Boucle tant que c'est égale à NO_KEY
        saisieDir=lectureFleche();      // on va affecter la direction de la flèche dans saisiDir
    }
    switch (saisieDir){                 // Switch selon la direction de la flèche (bas, droite, haut ou gauche);  
        case KEY_DOWN:                  // On arrête le terminal sans retour chariot et on retourne 0, 1, 2 ou 3 selon la direction et -1 ou -2 si c'est la touche echap ou autre
            finTerminalSansR();         // On arrête me terminal sans retour chariot
            return 0;           
            break;
        case KEY_RIGHT: 
            finTerminalSansR();
            return 1;
            break;
        case KEY_UP: 
            finTerminalSansR();
            return 2;
            break;
        case KEY_LEFT: 
            finTerminalSansR();
            return 3;
            break;
        case KEY_ESCAPE: 
            finTerminalSansR();
            return -1;
            break;
        default :
            finTerminalSansR();
            return -2;
            break;
    }
}
/*
* Fonction qui va initialiser un menu 
* Affecte le nombre de choix pour le menu
* Alloue dynamiquement le menu et la flèche
* Affecte une flèche à la première ligne du menu
*/

void initialisationMenu(menu *p, int nb){
    p->nbChoix=nb;
    p->menu=malloc((p->nbChoix)*sizeof(int)); 
    p->fleche=malloc((p->nbChoix)*sizeof(char));
    p->choix=malloc((p->nbChoix)*sizeof(char *));
    int i;
    for (i=0; i<(p->nbChoix); i++)
        p->choix[i]=malloc(12*sizeof(char));
    for (i=0; i<(p->nbChoix); i++)
        p->menu[i]=i+1;
    p->fleche[0]='>';
    p->choix[0]="Jouer";
    p->choix[1]="Sauvegarder";
    p->choix[2]="Chargement";
    p->choix[3]="Option";
    p->choix[4]="Quitter";
}

//Libère la mémoire alloué pour le menu et la flèche

void libereMemoireMenu(menu *p){
    free(p->menu);
    free(p->fleche);
}

/*
* Fonction qui va vérifié si la flèche est présente dans la case d'indice a 
* Retourne 1 si la flèche est bien à l'indice a
* 0 Sinon
*/

int flecheValide(menu *p, int a){
    if (p->fleche[a]=='>')
        return 1;
    return 0;
}

/*
* Fonction qui va afficher le menu
* \33[34;01m    \33[00m  sont les couleurs pour la sélection
* Il va afficher en blanc si la flèche n'est pas devant la sélection 
* Il va afficher en bleu si la flèche pointe la sélection
*/

void affichageMenu(menu *p){
    int i; 
    printf("Monter/descendre : flèche haut/bas; selectionner : flèche droite\n");
    for (i=0; i<(p->nbChoix); i++){
        if (flecheValide(p, i))
            printf(" %c \33[34;01m%d  %s\33[00m\n", p->fleche[i], p->menu[i], p->choix[i]);
        else
            printf("     %d  %s\n", p->menu[i], p->choix[i]);
    }
}


/*
* Fonction qui va retourné la position de la flèche dans flèche
*/

int getFleche(menu *p){
    int i, j=0;
    for (i=0; i<(p->nbChoix); i++){
        if (p->fleche[i]=='>')
            return j;
        j++;
    }
    return 0;
}

/*
* Fonction qui va lancer le menu 
* Si on appuie sur flèche du bas ou du haut alors la flèche dans p->flèche bouge selon la direction
* Si la flèche atteint la limite (haut ou bas) alors la flèche ne bouge plus
* On retourne le menu selectionné (1, 2, 3 ou 4)
*/

int lanceMenu (menu *p){
    int saisie, j;
    while (saisie!=1){
        clear_terminal();
        affichageMenu(p);
        saisie=saisieD();
        switch (saisie){
            case 0 : 
                j=getFleche(p);
                if (j==(p->nbChoix-1))
                    j=(p->nbChoix-2);
                p->fleche[j+1]='>';
                p->fleche[j]=' ';
                break;
            case 2 :
                j=getFleche(p);
                if (j==0)
                    j=1;
                p->fleche[j-1]='>';
                p->fleche[j]=' ';
                break;
            case 1 :
                return getFleche(p)+1;
        }
    }
    return 0;
}

/*
* Fonction menu qui va gérer le menu
* Initialise un menu
* Il va afficher le menu
* Et il va retourné le choix opté par l'utilisateur
*/

int menuD(){
    menu p;
    int i;
    initialisationMenu(&p, 5);
    affichageMenu(&p);
    i=lanceMenu(&p);
    libereMemoireMenu(&p);
    return i;
}

int jouer (jeu *p){
    int saisie=0;
    affichage(p);               // On affiche la grille
    while (saisie!=(-1)){     // Tant que ce n'est pas la fin de la partie et qu'on a pas appuyé sur la touche echap
        if (finPartie(p))    // Si c'est gagné ou perdu alors on retourne 1
            return 1;
        saisie=saisieD();               // On saisie la touche directionelle
        if(mouvement(p, saisie)){       // Si il y a eu un mouvement alors on ajoute une valeur aleatoire et on desincrémente le nb de case libre
            ajouteValAlea(p);           //score du joueur
            (*p).nbCasesLibres--;
        }
        clear_terminal();               // On clear le terminal
        affichage(p);                   // Et on réaffiche le grille
        printf("Score : %d\n",(*p).score);
    }
    return 0;
}


/*
* Fonction sauvegarde qui sauvegarde au maximum 5 parties de 2048
* On écrase la sauvegarde si celui-ci contient déjà une sauvegarde
* Il créera au maximum 5 fichiers binaires
*/
int sauvegarde (jeu *p) {
    FILE* fichsave;
    int choix=0;
    char pseudo[11], slot;
    printf("Saisissez votre pseudo :\n");
    scanf("%s", pseudo);
    while(choix<1 || choix>5){
        printf("Choisir un emplacement pour la sauvegarde (1, 2, 3, 4, 5)\n");
        scanf("%d", &choix);
    }
    printf("Il se peut que l'emplacement que vous avez choisi contient déjà une sauvegarde. Voulez-vous l'écraser? [O/n]");
    while(slot!='O' && slot!='n')
        scanf("%c", &slot);
    if (slot=='n')
        return 0;
    if (choix==1)
        fichsave=fopen("saveG1.bin", "w+b");
    else if (choix==2)
        fichsave=fopen("saveG2.bin", "w+b");
    else if (choix==3)
        fichsave=fopen("saveG3.bin", "w+b");
    else if (choix==4)
        fichsave=fopen("saveG4.bin", "w+b");
    else
        fichsave=fopen("saveG5.bin", "w+b");
    if (fichsave==NULL){
        printf("Impossible d'atteindre le fichier de sauvegarde.\n");
        return 0;
    }
    fwrite(pseudo, 20*sizeof(char), 1, fichsave);
    fwrite(&p->n, sizeof(int), 1, fichsave);
    fwrite(&p->valMax, sizeof(int), 1, fichsave);
    fwrite(&p->nbCasesLibres, sizeof(int), 1, fichsave);
    fwrite(&p->score , sizeof(int), 1, fichsave);
    fwrite(p->grille , (p->n)*(p->n)*sizeof(int), 1 , fichsave);
    fclose(fichsave);
    return 1;
} 

/*
* Fonction qui va charger une partie pré-enregistré.
* Si la sauvegarde n'existe pas alors l'utilisateur sera redirigé vers le menu principal
* Si on veut on peut aussi supprimer toutes les sauvegardes 
*/

int chargement (jeu *p) {
    FILE* fichsave;
    int slot=-1;
    char pseudo[20], rmAll='r';
    do{
        printf("Saisissez l'emplacement de sauvegarde à charger (1, 2, 3, 4, 5) ou si vous voulez supprimer toutes les sauvegardes taper 0: \n");
        scanf("%d", &slot);
    }while(slot<0 || slot>5);
    if (slot==1)
        fichsave=fopen("saveG1.bin", "rb");
    else if (slot==2)
        fichsave=fopen("saveG2.bin", "rb");
    else if (slot==3)
        fichsave=fopen("saveG3.bin", "rb");
    else if (slot==4)
        fichsave=fopen("saveG4.bin", "rb");
    else if (slot==5)
        fichsave=fopen("saveG5.bin", "rb");
    else{
        printf("Etes-vous sur de vouloir supprimer toutes les sauvegardes?[O/n]\n");
        while (rmAll!='O' && rmAll!='n')
            scanf("%c", &rmAll);
        if (rmAll=='O'){
            system("rm saveG*");
            return 0;
        }
        else
            chargement(p);
    }
    if (fichsave==NULL){
        printf("Il n'y a pas de partie dans cette emplacement. Retour au menu principal.\n");
        sleep(2);
        return 0;
    }
    fread(pseudo, 20*sizeof(char), 1, fichsave);
    if (pseudo[0]=='\0'){
        printf("Cet emplacement de sauvegarde ne contient aucune sauvegarde Retour au menu principal.\n");
        return 0;
    }
    fread(&p->n, sizeof(int), 1, fichsave);
    fread(&p->valMax, sizeof(int), 1, fichsave);
    fread(&p->nbCasesLibres, sizeof(int), 1, fichsave);
    fread(&p->score , sizeof(int), 1, fichsave);
    (*p).grille=malloc((p->n*p->n)*sizeof(int)); 
    fread(p->grille , (p->n)*(p->n)*sizeof(int), 1 , fichsave);
    fclose(fichsave);
    return 1;
} 

/*
* Cette fonction permet au joueur de saisir la valeur maximale à atteindre pour gagner.
* Elle permet aussi au joueur de choisir la taille de la grille (min 3 par 3 et max 7 par 7)
*/

void option (int *nbCases, int *valGagn){
    do {
        printf("Choisissez la valeur maximale : 512   1024   2048   4096  8192\n");
        scanf ("%d",valGagn);
    } while (*valGagn!=512 && *valGagn!=1024 && *valGagn!=2048 && *valGagn!=4096 && *valGagn!=8192);
    do {
        printf ("Choisissez le nombre de cases par lignes/colonnes (3-7)\n");
        scanf ("%d",nbCases);
    } while (*nbCases<3 || *nbCases>7);
}

void libereMemoire(jeu *p) {
    free(p->grille);
}

int main (void) {
    jeu p;
    //int nbCases,valGagn;
    int menu=0, nbCases, valGagn;
    while(menu!=5){
        menu=menuD();
        switch (menu){
            case 1 :
                initialiseJeu(&p, 4, 2048);
                ajouteValAlea(&p);
                ajouteValAlea(&p);
                p.nbCasesLibres-=2;
                jouer(&p);
                break;
            case 2 :
                sauvegarde(&p);
                break;
            case 3 :
                if (chargement(&p))
                    jouer(&p);
                break;
            case 4 :
                option(&nbCases, &valGagn);
                initialiseJeu(&p,nbCases,valGagn);
                ajouteValAlea(&p);
                ajouteValAlea(&p);
                p.nbCasesLibres-=2;
                jouer(&p);
                break;
            case 5 :
                //libereMemoire(&p);
                return 0;
                break;
        }
    }
    return 0;
}


