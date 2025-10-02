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
    : QMainWindow(parent),
      defaultText(QStringLiteral("HELLO WORLD!"))
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

    clockCheckBox = new QCheckBox("Show Clock", this);
    controlsLayout->addWidget(clockCheckBox);

    scrollCheckBox = new QCheckBox("Scroll", this);
    controlsLayout->addWidget(scrollCheckBox);

    mainLayout->addLayout(controlsLayout);

    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateMatrixText);
    connect(textInput, &QLineEdit::returnPressed, this, &MainWindow::updateMatrixText);
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);
    connect(clockCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleClock);
    connect(scrollCheckBox, &QCheckBox::toggled, matrixDisplay, &MatrixDisplay::setScrollEnabled);

    setWindowTitle("Matrix Display");
    resize(900, 220);

    matrixDisplay->setText(defaultText);
    textInput->setText(defaultText);
    scrollCheckBox->setChecked(matrixDisplay->requiresScrolling());
}

MainWindow::~MainWindow() {}

/**
 * @brief Met à jour le texte affiché sur la matrice LED
 */
void MainWindow::updateMatrixText()
{
    matrixDisplay->setText(textInput->text());
    bool needsScroll = matrixDisplay->requiresScrolling();
    if (scrollCheckBox->isChecked() != needsScroll) {
        scrollCheckBox->setChecked(needsScroll);
    }
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
 * @brief Active ou désactive le mode horloge sur la matrice LED.
 *        Force la désactivation du défilement en mode horloge et restaure
 *        un texte par défaut lors du retour en mode texte si aucun contenu n'a été saisi.
 */
void MainWindow::toggleClock(bool checked)
{
    if (checked) {
        scrollCheckBox->setChecked(false);
        matrixDisplay->setScrollEnabled(false);
        matrixDisplay->setDisplayMode(MatrixDisplay::Clock);
        textInput->setEnabled(false);
    } else {
        matrixDisplay->setDisplayMode(MatrixDisplay::Text);
        if (textInput->text().trimmed().isEmpty()) {
            textInput->setText(defaultText);
        }
        matrixDisplay->setText(textInput->text());
        bool needsScroll = matrixDisplay->requiresScrolling();
        if (scrollCheckBox->isChecked() != needsScroll) {
            scrollCheckBox->setChecked(needsScroll);
        }
        textInput->setEnabled(true);
    }
}