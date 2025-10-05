/**
 * @file matrixdisplay.h
 * @brief Déclaration de la classe MatrixDisplay pour l'affichage sur une matrice LED.
 * Cette classe hérite de QWidget et permet d'afficher du texte ou l'heure actuelle
 * sur une matrice LED simulée. Elle gère également le défilement du texte.
 * La couleur des pixels, le mode d'affichage (texte ou horloge) et
 * l'activation du défilement peuvent être configurés.
 */

#ifndef MATRIXDISPLAY_H
#define MATRIXDISPLAY_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QTimer>

/**
 * @class MatrixDisplay
 * @file matrixdisplay.h
 * @brief Déclaration de la classe MatrixDisplay pour l'affichage sur une matrice LED.
 * Cette classe hérite de QWidget et permet d'afficher du texte ou l'heure actuelle
 * sur une matrice LED simulée. Elle gère également le défilement du texte.
 * La couleur des pixels, le mode d'affichage (texte ou horloge) et
 * l'activation du défilement peuvent être configurés.
 *
 * La matrice virtuelle possède une grille fixe, reproduisant le comportement
 * d'un véritable panneau LED.
 */
class MatrixDisplay : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Modes d'affichage disponibles pour la matrice LED.
     */
    enum DisplayMode {
        Text,
        Clock
    };

    /**
     * @brief Constructeur de la matrice LED.
     */
    explicit MatrixDisplay(QWidget *parent = nullptr);

    /**
     * @brief Indique si le texte courant dépasse la largeur affichable.
     */
    bool requiresScrolling() const;

public slots:
    /**
     * @brief Met à jour le texte affiché sur la matrice.
     * @param text Chaîne à représenter (convertie en majuscules).
     */
    void setText(const QString &text);
    
    /**
     * @brief Modifie la couleur des pixels affichés.
     * @param color Couleur utilisée pour dessiner les LED.
     */
    void setColor(const QColor &color);
    
    /**
     * @brief Bascule entre les modes texte et horloge.
     * @param mode Mode souhaité.
     */
    void setDisplayMode(DisplayMode mode);

    /**
     * @brief Active ou désactive le défilement du texte en mode texte.
     * @param enabled Vrai pour activer le scroll, faux sinon.
     */
    void setScrollEnabled(bool enabled);

protected:
    /** 
     * @brief Dessine la matrice LED et le contenu (texte ou horloge).
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Gère le redimensionnement du widget et ajuste le défilement si nécessaire.
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Fournit une suggestion de taille pour le widget.
     * @return Taille suggérée en pixels.
     */
    QSize sizeHint() const override;

private slots:
    /**
     * @brief Gestion des mises à jour temporisées (horloge ou scroll).
     */
    void onTimeout();

private:
    /**
     * @brief Relance le timer de défilement si les conditions sont réunies.
     */
    void restartScrollIfNeeded();
    
    /**
     * @brief Calcule la taille d'une cellule en respectant la grille fixe.
     * @return Taille d'une cellule en pixels.
     */
    float calculateCellSize() const;

    /**
     * @brief Texte affiché sur la matrice (toujours en majuscules).
     */
    QString currentText;

    /**
     * @brief Couleur des pixels allumés.
     */
    QColor pixelColor;

    /**
     * @brief Mode d'affichage actuel (texte ou horloge).
     */
    DisplayMode currentMode;

    /**
     * @brief timeur pour gérer les mises à jour périodiques (scroll ou horloge).
     */
    QTimer *timer;

    /**
     * @brief Décalage horizontal courant utilisé pendant le défilement.
     */
    int scrollOffset;

    /**
     * @brief Indique si le défilement est autorisé en mode texte.
     */
    bool scrollEnabled;
};
#endif // MATRIXDISPLAY_H