#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Inclusion de mes headers
#include <QMainWindow>
#include <QString>
#include <QSlider>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QColorDialog>

// Déclaration anticipée de la classe MatrixDisplay
class MatrixDisplay;

/**
 * @file mainwindow.h
 * @author G. Maxime
 * @class MainWindow
 * @brief Déclaration de la classe MainWindow pour l'interface principale de l'application.
 * Cette classe hérite de QMainWindow et gère l'interface utilisateur principale,
 * y compris l'affichage de la matrice LED, la saisie de texte, la sélection de couleur
 * et le contrôle du mode horloge.
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
     * @brief Ouvre la boîte de dialogue permettant de choisir la couleur du fond.
     */
    void openColorPicker_Background();
    
    /**
     * @brief Active ou désactive le mode horloge et gère l'état du défilement.
     */
    void toggleClock(bool checked);

    /**
     * @brief Affiche ou masque le layout de contrôle (saisie, boutons, etc.).
     */
    void toggleControlsVisibility();

    /**
     * @brief Affiche le control de la vitesse si le scroll est activé
     */
    void toggleSpeedControlVisibility();

    /**
     * @brief Affiche le checkbox de bounce si le scroll est activé
     */
    void toggleBounceCheckboxVisibility();

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
     * @brief Case à cocher contrôlant le mode de défilement (rebond).
     */
    QCheckBox *bounceCheckBox;
    
    /**
     * @brief Texte utilisé lorsque l'utilisateur retourne en mode texte sans saisie préalable.
     */
    QString defaultText;

    /**
     * @brief Ajout d'un widget de controls pour contenir mon layout et pouvoir le faire disparaitres avec un raccourci clavier.
     */
    QWidget *controlsWidget = nullptr;

    /**
     * @brief Label pour indiquer le titre du slider de vitesse.
     */
    QLabel *speedLabel = nullptr;

    /**
     * @brief Slider pour ajuster la vitesse de défilement.
     */
    QSlider *speedSlider = nullptr;

    /**
     * @brief Bouton pour mettre à jour le texte affiché sur la matrice.
     */
    QPushButton *updateButton = nullptr;
};
#endif // MAINWINDOW_H