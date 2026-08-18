#include "md_ripple.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <algorithm>
#include <cmath>

MaterialRipple::MaterialRipple(QWidget *target, const QColor& color) : QWidget(target), target(target), rippleColor(color) {
    initialize();
}

MaterialRipple::MaterialRipple(QTableWidget *table, const QColor& color) : QWidget(table->viewport()), target(table->viewport()), rippleColor(color), table(table) {
    initialize();
}

bool MaterialRipple::eventFilter(QObject *watched, QEvent *event) {
    if (watched != target)
        return QWidget::eventFilter(watched, event);

    if (event->type() == QEvent::Resize) {
        if (table == nullptr)
            setGeometry(target->rect());
        else
            hide();
    } else if (event->type() == QEvent::MouseButtonPress) {
        const auto *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (table == nullptr) {
                start_ripple(mouseEvent->position());
            } else {
                QTableWidgetItem *item = table->itemAt(mouseEvent->position().toPoint());
                if (item != nullptr) {
                    const QRect cellRect = table->visualItemRect(item);
                    setGeometry(cellRect);
                    start_ripple(mouseEvent->position() - cellRect.topLeft());
                }
            }
        }
    }
    return false;
}

void MaterialRipple::paintEvent(QPaintEvent *) {
    const qreal linearProgress = std::clamp(elapsed.elapsed() / static_cast<qreal>(durationMs), 0.0, 1.0);
    const qreal easedProgress = 1.0 - std::pow(1.0 - linearProgress, 3.0);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    QColor animatedColor = rippleColor;
    animatedColor.setAlpha(static_cast<int>(82.0 * (1.0 - linearProgress)));
    painter.setBrush(animatedColor);
    painter.drawEllipse(rippleOrigin, maxRadius * easedProgress, maxRadius * easedProgress);
}

void MaterialRipple::initialize() {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setGeometry(target->rect());
    hide();

    target->installEventFilter(this);
    timer.setInterval(16);
    connect(&timer, &QTimer::timeout, this, [this] {
        if (elapsed.elapsed() >= durationMs) {
            timer.stop();
            hide();
            return;
        }
        update();
    });
}

void MaterialRipple::start_ripple(const QPointF& clickOrigin) {
    rippleOrigin = clickOrigin;
    const qreal farthestX = std::max(rippleOrigin.x(), width() - rippleOrigin.x());
    const qreal farthestY = std::max(rippleOrigin.y(), height() - rippleOrigin.y());
    maxRadius = std::hypot(farthestX, farthestY);
    elapsed.restart();
    show();
    raise();
    update();
    timer.start();
}
