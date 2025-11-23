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
#include <QColor>

// qCC_db
#include <ccLog.h>

//! Console theme manager
/** Manages color schemes for light and dark themes.
    Provides theme-aware colors for different log levels.
**/
class ccConsoleTheme
{
  public:
	//! Color scheme for a theme
	struct ColorScheme
	{
		QColor background;       //!< Background color
		QColor foreground;       //!< Default text color
		QColor errorColor;       //!< Error message color
		QColor warningColor;     //!< Warning message color
		QColor infoColor;        //!< Info message color
		QColor debugColor;       //!< Debug message color
		QColor successColor;     //!< Success/important message color
		QColor timestampColor;   //!< Timestamp color
		QColor categoryColor;    //!< Category tag color
		QColor selectionBg;      //!< Selection background
		QColor selectionFg;      //!< Selection foreground
		QColor alternateBg;      //!< Alternate row background (for zebra striping)
	};

	//! Default constructor
	ccConsoleTheme();

	//! Get the current color scheme based on palette
	/** \param isDark whether to use dark theme
	 **/
	ColorScheme getColorScheme(bool isDark) const;

	//! Get color for a specific log level
	/** \param level log level from ccLog::MessageLevelFlags
	    \param isDark whether to use dark theme
	 **/
	QColor getColorForLevel(int level, bool isDark) const;

	//! Get light theme color scheme
	inline const ColorScheme& lightTheme() const { return m_lightTheme; }

	//! Get dark theme color scheme
	inline const ColorScheme& darkTheme() const { return m_darkTheme; }

	//! Set light theme colors
	void setLightTheme(const ColorScheme& scheme);

	//! Set dark theme colors
	void setDarkTheme(const ColorScheme& scheme);

	//! Check if a palette is considered dark
	static bool isDarkPalette(const QPalette& palette);

  protected:
	ColorScheme m_lightTheme;
	ColorScheme m_darkTheme;

	//! Initialize default color schemes
	void initializeDefaultThemes();
};
