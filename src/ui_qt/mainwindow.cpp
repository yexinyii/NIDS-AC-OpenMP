#include <QApplication>
#include <QAbstractItemView>
#include <QColor>
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QIcon>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QStackedWidget>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolButton>

#include <vector>

#include "md_message_box.h"
#include "md_ripple.h"
#include "rules_manager.h"
#include "ui_qt_mainwindow.h"

namespace {

QString material_style(const bool dark) {
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
QPushButton[fab="true"] {
    color: #FFFFFF; background: #03A9F4; border: none;
    border-radius: 28px; font-size: 30px; font-weight: 400;
    padding: 0 0 4px 0;
}
QPushButton[fab="true"]:hover { background: #039BE5; }
QPushButton[fab="true"]:pressed { background: #0277BD; }
QToolButton[deleteRule="true"] { background: transparent; border: none; border-radius: 18px; padding: 4px; font-size: 20px; }
QToolButton[deleteRule="true"]:hover { background: rgba(211, 47, 47, 32); }
QToolButton[deleteRule="true"]:pressed { background: rgba(211, 47, 47, 56); }
QScrollBar:vertical { background: %1; width: 10px; margin: 0; }
QScrollBar::handle:vertical { background: #4FC3F7; min-height: 24px; border-radius: 5px; }
QScrollBar::handle:vertical:hover { background: #29B6F6; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
)")
        .arg(background, surface, raisedSurface, text, secondaryText, divider, dark ? "#81D4FA" : "#0277BD", dark ? "#01579B" : "#B3E5FC", dark ? "#262626" : "#E1F5FE", field, sideBar, selectedNav);
}

bool system_uses_dark_theme(const QPalette &palette) {
    return palette.color(QPalette::Window).lightness() < 128;
}

void load_rules_into_table(RulesManager& rulesManager, const QString& filePath, QTableWidget *rulesTable, QWidget *dialogParent) {
    std::vector<std::string> rules;
    std::vector<std::string> tags;
    std::vector<std::string> options;

    rulesManager.set_rules_file_path(filePath.toStdString());
    rulesManager.load_rules_from_file(rules, tags, options);

    rulesTable->setSortingEnabled(false);
    rulesTable->setRowCount(0);
    rulesTable->setRowCount(static_cast<int>(rules.size()));
    for (std::size_t index = 0; index < rules.size(); ++index) {
        auto *deleteButton = new QToolButton(rulesTable);
        deleteButton->setProperty("deleteRule", true);
        deleteButton->setToolTip(QStringLiteral("删除规则"));
        deleteButton->setText(QStringLiteral("🗑️"));
        new MaterialRipple(deleteButton, QColor("#D32F2F"));
        auto *ruleItem = new QTableWidgetItem(QString::fromStdString(rules[index]));
        auto *tagItem = new QTableWidgetItem(QString::fromStdString(tags[index]));
        auto *optionItem = new QTableWidgetItem(QString::fromStdString(options[index]));
        optionItem->setForeground(options[index] == "block" ? QColor("#D32F2F") : QColor("#0288D1"));
        rulesTable->setCellWidget(static_cast<int>(index), 0, deleteButton);
        rulesTable->setItem(static_cast<int>(index), 1, ruleItem);
        rulesTable->setItem(static_cast<int>(index), 2, tagItem);
        rulesTable->setItem(static_cast<int>(index), 3, optionItem);

        const std::string rule = rules[index];
        const std::string tag = tags[index];
        const std::string option = options[index];
        QObject::connect(deleteButton, &QToolButton::clicked, rulesTable, [&rulesManager, filePath, rulesTable, dialogParent, rule, tag, option] {
            const QString message = QStringLiteral("确定删除规则“%1”吗？").arg(QString::fromStdString(rule));
            if (MdMessageBox::confirm(dialogParent, QStringLiteral("删除规则"), message) != QMessageBox::Yes)
                return;

            try {
                if (!rulesManager.remove_rule(rule, tag, option)) {
                    MdMessageBox::show_warning(dialogParent, QStringLiteral("删除规则"), QStringLiteral("规则不存在，可能已被删除。"));
                    return;
                }
                load_rules_into_table(rulesManager, filePath, rulesTable, dialogParent);
            } catch (const std::exception& error) {
                MdMessageBox::show_critical(dialogParent, QStringLiteral("删除规则失败"), QString::fromUtf8(error.what()));
            }
        });
    }
    rulesTable->setSortingEnabled(true);
}

class SystemThemeWatcher final : public QObject
{
public:
    SystemThemeWatcher(QApplication *application, QWidget *window) : QObject(window), application(application), window(window) {
        apply_theme();
        application->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (watched == application && event->type() == QEvent::ApplicationPaletteChange)
            apply_theme();
        return QObject::eventFilter(watched, event);
    }

private:
    void apply_theme() {
        window->setStyleSheet(material_style(system_uses_dark_theme(application->palette())));
    }

    QApplication *application;
    QWidget *window;
};

} // namespace

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    Ui::Form ui;
    ui.setupUi(&window);
    RulesManager rulesManager;

    new SystemThemeWatcher(&app, &window);

    // Theme icons are used when available; the button text keeps a visible
    // symbol as a fallback on minimal desktop environments.
    ui.overviewButton->setIcon(QIcon::fromTheme("go-home", app.style()->standardIcon(QStyle::SP_DirHomeIcon)));
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
    new MaterialRipple(ui.rulesTable, QColor("#03A9F4"));
    new MaterialRipple(ui.addRuleButton);

    ui.maliciousMatchesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.rulesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.rulesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui.rulesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    const QString defaultRulesPath = QDir::current().absoluteFilePath(QStringLiteral("rules/rules.json"));
    if (QFileInfo::exists(defaultRulesPath)) {
        ui.rulePathEdit->setText(defaultRulesPath);
        try {
            load_rules_into_table(rulesManager, defaultRulesPath, ui.rulesTable, &window);
        } catch (const std::exception&) {
            // The user can inspect and retry the file through the settings page.
        }
    }

    // Each navigation button owns an explicit signal/slot connection.
    QObject::connect(ui.overviewButton, &QPushButton::clicked, ui.stackedWidget, [stack = ui.stackedWidget] { stack->setCurrentIndex(0); });
    QObject::connect(ui.rulesButton, &QPushButton::clicked, ui.stackedWidget, [stack = ui.stackedWidget] { stack->setCurrentIndex(1); });
    QObject::connect(ui.adaptersButton, &QPushButton::clicked, ui.stackedWidget, [stack = ui.stackedWidget] { stack->setCurrentIndex(2); });
    QObject::connect(ui.settingsButton, &QPushButton::clicked, ui.stackedWidget, [stack = ui.stackedWidget] { stack->setCurrentIndex(3); });

    QObject::connect(ui.chooseRuleFileButton, &QPushButton::clicked, &window, [&window, pathEdit = ui.rulePathEdit] {
        const QString currentPath = pathEdit->text().trimmed();
        const QString startPath = currentPath.isEmpty()
            ? QStringLiteral("rules")
            : currentPath;
        const QString selectedPath = QFileDialog::getOpenFileName(&window, QStringLiteral("选择规则文件"), QFileInfo(startPath).absoluteFilePath(), QStringLiteral("规则文件 (*.json);所有文件 (*)"));

        if (!selectedPath.isEmpty())
            pathEdit->setText(QFileInfo(selectedPath).absoluteFilePath());
    });

    QObject::connect(ui.saveSettingsButton, &QPushButton::clicked, &window, [&window, &rulesManager, pathEdit = ui.rulePathEdit, rulesTable = ui.rulesTable] {
        const QString filePath = pathEdit->text().trimmed();
        if (filePath.isEmpty()) {
            MdMessageBox::show_warning(&window, QStringLiteral("规则文件"), QStringLiteral("请先选择规则文件。"));
            return;
        }

        try {
            const QString absolutePath = QFileInfo(filePath).absoluteFilePath();
            load_rules_into_table(rulesManager, absolutePath, rulesTable, &window);
            pathEdit->setText(absolutePath);
            MdMessageBox::show_information(&window, QStringLiteral("设置已保存"), QStringLiteral("规则文件已加载，共 %1 条规则。").arg(rulesTable->rowCount()));
        } catch (const std::exception& error) {
            MdMessageBox::show_critical(&window, QStringLiteral("规则加载失败"), QString::fromUtf8(error.what()));
        }
    });

    QObject::connect(ui.addRuleButton, &QPushButton::clicked, &window, [&window, &rulesManager, pathEdit = ui.rulePathEdit, rulesTable = ui.rulesTable] {
        const QString filePath = pathEdit->text().trimmed();
        if (filePath.isEmpty()) {
            MdMessageBox::show_warning(&window, QStringLiteral("新增规则"), QStringLiteral("请先在设置页选择并保存规则文件。"));
            return;
        }

        QString rule;
        QString tag;
        QString option;
        if (!MdMessageBox::get_rule(&window, rule, tag, option))
            return;
        if (rule.isEmpty() || tag.isEmpty()) {
            MdMessageBox::show_warning(&window, QStringLiteral("新增规则"), QStringLiteral("规则和标签不能为空。"));
            return;
        }

        try {
            const QString absolutePath = QFileInfo(filePath).absoluteFilePath();
            rulesManager.set_rules_file_path(absolutePath.toStdString());
            rulesManager.add_rule(rule.toStdString(), tag.toStdString(), option.toStdString());
            load_rules_into_table(rulesManager, absolutePath, rulesTable, &window);
        } catch (const std::exception& error) {
            MdMessageBox::show_critical(&window, QStringLiteral("新增规则失败"), QString::fromUtf8(error.what()));
        }
    });

    window.show();
    return app.exec();
}
