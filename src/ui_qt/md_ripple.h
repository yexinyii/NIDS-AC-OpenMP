#ifndef MD_RIPPLE_H
#define MD_RIPPLE_H

#include <QColor>
#include <QElapsedTimer>
#include <QPointF>
#include <QTimer>
#include <QWidget>

class QTableWidget;

class MaterialRipple final : public QWidget {
public:
    explicit MaterialRipple(QWidget *target, const QColor& color = QColor(255, 255, 255));
    explicit MaterialRipple(QTableWidget *table, const QColor& color = QColor(3, 169, 244));

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void initialize();
    void start_ripple(const QPointF& clickOrigin);

    QWidget *target;
    QColor rippleColor;
    QTableWidget *table = nullptr;
    QTimer timer{this};
    QElapsedTimer elapsed;
    QPointF rippleOrigin;
    qreal maxRadius = 0.0;
    static constexpr int durationMs = 420;
};

#endif
