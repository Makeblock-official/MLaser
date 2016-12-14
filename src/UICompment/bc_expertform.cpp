#include <QSettings>
#include <QDebug>
#include <QListView>
#include "bc_expertform.h"
#include "ui_bc_expertform.h"

BC_ExpertForm::BC_ExpertForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BC_ExpertForm)
{
    ui->setupUi(this);
    ui->frameCarving->hide();
    QString path = QCoreApplication::applicationDirPath();
    QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    int c_index = confsetting->value("m_value").toInt();
    int c_type = confsetting->value("carv_type").toInt();
    confsetting->endGroup();
    ui->comboMaterial->setCurrentIndex(c_index);
    ui->comboBoxPicType->setCurrentIndex(c_type);
    pPowerOn = false;

    ui->comboMaterial->setStyleSheet("QComboBox{border:1px solid gray;}"
                                     "QComboBox QAbstractItemView::item{height:25px;}");
    ui->comboMaterial->setView(new QListView());
    ui->comboBoxPicType->setStyleSheet("QComboBox{border:1px solid gray;}"
                                       "QComboBox QAbstractItemView::item{height:25px;}");
    ui->comboBoxPicType->setView(new QListView());

    ui->btnSlicer->setStyleSheet(QString("QPushButton{background-color:rgb(107,106,110);}"));
    ui->btnCarve->setStyleSheet(QString("QPushButton{background-color:rgb(87,86,90);}"));
    pStop = true;


}

BC_ExpertForm::~BC_ExpertForm()
{
    delete ui;
}

void BC_ExpertForm::setElementEnable(bool bEnable)
{
//    ui->btnSlicer->setEnabled(bEnable);
//    ui->btnCarve->setEnabled(bEnable);
//    ui->btnOpenPanel->setEnabled(bEnable);
//    ui->frameCarving->setEnabled(bEnable);
//    ui->frameControl->setEnabled(bEnable);
//    ui->btnSetHome->setEnabled(bEnable);
//    ui->btnPreview->setEnabled(bEnable);
//    ui->btnPointLaser->setEnabled(bEnable);
//    ui->btnPower->setEnabled(bEnable);
}
QString BC_ExpertForm::getMaterialType()
{
    return ui->comboMaterial->currentText();
}

//拖拽按鈕
void BC_ExpertForm::on_toggleLaser_valueChanged(int value)
{
    //    if(value>=1)
    //    {
    //        ui->toggleLaser->setValue(2);
    //        ui->labelTag->setText(tr("激光已打开"));
    //        emit Sig_LaserOn("M4 P30");
    //    }
    //    else
    //    {
    //        ui->toggleLaser->setValue(0);
    //        ui->labelTag->setText(tr("激光已关闭"));
    //        emit Sig_LaserOn("M4 P0");
    //    }
}
//切割
void BC_ExpertForm::on_btnSlicer_clicked()
{
    //判断是否有文件导入
    emit Sig_Cutting();
    ui->frameControl->show();
    ui->frameCarving->hide();
    ui->btnSlicer->setStyleSheet(QString("QPushButton{background-color:rgb(107,106,110);}"));
    ui->btnCarve->setStyleSheet(QString("QPushButton{background-color:rgb(87,86,90);}"));
}
//雕刻
void BC_ExpertForm::on_btnCarve_clicked()
{
    emit Sig_Carve();
    ui->frameControl->hide();
    ui->frameCarving->show();
    ui->btnCarve->setStyleSheet(QString("QPushButton{background-color:rgb(107,106,110);}"));
    ui->btnSlicer->setStyleSheet(QString("QPushButton{background-color:rgb(87,86,90);}"));
}
//开操作平台
void BC_ExpertForm::on_btnOpenPanel_clicked()
{
    emit Sig_BD("showpannel");
}

//开始
void BC_ExpertForm::on_btnStart_clicked()
{
    if(pStop)
    {
        on_btnOK1_clicked();
        on_btnOK2_clicked();
        emit Sig_Print();
        setElementEnable(false);
    }
    else
    {
        emit Sig_Stop();
    }
}

void BC_ExpertForm::setButtonIcon(bool bp)
{
//    setElementEnable(false);
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

void BC_ExpertForm::on_btnStop_clicked()
{
    emit Sig_Cancle();
    setElementEnable(true);
}
//雕刻预览
void BC_ExpertForm::on_btnPreview_clicked()
{
    emit Sig_Preview();
}
//专家模式下，设置打印原点
void BC_ExpertForm::on_btnSetHome_clicked()
{
    emit Sig_LaserOn("G92 X0 Y0");
}
//材料变更
void BC_ExpertForm::on_comboMaterial_activated(int index)
{
    //每种材料对应了config中打印的参数
    QString mpath = QCoreApplication::applicationDirPath(); 
	QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* psetting = new QSettings(mallPath,QSettings::IniFormat);
    QString material;
    switch(index)
    {
       case -1:
	   	   break;
       case 0: material = "Wood";
           break;
       case 1: material = "Leather";
           break;
       case 2: material = "Paper";
           break;
       default: break;
    }
    psetting->beginGroup(material);
    QString speed = psetting->value("laserSpeed").toString();
    QString travlspeed = psetting->value("laserTravelSpeed").toString();
    QString times = psetting->value("times").toString();
    QString power = psetting->value("laserPowerHigh").toString();
    psetting->endGroup();
    ui->lineEditPixSpeed->setText(speed);
    ui->lineEditSpaceSpeed->setText(travlspeed);
    ui->lineEditPower->setText(power);
    ui->lineEditRepeat->setText(times);
    QString path = QCoreApplication::applicationDirPath();
    QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("material",material);
    confsetting->setValue("m_value",index);
    confsetting->endGroup();
}
//编辑打印次数
void BC_ExpertForm::on_lineEditRepeat_returnPressed()
{
	QString mpath = QCoreApplication::applicationDirPath(); 
	QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);

    QSettings* psetting = new QSettings(mallPath,QSettings::IniFormat);
    QString m ;//= ui->comboMaterial->currentText();
    switch(ui->comboMaterial->currentIndex())
    {
       case -1:
           break;
       case 0: m = "Wood";
           break;
       case 1: m = "Leather";
           break;
       case 2: m = "Paper";
           break;
       default: break;
    }
    psetting->beginGroup(m);
    QString times = ui->lineEditRepeat->text();
    psetting->setValue("times",times);
    psetting->endGroup();
}

//设置打印光强
void BC_ExpertForm::on_lineEditPower_returnPressed()
{
    QString mpath = QCoreApplication::applicationDirPath(); 
	QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* psetting = new QSettings(mallPath,QSettings::IniFormat);
    QString m ;//= ui->comboMaterial->currentText();
    switch(ui->comboMaterial->currentIndex())
    {
       case -1:
        break;
       case 0: m = "Wood";
           break;
       case 1: m = "Leather";
           break;
       case 2: m = "Paper";
           break;
       default: break;
    }
    psetting->beginGroup(m);
    QString power = ui->lineEditPower->text();
    if(power.toInt()>100)
    power = QString("100");
    psetting->setValue("laserPowerHigh",power);
    psetting->endGroup();
    ui->lineEditPower->setText(power);
}
//设置打印速度
void BC_ExpertForm::on_lineEditPixSpeed_returnPressed()
{
    QString mpath = QCoreApplication::applicationDirPath(); 
	QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* psetting = new QSettings(mallPath,QSettings::IniFormat);
    QString m ;//= ui->comboMaterial->currentText();
    switch(ui->comboMaterial->currentIndex())
    {
       case -1:
           break;
       case 0: m = "Wood";
           break;
       case 1: m = "Leather";
           break;
       case 2: m = "Paper";
           break;
       default: break;
    }
    psetting->beginGroup(m);
    QString speed = ui->lineEditPixSpeed->text();
    psetting->setValue("laserSpeed",speed);
    psetting->endGroup();
}
//空走速度
void BC_ExpertForm::on_lineEditSpaceSpeed_returnPressed()
{
    QString mpath = QCoreApplication::applicationDirPath(); 
	QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* psetting = new QSettings(mallPath,QSettings::IniFormat);
    QString m ;//= ui->comboMaterial->currentText();
    switch(ui->comboMaterial->currentIndex())
    {
       case -1:
       case 0: m = "Wood";
           break;
       case 1: m = "Leather";
           break;
       case 2: m = "Paper";
           break;
       default: break;
    }
    psetting->beginGroup(m);
    QString speed = ui->lineEditSpaceSpeed->text();
    psetting->setValue("laserTravelSpeed",speed);
    psetting->endGroup();
}

void BC_ExpertForm::languageUpdate()
{
    ui->retranslateUi(this);
    QString path = QCoreApplication::applicationDirPath();
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    int c_index = confsetting->value("m_value").toInt();
    int c_type = confsetting->value("carv_type").toInt();
    QString p_speed = confsetting->value("p_move_speed").toString();
    QString p_power = confsetting->value("p_power").toString();
    QString p_power_time = confsetting->value("p_power_time").toString();
    QString r_per_pixcel = confsetting->value("r_per_pixcel").toString();
    confsetting->endGroup();
    ui->comboMaterial->setCurrentIndex(c_index);
    ui->comboBoxPicType->setCurrentIndex(c_type);
    ui->comboMaterial->setCurrentText(ui->comboMaterial->itemText(c_index));
    ui->comboBoxPicType->setCurrentText(ui->comboBoxPicType->itemText(c_type));
    ui->lineEditCarvPiwer->setText(p_power);
    ui->lineEditCarvPixcel->setText(r_per_pixcel);
    ui->lineEditCarvSpeed->setText(p_speed);
    ui->lineEditCarvTime->setText(p_power_time);

    QString mpath = QCoreApplication::applicationDirPath(); 
	QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings* psetting = new QSettings(mallPath,QSettings::IniFormat);
	QString material;
    switch(c_index)
    {
       case -1:
	   	   break;
       case 0: material = "Wood";
           break;
       case 1: material = "Leather";
           break;
       case 2: material = "Paper";
           break;
       default: break;
    }
    psetting->beginGroup(material);
    QString speed = psetting->value("laserSpeed").toString();
    QString travlspeed = psetting->value("laserTravelSpeed").toString();
    QString times = psetting->value("times").toString();
    QString power = psetting->value("laserPowerHigh").toString();
    psetting->endGroup();
    ui->lineEditPixSpeed->setText(speed);
    ui->lineEditSpaceSpeed->setText(travlspeed);
    ui->lineEditPower->setText(power);
    ui->lineEditRepeat->setText(times);


}
void BC_ExpertForm::slotUpdateParmeter()
{
    QString dspath = QCoreApplication::applicationDirPath();
    QString dsname = "/ds/config.ini";
    QString dsallPath = QString("%1%2").arg(dspath).arg(dsname);
    QSettings *pset = new QSettings(dsallPath,QSettings::IniFormat);
    pset->beginGroup("laser");
    int type = pset->value("type").toInt();
    pset->endGroup();
    if(type ==0)        //切割
    {
        //获取材料相关的参数设置

        QSettings * pset = new QSettings(dsallPath,QSettings::IniFormat);
        pset->beginGroup("laser");
        QString times = pset->value("times").toString();
        QString laserPowerHigh=pset->value("laserPowerHigh").toString();
        QString laserPowerLow=pset->value("laserPowerLow").toString();
        QString laserSpeed=pset->value("laserSpeed").toString();
        QString laserTravelSpeed = pset->value("laserTravelSpeed").toString();
        pset->endGroup();
        QString path1 = QCoreApplication::applicationDirPath();
        QString name1 = "/Config.ini";
        QString allPath1 = QString("%1%2").arg(path1).arg(name1);
        QSettings * config = new QSettings(allPath1,QSettings::IniFormat);
        config->beginGroup("laser");
        config->setValue("times",times);
        config->setValue("laserPowerHigh",laserPowerHigh);
        config->setValue("laserPowerLow",laserPowerLow);
        config->setValue("laserSpeed",laserSpeed);
        config->setValue("laserTravelSpeed",laserTravelSpeed);
        config->endGroup();
        ui->lineEditPower->setText(laserPowerHigh);
        ui->lineEditRepeat->setText(times);
        ui->lineEditSpaceSpeed->setText(laserTravelSpeed);
        ui->lineEditPixSpeed->setText(laserSpeed);
        on_btnOK1_clicked();


    }
    else if(type==1)    //雕刻
    {
        //获取雕刻相关的参数设置
        QSettings * pset = new QSettings(dsallPath,QSettings::IniFormat);
        pset->beginGroup("laser");
        int carv_type=pset->value("carv_type").toInt();
        QString p_power_time = pset->value("p_power_time").toString();
        QString p_move_speed = pset->value("p_move_speed").toString();
        QString p_power = pset->value("p_power").toString();
        QString r_per_pixcel = pset->value("r_per_pixcel").toString();
        pset->endGroup();

        QString path1 = QCoreApplication::applicationDirPath();
        QString name1 = "/Config.ini";
        QString allPath1 = QString("%1%2").arg(path1).arg(name1);
        QSettings * config = new QSettings(allPath1,QSettings::IniFormat);
        config->beginGroup("laser");
        config->setValue("carv_type",carv_type);
        config->setValue("p_power_time",p_power_time);
        config->setValue("p_move_speed",p_move_speed);
        config->setValue("p_power",p_power);
        config->setValue("r_per_pixcel",r_per_pixcel);
        config->endGroup();

        ui->lineEditCarvTime->setText(p_power_time);
        ui->lineEditCarvSpeed->setText(p_move_speed);
        ui->lineEditCarvPiwer->setText(p_power);
        ui->lineEditCarvPixcel->setText(r_per_pixcel);
        on_btnOK2_clicked();
    }

}

//灰度雕刻，修改移动速度
void BC_ExpertForm::on_lineEditCarvSpeed_returnPressed()
{
    QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("p_move_speed",ui->lineEditCarvSpeed->text());
    confsetting->endGroup();
}

//灰度雕刻，修改激光强度---power
void BC_ExpertForm::on_lineEditCarvPiwer_returnPressed()
{
    QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("p_power",ui->lineEditCarvPiwer->text());
    confsetting->endGroup();
}

//灰度雕刻，修改烧刻时间
void BC_ExpertForm::on_lineEditCarvTime_returnPressed()
{
    QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("p_power_time",ui->lineEditCarvTime->text());
    confsetting->endGroup();
}

//灰度雕刻，修改像素间隔
void BC_ExpertForm::on_lineEditCarvPixcel_returnPressed()
{
    QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("r_per_pixcel",ui->lineEditCarvPixcel->text());
    confsetting->setValue("v_per_pixcel",ui->lineEditCarvPixcel->text());
    confsetting->endGroup();
}

//点射按下
void BC_ExpertForm::on_btnPointLaser_pressed()
{
    int power = ui->lineEditPower->text().toInt();
    QString cmd = tr("M4 P%1").arg(power);
    emit Sig_LaserOn(cmd);
}
//点射释放
void BC_ExpertForm::on_btnPointLaser_released()
{
    emit Sig_LaserOn("M4 P0");
}

//修改处理图片的算法
void BC_ExpertForm::on_comboBoxPicType_activated(int index)
{
    QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("carv_type",index);
    confsetting->endGroup();
}

void BC_ExpertForm::on_btnPower_clicked()
{
    if(pPowerOn)
    {
        emit Sig_LaserOn("M4 P0");
        pPowerOn = false;
    }
    else
    {
        Sig_LaserOn("M4 P25");
        pPowerOn = true;
    }
}
//切割参数使能
void BC_ExpertForm::on_btnOK1_clicked()
{
    QString times = ui->lineEditRepeat->text();
    QString speed = ui->lineEditPixSpeed->text();
    QString spaceSpeed = ui->lineEditSpaceSpeed->text();
    QString power = ui->lineEditPower->text();
    QString material ;
    switch(ui->comboMaterial->currentIndex())
    {
       case -1:
           break;
       case 0: material = "Wood";
           break;
       case 1: material = "Leather";
           break;
       case 2: material = "Paper";
           break;
       default: break;
    }
	QString mpath = QCoreApplication::applicationDirPath(); 
	QString mname = "/material.ini";
    QString mallPath = QString("%1%2").arg(mpath).arg(mname);
    QSettings * pset = new QSettings(mallPath,QSettings::IniFormat);
    pset->beginGroup(material);
    pset->setValue("times",times);
    pset->setValue("laserPowerHigh",power);
    pset->setValue("laserSpeed",speed);
    pset->setValue("laserTravelSpeed",spaceSpeed);

    QString path = QCoreApplication::applicationDirPath();
    QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* confsetting = new QSettings(allPath,QSettings::IniFormat);
    confsetting->beginGroup("laser");
    confsetting->setValue("material",material);
    confsetting->setValue("m_value",ui->comboMaterial->currentIndex());
    confsetting->endGroup();
    pset->endGroup();
}
//雕刻参数使能
void BC_ExpertForm::on_btnOK2_clicked()
{
    QString power = ui->lineEditCarvPiwer->text();
    QString pixcel =ui->lineEditCarvPixcel->text();
    QString speed =ui->lineEditCarvSpeed->text();
    QString time =ui->lineEditCarvTime->text();
	QString path = QCoreApplication::applicationDirPath(); 
	QString name = "/Config.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings * pset = new QSettings(allPath,QSettings::IniFormat);
    pset->beginGroup("laser");
    pset->setValue("p_power",power);
    pset->setValue("r_per_pixcel",pixcel);
    pset->setValue("v_per_pixcel",pixcel);
    pset->setValue("p_move_speed",speed);
    pset->setValue("p_power_time",time);
    pset->endGroup();
}
//eidt finish
void BC_ExpertForm::on_lineEditRepeat_editingFinished()
{
    on_btnOK1_clicked();
}

void BC_ExpertForm::on_lineEditPower_editingFinished()
{
    on_btnOK1_clicked();
}

void BC_ExpertForm::on_lineEditPixSpeed_editingFinished()
{
    on_btnOK1_clicked();
}

void BC_ExpertForm::on_lineEditSpaceSpeed_editingFinished()
{
    on_btnOK1_clicked();
}

void BC_ExpertForm::on_lineEditCarvSpeed_editingFinished()
{
    on_btnOK2_clicked();
}
void BC_ExpertForm::on_lineEditCarvPiwer_editingFinished()
{
    on_btnOK2_clicked();
}

void BC_ExpertForm::on_lineEditCarvTime_editingFinished()
{
    on_btnOK2_clicked();
}

void BC_ExpertForm::on_lineEditCarvPixcel_editingFinished()
{
    on_btnOK2_clicked();
}
