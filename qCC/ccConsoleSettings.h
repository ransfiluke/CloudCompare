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
#include <QFont>
#include <QString>
#include <QStringList>

//! Console settings manager
/** Manages all user-configurable settings for the console/terminal interface.
    Settings are persisted using QSettings.
**/
class ccConsoleSettings
{
  public:
	//! Theme mode
	enum ThemeMode
	{
		AutoDetect = 0, //!< Detect from system/application theme
		LightTheme = 1, //!< Force light theme
		DarkTheme  = 2  //!< Force dark theme
	};

	//! Timestamp format presets
	enum TimestampFormat
	{
		NoTimestamp   = 0, //!< No timestamp
		TimeOnly      = 1, //!< HH:mm:ss
		TimeWithMs    = 2, //!< HH:mm:ss.zzz
		DateTime      = 3, //!< yyyy-MM-dd HH:mm:ss
		DateTimeWithMs = 4, //!< yyyy-MM-dd HH:mm:ss.zzz
		ISO8601       = 5, //!< ISO 8601 format
		RelativeTime  = 6, //!< Relative time (e.g., "2s ago")
		Custom        = 7  //!< Custom format string
	};

	//! Log format style
	enum FormatStyle
	{
		Rich    = 0, //!< Full formatting with colors, icons, timestamps
		Compact = 1, //!< Minimal formatting to save space
		Raw     = 2  //!< Plain text, no formatting
	};

	//! Default constructor
	ccConsoleSettings();

	//! Load settings from persistent storage
	void load();

	//! Save settings to persistent storage
	void save();

	//! Reset to default values
	void reset();

	// Getters
	inline ThemeMode themeMode() const { return m_themeMode; }
	inline TimestampFormat timestampFormat() const { return m_timestampFormat; }
	inline QString customTimestampFormat() const { return m_customTimestampFormat; }
	inline FormatStyle formatStyle() const { return m_formatStyle; }
	inline QString formatTemplate() const { return m_formatTemplate; }
	inline bool useIcons() const { return m_useIcons; }
	inline bool useUnicodeIcons() const { return m_useUnicodeIcons; }
	inline int minLogLevel() const { return m_minLogLevel; }
	inline int maxLines() const { return m_maxLines; }
	inline bool autoScroll() const { return m_autoScroll; }
	inline bool showCategories() const { return m_showCategories; }
	inline QStringList enabledCategories() const { return m_enabledCategories; }
	inline QFont font() const { return m_font; }
	inline bool useMonospaceFont() const { return m_useMonospaceFont; }
	inline bool wordWrap() const { return m_wordWrap; }
	inline bool alternatingRowColors() const { return m_alternatingRowColors; }

	// Setters
	inline void setThemeMode(ThemeMode mode) { m_themeMode = mode; }
	inline void setTimestampFormat(TimestampFormat format) { m_timestampFormat = format; }
	inline void setCustomTimestampFormat(const QString& format) { m_customTimestampFormat = format; }
	inline void setFormatStyle(FormatStyle style) { m_formatStyle = style; }
	inline void setFormatTemplate(const QString& templ) { m_formatTemplate = templ; }
	inline void setUseIcons(bool use) { m_useIcons = use; }
	inline void setUseUnicodeIcons(bool use) { m_useUnicodeIcons = use; }
	inline void setMinLogLevel(int level) { m_minLogLevel = level; }
	inline void setMaxLines(int lines) { m_maxLines = lines; }
	inline void setAutoScroll(bool scroll) { m_autoScroll = scroll; }
	inline void setShowCategories(bool show) { m_showCategories = show; }
	inline void setEnabledCategories(const QStringList& categories) { m_enabledCategories = categories; }
	inline void setFont(const QFont& font) { m_font = font; }
	inline void setUseMonospaceFont(bool use) { m_useMonospaceFont = use; }
	inline void setWordWrap(bool wrap) { m_wordWrap = wrap; }
	inline void setAlternatingRowColors(bool alternate) { m_alternatingRowColors = alternate; }

	//! Get formatted timestamp string
	QString getTimestampString(const QDateTime& dateTime) const;

	//! Get default format template
	static QString defaultFormatTemplate();

  protected:
	// Theme settings
	ThemeMode m_themeMode;

	// Timestamp settings
	TimestampFormat m_timestampFormat;
	QString         m_customTimestampFormat;

	// Display settings
	FormatStyle m_formatStyle;
	QString     m_formatTemplate; // e.g., "{timestamp} [{level}] {message}"
	bool        m_useIcons;
	bool        m_useUnicodeIcons; // Unicode symbols vs Font icons

	// Filtering settings
	int         m_minLogLevel;     // Minimum log level to display
	QStringList m_enabledCategories; // Enabled category filters

	// Performance settings
	int  m_maxLines;   // Maximum lines before truncation
	bool m_autoScroll; // Auto-scroll to bottom

	// Visual settings
	bool  m_showCategories; // Show category tags
	QFont m_font;
	bool  m_useMonospaceFont;
	bool  m_wordWrap;
	bool  m_alternatingRowColors;
};
