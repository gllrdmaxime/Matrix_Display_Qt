#include "headers/matrixdisplay.h"
#include "headers/matrixfont.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <cmath>

// Ratio maximal de hauteur occupée par la matrice (ajuste si tu veux plus grand/petit)
static constexpr float kMaxHeightUsage = 0.75f;
// Taille minimale pour garder un rendu correct
static constexpr float kMinCellSize = 2.0f;
// Taille maximale optionnelle (0 => pas de limite explicite)
static constexpr float kMaxCellSize = 28.0f;

/**
 * @brief Constructeur de la classe MatrixDisplay
 * @param parent Pointeur vers le widget parent (par défaut nullptr)
 */
MatrixDisplay::MatrixDisplay(QWidget *parent)
    : QWidget(parent),
      pixelColor(0, 255, 0),
      currentMode(Text),
      scrollOffset(0),
      scrollEnabled(false)
{
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAutoFillBackground(false);
    setMinimumHeight(120);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MatrixDisplay::onTimeout);
}

QSize MatrixDisplay::sizeHint() const
{
    return {800, 140};
}

void MatrixDisplay::setText(const QString &text)
{
    currentText = text.toUpper();
    scrollOffset = 0;
    if (currentMode == Text) {
        restartScrollIfNeeded();
    }
    update();
}

void MatrixDisplay::setColor(const QColor &color)
{
    pixelColor = color;
    update();
}

void MatrixDisplay::setDisplayMode(DisplayMode mode)
{
    currentMode = mode;
    timer->stop();

    if (currentMode == Clock) {
        onTimeout();
        timer->start(1000);
    } else {
        restartScrollIfNeeded();
    }
    update();
}

void MatrixDisplay::setScrollEnabled(bool enabled)
{
    scrollEnabled = enabled;
    restartScrollIfNeeded();
    update();
}

/**
 * @brief Calcule une taille de cellule:
 * - Limite l'occupation verticale (kMaxHeightUsage).
 * - Essaie d'abord de faire tenir tout le texte en largeur.
 * - Si le texte ne tient pas et que le scroll est activé, on prend la taille dictée par la hauteur (limitée).
 * - Garde des cellules carrées (garantie de cercles).
 * - Aligne sur la grille des pixels (DPR) pour éviter les ellipses visuelles.
 */
float MatrixDisplay::calculateCellSize() const
{
    if (CHAR_ROWS <= 0)
        return kMinCellSize;

    int w = width();
    int h = height();
    if (w <= 0 || h <= 0)
        return kMinCellSize;

    // Hauteur réellement disponible (on n'utilise pas 100%)
    float usableHeight = h * kMaxHeightUsage;
    float sizeByHeight = usableHeight / static_cast<float>(CHAR_ROWS);

    int numChars = currentText.length();
    if (numChars <= 0) {
        float s = std::max(kMinCellSize, sizeByHeight);
        if (kMaxCellSize > 0.0f)
            s = std::min(s, kMaxCellSize);
        float dpr = devicePixelRatioF();
        if (dpr > 0.0f) s = std::floor(s * dpr) / dpr;
        return s;
    }

    constexpr float charCols = CHAR_COLS;
    constexpr float spacingCols = 1.0f;
    float totalCols = numChars * charCols + (numChars > 1 ? (numChars - 1) * spacingCols : 0.0f);

    float sizeByWidth = (totalCols > 0.0f) ? (static_cast<float>(w) / totalCols) : sizeByHeight;

    // Largeur projetée si on utilisait la taille maximale par la hauteur
    float projectedWidthWithHeight = totalCols * sizeByHeight;

    bool needsScrollIfHeight = projectedWidthWithHeight > w;
    bool allowScroll = scrollEnabled && needsScrollIfHeight;

    float chosen;
    if (allowScroll) {
        // On accepte le scroll: on privilégie la lisibilité verticale (mais limitée)
        chosen = sizeByHeight;
    } else {
        // On n'utilise pas toute la hauteur si la largeur peut tout contenir
        chosen = std::min(sizeByHeight, sizeByWidth);
    }

    // Bornes min / max
    if (chosen < kMinCellSize)
        chosen = kMinCellSize;
    if (kMaxCellSize > 0.0f && chosen > kMaxCellSize)
        chosen = kMaxCellSize;

    // Alignement sur la grille physique
    float dpr = devicePixelRatioF();
    if (dpr > 0.0f) {
        chosen = std::floor(chosen * dpr) / dpr;
    }

    return chosen;
}

void MatrixDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (currentMode == Text) {
        restartScrollIfNeeded();
    }
    update();
}


void MatrixDisplay::restartScrollIfNeeded()
{
    timer->stop();
    if (currentMode != Text || !scrollEnabled)
        return;

    const float cellSize = calculateCellSize();
    const float charWidthInCols = CHAR_COLS;
    const float spacingInCols = 1.0f;
    int numChars = currentText.length();
    if (numChars == 0)
        return;

    float totalTextWidthInCols = numChars * charWidthInCols
        + (numChars > 1 ? (numChars - 1) * spacingInCols : 0);
    float totalTextWidthInPixels = totalTextWidthInCols * cellSize;

    if (totalTextWidthInPixels > width()) {
        timer->start(50);
    } else {
        scrollOffset = 0;
    }
}

void MatrixDisplay::onTimeout()
{
    if (currentMode == Clock) {
        QString timeText = QTime::currentTime().toString("HH:mm");
        if (QTime::currentTime().second() % 2 == 0) {
            timeText[2] = ' ';
        }
        if (currentText != timeText) {
            currentText = timeText;
            update();
        }
    } else if (currentMode == Text && scrollEnabled) {
        const float cellSize = calculateCellSize();
        const float charWidthInCols = CHAR_COLS;
        const float spacingInCols = 1.0f;
        int numChars = currentText.length();
        float totalTextWidthInCols = numChars * charWidthInCols
            + (numChars > 1 ? (numChars - 1) * spacingInCols : 0);
        float totalTextWidthInPixels = totalTextWidthInCols * cellSize;

        if (totalTextWidthInPixels > width()) {
            scrollOffset = (scrollOffset + 2) % (int)(totalTextWidthInPixels + width());
            update();
        } else {
            timer->stop();
        }
    }
}

void MatrixDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), Qt::black);

    if (currentText.isEmpty()) {
        return;
    }

    const float cellSize = calculateCellSize();
    const float pixelDiameter = cellSize * 0.8f;
    const float pixelOffset = (cellSize - pixelDiameter) / 2.0f;
    const float charWidthInCols = CHAR_COLS;
    const float spacingInCols = 1.0f;

    int numChars = currentText.length();
    float totalTextWidthInCols = numChars * charWidthInCols
        + (numChars > 1 ? (numChars - 1) * spacingInCols : 0);
    float totalTextWidthInPixels = totalTextWidthInCols * cellSize;

    bool isScrolling = (currentMode == Text) && scrollEnabled && (totalTextWidthInPixels > width());
    float xOffset = isScrolling
                        ? (width() - scrollOffset)
                        : (width() - totalTextWidthInPixels) / 2.0f;

    // Centre verticalement la zone réelle (n'utilise pas toute la hauteur)
    float matrixHeight = CHAR_ROWS * cellSize;
    float yOffset = (height() - matrixHeight) / 2.0f;

    painter.setBrush(pixelColor);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < numChars; ++i) {
        char c = currentText[i].toLatin1();
        if (!matrix_font.count(c)) {
            continue;
        }
        const auto &charMap = matrix_font.at(c);
        float charStartX = i * (charWidthInCols + spacingInCols) * cellSize;

        for (int row = 0; row < CHAR_ROWS; ++row) {
            for (int col = 0; col < CHAR_COLS; ++col) {
                if (col < charMap[row].length() && charMap[row][col] == '1') {
                    float x = xOffset + charStartX + col * cellSize + pixelOffset;
                    float y = yOffset + row * cellSize + pixelOffset;
                    if (x + pixelDiameter < 0 || x > width())
                        continue;
                    painter.drawEllipse(QRectF(x, y, pixelDiameter, pixelDiameter));
                }
            }
        }
    }
}