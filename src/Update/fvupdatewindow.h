#ifndef FVUPDATEWINDOW_H
#define FVUPDATEWINDOW_H

#include <QWidget>
#include "../UICompment/qcptitle.h"
class QGraphicsScene;

namespace Ui {
class FvUpdateWindow;
}

class FvUpdateWindow : public QWidget
{
	Q_OBJECT
	
public:
	explicit FvUpdateWindow(QWidget *parent = 0);
	~FvUpdateWindow();

	// Update the current update proposal from FvUpdater
	bool UpdateWindowWithCurrentProposedUpdate();

	void closeEvent(QCloseEvent* event);
    void languageUpdate();

private:
	Ui::FvUpdateWindow*	m_ui;
	QGraphicsScene* m_appIconScene;
private slots:
     void urlChange(QUrl);
};

#endif // FVUPDATEWINDOW_H
