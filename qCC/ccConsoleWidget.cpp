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

#include "ccConsoleWidget.h"
#include "ccConsoleSettingsDlg.h"

// Qt
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QFile>
#include <QKeyEvent>
#include <QMenu>
#include <QPalette>
#include <QRegularExpression>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextStream>

ccConsoleWidget::ccConsoleWidget(QWidget* parent)
    : QPlainTextEdit(parent)
    , m_formatter(&m_settings)
    , m_autoScroll(true)
    , m_lineCount(0)
    , m_searchCaseSensitive(false)
    , m_searchUseRegex(false)
    , m_currentMatchIndex(-1)
    , m_logLevelFilter(0)
{
	// Initialize level filters (all enabled by default)
	m_levelEnabled[0] = true; // VERBOSE
	m_levelEnabled[1] = true; // STANDARD
	m_levelEnabled[2] = true; // IMPORTANT
	m_levelEnabled[3] = true; // WARNING
	m_levelEnabled[4] = true; // ERROR
	// Set read-only
	setReadOnly(true);

	// Enable line wrap by default
	setLineWrapMode(QPlainTextEdit::WidgetWidth);

	// Set a reasonable maximum block count for performance
	setMaximumBlockCount(10000);

	// Load settings
	loadSettings();

	// Apply initial theme
	applyTheme();

	// Set font
	if (m_settings.useMonospaceFont())
	{
		QFont monoFont("Monospace");
		monoFont.setStyleHint(QFont::TypeWriter);
		setFont(monoFont);
	}
}

ccConsoleWidget::~ccConsoleWidget()
{
}

void ccConsoleWidget::appendMessage(const QString&   message,
                                    int              level,
                                    const QDateTime& timestamp,
                                    const QString&   category)
{
	// Store the message
	LogEntry entry(message, level, timestamp, category);
	entry.formattedText = m_formatter.format(message, level, timestamp, category);
	m_messages.append(entry);

	// Check if message should be displayed based on filters
	if (!shouldDisplayMessage(level, category))
	{
		return;
	}

	// Check if message should be filtered out by settings
	if ((level & 7) < m_settings.minLogLevel())
	{
		return;
	}

	// Get color for this log level
	bool isDark = isDarkTheme();
	QColor textColor = m_theme.getColorForLevel(level, isDark);

	// Move cursor to end
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::End);

	// Set text color
	QTextCharFormat format;
	format.setForeground(QBrush(textColor));

	cursor.setCharFormat(format);

	// Append the message
	cursor.insertText(entry.formattedText + "\n");

	// Increment line count
	m_lineCount++;

	// Trim stored messages if necessary
	if (m_messages.size() > m_settings.maxLines())
	{
		// Remove old messages from storage
		int toRemove = m_messages.size() - m_settings.maxLines();
		m_messages.remove(0, toRemove);
	}

	// Trim displayed lines if necessary
	if (m_lineCount > m_settings.maxLines())
	{
		trimLines();
	}

	// Auto-scroll to bottom if enabled
	if (m_autoScroll)
	{
		QScrollBar* scrollBar = verticalScrollBar();
		scrollBar->setValue(scrollBar->maximum());
	}
}

void ccConsoleWidget::applyTheme()
{
	bool isDark = isDarkTheme();
	ccConsoleTheme::ColorScheme scheme = m_theme.getColorScheme(isDark);

	// Create and apply palette
	QPalette pal = palette();
	pal.setColor(QPalette::Base, scheme.background);
	pal.setColor(QPalette::Text, scheme.foreground);
	pal.setColor(QPalette::Highlight, scheme.selectionBg);
	pal.setColor(QPalette::HighlightedText, scheme.selectionFg);
	setPalette(pal);

	// Update stylesheet if alternating row colors are enabled
	if (m_settings.alternatingRowColors())
	{
		// Note: QPlainTextEdit doesn't support alternating row colors natively
		// This would need custom painting, which we'll skip for now
	}
}

void ccConsoleWidget::clearConsole()
{
	clear();
	m_messages.clear();
	m_lineCount = 0;
	clearSearch();
}

int ccConsoleWidget::lineCount() const
{
	return m_lineCount;
}

QString ccConsoleWidget::exportPlainText() const
{
	return toPlainText();
}

QString ccConsoleWidget::exportFiltered() const
{
	QString result;
	for (const LogEntry& entry : m_messages)
	{
		if (shouldDisplayMessage(entry.level, entry.category))
		{
			if ((entry.level & 7) >= m_settings.minLogLevel())
			{
				// Export raw message without formatting
				result += entry.message + "\n";
			}
		}
	}
	return result;
}

QString ccConsoleWidget::exportFormatted() const
{
	QString result;
	for (const LogEntry& entry : m_messages)
	{
		if (shouldDisplayMessage(entry.level, entry.category))
		{
			if ((entry.level & 7) >= m_settings.minLogLevel())
			{
				// Export formatted message
				result += entry.formattedText + "\n";
			}
		}
	}
	return result;
}

bool ccConsoleWidget::exportToFile(const QString& filename, bool filtered, bool includeFormatting)
{
	QFile file(filename);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		return false;
	}

	QTextStream out(&file);

	if (filtered)
	{
		// Export only filtered messages
		for (const LogEntry& entry : m_messages)
		{
			if (shouldDisplayMessage(entry.level, entry.category))
			{
				if ((entry.level & 7) >= m_settings.minLogLevel())
				{
					if (includeFormatting)
						out << entry.formattedText << Qt::endl;
					else
						out << entry.message << Qt::endl;
				}
			}
		}
	}
	else
	{
		// Export all messages
		for (const LogEntry& entry : m_messages)
		{
			if (includeFormatting)
				out << entry.formattedText << Qt::endl;
			else
				out << entry.message << Qt::endl;
		}
	}

	file.close();
	return true;
}

void ccConsoleWidget::setAutoScroll(bool enable)
{
	m_autoScroll = enable;
}

void ccConsoleWidget::loadSettings()
{
	m_settings.load();

	// Apply settings to widget
	setLineWrapMode(m_settings.wordWrap() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
	setMaximumBlockCount(m_settings.maxLines());
	m_autoScroll = m_settings.autoScroll();

	// Apply font
	if (m_settings.useMonospaceFont())
	{
		QFont monoFont("Monospace");
		monoFont.setStyleHint(QFont::TypeWriter);
		setFont(monoFont);
	}
	else
	{
		QFont font = m_settings.font();
		if (!font.family().isEmpty())
		{
			setFont(font);
		}
	}
}

void ccConsoleWidget::saveSettings()
{
	m_settings.save();
}

void ccConsoleWidget::showSettingsDialog()
{
	ccConsoleSettingsDlg dlg(&m_settings, this);
	if (dlg.exec() == QDialog::Accepted)
	{
		// Reload settings and refresh display
		loadSettings();
		applyTheme();
		refreshDisplay();
	}
}

void ccConsoleWidget::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::PaletteChange)
	{
		// Re-apply theme when palette changes (e.g., system theme switch)
		if (m_settings.themeMode() == ccConsoleSettings::AutoDetect)
		{
			applyTheme();
		}
	}

	QPlainTextEdit::changeEvent(event);
}

void ccConsoleWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu* menu = createStandardContextMenu();

	// Add custom actions
	menu->addSeparator();

	// Search action
	QAction* findAction = menu->addAction("Find... (Ctrl+F)");
	connect(findAction, &QAction::triggered, this, [this]() {
		// Emit a signal or call a method to show search widget
		// For now, we'll handle this via keyboard shortcut
	});

	menu->addSeparator();

	QAction* clearAction = menu->addAction("Clear Console");
	connect(clearAction, &QAction::triggered, this, &ccConsoleWidget::clearConsole);

	QAction* copyAllAction = menu->addAction("Copy All");
	connect(copyAllAction, &QAction::triggered, [this]() {
		QApplication::clipboard()->setText(exportPlainText());
	});

	// Export submenu
	QMenu* exportMenu = menu->addMenu("Export...");

	QAction* exportAllAction = exportMenu->addAction("Export All Messages");
	connect(exportAllAction, &QAction::triggered, [this]() {
		// This would need to integrate with file dialog
		// For now, just copy to clipboard
		QApplication::clipboard()->setText(exportFormatted());
	});

	QAction* exportFilteredAction = exportMenu->addAction("Export Filtered Messages");
	connect(exportFilteredAction, &QAction::triggered, [this]() {
		QApplication::clipboard()->setText(exportFiltered());
	});

	menu->addSeparator();

	// Filter submenu
	QMenu* filterMenu = menu->addMenu("Filter by Level");

	QAction* showAllAction = filterMenu->addAction("Show All");
	connect(showAllAction, &QAction::triggered, [this]() {
		for (int i = 0; i <= 4; ++i)
			setLogLevelEnabled(i, true);
	});

	filterMenu->addSeparator();

	QAction* verboseAction = filterMenu->addAction("Verbose");
	verboseAction->setCheckable(true);
	verboseAction->setChecked(isLogLevelEnabled(0));
	connect(verboseAction, &QAction::triggered, [this](bool checked) {
		setLogLevelEnabled(0, checked);
	});

	QAction* standardAction = filterMenu->addAction("Standard");
	standardAction->setCheckable(true);
	standardAction->setChecked(isLogLevelEnabled(1));
	connect(standardAction, &QAction::triggered, [this](bool checked) {
		setLogLevelEnabled(1, checked);
	});

	QAction* importantAction = filterMenu->addAction("Important");
	importantAction->setCheckable(true);
	importantAction->setChecked(isLogLevelEnabled(2));
	connect(importantAction, &QAction::triggered, [this](bool checked) {
		setLogLevelEnabled(2, checked);
	});

	QAction* warningAction = filterMenu->addAction("Warnings");
	warningAction->setCheckable(true);
	warningAction->setChecked(isLogLevelEnabled(3));
	connect(warningAction, &QAction::triggered, [this](bool checked) {
		setLogLevelEnabled(3, checked);
	});

	QAction* errorAction = filterMenu->addAction("Errors");
	errorAction->setCheckable(true);
	errorAction->setChecked(isLogLevelEnabled(4));
	connect(errorAction, &QAction::triggered, [this](bool checked) {
		setLogLevelEnabled(4, checked);
	});

	menu->addSeparator();

	QAction* autoScrollAction = menu->addAction("Auto-scroll");
	autoScrollAction->setCheckable(true);
	autoScrollAction->setChecked(m_autoScroll);
	connect(autoScrollAction, &QAction::triggered, this, &ccConsoleWidget::setAutoScroll);

	menu->addSeparator();

	// Settings action
	QAction* settingsAction = menu->addAction("Settings...");
	connect(settingsAction, &QAction::triggered, this, &ccConsoleWidget::showSettingsDialog);

	menu->exec(event->globalPos());
	delete menu;
}

void ccConsoleWidget::keyPressEvent(QKeyEvent* event)
{
	// Handle Ctrl+C for copy
	if (event->matches(QKeySequence::Copy))
	{
		QTextCursor cursor = textCursor();
		if (cursor.hasSelection())
		{
			QApplication::clipboard()->setText(cursor.selectedText().replace(QChar::ParagraphSeparator, '\n'));
		}
		return;
	}

	// Handle Ctrl+A for select all
	if (event->matches(QKeySequence::SelectAll))
	{
		selectAll();
		return;
	}

	// Pass other events to base class
	QPlainTextEdit::keyPressEvent(event);
}

void ccConsoleWidget::trimLines()
{
	// Remove old lines from the beginning
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::Start);

	int linesToRemove = m_lineCount - m_settings.maxLines();
	if (linesToRemove > 0)
	{
		for (int i = 0; i < linesToRemove; ++i)
		{
			cursor.select(QTextCursor::LineUnderCursor);
			cursor.removeSelectedText();
			cursor.deleteChar(); // Remove the newline
		}

		m_lineCount = m_settings.maxLines();
	}
}

bool ccConsoleWidget::isDarkTheme() const
{
	switch (m_settings.themeMode())
	{
	case ccConsoleSettings::LightTheme:
		return false;
	case ccConsoleSettings::DarkTheme:
		return true;
	case ccConsoleSettings::AutoDetect:
	default:
		return ccConsoleTheme::isDarkPalette(QApplication::palette());
	}
}

// Search functionality
int ccConsoleWidget::search(const QString& text, bool caseSensitive, bool useRegex)
{
	m_searchText = text;
	m_searchCaseSensitive = caseSensitive;
	m_searchUseRegex = useRegex;
	m_searchMatches.clear();
	m_currentMatchIndex = -1;

	if (text.isEmpty())
	{
		clearSearch();
		return 0;
	}

	// Search through all visible text
	QTextDocument* doc = document();
	QTextCursor cursor(doc);

	QTextDocument::FindFlags flags;
	if (caseSensitive)
		flags |= QTextDocument::FindCaseSensitively;

	int matchCount = 0;
	while (true)
	{
		if (useRegex)
		{
			QRegularExpression regex(text);
			if (!caseSensitive)
				regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
			cursor = doc->find(regex, cursor);
		}
		else
		{
			cursor = doc->find(text, cursor, flags);
		}

		if (cursor.isNull())
			break;

		m_searchMatches.append(cursor.blockNumber());
		matchCount++;
	}

	if (matchCount > 0)
	{
		m_currentMatchIndex = 0;
		highlightSearchResults();
	}

	emit searchResultsChanged(matchCount > 0 ? 1 : 0, matchCount);
	return matchCount;
}

void ccConsoleWidget::clearSearch()
{
	m_searchText.clear();
	m_searchMatches.clear();
	m_currentMatchIndex = -1;

	// Clear any extra selections (highlights)
	QList<QTextEdit::ExtraSelection> selections;
	setExtraSelections(selections);

	emit searchResultsChanged(0, 0);
}

bool ccConsoleWidget::findNext()
{
	if (m_searchMatches.isEmpty())
		return false;

	m_currentMatchIndex = (m_currentMatchIndex + 1) % m_searchMatches.size();
	highlightSearchResults();
	emit searchResultsChanged(m_currentMatchIndex + 1, m_searchMatches.size());
	return true;
}

bool ccConsoleWidget::findPrevious()
{
	if (m_searchMatches.isEmpty())
		return false;

	m_currentMatchIndex--;
	if (m_currentMatchIndex < 0)
		m_currentMatchIndex = m_searchMatches.size() - 1;

	highlightSearchResults();
	emit searchResultsChanged(m_currentMatchIndex + 1, m_searchMatches.size());
	return true;
}

void ccConsoleWidget::highlightSearchResults()
{
	QList<QTextEdit::ExtraSelection> selections;

	if (!m_searchText.isEmpty() && !m_searchMatches.isEmpty())
	{
		QTextDocument* doc = document();
		QTextCursor cursor(doc);

		QTextDocument::FindFlags flags;
		if (m_searchCaseSensitive)
			flags |= QTextDocument::FindCaseSensitively;

		// Highlight all matches
		QColor highlightColor = isDarkTheme() ? QColor(100, 100, 50) : QColor(255, 255, 100);
		QColor currentHighlightColor = isDarkTheme() ? QColor(150, 100, 50) : QColor(255, 200, 100);

		int matchIndex = 0;
		while (true)
		{
			if (m_searchUseRegex)
			{
				QRegularExpression regex(m_searchText);
				if (!m_searchCaseSensitive)
					regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
				cursor = doc->find(regex, cursor);
			}
			else
			{
				cursor = doc->find(m_searchText, cursor, flags);
			}

			if (cursor.isNull())
				break;

			QTextEdit::ExtraSelection selection;
			selection.cursor = cursor;
			selection.format.setBackground(matchIndex == m_currentMatchIndex ? currentHighlightColor : highlightColor);
			selections.append(selection);

			matchIndex++;

			// Scroll to current match
			if (matchIndex - 1 == m_currentMatchIndex)
			{
				setTextCursor(cursor);
			}
		}
	}

	setExtraSelections(selections);
}

// Filtering functionality
void ccConsoleWidget::setLogLevelFilter(int level)
{
	m_logLevelFilter = level;
	refreshDisplay();
}

void ccConsoleWidget::setLogLevelEnabled(int level, bool enabled)
{
	m_levelEnabled[level] = enabled;
	refreshDisplay();
}

bool ccConsoleWidget::isLogLevelEnabled(int level) const
{
	return m_levelEnabled.value(level, true);
}

bool ccConsoleWidget::shouldDisplayMessage(int level, const QString& category) const
{
	// Check minimum level filter
	int msgLevel = level & 7;
	if (msgLevel < m_logLevelFilter)
		return false;

	// Check per-level enable/disable
	if (!m_levelEnabled.value(msgLevel, true))
		return false;

	// Check category filter (if categories are enabled)
	if (m_settings.showCategories() && !category.isEmpty())
	{
		const QStringList& enabled = m_settings.enabledCategories();
		if (!enabled.isEmpty() && !enabled.contains(category))
			return false;
	}

	return true;
}

void ccConsoleWidget::refreshDisplay()
{
	// Save scroll position
	QScrollBar* scrollBar = verticalScrollBar();
	int scrollPos = scrollBar->value();
	bool wasAtBottom = (scrollPos == scrollBar->maximum());

	// Clear current display
	clear();
	m_lineCount = 0;

	// Rebuild from stored messages
	bool isDark = isDarkTheme();

	for (const LogEntry& entry : m_messages)
	{
		if (!shouldDisplayMessage(entry.level, entry.category))
			continue;

		if ((entry.level & 7) < m_settings.minLogLevel())
			continue;

		// Get color for this log level
		QColor textColor = m_theme.getColorForLevel(entry.level, isDark);

		// Move cursor to end
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::End);

		// Set text color
		QTextCharFormat format;
		format.setForeground(QBrush(textColor));

		cursor.setCharFormat(format);

		// Append the message
		cursor.insertText(entry.formattedText + "\n");
		m_lineCount++;
	}

	// Restore scroll position
	if (wasAtBottom || m_autoScroll)
	{
		scrollBar->setValue(scrollBar->maximum());
	}
	else
	{
		scrollBar->setValue(scrollPos);
	}

	// Reapply search highlights if active
	if (!m_searchText.isEmpty())
	{
		search(m_searchText, m_searchCaseSensitive, m_searchUseRegex);
	}
}
