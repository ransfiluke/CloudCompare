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

#include "ccConsoleSettings.h"

// Local
#include <ccPersistentSettings.h>

// Qt
#include <QDateTime>
#include <QSettings>

ccConsoleSettings::ccConsoleSettings()
    : m_themeMode(AutoDetect)
    , m_timestampFormat(TimeOnly)
    , m_customTimestampFormat("HH:mm:ss")
    , m_formatStyle(Rich)
    , m_formatTemplate(defaultFormatTemplate())
    , m_useIcons(true)
    , m_useUnicodeIcons(true)
    , m_minLogLevel(0)
    , m_maxLines(10000)
    , m_autoScroll(true)
    , m_showCategories(true)
    , m_useMonospaceFont(false)
    , m_wordWrap(true)
    , m_alternatingRowColors(false)
{
}

void ccConsoleSettings::load()
{
	QSettings settings;
	settings.beginGroup(ccPS::Console());

	// Theme settings
	m_themeMode = static_cast<ThemeMode>(settings.value("ThemeMode", AutoDetect).toInt());

	// Timestamp settings
	m_timestampFormat = static_cast<TimestampFormat>(settings.value("TimestampFormat", TimeOnly).toInt());
	m_customTimestampFormat = settings.value("CustomTimestampFormat", "HH:mm:ss").toString();

	// Display settings
	m_formatStyle = static_cast<FormatStyle>(settings.value("FormatStyle", Rich).toInt());
	m_formatTemplate = settings.value("FormatTemplate", defaultFormatTemplate()).toString();
	m_useIcons = settings.value("UseIcons", true).toBool();
	m_useUnicodeIcons = settings.value("UseUnicodeIcons", true).toBool();

	// Filtering settings
	m_minLogLevel = settings.value("MinLogLevel", 0).toInt();
	m_enabledCategories = settings.value("EnabledCategories", QStringList()).toStringList();

	// Performance settings
	m_maxLines = settings.value("MaxLines", 10000).toInt();
	m_autoScroll = settings.value("AutoScroll", true).toBool();

	// Visual settings
	m_showCategories = settings.value("ShowCategories", true).toBool();
	m_useMonospaceFont = settings.value("UseMonospaceFont", false).toBool();
	m_wordWrap = settings.value("WordWrap", true).toBool();
	m_alternatingRowColors = settings.value("AlternatingRowColors", false).toBool();

	// Font settings
	if (settings.contains("Font"))
	{
		m_font.fromString(settings.value("Font").toString());
	}

	settings.endGroup();
}

void ccConsoleSettings::save()
{
	QSettings settings;
	settings.beginGroup(ccPS::Console());

	// Theme settings
	settings.setValue("ThemeMode", static_cast<int>(m_themeMode));

	// Timestamp settings
	settings.setValue("TimestampFormat", static_cast<int>(m_timestampFormat));
	settings.setValue("CustomTimestampFormat", m_customTimestampFormat);

	// Display settings
	settings.setValue("FormatStyle", static_cast<int>(m_formatStyle));
	settings.setValue("FormatTemplate", m_formatTemplate);
	settings.setValue("UseIcons", m_useIcons);
	settings.setValue("UseUnicodeIcons", m_useUnicodeIcons);

	// Filtering settings
	settings.setValue("MinLogLevel", m_minLogLevel);
	settings.setValue("EnabledCategories", m_enabledCategories);

	// Performance settings
	settings.setValue("MaxLines", m_maxLines);
	settings.setValue("AutoScroll", m_autoScroll);

	// Visual settings
	settings.setValue("ShowCategories", m_showCategories);
	settings.setValue("UseMonospaceFont", m_useMonospaceFont);
	settings.setValue("WordWrap", m_wordWrap);
	settings.setValue("AlternatingRowColors", m_alternatingRowColors);

	// Font settings
	settings.setValue("Font", m_font.toString());

	settings.endGroup();
}

void ccConsoleSettings::reset()
{
	m_themeMode = AutoDetect;
	m_timestampFormat = TimeOnly;
	m_customTimestampFormat = "HH:mm:ss";
	m_formatStyle = Rich;
	m_formatTemplate = defaultFormatTemplate();
	m_useIcons = true;
	m_useUnicodeIcons = true;
	m_minLogLevel = 0;
	m_maxLines = 10000;
	m_autoScroll = true;
	m_showCategories = true;
	m_enabledCategories.clear();
	m_font = QFont();
	m_useMonospaceFont = false;
	m_wordWrap = true;
	m_alternatingRowColors = false;
}

QString ccConsoleSettings::getTimestampString(const QDateTime& dateTime) const
{
	switch (m_timestampFormat)
	{
	case NoTimestamp:
		return QString();
	case TimeOnly:
		return dateTime.toString("HH:mm:ss");
	case TimeWithMs:
		return dateTime.toString("HH:mm:ss.zzz");
	case DateTime:
		return dateTime.toString("yyyy-MM-dd HH:mm:ss");
	case DateTimeWithMs:
		return dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz");
	case ISO8601:
		return dateTime.toString(Qt::ISODate);
	case RelativeTime:
	{
		qint64 msecs = dateTime.msecsTo(QDateTime::currentDateTime());
		if (msecs < 1000)
			return QString("%1ms ago").arg(msecs);
		else if (msecs < 60000)
			return QString("%1s ago").arg(msecs / 1000);
		else if (msecs < 3600000)
			return QString("%1m ago").arg(msecs / 60000);
		else
			return QString("%1h ago").arg(msecs / 3600000);
	}
	case Custom:
		return dateTime.toString(m_customTimestampFormat);
	default:
		return dateTime.toString("HH:mm:ss");
	}
}

QString ccConsoleSettings::defaultFormatTemplate()
{
	return QStringLiteral("{timestamp} {icon} {message}");
}
