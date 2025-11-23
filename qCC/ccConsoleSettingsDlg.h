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
#include <QDialog>

class QComboBox;
class QSpinBox;
class QCheckBox;
class QLineEdit;
class QRadioButton;

//! Console settings dialog
/** Provides a UI for configuring console appearance and behavior.
**/
class ccConsoleSettingsDlg : public QDialog
{
	Q_OBJECT

  public:
	//! Constructor
	/** \param settings the settings object to edit
	    \param parent parent widget
	 **/
	explicit ccConsoleSettingsDlg(ccConsoleSettings* settings, QWidget* parent = nullptr);

  protected slots:
	//! Apply settings
	void apply();

	//! Reset to defaults
	void reset();

  private:
	void createWidgets();
	void updateFromSettings();

	ccConsoleSettings* m_settings;

	// Theme settings
	QComboBox*     m_themeModeCombo;

	// Timestamp settings
	QComboBox*     m_timestampFormatCombo;
	QLineEdit*     m_customTimestampEdit;

	// Display settings
	QComboBox*     m_formatStyleCombo;
	QLineEdit*     m_formatTemplateEdit;
	QCheckBox*     m_useIconsCheckBox;
	QCheckBox*     m_useUnicodeIconsCheckBox;

	// Performance settings
	QSpinBox*      m_maxLinesSpinBox;
	QCheckBox*     m_autoScrollCheckBox;

	// Visual settings
	QCheckBox*     m_useMonospaceFontCheckBox;
	QCheckBox*     m_wordWrapCheckBox;
};
