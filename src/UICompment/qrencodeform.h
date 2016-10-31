#ifndef QRENCODEFORM_H
#define QRENCODEFORM_H

#include <QWidget>
#include <QNetworkInterface>
#include <QHostAddress>
#include "encode/tcQrencode.h"
#include "qcptitle.h"

namespace Ui {
class QrencodeForm;
}

class QrencodeForm : public QWidget
{
    Q_OBJECT

public:
    explicit QrencodeForm(QWidget *parent = 0);
    ~QrencodeForm();
    void languageUpdate();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);
    void rencodeShow();

private:
    Ui::QrencodeForm *ui;
    TcQrencode* renders;
};

#endif // QRENCODEFORM_H
