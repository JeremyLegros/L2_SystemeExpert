// Devoir algorithmique - L2 Informatique
// A Completer et deposer exclusivement sur le site Moodle avant le 23 avril 23h59
//
// Numéro :
// Nom    : LEGROS
// Prenom : Jérémy

/*
A PROPOS DE CE CODE

La présentation en activant le mode DEBUG est bien plus détaillée
que la présentation demandée, elles peuvent servir pour vérifier/comprendre les actions qui sont effectuées.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# define ACTIVER_DEBUG FALSE // affichage détaillé
# define UTILISER_CHAINAGE_AVANT FALSE // choix du chainage FALSE : arriere
//=================================================================================================================
// PARTIE 1 : Code fourni pour le devoir - NE PAS MODIFIER
//=================================================================================================================


// Par commodité on definit un type booleen
// et deux constantes TRUE et FALSE
#define TRUE 1
#define FALSE 0
typedef unsigned char booleen;


//******************************************************************************
// Definition du type liste générique
//******************************************************************************


// Définition d'un type générique ELEMENT
// pour représenter les éléments stockés dans les maillons
typedef void ELEMENT;

// Définition du type MAILLON
// L'élément contenu dans un maillon est un pointeur vers n'importe quel type de donnée
// c'est à dire un pointeur de type void*
typedef struct maillon {
    ELEMENT*        elem;       // Pointeur vers un élement de la liste
    struct maillon* succ;       // maillon suivant dans la liste
} MAILLON;


// Définition du type LISTE
// Une liste est représentée par un pointeur sur le premier et le dernier élément ainsi et sa longueur
typedef struct liste {
    MAILLON* premier;
    MAILLON* dernier;
    int longueur;
} LISTE;



//******** Iterateur sur les listes *************

// Définition d'un type ITERATEUR pour parcourir les listes
typedef MAILLON* ITERATEUR;


// Retourne un iterateur positionné sur le premier élément de la liste l
ITERATEUR initIterateur(LISTE* l){
    if (l->premier == NULL){
        fprintf(stderr, "\n initIterateur : erreur liste vide");
        abort();
    }
    return l->premier;
}


// Retourne l'iterateur placé sur la position suivant l'iterateur i
ITERATEUR suivant(ITERATEUR i){
    if (i == NULL){
        fprintf(stderr,"iterateur suivant inexistant");
        abort();
    }
    else {
        return i->succ;
    }
}

// Retourne l'ELEMENT correspondant à la position d'un ITERATEUR dans une liste
ELEMENT* contenu(ITERATEUR i){
    return i->elem;
}

/* Le schéma général du parcours d'une liste l avec un iterateur est le suivant

 ITERATEUR i = initIterateur(l);
 while (i){
 ...
 e = contenu(i);  // accès à l'élément
 ...
 i = suivant(i);
 }
*/




// Constructeur liste vide
LISTE* listeVide(){
    LISTE* l = malloc(sizeof(LISTE));
    l->premier = NULL;
    l->dernier = NULL;
    l->longueur = 0;
    return l;
}

// Teste si la liste est vide
booleen estListeVide(LISTE* l){
    return l->premier == NULL;
}


// Remarque : toutes les opérations suivantes peuvent modifier la liste,
// il faut donc leur passer en paramètre l'adresse de la liste, d'où le type Liste*

// Ajout d'un élément e en tête de liste
void ajouterAuDebut(LISTE* l, ELEMENT* e){
    MAILLON* nouveau = (MAILLON*) malloc(sizeof(MAILLON));
    nouveau->elem = e;
    nouveau->succ = l->premier;
    l->premier = nouveau;
    // Cas où on ajoute l'élément dans une liste vide
    if (l->dernier == NULL) {
        l->dernier = l->premier;
    }
    l->longueur += 1;
}

// Ajout d'un élément e en fin de liste
void ajouterALaFin(LISTE* l, ELEMENT* e){
    // Cas où la liste est vide
    if (l->premier == NULL) {
        ajouterAuDebut(l, e);
    }
    else{
        MAILLON* nouveau = (MAILLON*) malloc(sizeof(MAILLON));
        nouveau->elem = e;
        nouveau->succ = NULL;
        l->dernier->succ = nouveau;
        l->dernier = nouveau;
    }
    l->longueur += 1;
}


// Suppression du premier élément
// Cette fonction retourne comme résultat un pointeur sur l'élément stocké dans le premier maillon
// ou provoque une erreur si la liste est vide
ELEMENT* supPremier(LISTE* l){
    if (estListeVide(l)) {
        fprintf(stderr, "\nsupPremier : erreur liste vide");
        abort();
    }
    MAILLON * premier = l->premier;
    ELEMENT * elem = premier->elem;

    l->premier = l->premier->succ;
    l->longueur -= 1;
    // Si on a supprimer le dernier maillon
    if (l->premier == NULL) {
        l->dernier = NULL;
    }
    // On libère l'espace mémoire utilisé par le premier maillon
    free(premier);
    return elem;
}



// Destruction d'une liste : les maillons sont supprimes et la liste ré-initialisée comme liste vide
void detruit (LISTE* l){
    while(! estListeVide(l))
        supPremier(l);
}




// Duplication d'une liste. Recopie de chacun des éléments dans une nouvelle liste
LISTE* duplique(LISTE* l){
    LISTE* copie = listeVide();
    MAILLON* courant;   // Le maillon courant dans la liste a dupliquer

    courant = l->premier;
    while(courant != NULL){
        ajouterALaFin(copie, courant->elem);
        courant = courant->succ;
    }
    return copie;
}




//******************************************************************************
// Definition du type FAIT
//******************************************************************************

typedef struct fait{
    int idFait;         // numero du fait
    char libelle[40];   // libelle
} FAIT;


//******************************************************************************
// Definition du type REGLE
//******************************************************************************

typedef struct regle{
    int idregle;                // Numero de la regle
    booleen marque;               // Pour savoir si la regle a ete utilisee : FALSE ou TRUE
    LISTE * hypothese;          // La liste des faits constituant l'hypothese
    LISTE * conclusion;         // La liste des faits constituant la conclusion
} REGLE;



//=================================================================================================================
// PARTIE 2 :                      COMPLETEZ LE CODE CI-DESSOUS
//=================================================================================================================

// creerFait : creer un nouveau fait avec la mémoire adéquate
// Entrée : le numero et le libelle d'un fait
// Sortie : retourne un pointeur sur un objet de type Fait
FAIT* creerFait(int id, const char* libelle)
{
  FAIT* leNouveaufait= (FAIT*) malloc(sizeof(FAIT));
  int i= 0; // indice servant à acceder aux caracteres de *libelle

  // on s'occupe de id en premier
  leNouveaufait->idFait= id;

  // puis de la recopie de *libelle dans libelle[]
  // j'ai préférée cette synthaxe *(libelle+i) à libelle++ par sûreté,
  //  je ne modifie pas l'adresse du pointeur, ce qui peut être utile
  // lors d'une mise à jour du code
  while (i < strlen(libelle))
  { leNouveaufait->libelle[i]= *(libelle+i);
    i++;
  }

  // Marquage de la chaine de caractere
  leNouveaufait->libelle[i]= '\0';

  return leNouveaufait;
}



// obtenirIdFait : permet d'obtenir l'id du fait en parametre
// Entrée : un fait
// Sortie : retourne idFait
// principe : on retourne le champ idFait de la structure FAIT
//            obtenu depuis un pointeur
int obtenirIdFait (FAIT* faitActuel)
{
  return (faitActuel->idFait);
}



// obtenirLibelleFait : permet d'obtenir le libelle du fait
// Entrée : un fait
// Sortie : retourne libelle
// principe : on retourne le libelle de la structure FAIT
//            depuis un pointeur
char* obtenirLibelleFait (FAIT* faitActuel)
{
  return (faitActuel->libelle);
}



// AfficheFait : affiche l'id et la chaine de caracteres du fait
// Entrée : un pointeur sur un Fait
// Sortie : rien
// Effet de  bord : affichage du fait et de son numero
// principe : on obtient le champ idFait, et le champ libelle de la
//            structure FAIT en parametre depuis un pointeur
//            et on les affiche sous la forme numéro : fait
//            La variable activer debug permet un affichage différent
//            plus complet destiné au débuggage
void afficheFait(FAIT* f)
{
  // AFFICHAGE DES NOMBRES A UN DIGIT : alignement du texte

  if (obtenirIdFait(f) < 10)
  {
    if (ACTIVER_DEBUG)
    { printf("FAIT n°0%d : \"%s\" ",obtenirIdFait(f),obtenirLibelleFait(f));
      return;
    }

    printf("0%d : %s ",obtenirIdFait(f),obtenirLibelleFait(f));
    return;
  }

  // AFFICHAGE DES NOMBRES A DEUX CHIFFRES

  if (ACTIVER_DEBUG)
    printf("FAIT n°%d : \"%s\" ",obtenirIdFait(f),obtenirLibelleFait(f));
  else
    printf("%d : %s ",obtenirIdFait(f),obtenirLibelleFait(f));
}




// viserPremierMaillon
// Entrée : une liste
// Sortie : un iterateur sur le premier maillon de cette liste
// principe : Juste un renommage plus explicite de initIterateur
//            on retourne un iterateur visant le premier maillon
//            de la liste donnée en parametre
ITERATEUR viserPremierMaillon (LISTE* listeActuelle)
{
  return initIterateur(listeActuelle);
}



// afficheBaseDeFaits
// Entrée : une liste dont les éléments sont des Fait
// Sortie : rien
// Effet de bord : affichage de tous les faits de la base de faits
// Principe : pour chaque maillon de la base de faits, on affiche
//            le fait contenu dans ce maillon
void afficheBaseDeFaits(LISTE* bdf)
 {
   ITERATEUR maillonActuelBaseDeFaits= viserPremierMaillon(bdf);

  // on parcourt la liste de fait, et on affiche chaque fait
  while(maillonActuelBaseDeFaits)
  { afficheFait(contenu(maillonActuelBaseDeFaits)), printf("\n");
    maillonActuelBaseDeFaits= suivant(maillonActuelBaseDeFaits);
  }
}



// obtenirListeHypothese : retourne la listes des hypotheses d'une regle
// Entrée : une regle
// Sortie : la liste hypothese de la regle
// Principe : retourne le pointeur de la liste hypothese
//            contenu dans la structure REGLE obtenu
//            en prenant la valeur du pointeur sur la regle
LISTE* obtenirListeHypothese (REGLE* regleActuelle)
{
  return (regleActuelle-> hypothese);
}



// obtenirListeConclusion : retourne la listes des conclusions d'une regle
// Entrée : une regle
// Sortie : la liste conclusion de la regle
// Principe : retourne le pointeur de la liste conclusion
//            contenu dans la structure REGLE obtenu
//            en prenant la valeur du pointeur sur la regle
LISTE* obtenirListeConclusion (REGLE* regleActuelle)
{
  return (regleActuelle-> conclusion);
}

// viserPremiereHypothese : retourne un iterateur sur la premiere hypothese
// Entrée : une regle
// Sortie : un iterateur dont le contenu est la premiere hypothese
// Principe : on récupère un iterateur positionne sur
//            le premier maillon de la liste hypothese
//            obtenu depuis la regle
ITERATEUR viserPremiereHypothese (REGLE* regleActuelle)
{
  return (obtenirListeHypothese(regleActuelle)->premier);
}

// viserDerniereHypothese : retourne un iterateur sur la derniere hypothese
// Entrée : une regle
// Sortie : un iterateur dont le contenu est la derniere hypothese
// Principe : on récupère un iterateur positionne sur
//            le dernier maillon de la liste hypothese
//            obtenu depuis la regle
ITERATEUR viserDerniereHypothese (REGLE* regleActuelle)
{
  return (obtenirListeHypothese(regleActuelle)->dernier);
}

// viserPremiereConclusion : retourne un iterateur sur la premiere conclusion
// Entrée : une regle
// Sortie : un iterateur dont le contenu est une conclusion
// Principe : on récupère un iterateur positionne sur
//            le premier maillon de la liste conclusion
//            obtenu depuis la regle
ITERATEUR viserPremiereConclusion (REGLE* regleActuelle)
{
  return (obtenirListeConclusion(regleActuelle)->premier);
}

// viserDerniereConclusion
// Entrée : une regle
// Sortie : un iterateur sur la derniere conclusion
// Principe : on récupère un iterateur positionne sur
//            le dernier maillon de la liste conclusion
//            obtenu depuis la regle
ITERATEUR viserDerniereConclusion (REGLE* regleActuelle)
{
  return (obtenirListeConclusion(regleActuelle)->dernier);
}


// obtenirIdRegle
// Entree : une regle
// Sortie : l'entier id de la regle
// Principe : retourne le champ idregle obtenu depuis
//            la structure REGLE qui est la valeur
//            contenu dans le pointeur vers la regle
int obtenirIdRegle (REGLE* regleActuelle)
{
  return regleActuelle->idregle;
}


// changerIdentifiantRegle :
// Entree : une regle
// Sortie : rien
// Effet de bord : change l'id de la règle en l'id donné en paramêtre
// Principe : on change la valeur du champs idregle de la regle en
//            parametre en obtenant la regle par le contenu
//            du pointeur sur cette regle
void changerIdentifiantRegle (REGLE* regleActuelle,int id)
{
  regleActuelle->idregle= id;
}


// regleAEteUtilisee : permet de connaitre si une regle a été utilisée
// Entrée : une regle
// Sortie : vrai si la regle a été utilisée, sinon faux
// Principe : retourne si le champ marque obtenu depuis
//            la structure REGLE qui est la valeur
//            contenu dans le pointeur vers la regle
//            est à vrai ou à faux
booleen regleAEteUtilisee (REGLE* regleActuelle)
{
  if (regleActuelle->marque)
    return TRUE;
  return FALSE;
}


// marquerRegleCommeUtilisee
// Entrée : une regle
// Sortie :
// Effet de bord : la marque de la regle est mise à TRUE
// Principe : change le champ marque obtenu depuis
//            la structure REGLE qui est la valeur
//            contenu dans le pointeur vers la regle
//            à vrai
void marquerRegleCommeUtilisee (REGLE* regleActuelle)
{
  regleActuelle->marque= TRUE;
}

// marquerRegleCommeNonUtilisee
// Entrée : une regle
// Sortie :
// Effet de bord : la marque de la regle est mise à FALSE
// Principe : change le champ marque obtenu depuis
//            la structure REGLE qui est la valeur
//            contenu dans le pointeur vers la regle
//            à faux
void marquerRegleCommeNonUtilisee (REGLE* regleActuelle)
{
  regleActuelle->marque= FALSE;
}

// initRegle : crée une regle 'vide/defaut' et réserve de la mémoire
// Entrée : le numéro d'une regle
// Sortie : un pointeur sur une structure regle dont les listes hypothese et de conclusion sont vides
REGLE* initRegle(int id)
{
  REGLE* laNouvelleRegle= (REGLE*) (malloc(sizeof(REGLE)));

  marquerRegleCommeNonUtilisee(laNouvelleRegle);
  changerIdentifiantRegle(laNouvelleRegle,id);

  laNouvelleRegle->conclusion= listeVide();
  laNouvelleRegle->hypothese= listeVide();

  return laNouvelleRegle;
}

// obtenirIdRegle


// ajouteHypothese
// Entrée : un pointeur sur une regle r
//          un pointeur sur un fait f
// Sortie : rien
// effet de bord : la regle r est modifiée en ajoutant le fait f  dans la liste des hypotheses
void ajouteHypothese(REGLE* r, FAIT* f)
{
  ajouterALaFin(obtenirListeHypothese(r),f);
}


// ajouteConclusion
// Entrée : un pointeur sur une regle r
//          un pointeur sur un fait f
// Sortie : rien
// effet de bord : la regle r est modifiée en ajoutant le fait f dans la liste des conclusions
void ajouteConclusion(REGLE* r, FAIT* f)
{
  ajouterALaFin(obtenirListeConclusion(r),f);
}


// afficheRegle
// Entrée : un pointeur sur une regle r
// Sortie :: rien
// Effet de bord : affichage de la regle sous la forme : SI (h1 ET h2 ET ...) ALORS (c1 ET c2 ET ...)
void afficheRegle(REGLE* r)
{
  ITERATEUR maillonActuelHypothese= viserPremierMaillon(obtenirListeHypothese(r)); // fonction initIterateur
  ITERATEUR maillonActuelConclusion=viserPremierMaillon(obtenirListeConclusion(r));

  ITERATEUR dernierMaillonHypothese= viserDerniereHypothese(r);
  ITERATEUR dernierMaillonConclusion= viserDerniereConclusion(r);

  // DEBUT AFFICHAGE DES HYPOTHESES

  if (ACTIVER_DEBUG)
  {
    if (obtenirIdRegle(r)<10)
      printf("\nREGLE n°0%d ", obtenirIdRegle(r));
    else
      printf("\nREGLE n°%d ", obtenirIdRegle(r));
  }


  // CAS: AFFICHAGE D'UNE SEULE HYPOTHESE

  if (maillonActuelHypothese == dernierMaillonHypothese)
  {
    if (ACTIVER_DEBUG)
      printf("\nSI\n  ( ");
    else
      printf("SI ( ");

    afficheFait(contenu(maillonActuelHypothese));

    if (ACTIVER_DEBUG)
      printf(")\nALORS\n  ( ");
    else
      printf(") ALORS ( ");
  }
  // CAS: AFFICHAGE DE PLUSIEURS HYPOTHESES
  //      MAIS ON S'ARRETE A L'AVANT DERNIER FAIT
  //      QUI DOIT ETRE AFFICHE DIFFEREMENT
  else
  {
    if (ACTIVER_DEBUG)
      printf("\nSI\n( ");
    else
      printf("SI ( ");
    while(maillonActuelHypothese != dernierMaillonHypothese)
    {
      if (ACTIVER_DEBUG)
        printf("\n   ");

      afficheFait(contenu(maillonActuelHypothese));

      printf(" ET ");

      maillonActuelHypothese= suivant(maillonActuelHypothese);
    }

    // GESTION DE L'AFFICHAGE DIFFERENTE POUR LE DERNIER FAIT

    if (ACTIVER_DEBUG)
      printf("\n   ");

    afficheFait(contenu(maillonActuelHypothese));

    if(ACTIVER_DEBUG)
      printf("\n)\nALORS\n   ( ");
    else
      printf(") ALORS ( " );
  }


  // CAS: AFFICHAGE DE CONCLUSION(S)

  // Remarque : l'énoncé du devoir ne propose qu'une seule conclusion
  // par règle, mais les spécifités de la fonction demandent à ce que
  // plusieurs conclusions soient gérées.

  // Si la liste de conclusions n'en contient qu'une seule
  if (maillonActuelConclusion == dernierMaillonConclusion)
  {
    afficheFait(contenu(maillonActuelConclusion));
    printf(")\n");
  }
  else
  {
    // gestion de l'affichage pour les conclusions avec un "ET"
    // ex: ( C1 " ET " C2), on s'arrete avant le dernier maillon
    while(maillonActuelConclusion != dernierMaillonConclusion)
    {
      if (ACTIVER_DEBUG)
        printf("\n   ");

      afficheFait(contenu(maillonActuelConclusion));
      printf(" ET ");
      maillonActuelConclusion= suivant(maillonActuelConclusion);
    }
    // gestion de l'affichage different pour la derniere conclusion
    if (ACTIVER_DEBUG)
      printf("\n   ");
    afficheFait(contenu(maillonActuelConclusion));

    if (ACTIVER_DEBUG)
      printf("\n)\n");
    else
      printf(")\n");
  }

  // FIN AFFICHAGE DE CONCLUSION(S)
}



// afficheBaseDeRegles
// Entrée : une liste de règles bdr (la base de regles)
// Sortie : rien
// Effet de nord : affichage de toutes les regles de la base de regles bdr
void afficheBaseDeRegles(LISTE* bdr)
{
  ITERATEUR maillonActuelBaseDeRegles= viserPremierMaillon(bdr);

  // Parcours de la liste et affichage
  while(maillonActuelBaseDeRegles)
  { afficheRegle(contenu(maillonActuelBaseDeRegles));
    maillonActuelBaseDeRegles= suivant(maillonActuelBaseDeRegles);
  }
}



//***********************************
// Pour tester et appliquer les regles
//



// existeFait : teste l'existence d'un fait dans une liste de fait
// Entrée : un fait f et une liste de fait lesfaits
// Sortie : le booleen TRUE si f est dans la liste lesfaits, FALSE sinon
// Le test est réalisé sur les numéros des faits
booleen existeFait(FAIT* f, LISTE* lesfaits)
{
  ITERATEUR maillonActuelDesFaits= viserPremierMaillon(lesfaits);

  // parcours de la liste de faits et vérification
  while(maillonActuelDesFaits)
  {
    if (obtenirIdFait(f)==obtenirIdFait(contenu(maillonActuelDesFaits)))
      return(TRUE);
    maillonActuelDesFaits= suivant(maillonActuelDesFaits);
  }
  return(FALSE);
}


// estApplicable : test l'applicabilité d'une regle
// Une règle est applicable si elle n'a pas déjà été appliquée
// et si tous les faits de l'hypothese sont dans la base de faits
// Entrée : une regle r et une liste de faits baseDeFaits
// Sortie : TRUE si la règle est applicable FALSE sinon
booleen estApplicable(REGLE* r, LISTE* baseDeFaits)
{
  ITERATEUR maillonActuelHypothese= viserPremierMaillon(obtenirListeHypothese(r));

  // Si la regle a déjà été utiilisée
  if (regleAEteUtilisee(r))
    return(FALSE);

  // recherche de l'existence des faits de l'hypothèse de la règle dans la // base de faits
  while (maillonActuelHypothese)
  {
    // si au moins un fait des hypothèses n'existe pas dans la base de faits
    // alors c'est fini
    if (!existeFait((contenu(maillonActuelHypothese)),baseDeFaits))
      return(FALSE);
    maillonActuelHypothese= suivant(maillonActuelHypothese);
  }
  return(TRUE);
}


// appliqueRegle :
// Appliquer une règle consiste à ajouter tous les faits de la conclusion dans la base de faits
// Entrée : une règle r et une liste de faits baseDeFaits
// Sortie : rien
// Effet de bord : tous les faits de la conclusion de r sont ajoutés dans la liste de faits
//                 et la règle est marquee comme etant utilisee

void appliqueRegle(REGLE* r, LISTE* baseDeFaits)
{
  ITERATEUR maillonActuelConclusion= viserPremierMaillon(obtenirListeConclusion(r));

  // Principe de précaution :
  // même si AppliqueRegle est utilisé après un test
  // avec la fonction estApplicable,je le teste ici
  if (regleAEteUtilisee(r))
  { fprintf(stderr,"\nfonction appliqueRegle: règle déjà appliquée");
    return;
  }

  // Affichage
  printf("Application de la règle %d : ", obtenirIdRegle(r));
  afficheRegle(r);

  // parcours et ajout
  // je considère le cas où il y aurait plusieurs conclusions par rapport au principe de fonctionnement
  // de la fonction
  while(maillonActuelConclusion)
  {
    ajouterALaFin(baseDeFaits,contenu(maillonActuelConclusion));
    maillonActuelConclusion= suivant(maillonActuelConclusion);
  }
  marquerRegleCommeUtilisee(r);
}





//***********************************
// Chainage avant

// Principe :
//     On parcourt la base de regles et on applique toutes les règles appliquables
//     Si lors d'un parcours de la base de regle au moins une regle a ete appliquee on recommence
//     On s'arrete lorsque plus aucune regle n'est applicable

// Entrée : la base des regles et la base des faits connus
// Sortie : rien
// Effets de bord : après exécution, toutes les règles qui ont été
//                  appliquées sont marquées et tous les faits déduits par
//                  le chaînage avant ont été ajoutés dans la base de faits

void chainageAvant(LISTE* baseDeRegles, LISTE* baseDeFaits)
{

  int nombreDeReglesAppliquées= 1; // valeur arbitraire
                                   // pour entrer dans le while

  // l'initialisation pertinente est placée dans le while
  ITERATEUR maillonActuelBaseDeRegles= NULL;

  // Boucle pour recommencer tant qu'une règle a été appliquée
  while (nombreDeReglesAppliquées > 0)
  {
    // on remet à zéro le nombre de règles pour le tour suivant
    nombreDeReglesAppliquées= 0;

    // on replace l'iterateur sur le premiere règle pour le tour suivant
    maillonActuelBaseDeRegles= viserPremierMaillon(baseDeRegles);

    // boucle pour appliquer les règles
    while(maillonActuelBaseDeRegles)
    {
      if (estApplicable(contenu(maillonActuelBaseDeRegles),baseDeFaits))
      {
        appliqueRegle(contenu(maillonActuelBaseDeRegles),baseDeFaits);
        nombreDeReglesAppliquées++;
      }
      maillonActuelBaseDeRegles= suivant(maillonActuelBaseDeRegles);
    }
  }
}

 // obtenirReglesIncluantConclusion :
 //   retourne une liste de règles qui contiennent la conclusion
 //   recherchée donnée en paramêtre
 // Entrée :
 //   une conclusion à comparer/chercher
 // Sortie : une liste de regles contenant la conclusion cherchée
 // Principe : on cherche dans chaque regle de la base de regle
 //            si la conclusion de la regle est la même que
 //            la celle que l'on a en parametre
 LISTE* obtenirReglesIncluantConclusion (FAIT* conclusion, LISTE* listeDeregles)
 {
   ITERATEUR maillonActuelListeRegles= viserPremierMaillon(listeDeregles);
   LISTE* listeDeRetour= listeVide();
   REGLE* regleActuelle= NULL;
   // chercher conclusion dans la base de Regles:
   // pour chaque regle
   while(maillonActuelListeRegles)
   {
     // chercher conclusion dans une regle
     regleActuelle= contenu(maillonActuelListeRegles);
     if (existeFait(conclusion,obtenirListeConclusion(regleActuelle)))
       ajouterALaFin(listeDeRetour,regleActuelle);
     maillonActuelListeRegles= suivant(maillonActuelListeRegles);
   }
   return listeDeRetour;
 }
 //*/



//***********************************
// Chainage arriere

// La fonction récursive demontreFait realise le chainage arriere
// Principe :
//    Si le fait a demontrer apparait dans la base de faits alors il est demontre
//    Sinon, pour chaque regle où unFait apparait dans la conclusion
//           on essaye de demontrer les faits contenus dans l'hypothese de la règle
//           et si on y arrive, on applique la regle et unFait est démontré
//    Lorsqu'on a exéminé toutes les règles sans avoir pu démontrer unFait
//    alors la demonstration est impossible

// Entree : la base de regles, la base de fait connus et un fait a demontrer
// Sortie : TRUE si le fait est demontrable, FALSE sinon
// Effets de bord : les regles appliquees au cours du processus sont marquees
//                  tous les faits intermediaire demontres au cours du processus sont ajoutes
//                  a la base de faits
booleen demontrerFait(LISTE* baseDeRegles, LISTE* baseDeFaits, FAIT* unFait)
{
  LISTE* listeDeRegles_a_utiliser= NULL; // liste de regles dont la
                                         // conclusion contient le fait recherché
  ITERATEUR maillonContenantUneRegle= NULL;
  REGLE* laRegle= NULL;
  LISTE* listeHypothese= NULL;
  ITERATEUR maillonContenantUneHypothese= NULL;
  FAIT* hypotheseAdemontrer= NULL;
  int nbHypothesesOK= 0;    // Permet de tester si le nombre d'hypotheses
  int nbHypothesesListe= 0; // d'une règle est le même que celui du nombre
                            // d'hypotheses au total dans la liste

  booleen auMoinsUneRegleVerifiee= FALSE; // Permet de savoir si le fait
                                          // a été démontré par une
                                          //  règle

  printf("==>Je cherche à démontrer le fait  ");
  afficheFait(unFait);
  printf("\n");

  // CAS 1 : la base de faits est vide ou la base de regles est vide
  if (estListeVide(baseDeRegles) || estListeVide(baseDeFaits))
  {
    if (ACTIVER_DEBUG)
      printf("\n   Cas 1: \n   Base de regles ou base de faits vide\n");

    afficheFait(unFait);
    printf( "n'est pas démontrable\n");
    return FALSE;
  }

  // CAS 2 : le fait existe dans la base de faits
  if (existeFait(unFait,baseDeFaits))
  {
    if (ACTIVER_DEBUG)
      printf("\n   Cas 2 :\n    Le fait existe dans la base de faits");
    else
      printf("   ");

    afficheFait(unFait);
    printf(" est démontré \n");
    return TRUE;
  }

  // CAS 3 : le fait n'apparait dans aucune conclusion de la base de regles
  // on cherche tout d'abord les regles utiles
  listeDeRegles_a_utiliser= obtenirReglesIncluantConclusion(unFait,baseDeRegles);

  // puis on teste
  if (estListeVide(listeDeRegles_a_utiliser))
  {
    if (ACTIVER_DEBUG)
    {  printf("\n   Cas 3 :");
       printf("\n   Aucune conclusion ne contient le fait ");
    }
    afficheFait(unFait);

    if (ACTIVER_DEBUG)
      printf("et ce fait n'est pas dans la base de faits");

    printf("n'est pas démontrable");
    return FALSE;
  }

  // CAS 4 : cas de récursion
  // on doit démontrer chaque hypothèse la liste de règles trouvées

  auMoinsUneRegleVerifiee= FALSE;
  maillonContenantUneRegle= viserPremierMaillon(listeDeRegles_a_utiliser);

  while (maillonContenantUneRegle)
  {
                         laRegle= contenu(maillonContenantUneRegle);
                  listeHypothese= obtenirListeHypothese(laRegle);
    maillonContenantUneHypothese= viserPremierMaillon(listeHypothese);
                  nbHypothesesOK= 0; // remise à zéro pour la regle suivante
               nbHypothesesListe= (listeHypothese->longueur)-1;

    while (maillonContenantUneHypothese)
    {
      hypotheseAdemontrer= contenu(maillonContenantUneHypothese);

      if (ACTIVER_DEBUG)
      { printf("\n==>Je cherche à démontrer l'hypothèse ");
        afficheFait(hypotheseAdemontrer);
        printf("\n");
      }

      if ( (existeFait(hypotheseAdemontrer, baseDeFaits)))
      {
        if (ACTIVER_DEBUG)
        {
          printf("\nL'hypothèse ");
          printf("existe dans la base de faits : ");
          afficheFait(hypotheseAdemontrer);
          printf("a été démontrée\n");
        }
        nbHypothesesOK++;
      }
      else
        if (demontrerFait(baseDeRegles,baseDeFaits,hypotheseAdemontrer))
        {
          if (ACTIVER_DEBUG)
          { printf("\nL'hypothèse ");
            afficheFait(hypotheseAdemontrer);
            printf("a été démontrée par récursion\n");
          }
          nbHypothesesOK++;
        }
        else
        {
          if (ACTIVER_DEBUG)
            printf("\nL'hypothèse ");
          else
            printf("\nLe fait ");

          afficheFait(hypotheseAdemontrer);
          printf("n'est pas démontrable\n");
        }
      maillonContenantUneHypothese= suivant(maillonContenantUneHypothese);
    }

    if (ACTIVER_DEBUG)
    {
      printf("\nnombre d'hypotheses OK : %d sur %d ",nbHypothesesOK,nbHypothesesListe);
      printf("pour la regle %d :",obtenirIdRegle(laRegle));
      afficheRegle(laRegle);
    }

    if (nbHypothesesOK == nbHypothesesListe)
    { // Si on arrive là c'est que le fait est démontrable
      appliqueRegle(laRegle, baseDeFaits);
      auMoinsUneRegleVerifiee= TRUE; // on le marque comme démontrable

      if (ACTIVER_DEBUG)
      { printf("La conclusion de la regle %d a été ajoutée à la base de faits\n", obtenirIdRegle(laRegle));
      }
    }
    else
    {
      if (ACTIVER_DEBUG)
      { printf("Il manque une ou plusieurs hypotheses à la regle %d qui n'est donc pas applicable",obtenirIdRegle(laRegle));
      }
    }
    maillonContenantUneRegle= suivant(maillonContenantUneRegle);
  }

  // on détruit la liste pour éviter un éventuel overflow de la mémoire
  detruit(listeDeRegles_a_utiliser);

 if (auMoinsUneRegleVerifiee)
 { if (ACTIVER_DEBUG)
   { printf("Le fait ");
     afficheFait(unFait);
     printf(" a été démontré par l'application d'une regle\n");
   }
   return TRUE;
 }
 return FALSE;
}


//=================================================================================================================
// PARTIE 2 :                      PROGRAMME PRINCIPAL
//=================================================================================================================

int main(int argc, const char * argv[])
{
    // Création des faits

    FAIT* fait01= creerFait(1,"a poils");
    FAIT* fait02= creerFait(2,"est un mammifère");
    FAIT* fait03= creerFait(3,"donne du lait");
    FAIT* fait04= creerFait(4,"mange de la viande");
    FAIT* fait05= creerFait(5,"est un carnivore");
    FAIT* fait06= creerFait(6,"a dents pointues");
    FAIT* fait07= creerFait(7,"a des griffes");
    FAIT* fait08= creerFait(8,"a les yeux vers avant");
    FAIT* fait09= creerFait(9,"a des sabots");
    FAIT* fait10= creerFait(10,"est un ongulé");
    FAIT* fait11= creerFait(11,"n'est pas un carnivore");
    FAIT* fait12= creerFait(12,"est de couleur brune");
    FAIT* fait13= creerFait(13,"a des tâches sombres");
    FAIT* fait14= creerFait(14,"est un guépard");
    FAIT* fait15= creerFait(15,"a des raies noires");
    FAIT* fait16= creerFait(16,"est un tigre");
    FAIT* fait17= creerFait(17,"a un long cou");
    FAIT* fait18= creerFait(18,"a des longues pattes");
    FAIT* fait19= creerFait(19,"est une girafe");
    FAIT* fait20= creerFait(20,"a des plumes");
    FAIT* fait21= creerFait(21,"est un zèbre");
    FAIT* fait22= creerFait(22,"est un oiseau");
    FAIT* fait23= creerFait(23,"vole");
    FAIT* fait24= creerFait(24,"ponds des oeufs");
    FAIT* fait25= creerFait(25,"ne vole pas");
    FAIT* fait26= creerFait(26,"est noir et blanc");
    FAIT* fait27= creerFait(27,"est une autruche");
    FAIT* fait28= creerFait(28,"nage");
    FAIT* fait29= creerFait(29,"est un pingouin");
    FAIT* fait30= creerFait(30,"est un aigle");

    // Création des regles

    // Initialisation des regles

    REGLE* regle01= initRegle(1);
    REGLE* regle02= initRegle(2);
    REGLE* regle03= initRegle(3);
    REGLE* regle04= initRegle(4);
    REGLE* regle05= initRegle(5);
    REGLE* regle06= initRegle(6);
    REGLE* regle07= initRegle(7);
    REGLE* regle08= initRegle(8);
    REGLE* regle09= initRegle(9);
    REGLE* regle10= initRegle(10);
    REGLE* regle11= initRegle(11);
    REGLE* regle12= initRegle(12);
    REGLE* regle13= initRegle(13);
    REGLE* regle14= initRegle(14);
    REGLE* regle15= initRegle(15);

    // Ajout de l'hypothèse de la règle 1
    ajouteHypothese(regle01,fait01);

    // Ajout de l'hypothèse de la règle 2
    ajouteHypothese(regle02,fait03);

    // Ajout de l'hypothèse de la règle 3
    ajouteHypothese(regle03,fait04);

    // Ajout des hypothèses de la règle 4
    ajouteHypothese(regle04,fait06);
    ajouteHypothese(regle04,fait07);
    ajouteHypothese(regle04,fait08);

    // Ajout des hypothèses de la règle 5
    ajouteHypothese(regle05,fait02);
    ajouteHypothese(regle05,fait09);

    // Ajout des hypothèses de la règle 6
    ajouteHypothese(regle06,fait02);
    ajouteHypothese(regle06,fait11);

    // Ajout des hypothèses de la règle 7
    ajouteHypothese(regle07,fait02);
    ajouteHypothese(regle07,fait05);
    ajouteHypothese(regle07,fait12);
    ajouteHypothese(regle07,fait13);

    // Ajout des hypothèses de la règle 8
    ajouteHypothese(regle08,fait02);
    ajouteHypothese(regle08,fait05);
    ajouteHypothese(regle08,fait12);
    ajouteHypothese(regle08,fait15);

    // Ajout des hypothèses de la règle 9
    ajouteHypothese(regle09,fait10);
    ajouteHypothese(regle09,fait17);
    ajouteHypothese(regle09,fait18);
    ajouteHypothese(regle09,fait13);

    // Ajout des hypothèses de la règle 10
    ajouteHypothese(regle10,fait10);
    ajouteHypothese(regle10,fait15);

    // Ajout des hypothèses de la règle 11
    ajouteHypothese(regle11,fait20);

    // Ajout des hypothèses de la règle 12
    ajouteHypothese(regle12,fait23);
    ajouteHypothese(regle12,fait24);

    // Ajout des hypothèses de la règle 13
    ajouteHypothese(regle13,fait22);
    ajouteHypothese(regle13,fait25);
    ajouteHypothese(regle13,fait17);
    ajouteHypothese(regle13,fait18);
    ajouteHypothese(regle13,fait26);

    // Ajout des hypothèses de la règle 14
    ajouteHypothese(regle14,fait22);
    ajouteHypothese(regle14,fait25);
    ajouteHypothese(regle14,fait28);
    ajouteHypothese(regle14,fait26);

    // Ajout des hypothèses de la règle 15
    ajouteHypothese(regle15,fait22);
    ajouteHypothese(regle15,fait05);

    // Ajout des Conclusions :

    // pour la regle 1
    ajouteConclusion(regle01,fait02);

    // pour la regle 2
    ajouteConclusion(regle02,fait02);

    // pour la regle 3
    ajouteConclusion(regle03,fait05);

    // pour la regle 4
    ajouteConclusion(regle04,fait05);

    // pour la regle 5
    ajouteConclusion(regle05,fait10);

    // pour la regle 6
    ajouteConclusion(regle06,fait10);

    // pour la regle 7
    ajouteConclusion(regle07,fait14);

    // pour la regle 8
    ajouteConclusion(regle08,fait16);

    // pour la regle 9
    ajouteConclusion(regle09,fait19);

    // pour la regle 10
    ajouteConclusion(regle10,fait21);

    // pour la regle 11
    ajouteConclusion(regle11,fait22);

    // pour la regle 12
    ajouteConclusion(regle12,fait22);

    // pour la regle 13
    ajouteConclusion(regle13,fait27);

    // pour la regle 14
    ajouteConclusion(regle14,fait29);

    // pour la regle 15
    ajouteConclusion(regle15,fait30);

    // Création de la base de regles

    LISTE* baseDeregles= listeVide();

    // Ajout des regles à la base de regles
    ajouterALaFin(baseDeregles, regle01);
    ajouterALaFin(baseDeregles, regle02);
    ajouterALaFin(baseDeregles, regle03);
    ajouterALaFin(baseDeregles, regle04);
    ajouterALaFin(baseDeregles, regle05);
    ajouterALaFin(baseDeregles, regle06);
    ajouterALaFin(baseDeregles, regle07);
    ajouterALaFin(baseDeregles, regle08);
    ajouterALaFin(baseDeregles, regle09);
    ajouterALaFin(baseDeregles, regle10);
    ajouterALaFin(baseDeregles, regle11);
    ajouterALaFin(baseDeregles, regle12);
    ajouterALaFin(baseDeregles, regle13);
    ajouterALaFin(baseDeregles, regle14);
    ajouterALaFin(baseDeregles, regle15);



    // Creation de la base de faits initiaux
    LISTE* baseFaits= listeVide();


    // Liste des faits pour l'ajout de faits

    // Ajouter le fait "a poils"
    // ajouterALaFin(baseFaits, fait01);

    // Ajouter le fait "est un mammifere"
    // ajouterALaFin(baseFaits, fait02);

    // Ajouter le fait "donne du lait"
     ajouterALaFin(baseFaits, fait03);

    // Ajouter le fait "mange de la viande"
    ajouterALaFin(baseFaits, fait04);

    // Ajouter le fait "est un carnivore
    // ajouterALaFin(baseFaits, fait05);

    // Ajouter le fait "a des dents pointues"
     ajouterALaFin(baseFaits, fait06);

    // Ajouter le fait "a des griffes"
    // ajouterALaFin(baseFaits, fait07);

    // Ajouter le fait "a les yeux vers avant"
    //  ajouterALaFin(baseFaits, fait08);

    // Ajouter le fait "a des sabots"
    // ajouterALaFin(baseFaits, fait09);

    // Ajouter le fait "est un ongulé"
    // ajouterALaFin(baseFaits, fait10);

    // Ajouter le fait "n'est pas un carnivore"
    // ajouterALaFin(baseFaits, fait11);

    // Ajouter le fait "est de couleur brune"
     ajouterALaFin(baseFaits, fait12);

    // Ajouter le fait "a des taches sombres"
    // ajouterALaFin(baseFaits, fait13);

    // Ajouter le fait "est un guépard"
    // ajouterALaFin(baseFaits, fait14);

    // Ajouter le fait "a des raies noires"
     ajouterALaFin(baseFaits, fait15);

    // Ajouter le fait "est un tigre"
    // ajouterALaFin(baseFaits, fait16);

    // Ajouter le fait "a un long cou"
    // ajouterALaFin(baseFaits, fait17);

    // Ajouter le fait "a des longues pattes"
    // ajouterALaFin(baseFaits, fait18);

    // Ajouter le fait "est une girafe"
    // ajouterALaFin(baseFaits, fait19);

    // Ajouter le fait "a des plumes"
    // ajouterALaFin(baseFaits, fait20);

    // Ajouter le fait "est un zèbre"
    // ajouterALaFin(baseFaits, fait21);

    // Ajouter le fait "est un oiseau"
    // ajouterALaFin(baseFaits, fait22);

    // Ajouter le fait "vole"
    // ajouterALaFin(baseFaits, fait23);

    // Ajouter le fait "ponds des oeufs"
    // ajouterALaFin(baseFaits, fait24);

    // Ajouter le fait "ne vole pas"
    // ajouterALaFin(baseFaits, fait25);

    // Ajouter le fait "est noir et blanc"
    // ajouterALaFin(baseFaits, fait26);

    // Ajouter le fait "est une autruche"
    // ajouterALaFin(baseFaits, fait27);

    // Ajouter le fait "nage"
    // ajouterALaFin(baseFaits, fait28);

    // Ajouter le fait "est un pingouin"
    // ajouterALaFin(baseFaits, fait29);

    // Ajouter le fait "est un aigle"
    // ajouterALaFin(baseFaits, fait30);

    // Affichage de la base regle
    printf("\n***********************\nLa base de regles est la suivante : \n");
    afficheBaseDeRegles(baseDeregles);

    // Affichage de la base de faits
    printf("\n***********************\nLa base de faits initiaux : \n");
    afficheBaseDeFaits(baseFaits);

    // Le Fait a demontrer (decomenter et completer la ligne suivante)
     FAIT* faitADemontrer = creerFait(14,"est un guépard");

    // ** Executer soit le chainage avant soit le chainage arriere **

   ////////////////// CHAINAGE AVANT //////////////////

   if (UTILISER_CHAINAGE_AVANT)
   {
     printf("\n***********************\nLancement du chainage avant pour démontrer : "); afficheFait(faitADemontrer);
     printf("\n");
     chainageAvant(baseDeregles, baseFaits);
     printf("\n\nEtat de la base de faits après le chaînage avant: \n");
     afficheBaseDeFaits(baseFaits);

     if (existeFait(faitADemontrer, baseFaits))
     {
         printf("\nLe "); afficheFait(faitADemontrer); printf(" est demontré\n");
     }
     else
     {   printf("\nLe "); afficheFait(faitADemontrer); printf(" n'est pas demontrable\n");
     }
   }
  else ////////////////// CHAINAGE ARRIERE //////////////////
  {
    printf("\n***********************\nLancement du chainage arrière pour démontrer : "); afficheFait(faitADemontrer);
    printf("\n");

    if(demontrerFait(baseDeregles, baseFaits, faitADemontrer))
    { printf("\nLe fait ");
      afficheFait(faitADemontrer);
      printf("est démontré");
    }
    else
    {
     printf("\nLe fait ");
     afficheFait(faitADemontrer);
     printf("n'est pas démontrable");
    }
  }
  return 0;
}
