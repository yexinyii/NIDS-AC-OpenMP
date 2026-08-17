#include <QApplication>
#include <QElapsedTimer>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QIcon>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPushButton>
#include <QStackedWidget>
#include <QStyle>
#include <QTimer>

#include <algorithm>
#include <cmath>

#include "ui_qt_mainwindow.h"

namespace {

QString materialStyle(const bool dark)
{
    const QString background = dark ? "#121212" : "#E1F5FE";
    const QString surface = dark ? "#212121" : "#FFFFFF";
    const QString raisedSurface = dark ? "#2C2C2C" : "#FFFFFF";
    const QString text = dark ? "#FFFFFF" : "#212121";
    const QString secondaryText = dark ? "#BDBDBD" : "#757575";
    const QString divider = dark ? "#424242" : "#B3E5FC";
    const QString field = dark ? "#292929" : "#E1F5FE";
    const QString sideBar = dark ? "#01579B" : "#0288D1";
    const QString selectedNav = dark ? "#003C6C" : "#01579B";
    return QString(R"(
QWidget#Form {
    background: %1;
    color: %4;
    font-family: "Roboto", "Noto Sans CJK SC", "Microsoft YaHei", sans-serif;
}
QFrame#sideBar { background: %11; border: none; }
QLabel#brandIcon { color: #FFFFFF; font-size: 28px; font-weight: 700; }
QLabel#brandName { color: #FFFFFF; font-size: 14px; font-weight: 700; }
QPushButton[nav="true"] {
    background: transparent; color: #E1F5FE; border: 0;
    border-left: 4px solid transparent; padding: 14px 12px;
    text-align: left; font-size: 14px;
}
QPushButton[nav="true"]:hover { background: rgba(255, 255, 255, 28); color: #FFFFFF; }
QPushButton[nav="true"]:pressed { background: rgba(255, 255, 255, 42); }
QPushButton[nav="true"]:checked {
    background: %12; color: #FFFFFF;
    border-left: 4px solid #81D4FA; font-weight: 600;
}
QLabel#versionLabel { color: #B3E5FC; font-size: 11px; }
QLabel[pageTitle="true"] { color: %4; font-size: 25px; font-weight: 500; }
QLabel[pageHint="true"], QLabel[cardTitle="true"] { color: %5; font-size: 13px; }
QLabel[value="true"] { color: %4; font-size: 22px; font-weight: 500; }
QFrame[card="true"] { background: %3; border: 1px solid %6; border-radius: 2px; }
QLabel#statusBadge {
    color: %7; background: %8; border-radius: 2px;
    padding: 5px 10px; font-weight: 600;
}
QTableWidget {
    background: %2; alternate-background-color: %9; color: %4;
    border: 1px solid %6; border-radius: 2px;
    gridline-color: %6; selection-background-color: #03A9F4;
    selection-color: #FFFFFF;
}
QTableWidget::item { padding: 7px; }
QHeaderView::section {
    background: %3; color: %5; padding: 10px; border: 0;
    border-bottom: 1px solid %6; font-weight: 500;
}
QComboBox, QLineEdit {
    background: %10; color: %4; border: 0;
    border-bottom: 2px solid #03A9F4; border-radius: 0;
    padding: 8px; min-height: 20px;
}
QComboBox QAbstractItemView { background: %2; color: %4; selection-background-color: #03A9F4; }
QPushButton[action="true"] {
    color: #FFFFFF; background: #03A9F4; border: 0;
    border-radius: 2px; padding: 10px 20px; font-weight: 500;
}
QPushButton[action="true"]:hover { background: #039BE5; }
QPushButton[action="true"]:pressed { background: #0277BD; }
QScrollBar:vertical { background: %1; width: 10px; margin: 0; }
QScrollBar::handle:vertical { background: #4FC3F7; min-height: 24px; border-radius: 5px; }
QScrollBar::handle:vertical:hover { background: #29B6F6; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
)")
        .arg(background, surface, raisedSurface, text, secondaryText, divider,
             dark ? "#81D4FA" : "#0277BD",
             dark ? "#01579B" : "#B3E5FC",
             dark ? "#262626" : "#E1F5FE", field, sideBar, selectedNav);
}

bool systemUsesDarkTheme(const QPalette &palette)
{
    return palette.color(QPalette::Window).lightness() < 128;
}

class SystemThemeWatcher final : public QObject
{
public:
    SystemThemeWatcher(QApplication *application, QWidget *window)
        : QObject(window), application_(application), window_(window)
    {
        applyTheme();
        application_->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == application_ && event->type() == QEvent::ApplicationPaletteChange)
            applyTheme();
        return QObject::eventFilter(watched, event);
    }

private:
    void applyTheme()
    {
        window_->setStyleSheet(
            materialStyle(systemUsesDarkTheme(application_->palette())));
    }

    QApplication *application_;
    QWidget *window_;
};

class MaterialRipple final : public QWidget
{
public:
    explicit MaterialRipple(QPushButton *button)
        : QWidget(button), button_(button)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
        setGeometry(button_->rect());
        hide();

        button_->installEventFilter(this);
        timer_.setInterval(16);
        connect(&timer_, &QTimer::timeout, this, [this] {
            if (elapsed_.elapsed() >= durationMs_) {
                timer_.stop();
                hide();
                return;
            }
            update();
        });
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched != button_)
            return QWidget::eventFilter(watched, event);

        if (event->type() == QEvent::Resize) {
            setGeometry(button_->rect());
        } else if (event->type() == QEvent::MouseButtonPress) {
            const auto *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                startRipple(mouseEvent->position());
        }
        return false;
    }

    void paintEvent(QPaintEvent *) override
    {
        const qreal linearProgress = std::clamp(
            elapsed_.elapsed() / static_cast<qreal>(durationMs_), 0.0, 1.0);
        const qreal easedProgress = 1.0 - std::pow(1.0 - linearProgress, 3.0);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 255, 255,
                                static_cast<int>(82.0 * (1.0 - linearProgress))));
        painter.drawEllipse(origin_, maxRadius_ * easedProgress,
                            maxRadius_ * easedProgress);
    }

private:
    void startRipple(const QPointF &origin)
    {
        origin_ = origin;
        const qreal farthestX = std::max(origin_.x(), width() - origin_.x());
        const qreal farthestY = std::max(origin_.y(), height() - origin_.y());
        maxRadius_ = std::hypot(farthestX, farthestY);
        elapsed_.restart();
        show();
        raise();
        update();
        timer_.start();
    }

    QPushButton *button_;
    QTimer timer_{this};
    QElapsedTimer elapsed_;
    QPointF origin_;
    qreal maxRadius_ = 0.0;
    static constexpr int durationMs_ = 420;
};

} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    Ui::Form ui;
    ui.setupUi(&window);

    new SystemThemeWatcher(&app, &window);

    // Theme icons are used when available; the button text keeps a visible
    // symbol as a fallback on minimal desktop environments.
    ui.overviewButton->setIcon(QIcon::fromTheme(
        "go-home", app.style()->standardIcon(QStyle::SP_DirHomeIcon)));
    ui.rulesButton->setIcon(QIcon::fromTheme("view-list-details"));
    ui.adaptersButton->setIcon(QIcon::fromTheme("network-wired"));
    ui.settingsButton->setIcon(QIcon::fromTheme("settings-configure"));

    // Material You style ripples expand from the exact left-click position.
    new MaterialRipple(ui.overviewButton);
    new MaterialRipple(ui.rulesButton);
    new MaterialRipple(ui.adaptersButton);
    new MaterialRipple(ui.settingsButton);
    new MaterialRipple(ui.chooseRuleFileButton);
    new MaterialRipple(ui.saveSettingsButton);
    new MaterialRipple(ui.refreshAdaptersButton);

    ui.maliciousMatchesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.rulesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Each navigation button owns an explicit signal/slot connection.
    QObject::connect(ui.overviewButton, &QPushButton::clicked, ui.stackedWidget,
                     [stack = ui.stackedWidget] { stack->setCurrentIndex(0); });
    QObject::connect(ui.rulesButton, &QPushButton::clicked, ui.stackedWidget,
                     [stack = ui.stackedWidget] { stack->setCurrentIndex(1); });
    QObject::connect(ui.adaptersButton, &QPushButton::clicked, ui.stackedWidget,
                     [stack = ui.stackedWidget] { stack->setCurrentIndex(2); });
    QObject::connect(ui.settingsButton, &QPushButton::clicked, ui.stackedWidget,
                     [stack = ui.stackedWidget] { stack->setCurrentIndex(3); });

    QObject::connect(ui.chooseRuleFileButton, &QPushButton::clicked, &window,
                     [&window, pathEdit = ui.rulePathEdit] {
        const QString currentPath = pathEdit->text().trimmed();
        const QString startPath = currentPath.isEmpty()
            ? QStringLiteral("rules")
            : currentPath;
        const QString selectedPath = QFileDialog::getOpenFileName(
            &window,
            QStringLiteral("选择规则文件"),
            QFileInfo(startPath).absoluteFilePath(),
            QStringLiteral("规则文件 (*.json);所有文件 (*)"));

        if (!selectedPath.isEmpty())
            pathEdit->setText(QFileInfo(selectedPath).absoluteFilePath());
    });

    window.show();
    return app.exec();
}
