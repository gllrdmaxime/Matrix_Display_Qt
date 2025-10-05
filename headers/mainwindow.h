/**
 * @file mainwindow.h
 * @brief Déclaration de la classe MainWindow pour l'interface principale de l'application.
 * Cette classe hérite de QMainWindow et gère l'interface utilisateur principale,
 * y compris l'affichage de la matrice LED, la saisie de texte, la sélection de couleur
 * et le contrôle du mode horloge.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

class MatrixDisplay;
class QLineEdit;
class QColorDialog;
class QCheckBox;


/**
* @class MainWindow
* @brief Fenêtre principale qui encapsule l'affichage matriciel et ses contrôles.
*/
class MainWindow : public QMainWindow
{
    /**
     * @brief Macro Qt pour la gestion des signaux et slots.
     */
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la fenêtre principale.
     * @param parent Widget parent (par défaut nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la fenêtre principale.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Synchronise la matrice avec le contenu du champ de saisie.
     */
    void updateMatrixText();

    /**
     * @brief Ouvre la boîte de dialogue permettant de choisir la couleur des LED.
     */
    void openColorPicker();
    
    /**
     * @brief Active ou désactive le mode horloge et gère l'état du défilement.
     */
    void toggleClock(bool checked);

private:
    /**
     * @brief Widget affichant la matrice LED.
     */
    MatrixDisplay *matrixDisplay;
    
    /**
     * @brief Champ permettant la saisie du texte utilisateur.
     */
    QLineEdit *textInput;
    
    /**
     * @brief Boîte de dialogue pour la sélection de la couleur des LED.
     */
    QColorDialog *colorDialog;
    
    /**
     * @brief Case à cocher contrôlant le mode horloge.
     */
    QCheckBox *clockCheckBox;
    
    /**
     * @brief Case à cocher contrôlant le défilement du texte.
     */
    QCheckBox *scrollCheckBox;
    
    /**
     * @brief Texte utilisé lorsque l'utilisateur retourne en mode texte sans saisie préalable.
     */
    QString defaultText;
};
#endif // MAINWINDOW_H