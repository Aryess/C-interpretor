#include "LecteurPhraseAvecTable.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecTable::LecteurPhraseAvecTable(string nomFich) :
	ls(nomFich), ts() {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::analyse() {
	programme();
	cout << "Syntaxe correcte." << endl;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::programme() {
// <programme> ::= debut <seqInst> fin <EOF>

	sauterSymCour("debut");
	seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::seqInst() {
// <seqInst> ::= <inst>  { <inst>  }

	do {
		if (ls.getSymCour()=="<VARIABLE>") {
			ts.chercheAjoute(ls.getSymCour());
		}
		inst();

	} while (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="si" || ls.getSymCour()=="seloncas" || ls.getSymCour()=="tantque" || ls.getSymCour()=="repeter"  || ls.getSymCour()=="pour" || ls.getSymCour()=="lire" || ls.getSymCour()=="ecrire");
	// tant que le symbole courant est un debut possible d'instruction...
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::inst() {
// <inst> ::= <affectation> ; | <instSi> | <instTq> | <instRepeter> | <instLire> ; | <instEcrire> ;

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
void LecteurPhraseAvecTable::affectation() {
// <affectation> ::= <variable> = <expression> | <chaine>

	testerSymCour("<VARIABLE>"); //sauterSymCour("<VARIABLE>");
	ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour("=");
	if(ls.getSymCour()=="<CHAINE>"){
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	}
	else{
		expression();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::expression() {
// <expression> ::= <terme> { <opAdd> <terme> }

	terme();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		opAdd();
		terme();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::facteur() {
//<facteur> ::= <entier>  |  <variable>  |  <opUnaire>  <expBool>  |  ( <expBool> )

	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>") {
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else if (ls.getSymCour()=="-" || ls.getSymCour()=="non") {
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
void LecteurPhraseAvecTable::opAdd() {
	// <opAdd> ::= + | -

	if (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		ls.suivant();
	} else {
		erreur("<opAdd>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::terme() {
	// <terme> ::= <facteur> { <opMult> <facteur> }

	facteur();
	while (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		opMult();
		facteur();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::opMult() {
	// <opMult> ::= * | /

	if (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		ls.suivant();
	} else {
		erreur("<opMult>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::termBool() {
	// <termBool> ::= <relation> { <opEt> <relation> }

	relation();
	while (ls.getSymCour()=="et") {
		opEt();
		relation();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::expBool() {
	// <expBool> ::= <termBool> { <opOu> <termBool> }

	termBool();
	while (ls.getSymCour()=="ou") {
		opOu();
		termBool();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::opEt() {
	// <opEt> ::= et

	if (ls.getSymCour()=="et"){
		ls.suivant();
	} else {
		erreur("<opEt>");
	}
}

/////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::opOu() {
	// <opOu> ::= ou

	if (ls.getSymCour()=="ou"){
		ls.suivant();
	} else {
		erreur("<opOu>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::relation() {
	// <relation> ::= <expression> { <opRel> <expression> }

	expression();
	while (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<" || ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()==">=") {
		opRel();
		expression();
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::opRel() {
	// <opRel> ::= == | != | < | <= | > | >=

	if (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<" || ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()==">=") {
		ls.suivant();
	} else {
		erreur("<opRel>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::opUnaire() {
	// <opUnaire> ::= - | non

	if (ls.getSymCour()=="-" || ls.getSymCour()=="non") {
		ls.suivant();
	} else {
		erreur("<opUnaire>");
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::instSi() {
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
void LecteurPhraseAvecTable::instTq() {
	//		<instTq> ::= tantque ( <expBool> ) <seqInst> fintantque

	sauterSymCour("tantque");
	sauterSymCour("(");
	expBool();
	sauterSymCour(")");
	seqInst();
	sauterSymCour("fintantque");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::instRepeter() {
	// <instRepeter> ::= repeter <seqInst> jusqua ( <expBool> )

	sauterSymCour("repeter");
	seqInst();
	sauterSymCour("jusqua");
	sauterSymCour("(");
	expBool();
	sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::instLire() {
	//	  <instLire> ::= lire ( <variable> )

	sauterSymCour("lire");
	sauterSymCour("(");
	testerSymCour("<VARIABLE>");
	ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour(")");

}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::instEcrire() {
	//	<instEcrire> ::= ecrire ( <expression> | <chaine> )

	sauterSymCour("ecrire");
	sauterSymCour("(");
	if (ls.getSymCour()=="<CHAINE>") {
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else{
		expression();
	}
	sauterSymCour(")");
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::instPour() {
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
void LecteurPhraseAvecTable::instSelonCas() {
	//<instSelonCas> ::= seloncas (<variable>) { cas <entier> : <seqInst> } [defaut : <seqInst>] finselon

	sauterSymCour("seloncas");
	sauterSymCour("(");
	testerSymCour("<VARIABLE>");
	ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour(")");
	while (ls.getSymCour() == "cas") {
		ls.suivant();
		testerSymCour("<ENTIER>");
		ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
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
void LecteurPhraseAvecTable::testerSymCour(string ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::sauterSymCour(string ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecTable::erreur(string mess) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}
