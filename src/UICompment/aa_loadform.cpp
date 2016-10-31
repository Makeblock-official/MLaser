#include "aa_loadform.h"
#include "ui_aa_loadform.h"

AA_LoadForm::AA_LoadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AA_LoadForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("引导页"));
    setWindowIcon(QIcon(":/Source/icon/logo.jpg"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameLoad);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

}

AA_LoadForm::~AA_LoadForm()
{
    delete ui;
}

void AA_LoadForm::on_btnFirst_clicked()
{
    emit Sig_AA("next");
}
//打开简单设置界面，这里需要从配置文件中读取用户设置的模式
void AA_LoadForm::on_btnLoad_clicked()
{
    emit Sig_AA("show");
}
void AA_LoadForm::languageUpdate()
{
    ui->retranslateUi(this);
}
