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

#include "ccConsoleTheme.h"

// Qt
#include <QPalette>

ccConsoleTheme::ccConsoleTheme()
{
	initializeDefaultThemes();
}

void ccConsoleTheme::initializeDefaultThemes()
{
	// Light theme colors
	m_lightTheme.background     = QColor(255, 255, 255);   // White
	m_lightTheme.foreground     = QColor(0, 0, 0);         // Black
	m_lightTheme.errorColor     = QColor(200, 0, 0);       // Dark red
	m_lightTheme.warningColor   = QColor(180, 100, 0);     // Dark orange/brown
	m_lightTheme.infoColor      = QColor(0, 100, 200);     // Blue
	m_lightTheme.debugColor     = QColor(100, 100, 100);   // Gray
	m_lightTheme.successColor   = QColor(0, 150, 0);       // Dark green
	m_lightTheme.timestampColor = QColor(100, 100, 100);   // Gray
	m_lightTheme.categoryColor  = QColor(0, 120, 180);     // Teal
	m_lightTheme.selectionBg    = QColor(200, 220, 255);   // Light blue
	m_lightTheme.selectionFg    = QColor(0, 0, 0);         // Black
	m_lightTheme.alternateBg    = QColor(245, 245, 245);   // Very light gray

	// Dark theme colors
	m_darkTheme.background     = QColor(30, 30, 30);       // Dark gray
	m_darkTheme.foreground     = QColor(220, 220, 220);    // Light gray
	m_darkTheme.errorColor     = QColor(255, 100, 100);    // Light red
	m_darkTheme.warningColor   = QColor(255, 180, 80);     // Orange
	m_darkTheme.infoColor      = QColor(100, 180, 255);    // Light blue
	m_darkTheme.debugColor     = QColor(150, 150, 150);    // Gray
	m_darkTheme.successColor   = QColor(120, 220, 120);    // Light green
	m_darkTheme.timestampColor = QColor(150, 150, 150);    // Gray
	m_darkTheme.categoryColor  = QColor(100, 200, 220);    // Cyan
	m_darkTheme.selectionBg    = QColor(60, 80, 120);      // Dark blue
	m_darkTheme.selectionFg    = QColor(255, 255, 255);    // White
	m_darkTheme.alternateBg    = QColor(35, 35, 35);       // Slightly lighter dark gray
}

ccConsoleTheme::ColorScheme ccConsoleTheme::getColorScheme(bool isDark) const
{
	return isDark ? m_darkTheme : m_lightTheme;
}

QColor ccConsoleTheme::getColorForLevel(int level, bool isDark) const
{
	const ColorScheme& scheme = isDark ? m_darkTheme : m_lightTheme;

	// Check for error
	if (level & ccLog::LOG_ERROR)
	{
		return scheme.errorColor;
	}
	// Check for warning
	else if (level & ccLog::LOG_WARNING)
	{
		return scheme.warningColor;
	}
	// Check for important
	else if ((level & 7) == ccLog::LOG_IMPORTANT)
	{
		return scheme.successColor;
	}
	// Check for debug
	else if (level & ccLog::DEBUG_FLAG)
	{
		return scheme.debugColor;
	}
	// Check for verbose
	else if ((level & 7) == ccLog::LOG_VERBOSE)
	{
		return scheme.infoColor;
	}
	// Standard message
	else
	{
		return scheme.foreground;
	}
}

void ccConsoleTheme::setLightTheme(const ColorScheme& scheme)
{
	m_lightTheme = scheme;
}

void ccConsoleTheme::setDarkTheme(const ColorScheme& scheme)
{
	m_darkTheme = scheme;
}

bool ccConsoleTheme::isDarkPalette(const QPalette& palette)
{
	// Calculate luminance of the window background color
	QColor bg = palette.color(QPalette::Window);

	// Using the relative luminance formula (ITU-R BT.709)
	double r = bg.redF();
	double g = bg.greenF();
	double b = bg.blueF();

	double luminance = 0.2126 * r + 0.7152 * g + 0.0722 * b;

	// If luminance is less than 0.5, consider it a dark theme
	return luminance < 0.5;
}
