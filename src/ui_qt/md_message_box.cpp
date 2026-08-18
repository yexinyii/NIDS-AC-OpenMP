#include "md_message_box.h"

#include "md_ripple.h"

#include <QAbstractButton>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QPushButton>

MdMessageBox::MdMessageBox(QWidget *parent) : QMessageBox(parent) {
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setTextFormat(Qt::PlainText);
    setMinimumWidth(380);
    apply_material_style();
}

QMessageBox::StandardButton MdMessageBox::confirm(QWidget *parent, const QString& title, const QString& message, StandardButton defaultButton) {
    MdMessageBox messageBox(parent);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(defaultButton);

    if (QAbstractButton *deleteButton = messageBox.button(QMessageBox::Yes))
        deleteButton->setText(QStringLiteral("删除"));
    if (QAbstractButton *cancelButton = messageBox.button(QMessageBox::No))
        cancelButton->setText(QStringLiteral("取消"));

    messageBox.install_button_ripples();
    return static_cast<QMessageBox::StandardButton>(messageBox.exec());
}

bool MdMessageBox::get_rule(QWidget *parent, QString& rule, QString& tag, QString& option) {
    MdMessageBox messageBox(parent);
    messageBox.setWindowTitle(QStringLiteral("新增规则"));
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Save);

    QWidget inputArea(&messageBox);
    QFormLayout inputLayout(&inputArea);
    inputLayout.setContentsMargins(8, 0, 8, 8);
    inputLayout.setSpacing(12);
    QLineEdit ruleEdit;
    QLineEdit tagEdit;
    QComboBox optionCombo;
    optionCombo.addItems({QStringLiteral("block"), QStringLiteral("permit")});
    inputLayout.addRow(QStringLiteral("规则"), &ruleEdit);
    inputLayout.addRow(QStringLiteral("标签"), &tagEdit);
    inputLayout.addRow(QStringLiteral("操作"), &optionCombo);

    QLabel hintLabel(QStringLiteral("填写需要添加的规则信息"), &messageBox);
    hintLabel.setObjectName(QStringLiteral("md_message_hint"));
    if (QLabel *defaultTextLabel = messageBox.findChild<QLabel *>(QStringLiteral("qt_msgbox_label")))
        defaultTextLabel->hide();

    auto *messageLayout = qobject_cast<QGridLayout *>(messageBox.layout());
    QDialogButtonBox *buttonBox = messageBox.findChild<QDialogButtonBox *>();
    if (messageLayout != nullptr) {
        if (buttonBox != nullptr)
            messageLayout->removeWidget(buttonBox);
        messageLayout->addWidget(&inputArea, 1, 0, 1, 2);
        messageLayout->addWidget(&hintLabel, 0, 0, 1, 2);
        if (buttonBox != nullptr)
            messageLayout->addWidget(buttonBox, 2, 0, 1, 2);
    }

    if (QAbstractButton *saveButton = messageBox.button(QMessageBox::Save))
        saveButton->setText(QStringLiteral("保存"));
    if (QAbstractButton *cancelButton = messageBox.button(QMessageBox::Cancel))
        cancelButton->setText(QStringLiteral("取消"));
    messageBox.install_button_ripples();

    if (messageBox.exec() != QMessageBox::Save)
        return false;

    rule = ruleEdit.text().trimmed();
    tag = tagEdit.text().trimmed();
    option = optionCombo.currentText();
    return true;
}

void MdMessageBox::show_information(QWidget *parent, const QString& title, const QString& message) {
    show_message(parent, title, message, QMessageBox::Information);
}

void MdMessageBox::show_warning(QWidget *parent, const QString& title, const QString& message) {
    show_message(parent, title, message, QMessageBox::Warning);
}

void MdMessageBox::show_critical(QWidget *parent, const QString& title, const QString& message) {
    show_message(parent, title, message, QMessageBox::Critical);
}

void MdMessageBox::show_message(QWidget *parent, const QString& title, const QString& message, Icon icon) {
    MdMessageBox messageBox(parent);
    messageBox.setWindowTitle(title);
    messageBox.setText(message);
    messageBox.setIcon(icon);
    messageBox.setStandardButtons(QMessageBox::Ok);
    messageBox.setDefaultButton(QMessageBox::Ok);

    if (QAbstractButton *okButton = messageBox.button(QMessageBox::Ok))
        okButton->setText(QStringLiteral("确定"));
    messageBox.install_button_ripples();
    messageBox.exec();
}

void MdMessageBox::apply_material_style() {
    const bool dark = palette().color(QPalette::Window).lightness() < 128;
    const QString surface = dark ? QStringLiteral("#212121") : QStringLiteral("#FFFFFF");
    const QString text = dark ? QStringLiteral("#FFFFFF") : QStringLiteral("#212121");
    const QString secondaryText = dark ? QStringLiteral("#BDBDBD") : QStringLiteral("#616161");
    const QString buttonHover = dark ? QStringLiteral("rgba(3, 169, 244, 48)") : QStringLiteral("#E1F5FE");

    setStyleSheet(QStringLiteral(R"(
QMessageBox {
    background: %1;
    color: %2;
    font-family: "Roboto", "Noto Sans CJK SC", "Microsoft YaHei", sans-serif;
}
QMessageBox QLabel {
    color: %2;
    font-size: 14px;
    min-width: 260px;
    padding: 12px 8px;
}
QMessageBox QPushButton {
    min-width: 72px;
    min-height: 36px;
    padding: 0 16px;
    color: #0288D1;
    background: transparent;
    border: none;
    border-radius: 2px;
    font-size: 13px;
    font-weight: 500;
}
QMessageBox QPushButton:hover { background: %4; }
QMessageBox QPushButton:pressed { background: rgba(3, 169, 244, 72); }
QMessageBox QPushButton:default {
    color: #FFFFFF;
    background: #03A9F4;
}
QMessageBox QPushButton:default:hover { background: #039BE5; }
QMessageBox QPushButton:default:pressed { background: #0277BD; }
QMessageBox QLabel#qt_msgbox_label { color: %3; }
QMessageBox QLabel#md_message_hint { color: %3; padding: 4px 8px 8px 8px; }
QMessageBox QLineEdit, QMessageBox QComboBox {
    min-height: 24px;
    padding: 7px 8px;
    color: %2;
    background: %1;
    border: none;
    border-bottom: 2px solid #03A9F4;
}
QMessageBox QComboBox QAbstractItemView {
    color: %2;
    background: %1;
    selection-background-color: #03A9F4;
}
)").arg(surface, text, secondaryText, buttonHover));
}

void MdMessageBox::install_button_ripples() {
    const auto messageButtons = buttons();
    for (QAbstractButton *button : messageButtons)
        new MaterialRipple(button);
}
