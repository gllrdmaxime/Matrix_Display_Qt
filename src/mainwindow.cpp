/**
 * @file mainwindow.cpp
 * @brief 
 */
#include "headers/mainwindow.h"
#include "headers/matrixdisplay.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QCheckBox>
#include <QColorDialog>

/**
 * @brief Constructeur de la classe MainWindow
 * @param parent Pointeur vers le widget parent (par défaut nullptr)
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    matrixDisplay = new MatrixDisplay(this);
    mainLayout->addWidget(matrixDisplay);

    QHBoxLayout *controlsLayout = new QHBoxLayout();

    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text to display");
    controlsLayout->addWidget(textInput);

    QPushButton *updateButton = new QPushButton("Update Text", this);
    controlsLayout->addWidget(updateButton);

    QPushButton *colorButton = new QPushButton("Change Color", this);
    controlsLayout->addWidget(colorButton);

    QCheckBox *clockCheckBox = new QCheckBox("Show Clock", this);
    controlsLayout->addWidget(clockCheckBox);

    QCheckBox *scrollCheckBox = new QCheckBox("Scroll", this); // Ajout
    controlsLayout->addWidget(scrollCheckBox);

    mainLayout->addLayout(controlsLayout);

    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateMatrixText);
    connect(textInput, &QLineEdit::returnPressed, this, &MainWindow::updateMatrixText);
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);
    connect(clockCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleClock);
    connect(scrollCheckBox, &QCheckBox::toggled, matrixDisplay, &MatrixDisplay::setScrollEnabled);

    setWindowTitle("Matrix Display");
    resize(900, 220);

    matrixDisplay->setText("HELLO WORLD!");
    scrollCheckBox->setChecked(false); // Activer le scroll par défaut si le texte est plus large que la fenêtre
}

MainWindow::~MainWindow() {}

/**
 * @brief Met à jour le texte affiché sur la matrice LED
 */
void MainWindow::updateMatrixText()
{
    matrixDisplay->setText(textInput->text());
}

/**
 * @brief Ouvre une boîte de dialogue pour sélectionner la couleur des pixels
 */
void MainWindow::openColorPicker()
{
    QColor color = QColorDialog::getColor(Qt::green, this, "Choose Pixel Color");
    if (color.isValid()) {
        matrixDisplay->setColor(color);
    }
}

/**
 * @brief Active ou désactive le mode horloge sur la matrice LED
 * @param checked Vrai si la case est cochée (mode horloge activé), faux sinon
 */
void MainWindow::toggleClock(bool checked)
{
    if (checked) {
        matrixDisplay->setDisplayMode(MatrixDisplay::Clock);
        textInput->setEnabled(false);
    } else {
        matrixDisplay->setDisplayMode(MatrixDisplay::Text);
        matrixDisplay->setText(textInput->text());
        textInput->setEnabled(true);
    }
}