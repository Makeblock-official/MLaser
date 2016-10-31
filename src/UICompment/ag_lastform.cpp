#include "ag_lastform.h"
#include "ui_ag_lastform.h"

AG_LastForm::AG_LastForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AG_LastForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(" ");
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameCheck);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

AG_LastForm::~AG_LastForm()
{
    delete ui;
}
//测试通过
void AG_LastForm::on_btnPass_clicked()
{

    emit Sig_LastSuccessful();
}
//测试不通过
void AG_LastForm::on_btnFailed_clicked()
{
    emit Sig_LastFailed();
}
void AG_LastForm::languageUpdate()
{
    ui->retranslateUi(this);
}
