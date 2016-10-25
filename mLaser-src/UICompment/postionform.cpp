#include <QVBoxLayout>
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
    setWindowTitle(tr("位置编辑"));
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
    ui->lineEditW->setText(QString::number(m.width()/10));
    ui->lineEditH->setText(QString::number(m.height()/10));
    old_w = ui->lineEditW->text().toDouble();
    old_h = ui->lineEditH->text().toDouble();

}
void PostionForm::on_btnCertern_clicked()
{
    QPointF m;
    m.setX(ui->lineEditW->text().toInt()*10);
    m.setY(ui->lineEditH->text().toInt()*10);
    emit Sig_WH(m);
}
void PostionForm::on_lineEditW_returnPressed()
{

}
//height edit finish
void PostionForm::on_lineEditH_editingFinished()
{
    double scale = old_w/old_h;

    int w  = (ui->lineEditH->text().toDouble())*scale;
    ui->lineEditW->setText(QString::number(w));
}

void PostionForm::on_lineEditW_editingFinished()
{
    double scale = old_h/old_w;
    int h  = scale*(ui->lineEditW->text().toDouble());
    ui->lineEditH->setText(QString::number(h));
}
