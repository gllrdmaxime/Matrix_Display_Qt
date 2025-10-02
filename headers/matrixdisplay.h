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

class MatrixDisplay : public QWidget
{
    Q_OBJECT

public:
    enum DisplayMode {
        Text,
        Clock
    };

    explicit MatrixDisplay(QWidget *parent = nullptr);

public slots:
    void setText(const QString &text);
    void setColor(const QColor &color);
    void setDisplayMode(DisplayMode mode);
    void setScrollEnabled(bool enabled);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    QSize sizeHint() const override; // Suggestion de taille pour le widget

private slots:
    void onTimeout();

private:
    void restartScrollIfNeeded(); // Redémarre le défilement si nécessaire
    float calculateCellSize() const;

    QString currentText;
    QColor pixelColor;
    DisplayMode currentMode;
    QTimer *timer;
    int scrollOffset;
    bool scrollEnabled;
};

#endif // MATRIXDISPLAY_H