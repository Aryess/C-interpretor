#include "LecteurPhraseSimple.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseSimple::LecteurPhraseSimple(string nomFich) :
	ls(nomFich) {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::analyse() {
	programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::programme() {
// <programme> ::= debut <seqInst> fin FIN_FICHIER
	
	sauterSymCour("debut");
	seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::seqInst() {
// <seqInst> ::= <inst> ; { <inst> ; }

	do {
		inst();

	} while (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="si" || ls.getSymCour()=="seloncas" || ls.getSymCour()=="tantque" || ls.getSymCour()=="repeter"  || ls.getSymCour()=="pour" || ls.getSymCour()=="lire" || ls.getSymCour()=="ecrire");
	// tant que le symbole courant est un debut possible d'instruction...
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::inst() {
// <inst> ::= <affectation> | <instSi> | <instTq> | <instRepeter>

	if(ls.getSymCour()=="si"){
		instSi();
	}
	else if(ls.getSymCour()=="tantque"){
		instTq();
	}
	else if(ls.getSymCour()=="repeter"){
		instRepeter();
	}
	else if(ls.getSymCour()=="pour"){
		instPour();
	}
	else if(ls.getSymCour()=="lire"){
		instLire();
		sauterSymCour(";");
	}
	else if(ls.getSymCour()=="ecrire"){
		instEcrire();
		sauterSymCour(";");
	}
	else if(ls.getSymCour()=="seloncas"){
		instSelonCas();
	}
	else{
		affectation();
		sauterSymCour(";");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::affectation() {
// <affectation> ::= <variable> = <expression> | <chaine>

	sauterSymCour("<VARIABLE>");
	sauterSymCour("=");
	if(ls.getSymCour()=="<CHAINE>"){
		ls.suivant();
	}
	else{
		expression();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::expression() {
// <expression> ::= <terme> { <opAdd> <terme> }

	terme();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		opAdd();
		terme();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::facteur() {
//<facteur> ::= <entier>  |  <variable>  |  <opUnaire>  <expBool>  |  ( <expBool> )

	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>")
		ls.suivant();
	else if (ls.getSymCour()=="-" || ls.getSymCour()=="non") {
		ls.suivant();
		expBool();
	} else if (ls.getSymCour()=="(") {
		ls.suivant();
		expBool();
		sauterSymCour(")");
	} else
		erreur("<facteur>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opAdd() {
	// <opAdd> ::= + | -

	if (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		ls.suivant();
	} else {
		erreur("<opAdd>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::terme() {
	// <terme> ::= <facteur> { <opMult> <facteur> }

	facteur();
	while (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		opMult();
		facteur();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opMult() {
	// <opMult> ::= * | /

	if (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		ls.suivant();
	} else {
		erreur("<opMult>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::termBool() {
	// <termBool> ::= <relation> { <opEt> <relation> }

	relation();
	while (ls.getSymCour()=="et") {
		opEt();
		relation();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::expBool() {
	// <expBool> ::= <termBool> { <opOu> <termBool> }

	termBool();
	while (ls.getSymCour()=="ou") {
		opOu();
		termBool();
	}
}

////////////////////////////////////////////////////////////////////////////////
/*
void LecteurPhraseSimple::opBool() {
	// <opBool> ::= et | ou

	if (ls.getSymCour()=="et" || ls.getSymCour()=="ou") {
		ls.suivant();
	} else {
		erreur("<opBool>");
	}
}
//*/
////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opEt() {
	// <opEt> ::= et

	if (ls.getSymCour()=="et"){
		ls.suivant();
	} else {
		erreur("<opEt>");
	}
}
///////////
/////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opOu() {
	// <opOu> ::= ou

	if (ls.getSymCour()=="ou"){
		ls.suivant();
	} else {
		erreur("<opOu>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::relation() {
	// <relation> ::= <expression> { <opRel> <expression> }

	expression();
	while (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<" || ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()==">=") {
		opRel();
		expression();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opRel() {
	// <opRel> ::= == | != | < | <= | > | >=

	if (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<" || ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()==">=") {
		ls.suivant();
	} else {
		erreur("<opRel>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::opUnaire() {
	// <opUnaire> ::= - | non

	if (ls.getSymCour()=="-" || ls.getSymCour()=="non") {
		ls.suivant();
	} else {
		erreur("<opUnaire>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instSi() {
	//	    <instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> }
	//					[ sinon <seqInst> ] finsi

	sauterSymCour("si");
	sauterSymCour("(");
	expBool();
	sauterSymCour(")");
	seqInst();
	while(ls.getSymCour()=="sinonsi"){
		ls.suivant();
		sauterSymCour("(");
		expBool();
		sauterSymCour(")");
		seqInst();
	}
	if(ls.getSymCour()=="sinon"){
		ls.suivant();
		seqInst();
	}
	sauterSymCour("finsi");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instTq() {
	//		<instTq> ::= tantque ( <expBool> ) <seqInst> fintantque

	sauterSymCour("tantque");
	sauterSymCour("(");
	expBool();
	sauterSymCour(")");
	seqInst();
	sauterSymCour("fintantque");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instRepeter() {
	// <instRepeter> ::= repeter <seqInst> jusqua ( <expBool> )

	sauterSymCour("repeter");
	seqInst();
	sauterSymCour("jusqua");
	sauterSymCour("(");
	expBool();
	sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instLire() {
	//	  <instLire> ::= lire ( <variable> )

	sauterSymCour("lire");
	sauterSymCour("(");
	sauterSymCour("<VARIABLE>");
	sauterSymCour(")");

}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instEcrire() {
	//	<instEcrire> ::= ecrire ( <expression> | <chaine> )

	sauterSymCour("ecrire");
	sauterSymCour("(");
	if (ls.getSymCour()=="<CHAINE>") {
		sauterSymCour("<CHAINE>");
	} else{
		expression();
	}
	sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instPour() {
	//	  <instPour> ::= pour ( <affectation>, <expBool> , <affectation>) <seqInst> finpour

	sauterSymCour("pour");
	sauterSymCour("(");
	affectation();
	sauterSymCour(";");
	expBool();
	sauterSymCour(";");
	affectation();
	sauterSymCour(")");
	seqInst();
	sauterSymCour("finpour");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::instSelonCas() {
	//<instSelonCas> ::= seloncas (<variable>) { cas <entier> : <seqInst> } [defaut : <seqInst>] finselon

	sauterSymCour("seloncas");
	sauterSymCour("(");
	sauterSymCour("<VARIABLE>");
	sauterSymCour(")");
	while (ls.getSymCour() == "cas") {
		ls.suivant();
		sauterSymCour("<ENTIER>");
		sauterSymCour(":");
		seqInst();
	}
	if (ls.getSymCour() == "defaut") {
		ls.suivant();
		sauterSymCour(":");
		seqInst();
	}
	sauterSymCour("finselon");

}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::testerSymCour(string ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::sauterSymCour(string ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseSimple::erreur(string mess) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}
