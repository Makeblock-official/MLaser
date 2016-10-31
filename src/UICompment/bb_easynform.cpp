#include "bb_easynform.h"
#include "ui_bb_easynform.h"

BB_EasynForm::BB_EasynForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BB_EasynForm)
{
    ui->setupUi(this);
//    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    ui->btnSlice->setStyleSheet(QString("QPushButton{background-color:rgb(107,106,110);}"));
    ui->btnDraw->setStyleSheet(QString("QPushButton{background-color:rgb(87,86,90);}"));
    pStop = true;
}

BB_EasynForm::~BB_EasynForm()
{
    delete ui;
}
void BB_EasynForm::setElementEnable(bool bEnable)
{

//    if(bEnable)
//    {
//        ui->btnDraw->setEnabled(true);
//        ui->btnSlice->setEnabled(true);
//    }
//    else
//    {
//        ui->btnDraw->setEnabled(false);
//        ui->btnSlice->setEnabled(false);
//    }
}
void BB_EasynForm::on_btnPre_clicked()
{
    emit Sig_BB("pre");
}
//切割
void BB_EasynForm::on_btnSlice_clicked()
{
    ui->btnSlice->setStyleSheet(QString("QPushButton{background-color:rgb(107,106,110);}"));
    ui->btnDraw->setStyleSheet(QString("QPushButton{background-color:rgb(87,86,90);}"));
    emit Sig_Cutting();
}
//雕刻
void BB_EasynForm::on_btnDraw_clicked()
{
    ui->btnSlice->setStyleSheet(QString("QPushButton{background-color:rgb(87,86,90);}"));
    ui->btnDraw->setStyleSheet(QString("QPushButton{background-color:rgb(107,106,110);}"));
    emit Sig_Carve();
}
//开始
void BB_EasynForm::on_btnStart_clicked()
{
    if(pStop)
    {
        emit Sig_Print();
    }
    else
    {
        emit Sig_Stop();
    }
}
void BB_EasynForm::setButtonIcon(bool bp)
{
    if(bp)
    {
        pStop = true;
        ui->btnStart->setIcon(QIcon(":/Source/icon/print_begin.png"));
        ui->btnStart->setText(tr("开始"));
    }
    else
    {
        pStop = false;
        ui->btnStart->setIcon(QIcon(":/Source/icon/print_stop.png"));
        ui->btnStart->setText(tr("暂停"));
    }
}
//停止
void BB_EasynForm::on_btnStop_clicked()
{
    emit Sig_Cancle();
}
//显示操作台
void BB_EasynForm::on_btnPannel_clicked()
{
    emit Sig_BB("showpannel");
}
void BB_EasynForm::languageUpdate()
{
    ui->retranslateUi(this);
}
