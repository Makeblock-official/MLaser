#include <QDebug>
#include "qrencodeform.h"
#include "ui_qrencodeform.h"

QrencodeForm::QrencodeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrencodeForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("二维码"));
    QIcon fix(QPixmap(":/Source/icon/logo.png"));
    setWindowIcon(fix);

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(ui->frame);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    ui->comboBox->setStyleSheet("QComboBox{border:1px solid gray;}"
                                "QComboBox QAbstractItemView::item{height:25px;}");
    ui->comboBox->setView(new QListView());

    //生成本机IP地址二维码
    renders = new TcQrencode();
    QStringList localIP;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            localIP.append(address.toString());
            qDebug() << localIP;
        }
    }
    ui->comboBox->addItems(localIP);



}

QrencodeForm::~QrencodeForm()
{
    delete ui;
}

void QrencodeForm::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString localIP = "http://"+arg1+":8080/files/upload.html";
    localIP = localIP.toLower();
    qDebug()<<localIP;
    QImage s = renders->encodeImage(localIP,8);
    ui->labelCode->setPixmap(QPixmap::fromImage(s));
}
void QrencodeForm::languageUpdate()
{
    ui->retranslateUi(this);
}
void QrencodeForm::rencodeShow()
{
    QString path = QCoreApplication::applicationDirPath();
    qDebug()<<"adasdaqweqeqweqw";
    QImage *img = new QImage;
    if(! ( img->load(path + "/qr.png") ) ) //加载图像
    {
      delete img;
      return;
    }
    ui->labelCode->setPixmap(QPixmap::fromImage(*img));
}
