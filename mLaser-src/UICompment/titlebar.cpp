#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include "titlebar.h"

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif
/*
 *编码风格:
 * 1. 私有变量，共有变量,以m_开头，头字母小写，后面单词再大写
 * 2. 类名均大写
 * 3. 函数名用驼峰命名法
 *
 */
TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(30);

    m_pIconLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);
    m_pMinimizeButton = new QPushButton(this);
    m_pMaximizeButton = new QPushButton(this);
    m_pCloseButton = new QPushButton(this);
//    this->setStyleSheet(QString("QWidget{background-color:gray;}"));
    m_pIconLabel->setFixedSize(16, 16);
//    m_pIconLabel->setPixmap(QPixmap(":/Source/icon/logo.jpg"));
    m_pIconLabel->setScaledContents(true);

    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pTitleLabel->setStyleSheet(QString("QLabel{color:white;}"));
    m_pTitleLabel->setFixedHeight(22);

    m_pMinimizeButton->setFixedSize(27, 22);
    m_pMaximizeButton->setFixedSize(27, 22);
    m_pCloseButton->setFixedSize(27, 22);

    m_pTitleLabel->setObjectName("whiteLabel");
    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");

    this->setStyleSheet(QString("QPushButton{border:none;}"));
    m_pMinimizeButton->setToolTip("Minimize");
    QPixmap min_icon(":/Source/icon/min.png");
    m_pMinimizeButton->setIcon(min_icon);
    m_pMinimizeButton->setIconSize(min_icon.rect().size());

    m_pMaximizeButton->setToolTip("Maximize");
    QPixmap max_icon(":/Source/icon/max.png");
    m_pMaximizeButton->setIcon(max_icon);
    m_pMaximizeButton->setIconSize(max_icon.rect().size());

    m_pCloseButton->setToolTip("Close");
    QPixmap close_icon(":/Source/icon/close.png");
    m_pCloseButton->setIcon(close_icon);
    m_pCloseButton->setIconSize(close_icon.rect().size());

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_pIconLabel);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pMinimizeButton);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pMaximizeButton);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pCloseButton);
    pLayout->setSpacing(5);
    pLayout->setContentsMargins(10, 0, 0, 0);
    QFrame *f = new QFrame();
    f->setLayout(pLayout);
    f->setObjectName("titleFrame");
    f->setStyleSheet(QString("QFrame{background-color:gray;}"));

    setLayout(pLayout);

    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

TitleBar::~TitleBar()
{

}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    emit m_pMaximizeButton->clicked();
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
       event->ignore();
#else
#endif
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            m_pTitleLabel->setText(pWidget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget *pWidget = qobject_cast<QWidget *>(obj);
        if (pWidget)
        {
            QIcon icon = pWidget->windowIcon();
            m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
            return true;
        }
    }
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {

        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized();
//            pWindow->hide();
        }
        else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close();
        }
    }
}

void TitleBar::updateMaximize()
{
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_pMaximizeButton->setToolTip(tr("Restore"));
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
            m_pMaximizeButton->setToolTip(tr("Maximize"));
        }

        m_pMaximizeButton->setStyle(QApplication::style());
    }
}
