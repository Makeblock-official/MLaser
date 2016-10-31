#include "firmsettingform.h"
#include "ui_firmsettingform.h"

FirmSettingForm::FirmSettingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirmSettingForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);

    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameSerial);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

}

FirmSettingForm::~FirmSettingForm()
{
    delete ui;
}
