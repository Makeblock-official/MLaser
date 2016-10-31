#ifndef FONT2SVG_H
#define FONT2SVG_H

#include <QWidget>

namespace Ui {
class Font2Svg;
}

class Font2Svg : public QWidget
{
    Q_OBJECT

public:
    explicit Font2Svg(QWidget *parent = 0);
    ~Font2Svg();
    void FontExportSvg(QString output);
private slots:
    void on_btnExportSvg_clicked();

private:
    Ui::Font2Svg *ui;
};

#endif // FONT2SVG_H
