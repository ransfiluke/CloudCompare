#pragma once

// ##########################################################################
// #                                                                        #
// #                              CLOUDCOMPARE                              #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 or later of the License.      #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #          COPYRIGHT: EDF R&D / TELECOM ParisTech (ENST-TSI)             #
// #                                                                        #
// ##########################################################################

// Qt
#include <QWidget>

class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;

//! Console search toolbar widget
/** Provides a search interface for the console with next/previous navigation,
    case sensitivity, and regex support.
**/
class ccConsoleSearchWidget : public QWidget
{
	Q_OBJECT

  public:
	//! Constructor
	explicit ccConsoleSearchWidget(QWidget* parent = nullptr);

	//! Get search text
	QString searchText() const;

	//! Get case sensitive state
	bool caseSensitive() const;

	//! Get regex state
	bool useRegex() const;

	//! Set focus to search field
	void focusSearchField();

  public slots:
	//! Update match counter
	void updateMatchCount(int current, int total);

  signals:
	//! Emitted when search text changes
	void searchChanged(const QString& text);

	//! Emitted when find next is clicked
	void findNext();

	//! Emitted when find previous is clicked
	void findPrevious();

	//! Emitted when close is clicked
	void closeRequested();

	//! Emitted when case sensitive changes
	void caseSensitiveChanged(bool enabled);

	//! Emitted when regex changes
	void regexChanged(bool enabled);

  private slots:
	void onSearchTextChanged();

  private:
	QLineEdit*   m_searchField;
	QPushButton* m_prevButton;
	QPushButton* m_nextButton;
	QPushButton* m_closeButton;
	QCheckBox*   m_caseSensitiveCheckBox;
	QCheckBox*   m_regexCheckBox;
	QLabel*      m_matchLabel;
};
