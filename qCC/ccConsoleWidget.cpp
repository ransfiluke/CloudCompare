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

// Qt
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QPalette>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>

ccConsoleWidget::ccConsoleWidget(QWidget* parent)
    : QPlainTextEdit(parent)
    , m_formatter(&m_settings)
    , m_autoScroll(true)
    , m_lineCount(0)
{
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
	// Check if message should be filtered out
	if ((level & 7) < m_settings.minLogLevel())
	{
		return;
	}

	// Format the message
	QString formattedMessage = m_formatter.format(message, level, timestamp, category);

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
	cursor.insertText(formattedMessage + "\n");

	// Increment line count
	m_lineCount++;

	// Trim lines if necessary
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
	m_lineCount = 0;
}

int ccConsoleWidget::lineCount() const
{
	return m_lineCount;
}

QString ccConsoleWidget::exportPlainText() const
{
	return toPlainText();
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

	QAction* clearAction = menu->addAction("Clear Console");
	connect(clearAction, &QAction::triggered, this, &ccConsoleWidget::clearConsole);

	QAction* copyAllAction = menu->addAction("Copy All");
	connect(copyAllAction, &QAction::triggered, [this]() {
		QApplication::clipboard()->setText(exportPlainText());
	});

	menu->addSeparator();

	QAction* autoScrollAction = menu->addAction("Auto-scroll");
	autoScrollAction->setCheckable(true);
	autoScrollAction->setChecked(m_autoScroll);
	connect(autoScrollAction, &QAction::triggered, this, &ccConsoleWidget::setAutoScroll);

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
