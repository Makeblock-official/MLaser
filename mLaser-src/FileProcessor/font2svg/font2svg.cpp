#include <QDebug>
#include <QWebFrame>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "font2svg.h"
#include "ui_font2svg.h"

Font2Svg::Font2Svg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Font2Svg)
{
    ui->setupUi(this);
    ui->webView->load(QUrl("qrc:/FileProcessor/font2svg/index.html"));
    this->setWindowTitle(tr("FontEdit"));
}

Font2Svg::~Font2Svg()
{
    delete ui;
}

void Font2Svg::FontExportSvg(QString output)
{
    QVariant po = ui->webView->page()->mainFrame()->evaluateJavaScript("outSVG();");
    QString outs = po.toString();
    QFile file(output);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::critical(NULL, tr("提示"), tr("无法创建文件"));
        return ;
    }
    QTextStream out(&file);
    out<<outs;
    file.close();
    this->close();
}

void Font2Svg::on_btnExportSvg_clicked()
{
    QString path = QFileDialog::getSaveFileName();
    FontExportSvg(path);
}
