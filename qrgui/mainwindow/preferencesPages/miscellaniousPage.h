#pragma once

#include "page.h"

namespace Ui {
	class PreferencesMiscellaniousPage;
}

class PreferencesMiscellaniousPage : public PreferencesPage
{
	Q_OBJECT

public:
	explicit PreferencesMiscellaniousPage(QWidget *parent = 0);
	~PreferencesMiscellaniousPage();

	void save();

signals:
	void iconsetChanged();

protected:
	void changeEvent(QEvent *e);

private slots:
	void browseImagesPath();

private:
	Ui::PreferencesMiscellaniousPage *mUi;

	QString mLastIconsetPath;
};
