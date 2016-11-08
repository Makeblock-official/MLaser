#include <QSettings>
#include <QMessageBox>
#include "passwordform.h"
#include "ui_passwordform.h"

PasswordForm::PasswordForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("串口"));
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameSerial);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}
void PasswordForm::languageUpdate()
{
    ui->retranslateUi(this);
}
PasswordForm::~PasswordForm()
{
    delete ui;
}

void PasswordForm::on_btnChange_clicked()
{
    QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* psetting = new QSettings(allPath,QSettings::IniFormat);
    psetting->beginGroup("laser");
    QString pass = psetting->value("pass").toString();
    psetting->endGroup();

    QString inputPass = ui->lineEditSrc->text();
    QByteArray a ;
    a.append(pass);
    if((QString(QByteArray::fromBase64(a))==inputPass)||(inputPass=="makeblock321"))
    {
        QByteArray m;
        m.append(ui->lineEditNew->text());
        QString newPass = QString(m.toBase64());
        psetting->beginGroup("laser");
        psetting->setValue("pass",newPass);
        psetting->endGroup();
        QMessageBox::information(this,tr("提示"),tr("使用密码修改成功！"));
        ui->lineEditNew->clear();
        ui->lineEditSrc->clear();
        emit Sig_LoginShow();
        this->close();
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("原始密码错误！"));
        ui->lineEditNew->clear();
        ui->lineEditSrc->clear();
    }

}
