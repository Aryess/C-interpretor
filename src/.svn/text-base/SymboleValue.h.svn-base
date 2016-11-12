#ifndef SYMBOLEVALUE_H
#define SYMBOLEVALUE_H

#include <string.h>
#include <iostream>
using namespace std;

#include "Symbole.h"
#include "Arbre.h"

class SymboleValue : public Symbole,  // un symbole valué est un symbole qui a une valeur, définie ou pas
                     public Noeud  {  // et c'est aussi une feuille de l'arbre abstrait
public:
	  SymboleValue(Symbole s); // construit un symbole valué à partir d'un symbole existant s
	  ~SymboleValue( ) {}
	  void        afficher (unsigned short indentation=0);                        	// affiche le symbole en tant que noeud de l'arbre
	  inline int  getValeur()              { return valeur;                     }// accesseur
	  inline void setValeur(int valeur)    { this->valeur=valeur; this-> contenu="";defini=true;  } 	// accesseur
	  inline void setValeur(string valeur)    { this->contenu=valeur; this->valeur=NULL; defini=true;  } 	// accesseur
	  inline bool estDefini()              { return defini;                     } 	// accesseur
	  inline string getContenu()              { return contenu;                     }// accesseur
	  void genererCpp(unsigned short indentation=0);
	  bool operator == (SymboleValue valeur) ;

	  friend ostream & operator << (ostream & cout, SymboleValue symb); // affiche un symbole value sur cout

private:
	  bool defini;	// indique si la valeur du symbole est définie
	  int  valeur;	// valeur du symbole si elle est définie, zéro sinon
	  string contenu; //valeur du symbole si c'est une chaine

};

#endif /* SYMBOLEVALUE_H */
