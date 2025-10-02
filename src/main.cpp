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