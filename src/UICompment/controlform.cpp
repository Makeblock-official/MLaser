#include "controlform.h"
#include "ui_controlform.h"

ControlForm::ControlForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("操控台");
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->framePannel);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

ControlForm::~ControlForm()
{
    delete ui;
}
//X-
void ControlForm::on_btnXp_clicked()
{
    emit Sig_ToLaser(tr("G91"));
    emit Sig_ToLaser(tr("G0 X-%1 F3000").arg(10));
}
//X+
void ControlForm::on_btnXa_clicked()
{
    emit Sig_ToLaser(tr("G91"));
    emit Sig_ToLaser(tr("G0 X%1 F3000").arg(10));
}
//Y+
void ControlForm::on_btnYa_clicked()
{
    emit Sig_ToLaser(tr("G91"));
    emit Sig_ToLaser(tr("G0 Y%1 F3000").arg(10));
}
//Y-
void ControlForm::on_btnYp_clicked()
{
    emit Sig_ToLaser(tr("G91"));
    emit Sig_ToLaser(tr("G0 Y-%1 F3000").arg(10));
}
//Home
void ControlForm::on_btnHome_clicked()
{
    emit Sig_ToLaser(tr("G28 X Y"));
}
void ControlForm::languageUpdate()
{
    ui->retranslateUi(this);
}
//使用方向键来调节XY的运动
void ControlForm::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Up)
    {
        on_btnYa_clicked();
    }
    else if(e->key()==Qt::Key_Down)
    {
        on_btnYp_clicked();
    }
    else if(e->key()==Qt::Key_Left)
    {
        on_btnXp_clicked();

    }
    else if(e->key()==Qt::Key_Right)
    {
        on_btnXa_clicked();

    }
    else if(e->key()==Qt::Key_Space)
    {
        on_btnHome_clicked();
    }
    //    e->ignore();
    QWidget::keyPressEvent(e);
}
