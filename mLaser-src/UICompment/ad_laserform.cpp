#include "ad_laserform.h"
#include "ui_ad_laserform.h"

AD_LaserForm::AD_LaserForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AD_LaserForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("检测串口");
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameLaser);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    //    ui->webView->setHtml();
    bPower = false;
}

AD_LaserForm::~AD_LaserForm()
{
    delete ui;
}

void AD_LaserForm::on_btnPre_clicked()
{
    emit Sig_AD("pre");
}

void AD_LaserForm::on_btnLaser_clicked()
{
    if(bPower)
    {
        bPower = false;
        emit Sig_ToLaser("M4 P0");
    }

    else
    {
        bPower = true;
        emit Sig_ToLaser("M4 P25");
    }

}

void AD_LaserForm::on_btnNext_clicked()
{
    emit Sig_AD("next");
}
void AD_LaserForm::languageUpdate()
{
    ui->retranslateUi(this);
}
