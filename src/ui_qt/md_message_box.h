#ifndef MD_MESSAGE_BOX_H
#define MD_MESSAGE_BOX_H

#include <QMessageBox>

class MdMessageBox final : public QMessageBox {
public:
    explicit MdMessageBox(QWidget *parent = nullptr);

    static StandardButton confirm(QWidget *parent, const QString& title, const QString& message, StandardButton defaultButton = No);
    static bool get_rule(QWidget *parent, QString& rule, QString& tag, QString& option);
    static void show_information(QWidget *parent, const QString& title, const QString& message);
    static void show_warning(QWidget *parent, const QString& title, const QString& message);
    static void show_critical(QWidget *parent, const QString& title, const QString& message);

private:
    static void show_message(QWidget *parent, const QString& title, const QString& message, Icon icon);
    void apply_material_style();
    void install_button_ripples();
};

#endif
