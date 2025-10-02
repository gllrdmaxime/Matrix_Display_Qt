#include "headers/matrixdisplay.h"
#include "headers/matrixfont.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <QSizePolicy>
#include <cmath>
#include <algorithm>

static constexpr float kMaxHeightUsage = 0.95f;
static constexpr float kMinCellSize = 2.0f;
static constexpr float kMaxCellSize = 36.0f;
static constexpr int kMatrixCols = 100;
static constexpr int kMatrixRows = CHAR_ROWS + 2;
static constexpr float kDefaultCellSize = 12.0f;

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
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    const int defaultWidth = static_cast<int>(kMatrixCols * kDefaultCellSize);
    const int defaultHeight = static_cast<int>(kMatrixRows * kDefaultCellSize);
    setMinimumSize(defaultWidth / 2, defaultHeight / 2);
    setBaseSize(defaultWidth, defaultHeight);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MatrixDisplay::onTimeout);
}

QSize MatrixDisplay::sizeHint() const
{
    return {
        static_cast<int>(kMatrixCols * kDefaultCellSize),
        static_cast<int>(kMatrixRows * kDefaultCellSize)
    };
}

void MatrixDisplay::setText(const QString &text)
{
    currentText = text.toUpper();
    scrollOffset = 0.0f;
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
    if (currentMode == mode)
        return;

    currentMode = mode;
    timer->stop();

    if (currentMode == Clock) {
        scrollEnabled = false;
        scrollOffset = 0.0f;
        onTimeout();
        timer->start(1000);
    } else {
        restartScrollIfNeeded();
    }
    update();
}

void MatrixDisplay::setScrollEnabled(bool enabled)
{
    bool allowScroll = enabled && currentMode == Text;
    if (scrollEnabled == allowScroll)
        return;

    scrollEnabled = allowScroll;
    scrollOffset = 0.0f;

    if (currentMode == Text) {
        timer->stop();
        if (scrollEnabled) {
            restartScrollIfNeeded();
        }
    }

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
    if (kMatrixRows <= 0)
        return kMinCellSize;

    int w = width();
    int h = height();
    if (w <= 0 || h <= 0)
        return kMinCellSize;

    float usableHeight = h * kMaxHeightUsage;
    float sizeByHeight = usableHeight / static_cast<float>(kMatrixRows);
    float sizeByWidth = static_cast<float>(w) / static_cast<float>(kMatrixCols);

    float chosen = std::min(sizeByHeight, sizeByWidth);

    if (chosen < kMinCellSize)
        chosen = kMinCellSize;
    if (kMaxCellSize > 0.0f && chosen > kMaxCellSize)
        chosen = kMaxCellSize;

    float dpr = devicePixelRatioF();
    if (dpr > 0.0f) {
        chosen = std::ceil(chosen * dpr) / dpr;
    } else {
        chosen = std::ceil(chosen);
    }

    return chosen;
}

void MatrixDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    restartScrollIfNeeded();
    update();
}


void MatrixDisplay::restartScrollIfNeeded()
{
    timer->stop();
    scrollOffset = 0.0f;
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
    float matrixWidth = kMatrixCols * cellSize;

    if (totalTextWidthInPixels > matrixWidth) {
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
        float matrixWidth = kMatrixCols * cellSize;

        if (totalTextWidthInPixels > matrixWidth) {
            int wrapWidth = static_cast<int>(totalTextWidthInPixels + matrixWidth);
            if (wrapWidth > 0) {
                scrollOffset += cellSize;
                if (scrollOffset >= wrapWidth) {
                    scrollOffset = std::fmod(scrollOffset, wrapWidth);
                }
            }
            update();
        } else {
            timer->stop();
            scrollOffset = 0;
        }
    }
}

bool MatrixDisplay::requiresScrolling() const
{
    if (currentText.isEmpty())
        return false;

    const float cellSize = calculateCellSize();
    const float charWidthInCols = CHAR_COLS;
    const float spacingInCols = 1.0f;
    const int numChars = currentText.length();
    const float totalTextWidthInCols = numChars * charWidthInCols
        + (numChars > 1 ? (numChars - 1) * spacingInCols : 0.0f);
    const float totalTextWidthInPixels = totalTextWidthInCols * cellSize;
    const float matrixWidth = kMatrixCols * cellSize;

    return totalTextWidthInPixels > matrixWidth;
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
    float matrixWidth = kMatrixCols * cellSize;
    float matrixHeight = kMatrixRows * cellSize;
    float xBase = (width() - matrixWidth) / 2.0f;
    float yOffset = (height() - matrixHeight) / 2.0f;

    painter.setClipRect(QRectF(xBase, yOffset, matrixWidth, matrixHeight));

    int numChars = currentText.length();
    float totalTextWidthInCols = numChars * charWidthInCols
        + (numChars > 1 ? (numChars - 1) * spacingInCols : 0);
    float totalTextWidthInPixels = totalTextWidthInCols * cellSize;

    bool isScrolling = (currentMode == Text) && scrollEnabled && (totalTextWidthInPixels > matrixWidth);
    float xOffset = isScrolling
                        ? (xBase + matrixWidth - scrollOffset)
                        : (xBase + (matrixWidth - totalTextWidthInPixels) / 2.0f);
    
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
                    if (x + pixelDiameter < xBase || x > xBase + matrixWidth)
                        continue;
                    painter.drawEllipse(QRectF(x, y, pixelDiameter, pixelDiameter));
                }
            }
        }
    }
}