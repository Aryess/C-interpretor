// Contient toutes les spécifications de classes nécessaires
// pour représenter l'arbre abstrait
#ifndef ARBRE_H_
#define ARBRE_H_

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud {
// classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
  public:
    virtual int  getValeur() =0 ; // méthode pure (non implémentée) qui rend la classe abstraite
    virtual void afficher(unsigned short indentation=0) { cout << setw(4*indentation) << " "; }
      // pour afficher un noeud avec un décalage (indentation) proportionnel à son niveau dans l'arbre
    virtual void genererCpp(unsigned short indentation=0) { cout << setw(4*indentation) << " "; }
          // pour afficher le code Cpp généré par un noeud

    virtual ~Noeud() {} // présence d'un destructeur virtuel conseillée dans les classes abstraites
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
// classe pour représenter un noeud "sequence d'instruction"
// qui a autant de fils que d'instructions dans la séquence
  public:
     NoeudSeqInst();   // construit une séquence d'instruction vide
    ~NoeudSeqInst() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); // évalue chaque instruction de la séquence
    void afficher(unsigned short indentation=0); // affiche la séquence d'instructions
    void ajouteInstruction(Noeud* instruction);  // ajoute une instruction à la séquence
    void genererCpp(unsigned short indentation=0); //affiche le code généré

  private:
    vector<Noeud *> tabInst; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
// classe pour représenter un noeud "affectation"
// composé de 2 fils : la variable et l'expression qu'on lui affecte
  public:
     NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
    ~NoeudAffectation() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); // évalue l'expression et affecte sa valeur à la variable
    void afficher(unsigned short indentation=0); // affiche l'affectation
    void genererCpp(unsigned short indentation=0); //affiche le code généré

  private:
    Noeud* variable;
    Noeud* expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
// classe pour représenter un noeud "opération arithmétique" composé d'un opérateur (+-*/)
// et de 2 fils : l'opérande gauche et l'opérande droit
  public:
    NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
     // construit une opération binaire : operandeGauche operateur OperandeDroit
   ~NoeudOperateurBinaire() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); // évalue l'operande gauche, l'operande droit et applique l'opérateur
    void afficher(unsigned short indentation=0); // affiche l'opération
    void genererCpp(unsigned short indentation=0); //affiche le code généré

  private:
    Symbole operateur;
    Noeud*  operandeGauche;
    Noeud*  operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstCond : public Noeud {
// classe pour représenter un noeud abstrait "instCond" composé des inst intSi, instPour, instTq, instRepeter
  public:
	NoeudInstCond(Noeud* cond, Noeud* seq);
     // construit une instCond
   ~NoeudInstCond() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur() =0; // methode abstraite a redefinir
    void afficher(unsigned short indentation=0) =0; // affiche l'opération
    void genererCpp(unsigned short indentation=0) {} //affiche le code généré

  protected:
    Noeud*  cond;
    Noeud*  seq;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstTq : public NoeudInstCond {
// classe pour représenter un noeud "tant que"
  public:
	NoeudInstTq(Noeud* cond, Noeud* seq);
     // construit une condition tq : tantque ( cond ) seq fintantque
   ~NoeudInstTq() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); /// évalue l'expression et affecte sa valeur à la variable
    void afficher(unsigned short indentation=0); // affiche l'opération
    void genererCpp(unsigned short indentation=0); //affiche le code généré
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstRepeter : public NoeudInstCond {
// classe pour représenter un noeud "repeter tant que"
  public:
	NoeudInstRepeter(Noeud* cond, Noeud* seq);
	// construit une condition repeter : repeter seq jusqua ( cond )
   ~NoeudInstRepeter() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); // évalue l'expression et affecte sa valeur à la variable
    void afficher(unsigned short indentation=0); // affiche l'opération
    void genererCpp(unsigned short indentation=0); //affiche le code généré
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstPour : public NoeudInstCond {
// classe pour représenter un noeud "pour" composé de deux fils : l'affectation et l'iteration
  public:
	NoeudInstPour(Noeud* cond, Noeud* seq, Noeud* affect, Noeud* iter);
	// construit une condition repeter : repeter seq jusqua ( cond )
   ~NoeudInstPour() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); // évalue l'expression et affecte sa valeur à la variable
    void afficher(unsigned short indentation=0); // affiche l'opération
    void genererCpp(unsigned short indentation=0); //affiche le code généré

  private:
    Noeud * affect;
    Noeud * iter;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSi : public NoeudInstCond {
// classe pour représenter un noeud "si" composé de deux fils : l'affectation et l'iteration
  public:
	NoeudInstSi(Noeud* cond, Noeud* seq);
	// construit une condition repeter : repeter seq jusqua ( cond )
   ~NoeudInstSi() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); // évalue l'expression et affecte sa valeur à la variable
    void afficher(unsigned short indentation=0); // affiche l'opération
    void ajouteSinonSi(Noeud* cond, Noeud* seq);  // ajoute une instruction à la séquence
    void genererCpp(unsigned short indentation=0); //affiche le code généré

  private:
    vector<NoeudInstSi *> tabS ;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstES : public Noeud {
// classe pour représenter un noeud "si" composé de deux fils : l'affectation et l'iteration
  public:
	NoeudInstES(Symbole action, Noeud* var);
	// construit une condition repeter : repeter seq jusqua ( cond )
   ~NoeudInstES() {} // à cause du destructeur virtuel de la classe Noeud
    int getValeur(); // évalue l'expression et affecte sa valeur à la variable
    void afficher(unsigned short indentation=0); // affiche l'opération
    void genererCpp(unsigned short indentation=0); //affiche le code généré

  private:
    Symbole action;
    Noeud * var;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstCas : public Noeud {
	public :
		NoeudInstCas(Noeud* var, Noeud* seq);
		// construit une condition repeter : repeter seq jusqua ( cond )
		~NoeudInstCas() {} // à cause du destructeur virtuel de la classe Noeud
		void afficher(unsigned short indentation=0);
		void genererCpp(unsigned short indentation=0); //affiche le code généré
		int getValeur() { return 0;}
		int getValeur(int val);

	private :
		Noeud* var;
		Noeud* seq;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSelon : public Noeud {
	public :
	 NoeudInstSelon(Noeud* var);
		// construit une condition repeter : repeter seq jusqua ( cond )
	 ~NoeudInstSelon() {} // à cause du destructeur virtuel de la classe Noeud
	 void afficher(unsigned short indentation=0);
	 int getValeur();
	 void ajouteCas(Noeud* var, Noeud* seq);  // ajoute une instruction à la séquence
	 void genererCpp(unsigned short indentation=0); //affiche le code généré

	private :
	 vector<NoeudInstCas *> tabS ;
	 Noeud*  var;
};


#endif /* ARBRE_H_ */
