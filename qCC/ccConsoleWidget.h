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

// Local
#include "ccConsoleSettings.h"
#include "ccConsoleTheme.h"
#include "ccLogFormatter.h"

// Qt
#include <QPlainTextEdit>
#include <QDateTime>

//! Terminal-like console widget
/** QPlainTextEdit-based console widget with theme support,
    colored output, and customizable formatting.
**/
class ccConsoleWidget : public QPlainTextEdit
{
	Q_OBJECT

  public:
	//! Constructor
	explicit ccConsoleWidget(QWidget* parent = nullptr);

	//! Destructor
	~ccConsoleWidget() override;

	//! Append a log message
	/** \param message the message text
	    \param level log level flags
	    \param timestamp timestamp of the message
	    \param category optional category tag
	 **/
	void appendMessage(const QString&   message,
	                   int              level,
	                   const QDateTime& timestamp = QDateTime::currentDateTime(),
	                   const QString&   category = QString());

	//! Get console settings
	ccConsoleSettings& settings() { return m_settings; }

	//! Get console settings (const)
	const ccConsoleSettings& settings() const { return m_settings; }

	//! Get console theme
	ccConsoleTheme& theme() { return m_theme; }

	//! Get console theme (const)
	const ccConsoleTheme& theme() const { return m_theme; }

	//! Apply current theme
	void applyTheme();

	//! Clear the console
	void clearConsole();

	//! Get total line count
	int lineCount() const;

	//! Export console content as plain text
	QString exportPlainText() const;

	//! Set whether to auto-scroll to bottom
	void setAutoScroll(bool enable);

	//! Get auto-scroll state
	bool autoScroll() const { return m_autoScroll; }

  public slots:
	//! Load settings from persistent storage
	void loadSettings();

	//! Save settings to persistent storage
	void saveSettings();

  protected:
	//! Override to handle palette changes (for theme switching)
	void changeEvent(QEvent* event) override;

	//! Override for custom context menu
	void contextMenuEvent(QContextMenuEvent* event) override;

	//! Override for copy support
	void keyPressEvent(QKeyEvent* event) override;

	//! Trim lines if exceeding max line limit
	void trimLines();

	//! Check if current palette is dark
	bool isDarkTheme() const;

  private:
	ccConsoleSettings m_settings;
	ccConsoleTheme    m_theme;
	ccLogFormatter    m_formatter;
	bool              m_autoScroll;
	int               m_lineCount;
};
