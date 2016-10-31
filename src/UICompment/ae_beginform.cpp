#include "ae_beginform.h"
#include "ui_ae_beginform.h"

AE_BeginForm::AE_BeginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AE_BeginForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle("开始检测");
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameBegin);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
    movie = new QMovie(":/Source/gif/M.gif");
    ui->labelMovie->setMovie(movie);
    movie->start();
//    ui->textBrowser->append(tr(" 1.注意第一次测试\n 2.激光强度调节\n 3.调节到最合适的点\n"));
}

AE_BeginForm::~AE_BeginForm()
{
    delete ui;
}

void AE_BeginForm::on_btnPre_clicked()
{
    emit Sig_AE("pre");
}

void AE_BeginForm::on_pushButton_clicked()
{
    emit Sig_AE("next");
}
void AE_BeginForm::languageUpdate()
{
    ui->retranslateUi(this);
}
//开始测试，打印一个默认的模型
void AE_BeginForm::on_pushButton_2_clicked()
{
    QStringList strlist;
    strlist.append("./firsttest.gcode");
    emit Sig_PrintTestFile(strlist);
}
