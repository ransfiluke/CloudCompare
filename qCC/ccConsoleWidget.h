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
#include <QVector>

class QLineEdit;
class QPushButton;
class QToolBar;
class QCheckBox;

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

	//! Export visible (filtered) messages only
	QString exportFiltered() const;

	//! Export with formatting preserved
	QString exportFormatted() const;

	//! Export to file
	/** \param filename output file path
	    \param filtered whether to export only filtered messages
	    \param includeFormatting whether to preserve formatting
	    \return true on success
	 **/
	bool exportToFile(const QString& filename, bool filtered = false, bool includeFormatting = false);

	//! Set whether to auto-scroll to bottom
	void setAutoScroll(bool enable);

	//! Get auto-scroll state
	bool autoScroll() const { return m_autoScroll; }

	//! Search for text in console
	/** \param text search query
	    \param caseSensitive whether search is case sensitive
	    \param useRegex whether to treat text as regex
	    \return number of matches found
	 **/
	int search(const QString& text, bool caseSensitive = false, bool useRegex = false);

	//! Clear search highlights
	void clearSearch();

	//! Find next occurrence
	bool findNext();

	//! Find previous occurrence
	bool findPrevious();

	//! Set log level filter
	/** \param level minimum log level to display (0-4)
	 **/
	void setLogLevelFilter(int level);

	//! Get current log level filter
	int logLevelFilter() const { return m_logLevelFilter; }

	//! Enable/disable specific log level
	/** \param level the log level to toggle
	    \param enabled whether to show this level
	 **/
	void setLogLevelEnabled(int level, bool enabled);

	//! Check if a log level is enabled
	bool isLogLevelEnabled(int level) const;

	//! Refresh display (rebuild from stored messages with current filters)
	void refreshDisplay();

  public slots:
	//! Load settings from persistent storage
	void loadSettings();

	//! Save settings to persistent storage
	void saveSettings();

	//! Show settings dialog
	void showSettingsDialog();

  signals:
	//! Emitted when search results change
	void searchResultsChanged(int currentMatch, int totalMatches);

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

	//! Apply search highlight to text
	void highlightSearchResults();

	//! Check if a message should be displayed based on current filters
	bool shouldDisplayMessage(int level, const QString& category) const;

  private:
	//! Stored log entry
	struct LogEntry
	{
		QString   message;
		int       level;
		QDateTime timestamp;
		QString   category;
		QString   formattedText; // Pre-formatted text for performance

		LogEntry() : level(0) {}
		LogEntry(const QString& msg, int lvl, const QDateTime& ts, const QString& cat = QString())
		    : message(msg), level(lvl), timestamp(ts), category(cat)
		{
		}
	};

	ccConsoleSettings       m_settings;
	ccConsoleTheme          m_theme;
	ccLogFormatter          m_formatter;
	bool                    m_autoScroll;
	int                     m_lineCount;

	// Message storage for filtering
	QVector<LogEntry>       m_messages;

	// Search state
	QString                 m_searchText;
	bool                    m_searchCaseSensitive;
	bool                    m_searchUseRegex;
	QVector<int>            m_searchMatches; // Line numbers with matches
	int                     m_currentMatchIndex;

	// Filter state
	int                     m_logLevelFilter; // Minimum level to show
	QMap<int, bool>         m_levelEnabled;   // Per-level enable/disable
};
