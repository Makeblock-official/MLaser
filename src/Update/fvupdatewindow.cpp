#include "fvupdatewindow.h"
#include "ui_fvupdatewindow.h"
#include "fvupdater.h"
#include "fvavailableupdate.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>


FvUpdateWindow::FvUpdateWindow(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::FvUpdateWindow)
{
	m_ui->setupUi(this);

	m_appIconScene = 0;

	// Delete on close
	setAttribute(Qt::WA_DeleteOnClose, true);
    m_ui->releaseNotesWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	// Set the "new version is available" string
	QString newVersString = m_ui->newVersionIsAvailableLabel->text().arg(QString::fromUtf8(FV_APP_NAME));
	m_ui->newVersionIsAvailableLabel->setText(newVersString);

	// Connect buttons
	connect(m_ui->installUpdateButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(InstallUpdate()));
	connect(m_ui->skipThisVersionButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(SkipUpdate()));
	connect(m_ui->remindMeLaterButton, SIGNAL(clicked()),
			FvUpdater::sharedUpdater(), SLOT(RemindMeLater()));
    connect(m_ui->releaseNotesWebView->page(),SIGNAL(linkClicked(QUrl)),this,SLOT(urlChange(QUrl)));

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    TitleBar *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    setWindowTitle(tr("更新"));
    setWindowIcon(QIcon(":/Source/icon/logo.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->addWidget(pTitleBar);
    pLayout->addWidget(m_ui->frame);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);
}

FvUpdateWindow::~FvUpdateWindow()
{
	m_ui->releaseNotesWebView->stop();
	delete m_ui;
}

bool FvUpdateWindow::UpdateWindowWithCurrentProposedUpdate()
{
    QString path = QCoreApplication::applicationDirPath();
    QString name = "/mLaser.ini";
    QString allPath = QString("%1%2").arg(path).arg(name);
    QSettings* psetting = new QSettings(allPath,QSettings::IniFormat);
    psetting->beginGroup("mode");
    QString language = psetting->value("language").toString();
    psetting->endGroup();
	FvAvailableUpdate* proposedUpdate = FvUpdater::sharedUpdater()->GetProposedUpdate();
	if (! proposedUpdate) {
		return false;
	}
	QString downloadString = m_ui->wouldYouLikeToDownloadLabel->text()
            .arg(QString::fromUtf8(FV_APP_NAME), proposedUpdate->GetEnclosureVersion(), FV_APP_VERSION);
	m_ui->wouldYouLikeToDownloadLabel->setText(downloadString);
	m_ui->releaseNotesWebView->stop();
    if(language == "zh")
    {
        const QUrl url1("http://learn.makeblock.com/cn/software/#mlaser");
        m_ui->releaseNotesWebView->load(url1);
    }
    else
    {
        const QUrl url2("http://learn.makeblock.com/en/software/#mlaser");
        m_ui->releaseNotesWebView->load(url2);
    }

	return true;
}

void FvUpdateWindow::closeEvent(QCloseEvent* event)
{
	FvUpdater::sharedUpdater()->updaterWindowWasClosed();
	event->accept();
}
void FvUpdateWindow::urlChange(QUrl url)
{
    qDebug()<<"aaaa"<<url;
    QDesktopServices::openUrl(url);
}

void FvUpdateWindow::languageUpdate()
{
    m_ui->retranslateUi(this);
}
