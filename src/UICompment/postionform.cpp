#include <QVBoxLayout>
#include <QSettings>
#include "postionform.h"
#include "ui_postionform.h"

PostionForm::PostionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PostionForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("PositionEdit"));
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->framePos);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
//    scale = 1.0;
    old_w = 1.0;
    old_h = 1.0;
}

PostionForm::~PostionForm()
{
    delete ui;
}
void PostionForm::languageUpdate()
{
    ui->retranslateUi(this);
}
void PostionForm::setUIElement(QRectF m)
{
//    ui->lineEditX->setText(QString::number(m.x()));
//    ui->lineEditY->setText(QString::number(m.y()));

    //mm和inch切换显示
    QString path = QCoreApplication::applicationDirPath();
    QString name = "/mLaser.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* psetting = new QSettings(allPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    QString unit = psetting->value("unit").toString();
    psetting->endGroup();

    if (unit == "inch")
    {
        ui->lineEditW->setText(QString::number(m.width()/254.0));
        ui->lineEditH->setText(QString::number(m.height()/254.0));
    }

    if (unit == "mm")
    {
        ui->lineEditW->setText(QString::number(m.width()/10.0));
        ui->lineEditH->setText(QString::number(m.height()/10.0));
    }


//    ui->lineEditW->setText(QString::number(m.width()/10));
//    ui->lineEditH->setText(QString::number(m.height()/10));
    old_w = ui->lineEditW->text().toDouble();
    old_h = ui->lineEditH->text().toDouble();

}
void PostionForm::on_btnCertern_clicked()
{
    //mm和inch切换
    QString path = QCoreApplication::applicationDirPath();
    QString name = "/mLaser.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* psetting = new QSettings(allPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    QString unit = psetting->value("unit").toString();
    psetting->endGroup();

    QPointF m;
    if (unit == "inch")
    {
        m.setX(ui->lineEditW->text().toDouble()*254.0);
        m.setY(ui->lineEditH->text().toDouble()*254.0);
    }


    if (unit == "mm")
    {
        m.setX(ui->lineEditW->text().toDouble()*10.0);
        m.setY(ui->lineEditH->text().toDouble()*10.0);
    }
    emit Sig_WH(m);
}
void PostionForm::on_lineEditW_returnPressed()
{

}
//height edit finish
void PostionForm::on_lineEditH_editingFinished()
{
    double scale = old_w/old_h;

    double w  = (ui->lineEditH->text().toDouble())*scale;
    ui->lineEditW->setText(QString::number(w));
}

void PostionForm::on_lineEditW_editingFinished()
{
    double scale = old_h/old_w;
    double h  = scale*(ui->lineEditW->text().toDouble());
    ui->lineEditH->setText(QString::number(h));
}
