#ifndef CONNECT_DIALOG_H
#define CONNECT_DIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

    const QString& host() const;
    quint16  port() const;
    bool use_ssl() const;

private:
    Ui::ConnectDialog *ui;
};

#endif // CONNECT_DIALOG_H
