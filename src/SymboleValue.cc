#include "SymboleValue.h"
#include <stdlib.h>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////
SymboleValue::SymboleValue(Symbole s) :
	Symbole(s.getChaine()) {
	if (s == "<ENTIER>") {
		valeur = atoi(s.getChaine().c_str()); // c_str convertit une string en char*
		defini = true;
	}
	else if(s == "<CHAINE>") {
		valeur = NULL;
		contenu = s.getChaine();
		defini = true;
	}
	else {
		valeur = 0;
		defini = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
void SymboleValue::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Feuille - Symbole value : " << getChaine() << endl; // on affiche la chaine du symbole
}

////////////////////////////////////////////////////////////////////////////////
// Attention : cette fonction (operator) n'est pas membre de la classe SymboleValue
ostream & operator <<(ostream & cout, SymboleValue symb) {
	cout << (Symbole) symb << "\t\t - Valeur=";
	if (symb.defini && symb.valeur != NULL)
		cout << symb.valeur << " ";
	else if(symb.defini && symb.valeur == NULL){
		cout << symb.contenu << " ";
	}
	else
		cout << "indefinie ";
	return cout;
}

////////////////////////////////////////////////////////////////////////////////
bool SymboleValue::operator == (SymboleValue valeur) {
	return (this->getChaine() == valeur.getChaine() && this->getValeur() == valeur.getValeur());
}

////////////////////////////////////////////////////////////////////////////////
void SymboleValue::genererCpp(unsigned short indentation) {
	string text = "";
	if(this->getValeur() == NULL)
			text= this->getChaine() ;
	else {
		stringstream out;
		out << this->getValeur();
		text = out.str();
	}
	cout << text;
}
