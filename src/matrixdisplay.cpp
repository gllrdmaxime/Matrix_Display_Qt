/**
 * @file matrixdisplay.cpp
 * @author G. Maxime
 * @brief Implémentation de la classe MatrixDisplay pour l'affichage sur une matrice LED.
 * Cette classe hérite de QWidget et permet d'afficher du texte ou l'heure actuelle
 * sur une matrice LED simulée. Elle gère également le défilement du texte.
 * La couleur des pixels, le mode d'affichage (texte ou horloge) et
 * l'activation du défilement peuvent être configurés.
 */

 // Inclusion de mes headers
#include "headers/matrixdisplay.h"
#include "headers/matrixfont.h"

// Inclusion des headers Qt nécessaires
#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <QSizePolicy>

// Inclusion des headers standard nécessaires
#include <cmath>
#include <algorithm>

/**
 * @brief Constantes de configuration pour la matrice LED.
 */
static constexpr float kMaxHeightUsage = 0.95f;
static constexpr float kMinCellSize = 2.0f;
static constexpr float kMaxCellSize = 36.0f;
static constexpr int MatrixCols = 100;
static constexpr int MatrixRows = CHAR_ROWS + 4;
static constexpr float kDefaultCellSize = 12.0f;
static constexpr int defaultScrollIntervalMs = 50;

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

    const int defaultWidth = static_cast<int>(MatrixCols * kDefaultCellSize);
    const int defaultHeight = static_cast<int>(MatrixRows * kDefaultCellSize);
    setMinimumSize(defaultWidth / 2, defaultHeight / 2);
    setBaseSize(defaultWidth, defaultHeight);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MatrixDisplay::onTimeout);
}

QSize MatrixDisplay::sizeHint() const
{
    return {
        static_cast<int>(MatrixCols * kDefaultCellSize),
        static_cast<int>(MatrixRows * kDefaultCellSize)
    };
}

void MatrixDisplay::setScrollMode(ScrollMode mode)
{
    if (scrollMode == mode)
        return;

    scrollMode = mode;
    scrollOffset = 0.0f;
    scrollDirection = 1;
    restartScrollIfNeeded();
    update();
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

void MatrixDisplay::setColor_Background(const QColor &color)
{
    backgroundColor = color;
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

void MatrixDisplay::setScrollInterval(int interval)
{
    if (interval <= 0) {
        return;
    }

    if (timer->isActive()) {
        timer->stop();
        scrollIntervalMs = (2.0f/interval * defaultScrollIntervalMs);
        timer->start(scrollIntervalMs);
    }
}


float MatrixDisplay::calculateCellSize() const
{
    if (MatrixRows <= 0)
        return kMinCellSize;

    int w = width();
    int h = height();
    if (w <= 0 || h <= 0)
        return kMinCellSize;

    float usableHeight = h * kMaxHeightUsage;
    float sizeByHeight = usableHeight / static_cast<float>(MatrixRows);
    float sizeByWidth = static_cast<float>(w) / static_cast<float>(MatrixCols);
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

    if (currentMode == Text){
        restartScrollIfNeeded();
    }
    onTimeout();
    update();
}


void MatrixDisplay::restartScrollIfNeeded()
{
    timer->stop();
    scrollOffset = 0.0f;

    if (currentMode != Text || !scrollEnabled)
        return;

    if (currentText.isEmpty())
        return;

    timer->start(scrollIntervalMs);
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
        int numChars = currentText.length();

        float totalTextWidthInCols = numChars * charWidthInCols
            + (numChars > 1 ? (numChars - 1) : 0);
        
        float totalTextWidthInPixels = totalTextWidthInCols * cellSize;
        float matrixWidth = MatrixCols * cellSize;

        if (scrollMode == bounceMode) {
            float bounceRange = (totalTextWidthInPixels >= matrixWidth)
                                    ? (totalTextWidthInPixels - matrixWidth)
                                    : (matrixWidth - totalTextWidthInPixels);
            if (bounceRange <= 0.0f) {
                scrollOffset = 0.0f;
                scrollDirection = 1;
                update();
                return;
            }

            scrollOffset += scrollDirection * cellSize;

            if (scrollOffset >= bounceRange) {
                scrollOffset = bounceRange;
                scrollDirection = -1;
            } else if (scrollOffset <= 0.0f) {
                scrollOffset = 0.0f;
                scrollDirection = 1;
            }
            update();
        } else {
            int wrapWidth = static_cast<int>(totalTextWidthInPixels + matrixWidth);
            if (wrapWidth > 0) {
                scrollOffset += cellSize;
                if (scrollOffset >= wrapWidth) {
                    scrollOffset = std::fmod(scrollOffset, wrapWidth);
                }
            }
            update();
        }
    }
}

bool MatrixDisplay::requiresScrolling() const
{
    if (currentText.isEmpty())
        return false;
    const float cellSize = calculateCellSize();
    const float charWidthInCols = CHAR_COLS;
    const int numChars = currentText.length();
    const float totalTextWidthInCols = numChars * charWidthInCols
        + (numChars > 1 ? (numChars - 1) : 0.0f);
    const float totalTextWidthInPixels = totalTextWidthInCols * cellSize;
    const float matrixWidth = MatrixCols * cellSize;

    return totalTextWidthInPixels > matrixWidth;
}

void MatrixDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(rect(), backgroundColor);
    if (currentText.isEmpty()) {
        return;
    }
    const float cellSize = calculateCellSize();
    const float pixelDiameter = cellSize * 0.8f;
    const float pixelOffset = (cellSize - pixelDiameter) / 2.0f;
    const float charWidthInCols = CHAR_COLS;
    float matrixWidth = MatrixCols * cellSize;
    float matrixHeight = MatrixRows * cellSize;
    float xBase = (width() - matrixWidth) / 2.0f;
    float yOffset = (height() - matrixHeight) / 2.0f;
    painter.setClipRect(QRectF(xBase, yOffset, matrixWidth, matrixHeight));
    int numChars = currentText.length();
    float totalTextWidthInCols = numChars * charWidthInCols
        + (numChars > 1 ? (numChars - 1) : 0);
    float totalTextWidthInPixels = totalTextWidthInCols * cellSize;
    bool isScrolling = (currentMode == Text) && scrollEnabled;
    float xOffset = xBase + (matrixWidth - totalTextWidthInPixels) / 2.0f;
    if (isScrolling) {
        if (scrollMode == bounceMode) {
            if (totalTextWidthInPixels >= matrixWidth) {
                // Ajout de 2 pixel de marge pour éviter de changer de sens au moment ou la première lettre arrive tout à gauche de la matrice d'affichage
                xOffset = xBase - scrollOffset + 2.0f * cellSize;
            } else {
                float bounceRange = matrixWidth - totalTextWidthInPixels;
                float centeredBase = xBase + bounceRange / 2.0f;
                xOffset = centeredBase + (scrollOffset - bounceRange / 2.0f);
            }
        } else {
            xOffset = xBase + matrixWidth - scrollOffset;
        }
    }
   
    
    
    painter.setPen(Qt::NoPen);

    painter.setBrush(Qt::black);
    for (int row = 0; row < MatrixRows; ++row) {
        for (int col = 0; col < MatrixCols; ++col) {
            float x = xBase + col * cellSize + pixelOffset;
            float y = yOffset + row * cellSize + pixelOffset;
            painter.drawEllipse(QRectF(x, y, pixelDiameter, pixelDiameter));
        }
    }

    for (int i = 0; i < numChars; ++i) {
        char c = currentText[i].toLatin1();
        if (!matrix_font.count(c)) {
            continue;
        }
        const auto &charMap = matrix_font.at(c);
        float charStartX = i * (charWidthInCols) * cellSize;
        
        for (int row = 0 ; row < CHAR_ROWS ; ++row) {
            for (int col = 0; col < CHAR_COLS; ++col) {
                if (col < charMap[row].length() && charMap[row][col] == '1') {
                    float x = xOffset + charStartX + col * cellSize + pixelOffset;
                    float y = yOffset + (row + (MatrixRows - CHAR_ROWS) / 2.0f) * cellSize + pixelOffset;
                    painter.setBrush(pixelColor);
                    if (x + pixelDiameter < xBase || x > xBase + matrixWidth)
                        continue;
                    painter.drawEllipse(QRectF(x, y, pixelDiameter, pixelDiameter));
                }
            }
        }
    }
}