#include <stdlib.h>
#include "Arbre.h"
#include "Symbole.h"
#include "SymboleValue.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : tabInst() {
}

////////////////////////////////////////////////////////////////////////////////
int NoeudSeqInst::getValeur() {
  int valeur=0;
  for (unsigned int i=0; i<tabInst.size(); i++)
    valeur = tabInst[i]->getValeur();  // on evalue chaque instruction de la séquence
  return valeur; // par convention, resultat = valeur de la derniere instruction
}

////////////////////////////////////////////////////////////////////////////////
void NoeudSeqInst::afficher(unsigned short indentation) {
  Noeud::afficher(indentation);
  cout << "Noeud - Sequence de " << tabInst.size() << " instruction(s)" << endl;
  for (unsigned int i=0; i<tabInst.size(); i++)
    tabInst[i]->afficher(indentation+1); // on affiche les fils en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
void NoeudSeqInst::ajouteInstruction(Noeud* instruction) {
  tabInst.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
void NoeudSeqInst::genererCpp(unsigned short indentation) {
	for (unsigned int i=0; i<tabInst.size(); i++){
		tabInst[i]->genererCpp(indentation+1);
		cout << endl;
	}
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression) {
  this->variable=variable;
  this->expression=expression;
}

////////////////////////////////////////////////////////////////////////////////
int NoeudAffectation::getValeur() {
  int valeur = expression->getValeur(); // on évalue l'expression
  if(valeur==NULL){
	  string chaine = ((SymboleValue*)expression)->getChaine();
	  ((SymboleValue*)variable)->setValeur(chaine);
	  valeur==1;
  }
  else
	  ((SymboleValue*)variable)->setValeur(valeur); // on affecte la variable
  return valeur; // par convention, une affectation a pour valeur la valeur affectée
}

////////////////////////////////////////////////////////////////////////////////
void NoeudAffectation::afficher(unsigned short indentation) {
  Noeud::afficher(indentation);
  cout << "Noeud - Affectation" << endl;
  variable->afficher(indentation+1);   // on affiche variable et expression
  expression->afficher(indentation+1); // en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
void NoeudAffectation::genererCpp(unsigned short indentation) {
	Noeud::genererCpp(indentation);
	variable->genererCpp(); 
	cout << " = ";
	expression->genererCpp();
	cout << ";";
}


////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire ( Symbole operateur,
                                               Noeud* operandeGauche,
                                               Noeud* operandeDroit) {
  this->operateur=operateur;
  this->operandeGauche=operandeGauche;
  this->operandeDroit=operandeDroit;
}

////////////////////////////////////////////////////////////////////////////////
int NoeudOperateurBinaire::getValeur() { 
  int valeur=0;
  //non
  int og=operandeGauche->getValeur();
  int od=operandeDroit->getValeur();
  if      (this->operateur=="+") valeur = og+od; 
  else if (this->operateur=="-") valeur = og-od;
  else if (this->operateur=="*") valeur = og*od; 
  else if (this->operateur=="ou") valeur = (og || od);
  else if (this->operateur=="et") valeur = (og && od);
  else if (this->operateur=="==") valeur = (og == od);
  else if (this->operateur=="!=") valeur = (og != od);
  else if (this->operateur=="<") valeur = (og < od);
  else if (this->operateur=="<=") valeur = (og <= od);
  else if (this->operateur==">=") valeur = (og >= od);
  else if (this->operateur==">") valeur = (og > od);
  else if (this->operateur=="non") valeur = (!od);
  else  // this->operateur=="/"
	if (od!=0)
		valeur = og/od;
  	else {
	  	cout << "Erreur pendant l'interpretation : division par zero" << endl;
	  	exit(0); // plus tard on levera une exception
  	}
  return valeur;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudOperateurBinaire::afficher(unsigned short indentation) {
  Noeud::afficher(indentation);
  cout << "Noeud - Operateur Binaire \"" << this->operateur.getChaine() << "\" applique a : " << endl;
  operandeGauche->afficher(indentation+1);  // on affiche fils gauche et fils droit
  operandeDroit->afficher(indentation+1);   // en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
void NoeudOperateurBinaire::genererCpp(unsigned short indentation) {
	operandeGauche->genererCpp(indentation);
	cout	<<  this->operateur.getChaine();
	operandeDroit->genererCpp();
	cout << ";";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstCond
////////////////////////////////////////////////////////////////////////////////

NoeudInstCond::NoeudInstCond(Noeud* cond, Noeud* seq) {
	this->cond=cond;
	this->seq=seq;
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstTq
////////////////////////////////////////////////////////////////////////////////

NoeudInstTq::NoeudInstTq(Noeud* cond, Noeud* seq) : NoeudInstCond(cond, seq) {
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstTq::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - Tant que (";
	cond->afficher(indentation);
	cout << ") faire : " << endl;
	seq->afficher(indentation+1);   // en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
int NoeudInstTq::getValeur() {
	int val=0;

	while(this->cond->getValeur()== 1){
		val=this->seq->getValeur();
	}

	return val;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstTq::genererCpp(unsigned short indentation) {
	Noeud::genererCpp(indentation);
	cout << "while ( " ;
	cond->genererCpp();
	cout << " ) { " << endl;
	seq->genererCpp(indentation +1);
	Noeud::genererCpp(indentation);
	cout << "} ";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstRepeter
////////////////////////////////////////////////////////////////////////////////

NoeudInstRepeter::NoeudInstRepeter(Noeud* cond, Noeud* seq) : NoeudInstCond(cond, seq) {
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstRepeter::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - Faire : " << endl;
	seq->afficher(indentation+1);   // en augmentant l'indentation
	cout << "Tant que (";
	cond->afficher(indentation);
	cout << ")" << endl;
}

////////////////////////////////////////////////////////////////////////////////
int NoeudInstRepeter::getValeur() {
	int val=0;

	do{
		val=this->seq->getValeur();
	}while(this->cond->getValeur()== 1);

	return val;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstRepeter::genererCpp(unsigned short indentation) {
	Noeud::genererCpp(indentation);
	cout << "do { " << endl;
	seq->genererCpp(indentation +1);
	Noeud::genererCpp(indentation);
	cout << "} while ( ";
	cond->genererCpp();
	cout << " ); ";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstPour
////////////////////////////////////////////////////////////////////////////////

NoeudInstPour::NoeudInstPour(Noeud* cond, Noeud* seq, Noeud* affect, Noeud* iter) : NoeudInstCond(cond, seq) {
	this->affect=affect;
	this->iter=iter;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstPour::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	  cout << "Noeud - Pour. Condition initiale : " << endl;
	  affect->afficher(indentation+1);   // en augmentant l'indentation
	  cout << setw((4*indentation)+2) << " ";
	  cout << "Condition d'arret : "<< endl;
	  cond->afficher(indentation+1);
	  cout << setw((4*indentation)+2) << " ";
	  cout << "Iteration : " << endl;
	  iter->afficher(indentation+1);
	  cout << setw((4*indentation)+2) << " ";
	  cout << "Sequence : " << endl;
	  seq->afficher(indentation+2);
}

////////////////////////////////////////////////////////////////////////////////
int NoeudInstPour::getValeur() {
	int val=0, i;

	for(i=affect->getValeur();cond->getValeur()==1;i=iter->getValeur()){
		val=seq->getValeur();
	}

	return val;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstPour::genererCpp(unsigned short indentation) {
	Noeud::genererCpp(indentation);
	cout << "for( ";
	affect->genererCpp();
	cout << "; ";
	cond->genererCpp();
	cout << "; ";
	iter->genererCpp();
	cout << ") {" << endl;
	seq->genererCpp(indentation +1);
	Noeud::genererCpp(indentation);
	cout << "} ";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* cond, Noeud* seq) : NoeudInstCond(cond, seq), tabS() {
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstSi::afficher(unsigned short indentation) {
	int i = 0;
	Noeud::afficher(indentation);
	cout << "Noeud - Si (";
	cond->afficher(indentation);
	cout << ") faire : " << endl;
	seq->afficher(indentation+1);   // en augmentant l'indentation
	while(i<tabS.size()){
		cout << "Sinon";
		(tabS[i]->cond)->afficher();
		i++;
	}//fin while
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstSi::ajouteSinonSi(Noeud* cond, Noeud* seq){
	NoeudInstSi* nd = new NoeudInstSi(cond, seq);
	tabS.push_back(nd);
}

////////////////////////////////////////////////////////////////////////////////
int NoeudInstSi::getValeur(){
	int val=0;
	int test=0;
	unsigned int i=0;

	if(cond->getValeur()== 1){
		val=seq->getValeur();
	}
	else{
		while(test==0 && i<tabS.size()){
			if( ((tabS[i]->cond)->getValeur())==1){
				val=(tabS[i]->seq)->getValeur();
				test = 1;
			}
			i++;
		}//fin while
	}//fin else

	return val;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstSi::genererCpp(unsigned short indentation) {
	int i=0;
	Noeud::genererCpp(indentation);
	cout << "if (";
	cond->genererCpp();
	cout << ") {" << endl;
	seq->genererCpp(indentation +1);
	Noeud::genererCpp(indentation);
	cout << "} " << endl;
	while(i<(tabS.size()-1)){
		Noeud::genererCpp(indentation);
		cout << "else if (";
		(tabS[i]->cond)->genererCpp();
		cout << ") {" << endl;
		(tabS[i]->seq)->genererCpp(indentation+1);
		i++;
		cout << "} " << endl;
	}//fin while
	Noeud::genererCpp(indentation);
	if (  (tabS[i]->cond)->getValeur() != 1 ){
		cout << "else if (";
		(tabS[i]->cond)->genererCpp();
		cout << ") {" << endl;
	}
	else{
		cout << "else {" << endl;
	}
	(tabS[i]->seq)->genererCpp(indentation+1);
	Noeud::genererCpp(indentation);
	cout << "}";
}
////////////////////////////////////////////////////////////////////////////////
// NoeudInstES
////////////////////////////////////////////////////////////////////////////////

NoeudInstES::NoeudInstES(Symbole action, Noeud* var) : action(action), var(var){
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstES::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - " << action << endl;
	var->afficher(indentation+1);
}


////////////////////////////////////////////////////////////////////////////////
int NoeudInstES::getValeur(){
	string val = "";
	if(action=="lire"){
		cout << "Entrez une valeur : " ;
		cin >> val;
		 Symbole symb(val);
		 if(symb=="<ENTIER>"){
			int nb = atoi(val.c_str());
			((SymboleValue* )var)->setValeur(nb);
		}
		else {
			 ((SymboleValue* )var)->setValeur("\"" + val + "\"");
		}
	}
	else{
		if(var->getValeur() == NULL){
			string s = ((SymboleValue *)var)->getContenu();
			cout << s.substr(1,s.size()-2) << endl;
		}
		else{
			cout << var->getValeur() << endl;
		}


	}
	return var->getValeur();
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstES::genererCpp(unsigned short indentation) {
	Noeud::genererCpp(indentation);
	if(action=="lire"){
		cout << "cout << \"Entrez une valeur : \" ;" << endl;
		cout << "cin >> ";
		var->genererCpp();
	}
	else{
		cout << "cout << ";
		var->genererCpp();
		cout << " << endl;";
	}
}
////////////////////////////////////////////////////////////////////////////////
// NoeudInstSelon
////////////////////////////////////////////////////////////////////////////////

NoeudInstSelon::NoeudInstSelon(Noeud* uneVar) {

	this->var = (SymboleValue *)uneVar;
}

////////////////////////////////////////////////////////////////////////////////
int NoeudInstSelon::getValeur(){
	int val=0;
	unsigned int i=0;
	while(val==0 && i<tabS.size()) {

		val = tabS[i]->getValeur(var->getValeur());
		i++;
	} // Fin while

	return val;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstSelon::afficher(unsigned short indentation) {
	int i = 0;
	Noeud::afficher(indentation);
	cout << "Noeud - SelonCas (";
	var->afficher(indentation);
	cout << ")"<< endl;
	while(i<tabS.size()){
		tabS[i]->afficher(indentation+1);
		i++;
	}//fin while

}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstSelon::ajouteCas(Noeud* var, Noeud* seq){
	NoeudInstCas* nd = new NoeudInstCas(var, seq);
	tabS.push_back(nd);
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstSelon::genererCpp(unsigned short indentation) {
	int i = 0;
	Noeud::genererCpp(indentation);
	cout << "switch (";
	var->genererCpp(); 
	cout << ") {" << endl;
	while(i<tabS.size()){
		tabS[i]->genererCpp(indentation +1);
		i++;
	}//fin while
	Noeud::genererCpp(indentation);
	cout << "}";
}
////////////////////////////////////////////////////////////////////////////////
// NoeudInstCas
////////////////////////////////////////////////////////////////////////////////

NoeudInstCas::NoeudInstCas(Noeud* var, Noeud* seq) : var(var), seq(seq) {
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstCas::afficher(unsigned short indentation) {
	Noeud::afficher(indentation);
	cout << "Noeud - Cas ";
	var->afficher(indentation);
	cout << " faire : " << endl;
	seq->afficher(indentation+1);   // en augmentant l'indentation
}

////////////////////////////////////////////////////////////////////////////////
int NoeudInstCas::getValeur(int val) {
	int valeur = 0;
	if (var->getValeur() == val) {
		seq->getValeur();
		valeur = 1;
	}

	return valeur;
}

////////////////////////////////////////////////////////////////////////////////
void NoeudInstCas::genererCpp(unsigned short indentation) {
	if(var->getValeur() != NULL){
		Noeud::genererCpp(indentation);
		cout << "case ";
		var->genererCpp();
		cout << " : " << endl;
	}
	else {
		Noeud::genererCpp(indentation);
		cout << "default : " << endl;
		}
		seq->genererCpp(indentation+1);
		Noeud::genererCpp(indentation+2);
		cout << "break;" << endl;
}
