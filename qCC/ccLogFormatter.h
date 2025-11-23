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

// Qt
#include <QDateTime>
#include <QString>

//! Log message formatter
/** Formats log messages according to settings (icons, timestamps, etc.)
    Supports template-based formatting with placeholders.
**/
class ccLogFormatter
{
  public:
	//! Constructor
	/** \param settings console settings to use for formatting
	 **/
	explicit ccLogFormatter(const ccConsoleSettings* settings);

	//! Format a log message
	/** \param message the raw message text
	    \param level log level flags
	    \param timestamp timestamp of the message
	    \param category optional category tag
	    \return formatted message string
	 **/
	QString format(const QString& message,
	               int            level,
	               const QDateTime& timestamp,
	               const QString& category = QString()) const;

	//! Format for plain text export (no icons or formatting)
	QString formatPlain(const QString& message,
	                    int            level,
	                    const QDateTime& timestamp,
	                    const QString& category = QString()) const;

	//! Get icon/symbol for a log level
	/** \param level log level flags
	    \param useUnicode whether to use Unicode symbols
	    \return icon/symbol string
	 **/
	static QString getIcon(int level, bool useUnicode = true);

	//! Get text representation of log level
	/** \param level log level flags
	    \return level name (e.g., "ERROR", "WARNING", "INFO")
	 **/
	static QString getLevelName(int level);

	//! Get short level representation (single character or abbreviation)
	/** \param level log level flags
	    \return short level string (e.g., "E", "W", "I")
	 **/
	static QString getLevelShort(int level);

  protected:
	const ccConsoleSettings* m_settings;

	//! Apply template formatting
	QString applyTemplate(const QString& message,
	                      int            level,
	                      const QDateTime& timestamp,
	                      const QString& category,
	                      bool           plain = false) const;
};
