#include "LecteurPhraseAvecArbre.h"

#include <stdlib.h>
#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
LecteurPhraseAvecArbre::LecteurPhraseAvecArbre(string nomFich) :
	ls(nomFich), ts() {
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::analyse() {
	arbre = programme();
	cout << "Syntaxe correcte." << endl << endl;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::genererCpp(unsigned short indentation){
	cout << "#include <stdlib.h>" << endl;
	cout << "#include <iostream>" << endl;
	cout << "using namespace std;" << endl << endl ;
	cout<< "int main(int argc, char* argv[]) {" << endl;
	this->getArbre()->genererCpp();
	cout<< "}" << endl;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::programme() {
// <programme> ::= debut <seqInst> fin <EOF>

	sauterSymCour("debut");
	Noeud* si = seqInst();
	sauterSymCour("fin");
	testerSymCour("<FINDEFICHIER>");
	return si;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::seqInst() {
// <seqInst> ::= <inst>  { <inst>  }

	NoeudSeqInst* si = new NoeudSeqInst();
	do {
		if (ls.getSymCour()=="<VARIABLE>") {
			ts.chercheAjoute(ls.getSymCour());
		}
		si->ajouteInstruction(inst());
		
	} while (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="si" || ls.getSymCour()=="seloncas" || ls.getSymCour()=="tantque" || ls.getSymCour()=="repeter"  || ls.getSymCour()=="pour" || ls.getSymCour()=="lire" || ls.getSymCour()=="ecrire");
	// tant que le symbole courant est un debut possible d'instruction...

	return si;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::inst() {
// <inst> ::= <affectation> ; | <instSi> | <instTq> | <instRepeter> | <instLire> ; | <instEcrire> ; | <instSelonCas>

	Noeud* inst;
	if(ls.getSymCour()=="si"){
		inst = instSi();
	}
	else if(ls.getSymCour()=="tantque"){
		inst = instTq();
	}
	else if(ls.getSymCour()=="repeter"){
		inst = instRepeter();
	}
	else if(ls.getSymCour()=="pour"){
		inst = instPour();
	}
	else if(ls.getSymCour()=="lire"){
		inst = instLire();
		sauterSymCour(";");
	}
	else if(ls.getSymCour()=="ecrire"){
		inst = instEcrire();
		sauterSymCour(";");
	}
	else if(ls.getSymCour()=="seloncas"){
		inst = instSelonCas();
	}
	else{
		inst = affectation();
		sauterSymCour(";");	
	}

	return inst;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::affectation() {
// <affectation> ::= <variable> = <expression> | <chaine>


	testerSymCour("<VARIABLE>");
	Noeud* var = ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour("=");
	Noeud* exp;
	if(ls.getSymCour()=="<CHAINE>"){
		exp=ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else {
		exp = expression();
	}

	return new NoeudAffectation(var, exp);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::expression() {
// <expression> ::= <terme> { <opAdd> <terme> }

	Noeud* exp = terme();
	while (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		Symbole symb = opAdd();
		exp = new NoeudOperateurBinaire(symb, exp, terme());
	}

	return exp;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::facteur() {
//<facteur> ::= <entier>  |  <variable>  |  <opUnaire>  <expBool>  |  ( <expBool> )

	Noeud* nd = NULL;
	if (ls.getSymCour()=="<VARIABLE>" || ls.getSymCour()=="<ENTIER>") {
		nd = ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else if (ls.getSymCour()=="-" || ls.getSymCour()=="non") {
		Symbole symb = opUnaire();
		Symbole symbo("0");
		nd = new NoeudOperateurBinaire(symb, new SymboleValue(symbo),expBool());
	} else if (ls.getSymCour()=="(") {
		ls.suivant();
		nd = expBool();
		sauterSymCour(")");
	} else {
		erreur("<facteur>");
	}

	return nd;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opAdd() {
	// <opAdd> ::= + | -

	Symbole symb;
	if (ls.getSymCour()=="+" || ls.getSymCour()=="-") {
		symb = ls.getSymCour();
		ls.suivant();
	} else {
		erreur("<opAdd>");
	}

	return symb;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::terme() {
	// <terme> ::= <facteur> { <opMult> <facteur> }

	Noeud* nd = facteur();
	while (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		Symbole symb = opMult();
		nd = new NoeudOperateurBinaire(symb, nd, facteur());
	}
	return nd;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opMult() {
	// <opMult> ::= * | /

	Symbole symb;
	if (ls.getSymCour()=="*" || ls.getSymCour()=="/") {
		symb = ls.getSymCour();
		ls.suivant();
	} else {
		erreur("<opMult>");
	}

	return symb;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::termBool() {
	// <termBool> ::= <relation> { <opEt> <relation> }

	Noeud* nd = relation();
	while (ls.getSymCour()=="et") {
		Symbole symb = opEt();
		nd = new NoeudOperateurBinaire(symb, nd, relation());
	}
	return nd;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::expBool() {
	// <expBool> ::= <termBool> { <opOu> <termBool> }

	Noeud* nd = termBool();
	while (ls.getSymCour()=="ou") {
		Symbole symb = opOu();
		nd = new NoeudOperateurBinaire(symb, nd, termBool());
	}
	return nd;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opEt() {
	// <opEt> ::= et

	Symbole symb;
	if (ls.getSymCour()=="et"){
		symb = ls.getSymCour();
		ls.suivant();
	} else {
		erreur("<opEt>");
	}
	return symb;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opOu() {
	// <opOu> ::= ou

	Symbole symb;
	if (ls.getSymCour()=="ou"){
		symb = ls.getSymCour();
		ls.suivant();
	} else {
		erreur("<opOu>");
	}
	return symb;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::relation() {
	// <relation> ::= <expression> { <opRel> <expression> }

	Noeud* nd = expression();
	while (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<" || ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()==">=") {
		Symbole symb = opRel();
		nd = new NoeudOperateurBinaire(symb, nd, expression());
	}

	return nd;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opRel() {
	// <opRel> ::= == | != | < | <= | > | >=

	Symbole symb;
	if (ls.getSymCour()=="==" || ls.getSymCour()=="!=" || ls.getSymCour()=="<" || ls.getSymCour()=="<=" || ls.getSymCour()==">" || ls.getSymCour()==">=") {
		symb = ls.getSymCour();
		ls.suivant();
	} else {
		erreur("<opRel>");
	}

	return symb;
}

////////////////////////////////////////////////////////////////////////////////
Symbole LecteurPhraseAvecArbre::opUnaire() {
	// <opUnaire> ::= - | non

	Symbole symb;
	if (ls.getSymCour()=="-" || ls.getSymCour()=="non") {
		symb = ls.getSymCour();
		ls.suivant();
	} else {
		erreur("<opUnaire>");
	}

	return symb;

}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instSi() {
	//	    <instSi> ::= si ( <expBool> ) <seqInst> { sinonsi ( <expBool> ) <seqInst> }
	//					[ sinon <seqInst> ] finsi

	sauterSymCour("si");
	sauterSymCour("(");
	Noeud* cond = expBool();
	sauterSymCour(")");
	Noeud* seq = seqInst();
	NoeudInstSi* nd = new NoeudInstSi(cond, seq);
	while(ls.getSymCour()=="sinonsi"){
		ls.suivant();
		sauterSymCour("(");
		cond = expBool();
		sauterSymCour(")");
		seq = seqInst();
		nd->ajouteSinonSi(cond, seq);
	}
	if(ls.getSymCour()=="sinon"){
		ls.suivant();
		Symbole symbo("1");
		nd->ajouteSinonSi((new SymboleValue(symbo)),seqInst());
	}
	sauterSymCour("finsi");

	return nd;
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instTq() {
	//		<instTq> ::= tantque ( <expBool> ) <seqInst> fintantque

	sauterSymCour("tantque");
	sauterSymCour("(");
	Noeud* cond = expBool();
	sauterSymCour(")");
	Noeud* seq = seqInst();
	sauterSymCour("fintantque");

	return new NoeudInstTq(cond, seq);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instRepeter() {
	// <instRepeter> ::= repeter <seqInst> jusqua ( <expBool> )

	sauterSymCour("repeter");
	Noeud* seq = seqInst();
	sauterSymCour("jusqua");
	sauterSymCour("(");
	Noeud* cond = expBool();
	sauterSymCour(")");
	
	return new NoeudInstRepeter(cond, seq);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instLire() {
	//	  <instLire> ::= lire ( <variable> )

	sauterSymCour("lire");
	sauterSymCour("(");
	testerSymCour("<VARIABLE>");
	Noeud* nd = ts.chercheAjoute(ls.getSymCour());
	ls.suivant();
	sauterSymCour(")");
	Symbole symb("lire");
	return new NoeudInstES(symb, nd);

}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instEcrire() {
	//	<instEcrire> ::= ecrire ( <expression> | <chaine> )

	sauterSymCour("ecrire");
	sauterSymCour("(");
	Noeud* nd;
	if (ls.getSymCour()=="<CHAINE>") {
		nd = ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
	} else{
		nd = expression();
	}
	sauterSymCour(")");
	Symbole symb("ecrire");
	return new NoeudInstES(symb, nd);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instPour() {
	//	  <instPour> ::= pour ( <affectation>, <expBool> , <affectation>) <seqInst> finpour

	sauterSymCour("pour");
	sauterSymCour("(");
	Noeud* affect = affectation();
	sauterSymCour(";");
	Noeud* cond = expBool();
	sauterSymCour(";");
	Noeud* iter = affectation();
	sauterSymCour(")");
	Noeud* seq = seqInst();
	sauterSymCour("finpour");

	return new NoeudInstPour(cond, seq, affect, iter);
}

////////////////////////////////////////////////////////////////////////////////
Noeud* LecteurPhraseAvecArbre::instSelonCas() {
	// <instSelonCas> ::= seloncas (<variable>) { cas <entier> : <seqInst> } [defaut : <seqInst>] finselon


	sauterSymCour("seloncas");
	sauterSymCour("(");
	testerSymCour("<VARIABLE>");
	Noeud* variable = ts.chercheAjoute(ls.getSymCour());
	NoeudInstSelon* nd = new NoeudInstSelon(variable);
	ls.suivant();
	sauterSymCour(")");
	while (ls.getSymCour() == "cas") {
		ls.suivant();
		testerSymCour("<ENTIER>");
		Noeud* symb = ts.chercheAjoute(ls.getSymCour());
		ls.suivant();
		sauterSymCour(":");
		nd->ajouteCas(symb, seqInst());
	}
	if (ls.getSymCour() == "defaut") {
		ls.suivant();
		sauterSymCour(":");
		nd->ajouteCas(variable, seqInst());
	}
	sauterSymCour("finselon");

	return nd;
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::testerSymCour(string ch) {
	if (ls.getSymCour() != ch) {
		cout << endl << "-------- Erreur ligne " << ls.getLigne()
				<< " - Colonne " << ls.getColonne() << endl << "   Attendu : "
				<< ch << endl << "   Trouve  : " << ls.getSymCour() << endl
				<< endl;
		exit(0); // plus tard, on levera une exception
	}
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::sauterSymCour(string ch) {
	testerSymCour(ch);
	ls.suivant();
}

////////////////////////////////////////////////////////////////////////////////
void LecteurPhraseAvecArbre::erreur(string mess) {
	cout << endl << "-------- Erreur ligne " << ls.getLigne() << " - Colonne "
			<< ls.getColonne() << endl << "   Attendu : " << mess << endl
			<< "   Trouve  : " << ls.getSymCour() << endl << endl;
	exit(0); // plus tard, on levera une exception
}
