/**
 * @file mainwindow.cpp
 * @author G. Maxime
 * @brief Implémentation de la classe MainWindow pour l'interface principale de l'application.
 * Cette classe hérite de QMainWindow et gère l'interface utilisateur principale,
 * y compris l'affichage de la matrice LED, la saisie de texte, la sélection de couleur
 * et le contrôle du mode horloge.
 */

// Inclusion de mes headers
#include "headers/mainwindow.h"
#include "headers/matrixdisplay.h"

// Inclusion des headers Qt nécessaires
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QCheckBox>
#include <QColorDialog>
#include <QShortcut>
#include <QKeySequence>
#include <QLabel>

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

    controlsWidget = new QWidget(this);

    QHBoxLayout *controlsLayout = new QHBoxLayout(controlsWidget);

    textInput = new QLineEdit(this);
    textInput->setPlaceholderText("Enter text to display");
    controlsLayout->addWidget(textInput);

    updateButton = new QPushButton("Update Text", this);
    controlsLayout->addWidget(updateButton);

    QPushButton *colorButton = new QPushButton("Change Pixel Color", this);
    controlsLayout->addWidget(colorButton);

    QPushButton *colorButton_Background = new QPushButton("Change Background Color", this);
    controlsLayout->addWidget(colorButton_Background);

    clockCheckBox = new QCheckBox("Show Clock", this);
    controlsLayout->addWidget(clockCheckBox);

    scrollCheckBox = new QCheckBox("Scroll", this);
    controlsLayout->addWidget(scrollCheckBox);

    bounceCheckBox = new QCheckBox("Bounce Scroll", this);
    controlsLayout->addWidget(bounceCheckBox);

    speedLabel = new QLabel("Speed:", this);
    controlsLayout->addWidget(speedLabel);
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 5);
    speedSlider->setValue(2);
    speedSlider->setSingleStep(1);
    speedSlider->setToolTip("Adjust Scroll Speed");
    connect(speedSlider, &QSlider::valueChanged, [this](int value) {
        if (matrixDisplay) {
            matrixDisplay->setScrollInterval(value);
        }
    });

    controlsLayout->addWidget(speedSlider);
    
    mainLayout->addWidget(controlsWidget);

    // Ajout du raccourci clavier Ctrl+H pour afficher/masquer les contrôles
    auto *toggleShortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+H")), this);
    toggleShortcut->setContext(Qt::ApplicationShortcut);
    connect(toggleShortcut, &QShortcut::activated, this, &MainWindow::toggleControlsVisibility);

    // Connexions des signaux du controllLayout aux slots 
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateMatrixText);
    connect(textInput, &QLineEdit::returnPressed, this, &MainWindow::updateMatrixText);
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::openColorPicker);
    connect(colorButton_Background, &QPushButton::clicked, this, &MainWindow::openColorPicker_Background);
    connect(clockCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleClock);
    connect(scrollCheckBox, &QCheckBox::toggled, matrixDisplay, &MatrixDisplay::setScrollEnabled);
    connect(scrollCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleSpeedControlVisibility);
    connect(scrollCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleBounceCheckboxVisibility);
    connect(bounceCheckBox, &QCheckBox::toggled, [this](bool checked){
        if (matrixDisplay) {
            if (checked) {
                matrixDisplay->setScrollMode(MatrixDisplay::bounceMode);
            } else {
                matrixDisplay->setScrollMode(MatrixDisplay::defaultMode);
            }
        }
    });


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
 * @brief Ouvre une boîte de dialogue pour sélectionner la couleur du fond
 */
void MainWindow::openColorPicker_Background()
{
    QColor color = QColorDialog::getColor(Qt::darkGray, this, "Choose Background Color");
    if (color.isValid()) {
        matrixDisplay->setColor_Background(color);
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
        scrollCheckBox->setVisible(false);
        matrixDisplay->setScrollEnabled(false);
        matrixDisplay->setDisplayMode(MatrixDisplay::Clock);
        textInput->setEnabled(false);
        textInput->setVisible(false);

        this->toggleSpeedControlVisibility();
        this->toggleBounceCheckboxVisibility();
        this->updateButton->setVisible(false);
    } else {
        scrollCheckBox->setVisible(true);
        textInput->setVisible(true);
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

        this->updateButton->setVisible(true);
        this->toggleSpeedControlVisibility();
        this->toggleBounceCheckboxVisibility();
    }
}

void MainWindow::toggleControlsVisibility()
{
    if (!controlsWidget) return;
    controlsWidget->setVisible(!controlsWidget->isVisible());
}

void MainWindow::toggleSpeedControlVisibility()
{
    if (!speedSlider) return;
    speedSlider->setVisible(scrollCheckBox->isChecked());
    speedLabel->setVisible(scrollCheckBox->isChecked());
}

void MainWindow::toggleBounceCheckboxVisibility()
{
    if (scrollCheckBox->isChecked())
        bounceCheckBox->setVisible(true);
    else
        bounceCheckBox->setVisible(false);
}