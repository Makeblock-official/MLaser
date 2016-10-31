#include "ba_easyform.h"
#include "ui_ba_easyform.h"

BA_EasyForm::BA_EasyForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BA_EasyForm)
{
    ui->setupUi(this);
//    setWindowFlags(Qt::FramelessWindowHint);
    this->update();
//    menu = ui->btnMateril->getMenu();
//    menu->addAction(tr("木板"),this,SLOT(slotMa()));
//    menu->addAction(tr("纸片"),this,SLOT(slotMb()));
//    menu->addAction(tr("帆布"),this,SLOT(slotMc()));
//    menu->addAction(tr("橡胶"),this,SLOT(slotMd()));

}

BA_EasyForm::~BA_EasyForm()
{
    delete ui;
}

//返回主页
void BA_EasyForm::on_btnBackHome_clicked()
{
     emit Sig_BA("backhome");
}
//下一页
void BA_EasyForm::on_btnNext_clicked()
{
    emit Sig_BA("next");
}
//显示操作台
void BA_EasyForm::on_btnPannel_clicked()
{
    emit Sig_BA("showpannel");
}
//添加文字
void BA_EasyForm::on_btnAddText_clicked()
{
    emit Sig_BA("showtext");
}
//设置当前位置为雕刻起点
void BA_EasyForm::on_btnSetHome_clicked()
{
    emit Sig_BA("sethome");
}

//设置打印材料
void BA_EasyForm::slotMa()
{

}
void BA_EasyForm::slotMb()
{

}
void BA_EasyForm::slotMc()
{

}

void BA_EasyForm::slotMd()
{

}
//雕刻预览
void BA_EasyForm::on_btnMateril_clicked()
{
    emit Sig_Preview();
}
void BA_EasyForm::languageUpdate()
{
    ui->retranslateUi(this);
}
