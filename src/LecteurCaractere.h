#ifndef LECTEURCARACTERE_H_
#define LECTEURCARACTERE_H_

/** un LECTEURCARACTERE est défini par
 * - un caractère courant
 * - une ligne
 * - une colone
 */

#include <fstream>
/*! \file LecteurCaractere.h
 * \author Inconnu
 * \date 18.04.2011
 */
using namespace std;

/*! \class LecteurCaractere
 * \brief La classe lit un caractère
 *
 * Cette classe permet de lire un caractère courant à l'aide d'une composition d'objets (1 objet de type caractère et 2 de type entier)
 */
class LecteurCaractere {

  ////////////////////////////////////////////////////////////////////////////////
  public :

	/*! \fn LecteurCaractere(string nomFich)
	 * \brief Constructeur de la classe LecteurCaractere
	 * Construit le lecteur pour parcourir nomFich
	 * \param nomFich - nom du fichier
	 */
    LecteurCaractere(string nomFich);

    /*! \fn inline char getCarCour()
	 * \brief Accesseur
	 *
	 * Accesseur sans paramètre permettant de retourner le caractère courant
	 * \return la valeur du caractère courant
	 */
    inline char         getCarCour() { return carCour; }

    /*! \fn inline unsigned int getLigne()
   	 * \brief Accesseur
   	 *
   	 * Accesseur sans paramètre permettant de retourner la ligne
   	 * \return la valeur de la ligne
   	 */
    inline unsigned int getLigne()   { return ligne;   }

    /*! \fn inline unsigned int getColonne()
	 * \brief Accesseur
	 *
	 * Accesseur sans paramètre permettant de retourner la colonne
	 * \return la valeur de la colonne
	 */
    inline unsigned int getColonne() { return colonne; }

    /*! \fn void suivant()
	 * \brief Procédure
	 *
	 * Procédure qui passe au caractère suivant, s'il existe
	 */
    void suivant();

  ////////////////////////////////////////////////////////////////////////////////
  private :

    /*! \var ifstream f
     * \brief le fichier texte que l'on parcourt
     */
    ifstream f;

    /*! \var char carCour
     * \brief le caractere courant
     */
    char carCour;

    /*! \var unsigned int ligne
     * \brief numéro de ligne du caractère courant
     */
    unsigned int ligne;

    /*! \var unsigned int colone
     * \brief numéro de colonne du caractère courant
     */
    unsigned int colonne;
};

#endif /* LECTEURCARACTERE_H_ */
