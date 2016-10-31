#include "ah_hexform.h"
#include "ui_ah_hexform.h"

AH_HexForm::AH_HexForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AH_HexForm)
{
    ui->setupUi(this);
    hex = new HexLoadForm();
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("烧录固件"));
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameCheck);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

AH_HexForm::~AH_HexForm()
{
    delete ui;
}
//烧录固件界面显示
void AH_HexForm::on_btnHex_clicked()
{
    hex->show();
}
//下一步
void AH_HexForm::on_btnNext_clicked()
{
    emit Sig_AHNextStep();
}

void AH_HexForm::languageUpdate()
{
    ui->retranslateUi(this);
    hex->languageUpdate();
}
