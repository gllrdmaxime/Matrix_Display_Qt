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

class MatrixDisplay;
class QLineEdit;
class QColorDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateMatrixText();
    void openColorPicker();
    void toggleClock(bool checked);

private:
    MatrixDisplay *matrixDisplay;
    QLineEdit *textInput;
    QColorDialog *colorDialog;
};
#endif // MAINWINDOW_H