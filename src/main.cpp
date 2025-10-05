/**
 * @file main.cpp
 * @author G. Maxime
 * @date 2025-10-05
 * @mainpage
 * @brief Projet S9 - Programmation des interfaces graphiques et interactives
 * 
 * Sujet : Affichage sur une matrice LED
 * 
 * L'objectif de ce projet est de développer une application Qt permettant d'afficher du texte
 * ou l'heure actuelle sur une matrice LED simulée. L'application doit offrir une interface
 * utilisateur intuitive pour la saisie de texte, la sélection de la couleur des LED, et
 * le choix entre l'affichage du texte ou de l'horloge. De plus, l'application doit gérer
 * le défilement du texte si celui-ci dépasse la largeur de la matrice.
*/
 
#include "headers/mainwindow.h"

#include <QApplication>

/**
 * @brief Fonction principale de l'application
 * @param argc Nombre d'arguments
 * @param argv Tableau des arguments
 * @return Code de retour de l'application
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}