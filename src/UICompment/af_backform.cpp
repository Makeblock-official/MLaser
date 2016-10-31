#include "af_backform.h"
#include "ui_af_backform.h"

AF_BackForm::AF_BackForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AF_BackForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(" ");
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameBack);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

AF_BackForm::~AF_BackForm()
{
    delete ui;
}

void AF_BackForm::on_pushButton_clicked()
{
    emit Sig_AF("back");
}
void AF_BackForm::languageUpdate()
{
    ui->retranslateUi(this);
}
