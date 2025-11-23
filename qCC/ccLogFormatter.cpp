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

#include "ccLogFormatter.h"

// qCC_db
#include <ccLog.h>

ccLogFormatter::ccLogFormatter(const ccConsoleSettings* settings)
    : m_settings(settings)
{
}

QString ccLogFormatter::format(const QString&   message,
                               int              level,
                               const QDateTime& timestamp,
                               const QString&   category) const
{
	if (!m_settings)
		return message;

	// Check format style
	switch (m_settings->formatStyle())
	{
	case ccConsoleSettings::Raw:
		return message;

	case ccConsoleSettings::Compact:
	case ccConsoleSettings::Rich:
		return applyTemplate(message, level, timestamp, category, false);

	default:
		return message;
	}
}

QString ccLogFormatter::formatPlain(const QString&   message,
                                    int              level,
                                    const QDateTime& timestamp,
                                    const QString&   category) const
{
	return applyTemplate(message, level, timestamp, category, true);
}

QString ccLogFormatter::applyTemplate(const QString&   message,
                                      int              level,
                                      const QDateTime& timestamp,
                                      const QString&   category,
                                      bool             plain) const
{
	if (!m_settings)
		return message;

	QString result = m_settings->formatTemplate();

	// Replace placeholders
	// {timestamp}
	if (result.contains("{timestamp}"))
	{
		QString timestampStr = m_settings->getTimestampString(timestamp);
		if (!timestampStr.isEmpty())
		{
			result.replace("{timestamp}", timestampStr);
		}
		else
		{
			// Remove timestamp placeholder if no timestamp should be shown
			result.replace("{timestamp}", "");
			// Clean up double spaces
			result.replace("  ", " ");
		}
	}

	// {icon}
	if (result.contains("{icon}"))
	{
		if (m_settings->useIcons() && !plain)
		{
			result.replace("{icon}", getIcon(level, m_settings->useUnicodeIcons()));
		}
		else
		{
			result.replace("{icon}", "");
		}
	}

	// {level}
	if (result.contains("{level}"))
	{
		result.replace("{level}", getLevelName(level));
	}

	// {level_short}
	if (result.contains("{level_short}"))
	{
		result.replace("{level_short}", getLevelShort(level));
	}

	// {category}
	if (result.contains("{category}"))
	{
		if (!category.isEmpty() && m_settings->showCategories())
		{
			result.replace("{category}", QString("[%1]").arg(category));
		}
		else
		{
			result.replace("{category}", "");
		}
	}

	// {message}
	if (result.contains("{message}"))
	{
		result.replace("{message}", message);
	}

	// Clean up any double spaces that might have been created
	while (result.contains("  "))
	{
		result.replace("  ", " ");
	}

	return result.trimmed();
}

QString ccLogFormatter::getIcon(int level, bool useUnicode)
{
	if (!useUnicode)
	{
		// ASCII fallback
		if (level & ccLog::LOG_ERROR)
			return "[X]";
		else if (level & ccLog::LOG_WARNING)
			return "[!]";
		else if ((level & 7) == ccLog::LOG_IMPORTANT)
			return "[+]";
		else if (level & ccLog::DEBUG_FLAG)
			return "[D]";
		else if ((level & 7) == ccLog::LOG_VERBOSE)
			return "[i]";
		else
			return "[-]";
	}

	// Unicode symbols
	if (level & ccLog::LOG_ERROR)
		return QString::fromUtf8("\u274C"); // ❌ Cross mark
	else if (level & ccLog::LOG_WARNING)
		return QString::fromUtf8("\u26A0"); // ⚠️  Warning sign
	else if ((level & 7) == ccLog::LOG_IMPORTANT)
		return QString::fromUtf8("\u2713"); // ✓ Check mark
	else if (level & ccLog::DEBUG_FLAG)
		return QString::fromUtf8("\u1F527"); // 🔧 Wrench (debug tool)
	else if ((level & 7) == ccLog::LOG_VERBOSE)
		return QString::fromUtf8("\u2139"); // ℹ️  Information
	else
		return QString::fromUtf8("\u2022"); // • Bullet point
}

QString ccLogFormatter::getLevelName(int level)
{
	if (level & ccLog::LOG_ERROR)
		return "ERROR";
	else if (level & ccLog::LOG_WARNING)
		return "WARNING";
	else if ((level & 7) == ccLog::LOG_IMPORTANT)
		return "IMPORTANT";
	else if (level & ccLog::DEBUG_FLAG)
		return "DEBUG";
	else if ((level & 7) == ccLog::LOG_VERBOSE)
		return "VERBOSE";
	else
		return "INFO";
}

QString ccLogFormatter::getLevelShort(int level)
{
	if (level & ccLog::LOG_ERROR)
		return "E";
	else if (level & ccLog::LOG_WARNING)
		return "W";
	else if ((level & 7) == ccLog::LOG_IMPORTANT)
		return "I";
	else if (level & ccLog::DEBUG_FLAG)
		return "D";
	else if ((level & 7) == ccLog::LOG_VERBOSE)
		return "V";
	else
		return " ";
}
