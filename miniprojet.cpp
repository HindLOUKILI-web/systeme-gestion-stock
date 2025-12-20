#include<stdio.h>
#include<stdlib.h>
#include<string.h>

 
 //les structures de donnees
 
 
 //-----------produits----------//
 typedef struct Produit {
    int id;
    char nom[50];
    int quantite;
    int seuil;
    float prix;
    struct Produit *suivant;
} Produit;

//---------fournisseurs------------//
typedef struct Fournisseur {
    int id;
    char nom[50];
    char contact[50];
    struct Fournisseur *suivant;
} Fournisseur;

//----------mouvements-----------//
typedef struct Mouvement {
    int idProduit;
    char type[10]; // ENTREE / SORTIE
    int quantite;
    struct Mouvement *prev;
    struct Mouvement *next;
} Mouvement;

//----------pile(historique)---------//
typedef struct StackNode {
    char action[100];
    struct StackNode *suivant;
} StackNode;

//--------file(alertes)-----------//
typedef struct noeud {
    char message[100];
    struct noeud *suivant;
} noeud;

typedef struct {
    noeud *front, *rear;
} FileAlertes;
 
 
//-------fonctions des produits-----------//


/*creer un produit */
Produit* creerProduit(int id, char nom[], int q, int seuil, float prix) {
    Produit *p;
	p=(Produit*)malloc(sizeof(Produit));
    p->id = id;
    strcpy(p->nom, nom);
    p->quantite = q;
    p->seuil = seuil;
    p->prix = prix;
    p->suivant = NULL;
    return p;
}
/*rechercher produit */
Produit* rechercherProduit(Produit *p, int id) {
    while (p) {
        if (p->id == id) return p;
        p = p->suivant;
    }
    return NULL;
}
/*ajouter produit */
void ajouterProduit(Produit **l) {
    int id, q, seuil;
    float prix;
    char nom[50];

    printf("ID: ");
	scanf("%d",&id);
    if (rechercherProduit(*l,id)) {
        printf("Produit existe deja.\n");
        return ;
    }
   	printf("Nom: ");
    scanf("%s",nom);
    printf("Quantite: ");
    scanf("%d",&q);
	printf("Seuil: ");
	scanf("%d",&seuil);
	printf("Prix: ");
	scanf("%f",&prix);

    Produit *p = creerProduit(id,nom,q,seuil,prix);
    p->suivant = *l;
    *l = p;
}
/*modifier produit */
void modifierProduit(Produit *l) {
    int id;
    printf("ID produit: ");
	scanf("%d",&id);
    Produit *p = rechercherProduit(l,id);
    if (!p) { 
	    printf("Introuvable\n");
	}
	else {
		printf("Nouveau prix: ");
		scanf("%f",&p->prix);
		printf("Nouvelle quantite: ");
		scanf("%d",&p->quantite);
	}
}
/*afficher produit*/
void afficherProduits(Produit *l) {
    while (l) {
        printf("ID: %d | Nom: %s | Q: %d | Seuil: %d | Prix: %.2f\n", l->id,l->nom,l->quantite,l->seuil,l->prix);
        l = l->suivant;
    }
}
/*supprimer produit */
void supprimerProduit(Produit **l, int id) {
    Produit *courant=*l,*prec=NULL;
    while(courant && courant->id!=id){ 
	    prec=courant;courant=courant->suivant;
	}
    if(!courant) return;//je peux ajouter printf('produit introuvable donc impossible de supprimer')
    if(!prec) *l=courant->suivant;
    else prec->suivant=courant->suivant;
    free(courant);
}
/*trier les produits (A-->Z) */
void trierProduitsParNom(Produit **liste) {
    Produit *courant = *liste;
    Produit *listeTriee = NULL;

    while (courant != NULL) {
        Produit *suivant = courant->suivant;

        /* insertion ordonnee par nom */
        if (listeTriee == NULL || strcmp(courant->nom, listeTriee->nom) < 0) {
		    courant->suivant = listeTriee;
            listeTriee = courant;
        }
		else {
            Produit *p = listeTriee;
            while (p->suivant != NULL && strcmp(p->suivant->nom, courant->nom) < 0) {
                p = p->suivant;
            }
            courant->suivant = p->suivant;
            p->suivant = courant;
        }
        courant = suivant;
    }
    *liste = listeTriee;
}
/*trier les produits par quantite */
void trierProduitsParQuantite(Produit **liste) {
    Produit *courant = *liste;
    Produit *listeTriee = NULL;
    while (courant != NULL) {
        Produit *suivant = courant->suivant;
        /* insertion ordonnee par quantite */
        if (listeTriee == NULL || courant->quantite < listeTriee->quantite) {
            courant->suivant = listeTriee;
            listeTriee = courant;
        }
		else {
            Produit *p = listeTriee;
            while (p->suivant != NULL && p->suivant->quantite < courant->quantite) {
                p = p->suivant;
            }
            courant->suivant = p->suivant;
            p->suivant = courant;
        }
        courant = suivant;
    }
    *liste = listeTriee;
}

//---------fonctions des fournisseurs------------//
/*creer fournisseur */
Fournisseur* creerFournisseur(int id, const char *nom, const char *contact) {
    Fournisseur *f;
	f=(Fournisseur*)malloc(sizeof(Fournisseur));
	
    if (f == NULL) {
        printf("Erreur d'allocation memoire\n");
        return NULL;
    }

    f->id = id;
    strcpy(f->nom, nom);
    strcpy(f->contact, contact);

    f->suivant = NULL;

    return f;
}
/*ajouter fournisseur */
void ajouterFournisseur(Fournisseur **l) {
    int id;
    char nom[50], contact[50];

    printf("ID: ");
    scanf("%d",&id);
    printf("Nom: ");
    scanf("%s",nom);
    printf("Contact: ");
    scanf("%s",contact);

    Fournisseur *f = creerFournisseur(id, nom, contact);
    f->suivant = *l;
    *l = f;
}
/*afficher fournisseur */
void afficherFournisseurs(Fournisseur *l) {
    while(l) {
        printf("ID: %d | Nom: %s | Contact: %s\n",l->id,l->nom,l->contact);
        l=l->suivant;
    }
}
/*modifier fournisseur */
void modifierFournisseur(Fournisseur *liste) {
    int id;
    printf("ID du fournisseur a modifier : ");
    scanf("%d", &id);

    /* Recherche du fournisseur */
    while (liste != NULL && liste->id != id) {
        liste = liste->suivant;
    }

    if (liste == NULL) {
        printf("Fournisseur introuvable.\n");
        return;
    }

    /* Modification des informations */
    printf("Nouveau nom : ");
    scanf("%s", liste->nom);

    printf("Nouveau contact : ");
    scanf("%s", liste->contact);

    printf("Fournisseur modifie avec succes.\n");
}

//--------fonctions des mouvements---------//
/*creer mouvement */
Mouvement* creerMouvement(int idProduit, const char *type, int quantite) {
    Mouvement *m;
	m=(Mouvement*)malloc(sizeof(Mouvement));
    if (m == NULL) {
        printf("Erreur d'allocation memoire\n");
        return NULL;
    }

    m->idProduit = idProduit;
    strcpy(m->type, type);   // "ENTREE" ou "SORTIE"
    m->quantite = quantite;
    m->prev = NULL;
    m->next = NULL;
    return m;
}
/*enregistrer mouvement */
void enregistrerMouvement(Mouvement **head, int idP, char type[], int q) {
    Mouvement *m = creerMouvement(idP, type, q);
    if (!m) return;

    m->next = *head;
    if (*head) (*head)->prev = m;
    *head = m;
}
/*afficher mouvements */
void afficherMouvements(Mouvement *h) {
    while(h) {
        printf("Produit:%d | %s | Q:%d\n", h->idProduit,h->type,h->quantite);
        h=h->next;
    }
}

//------pile(historique)----------//
/*initialiser la pile */
void initialiserPile(StackNode **p) {
    *p= NULL;
}
/*empiler la pile la fonction push */
void push(StackNode **s, char action[]) {
    StackNode *n;
	n=(StackNode*)malloc(sizeof(StackNode));
    strcpy(n->action,action);
    n->suivant = *s;
    *s = n;
}
/*afficher historique */
void afficherHistorique(StackNode *s) {
    while(s) {
        printf("%s\n",s->action);
        s=s->suivant;
    }
}

//----------alertes---------------//
/*initialiser la file */
void initFile(FileAlertes *f){
    f->front=f->rear=NULL;
}
/*ajouter alerte */
void ajouterAlerte(FileAlertes *f, char msg[]) {
    noeud *n;
	n=(noeud*)malloc(sizeof(noeud));
    strcpy(n->message,msg);
    n->suivant=NULL;
    if(!f->rear) f->front=f->rear=n;
    else {f->rear->suivant=n; f->rear=n;}
}
/*afficher alerte */
void afficherAlertes(FileAlertes *f) {
    noeud *c=f->front;
    while(c) {
        printf("ALERTE: %s\n",c->message);
        c=c->suivant;
    }
}
/*verifier seuil de reapprovisionnement */
void verifierTousLesSeuils(Produit *p, FileAlertes *f) {
    while (p) {
        if (p->quantite <= p->seuil) {
            char msg[100];
            sprintf(msg,"Stock bas pour %s (Q=%d)",p->nom,p->quantite);
            ajouterAlerte(f,msg);
        }
        p = p->suivant;
    }
}

//-------liberer la memoire-----------//
/*liberer les produits */
void libererProduits(Produit *l) {
    Produit *courant;
    while (l) {
        courant = l;
        l = l->suivant;
        free(courant);
    }
}
/*liberer les fournisseurs */
void libererFournisseurs(Fournisseur *l) {
    Fournisseur *courant;
    while (l) {
        courant = l;
        l = l->suivant;
        free(courant);
    }
}
/*liberer les mouvements */
void libererMouvements(Mouvement *m) {
    Mouvement *courant;
    while (m) {
        courant = m;
        m = m->next;
        free(courant);
    }
}
/*liberer la pile (historique) */
void libererPile(StackNode *s) {
    StackNode *courant;
    while (s) {
        courant = s;
        s = s->suivant;
        free(courant);
    }
}
/*liberer la file (alertes) */
void libererAlertes(FileAlertes *f) {
    noeud *courant;
    while (f->front) {
        courant = f->front;
        f->front = f->front->suivant;
        free(courant);
    }
    f->rear = NULL;
}

//-------------menu-------------//
void menu() {
    printf("\n===== GESTION DE STOCK =====\n");
    printf("1. Ajouter un produit\n");
    printf("2. Rechercher un produit\n");
    printf("3. Modifier un produit\n");
    printf("4. Supprimer un produit\n");
    printf("5. Afficher tous les produits\n");
    printf("6. Trier les produits avec l'ordre alphabetique\n");
    printf("7. Trier les produits avec la quantite\n");
    printf("8. Ajouter un fournisseur\n");
    printf("9. Modifier un fournisseur\n");
    printf("10. Afficher les fournisseurs\n");
    printf("11. Enregistrer un mouvement\n");
    printf("12. Afficher les mouvements\n");
    printf("13. Afficher historique\n");
    printf("14. Afficher alertes\n");
    printf("15. Verifier les seuils de reapprovisionnement\n");
    printf("0. Quitter\n");
    printf("Choix: ");
}
int main() {
    Produit *stock = NULL;
    Fournisseur *fournisseurs = NULL;
    Mouvement *mouvements = NULL;
    StackNode *historique = NULL;
    FileAlertes alertes;

    initialiserPile(&historique);
    initFile(&alertes);

    int choix, id, q;
    char type[10], action[100];

    do {
        menu();
        scanf("%d", &choix);

        switch (choix) {

        case 1:
            ajouterProduit(&stock);
            break;
            
        case 2:
            printf("ID produit a rechercher: ");
            scanf("%d",&id);
            {
                Produit *p = rechercherProduit(stock,id);
                if (p)
                    printf("ID:%d | Nom:%s | Q:%d | Seuil:%d | Prix:%.2f\n", p->id,p->nom,p->quantite,p->seuil,p->prix);
                else
                    printf("Produit introuvable.\n");
            }
            break;

        case 3:
            modifierProduit(stock);
            break;

        case 4:
            printf("ID produit a supprimer: ");
            scanf("%d",&id);
            supprimerProduit(&stock,id);
            break;
        case 5:
            afficherProduits(stock);
            break;
            
        case 6:
            trierProduitsParNom(&stock);
            printf("Produits tries par nom.\n");
            afficherProduits(stock);
            break;

        case 7:
            trierProduitsParQuantite(&stock);
            printf("Produits tries par quantite.\n");
            afficherProduits(stock);
            break;

        case 8:
            ajouterFournisseur(&fournisseurs);
            break;

        case 9:
            modifierFournisseur(fournisseurs);
            break;
            
        case 10:
            afficherFournisseurs(fournisseurs);
            break;
            
        case 11:{
            printf("ID Produit: ");
            scanf("%d", &id);
            while(getchar() != '\n'); 

            Produit *p = rechercherProduit(stock, id);
            if (!p) {
                printf("Produit introuvable.\n");
                break;
            }

            printf("Type (ENTREE/SORTIE): ");
            scanf("%s", type);
            while(getchar() != '\n');

            printf("Quantite: ");
            scanf("%d", &q);
            while(getchar() != '\n');

            if (strcmp(type, "SORTIE") == 0) {
                if (p->quantite < q) {
                    printf("Stock insuffisant.\n");
                    break;
                }
                p->quantite -= q;
            }
			else if (strcmp(type, "ENTREE") == 0) {
                p->quantite += q;
            }
			else {
                printf("Type invalide.\n");
                break;
            }

            enregistrerMouvement(&mouvements, id, type, q);
            sprintf(action,"Mouvement %s | Produit %d | Q=%d", type, id, q);
            push(&historique, action);

            break;
}

        case 12:
            afficherMouvements(mouvements);
            break;

        case 13:
            afficherHistorique(historique);
            break;

        case 14:
            afficherAlertes(&alertes);
            break;

        case 15:
            if (stock)
                verifierTousLesSeuils(stock,&alertes);
            break;

        case 0:
            printf("Fin du programme.\n");
            break;

        default:
            printf("Choix invalide.\n");
        }

    } while (choix != 0);
    
    libererProduits(stock);
    libererFournisseurs(fournisseurs);
    libererMouvements(mouvements);
    libererPile(historique);
    libererAlertes(&alertes);

	return 0;
}
