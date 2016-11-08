#include <QSettings>
#include <QDebug>
#include <qmath.h>
#include <QPainter>
#include "textform.h"
#include "ui_textform.h"

TextForm::TextForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("文字编辑"));
    QIcon fix(QPixmap(":/Source/icon/logo.png"));
    setWindowIcon(fix);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frameText);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    //font setting
    blod = false;
    bItly = false;
	QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("font");
    int pfont = psetting->value("font").toInt();
    int size = psetting->value("font_size").toInt();
    ui->comboBoxFontSize->setCurrentIndex(size);
    ui->fontComboBox->setCurrentIndex(pfont);
    psetting->endGroup();

    fonts = ui->fontComboBox->currentFont();
    fonts.setPixelSize(ui->comboBoxFontSize->currentText().toInt());
    ui->plainTextEdit->setFont(fonts);
}

TextForm::~TextForm()
{
    delete ui;
}


void TextForm::on_btnOk_clicked()
{
    QString path = QCoreApplication::applicationDirPath();
    QString contents = ui->plainTextEdit->toPlainText();
    QStringList pi = contents.split("\n");
    QFontMetricsF fm(fonts);
    qreal a = 0;
    qreal b = fm.height();

    for(int i=0;i<pi.size();i++)
    {
        a = qMax(a,fm.width(pi.at(i)));
    }
    QImage text_pic(a,(b+3)*pi.size(),QImage::Format_ARGB32);
    QRectF f(0,0,a,(b+3)*pi.size());

    QPainter painter(&text_pic);
    painter.fillRect(text_pic.rect(), Qt::white);
    painter.setFont(fonts);

    painter.setCompositionMode(QPainter::CompositionMode_Source);

    painter.drawText(f,contents);
    painter.end();
    QFile::remove(path + "/font1.jpg");
    text_pic.save(path + "/font1.jpg","JPEG",100);
    emit Sig_AddFont(path + "/font1.jpg");
    this->close();
}

void TextForm::on_fontComboBox_activated(const QString &arg1)
{
    fonts = ui->fontComboBox->currentFont();
    fonts.setPointSize(ui->comboBoxFontSize->currentText().toInt());
    ui->plainTextEdit->setFont(fonts);
    updateConfig();
}

void TextForm::on_btnBlod_clicked()
{
    if(blod)
    {
        blod = false;
    }
    else
    {
        blod = true;
    }
    fonts.setBold(blod);
    ui->plainTextEdit->setFont(fonts);
}

void TextForm::on_comboBoxFontSize_activated(const QString &arg1)
{
    fonts.setPixelSize(arg1.toInt());
    ui->plainTextEdit->setFont(fonts);
    updateConfig();
}
void TextForm::updateConfig()
{
    QString lpath = QCoreApplication::applicationDirPath(); 
	QString lname = "/mLaser.ini";
    QString lallPath = QString("%1%2").arg(lpath).arg(lname);
    QSettings* psetting = new QSettings(lallPath,QSettings::IniFormat);
    psetting->beginGroup("font");
    int font = ui->fontComboBox->currentIndex();
    int font_size = ui->comboBoxFontSize->currentIndex();
    psetting->setValue("font",font);
    psetting->setValue("font_size",font_size);
    psetting->endGroup();
}

void TextForm::on_btnCancle_clicked()
{
    this->close();
}
//斜体
void TextForm::on_btnItly_clicked()
{
    if(bItly)
    {
        fonts.setItalic(false);
        bItly = false;
    }
    else
    {
        fonts.setItalic(true);
        bItly = true;
    }
}
void TextForm::languageUpdate()
{
    ui->retranslateUi(this);
}
