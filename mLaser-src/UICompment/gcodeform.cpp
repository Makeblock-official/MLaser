#include <QFileDialog>
#include <QTextStream>

#include "gcodeform.h"
#include "ui_gcodeform.h"

GcodeForm::GcodeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GcodeForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("G代码模式"));
    QIcon fix(QPixmap(":/Source/icon/logo.png"));
    setWindowIcon(fix);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frame);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

GcodeForm::~GcodeForm()
{
    delete ui;
}

void GcodeForm::on_btnLoad_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Gcode"),".",tr("Gcode (*.gcode)"));
    QFile gcode(file);
    if(!gcode.open(QIODevice::ReadWrite))
        return ;
    QTextStream out(&gcode);
    QString strAll = out.readAll();
    ui->textBrowser->append(strAll);
    gcode.close();
}

void GcodeForm::on_btnClear_clicked()
{
    ui->textBrowser->clear();
}

void GcodeForm::on_btnInput_clicked()
{
    QString cmd = ui->lineEdit->text();
    emit Sig_RCmds(cmd);
}
void GcodeForm::languageUpdate()
{
    ui->retranslateUi(this);
}
