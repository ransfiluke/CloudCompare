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

#include "ccConsoleSettingsDlg.h"

// Qt
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

ccConsoleSettingsDlg::ccConsoleSettingsDlg(ccConsoleSettings* settings, QWidget* parent)
    : QDialog(parent)
    , m_settings(settings)
{
	setWindowTitle("Console Settings");
	setMinimumWidth(500);

	createWidgets();
	updateFromSettings();
}

void ccConsoleSettingsDlg::createWidgets()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	// Theme Group
	QGroupBox* themeGroup = new QGroupBox("Theme", this);
	QFormLayout* themeLayout = new QFormLayout(themeGroup);

	m_themeModeCombo = new QComboBox(this);
	m_themeModeCombo->addItem("Auto-detect", ccConsoleSettings::AutoDetect);
	m_themeModeCombo->addItem("Light Theme", ccConsoleSettings::LightTheme);
	m_themeModeCombo->addItem("Dark Theme", ccConsoleSettings::DarkTheme);
	themeLayout->addRow("Theme Mode:", m_themeModeCombo);

	mainLayout->addWidget(themeGroup);

	// Timestamp Group
	QGroupBox* timestampGroup = new QGroupBox("Timestamps", this);
	QFormLayout* timestampLayout = new QFormLayout(timestampGroup);

	m_timestampFormatCombo = new QComboBox(this);
	m_timestampFormatCombo->addItem("No Timestamp", ccConsoleSettings::NoTimestamp);
	m_timestampFormatCombo->addItem("Time Only (HH:mm:ss)", ccConsoleSettings::TimeOnly);
	m_timestampFormatCombo->addItem("Time with ms (HH:mm:ss.zzz)", ccConsoleSettings::TimeWithMs);
	m_timestampFormatCombo->addItem("Date & Time", ccConsoleSettings::DateTime);
	m_timestampFormatCombo->addItem("Date & Time with ms", ccConsoleSettings::DateTimeWithMs);
	m_timestampFormatCombo->addItem("ISO 8601", ccConsoleSettings::ISO8601);
	m_timestampFormatCombo->addItem("Relative Time", ccConsoleSettings::RelativeTime);
	m_timestampFormatCombo->addItem("Custom Format", ccConsoleSettings::Custom);
	timestampLayout->addRow("Format:", m_timestampFormatCombo);

	m_customTimestampEdit = new QLineEdit(this);
	m_customTimestampEdit->setPlaceholderText("e.g., HH:mm:ss");
	timestampLayout->addRow("Custom Format:", m_customTimestampEdit);

	mainLayout->addWidget(timestampGroup);

	// Display Group
	QGroupBox* displayGroup = new QGroupBox("Display", this);
	QFormLayout* displayLayout = new QFormLayout(displayGroup);

	m_formatStyleCombo = new QComboBox(this);
	m_formatStyleCombo->addItem("Rich (Full formatting)", ccConsoleSettings::Rich);
	m_formatStyleCombo->addItem("Compact (Minimal)", ccConsoleSettings::Compact);
	m_formatStyleCombo->addItem("Raw (No formatting)", ccConsoleSettings::Raw);
	displayLayout->addRow("Style:", m_formatStyleCombo);

	m_formatTemplateEdit = new QLineEdit(this);
	m_formatTemplateEdit->setPlaceholderText("e.g., {timestamp} {icon} {message}");
	displayLayout->addRow("Format Template:", m_formatTemplateEdit);

	m_useIconsCheckBox = new QCheckBox("Show message type icons", this);
	displayLayout->addRow("", m_useIconsCheckBox);

	m_useUnicodeIconsCheckBox = new QCheckBox("Use Unicode symbols (✓, ⚠️, ❌)", this);
	displayLayout->addRow("", m_useUnicodeIconsCheckBox);

	mainLayout->addWidget(displayGroup);

	// Performance Group
	QGroupBox* performanceGroup = new QGroupBox("Performance", this);
	QFormLayout* performanceLayout = new QFormLayout(performanceGroup);

	m_maxLinesSpinBox = new QSpinBox(this);
	m_maxLinesSpinBox->setRange(100, 100000);
	m_maxLinesSpinBox->setSingleStep(1000);
	m_maxLinesSpinBox->setToolTip("Maximum number of lines to keep in console");
	performanceLayout->addRow("Max Lines:", m_maxLinesSpinBox);

	m_autoScrollCheckBox = new QCheckBox("Auto-scroll to bottom", this);
	performanceLayout->addRow("", m_autoScrollCheckBox);

	mainLayout->addWidget(performanceGroup);

	// Visual Group
	QGroupBox* visualGroup = new QGroupBox("Visual", this);
	QFormLayout* visualLayout = new QFormLayout(visualGroup);

	m_useMonospaceFontCheckBox = new QCheckBox("Use monospace font", this);
	visualLayout->addRow("", m_useMonospaceFontCheckBox);

	m_wordWrapCheckBox = new QCheckBox("Word wrap", this);
	visualLayout->addRow("", m_wordWrapCheckBox);

	mainLayout->addWidget(visualGroup);

	// Buttons
	QDialogButtonBox* buttons = new QDialogButtonBox(
	    QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults,
	    this);

	connect(buttons, &QDialogButtonBox::accepted, this, &ccConsoleSettingsDlg::apply);
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttons->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked,
	        this, &ccConsoleSettingsDlg::reset);

	mainLayout->addWidget(buttons);
}

void ccConsoleSettingsDlg::updateFromSettings()
{
	if (!m_settings)
		return;

	m_themeModeCombo->setCurrentIndex(m_themeModeCombo->findData(m_settings->themeMode()));
	m_timestampFormatCombo->setCurrentIndex(m_timestampFormatCombo->findData(m_settings->timestampFormat()));
	m_customTimestampEdit->setText(m_settings->customTimestampFormat());
	m_formatStyleCombo->setCurrentIndex(m_formatStyleCombo->findData(m_settings->formatStyle()));
	m_formatTemplateEdit->setText(m_settings->formatTemplate());
	m_useIconsCheckBox->setChecked(m_settings->useIcons());
	m_useUnicodeIconsCheckBox->setChecked(m_settings->useUnicodeIcons());
	m_maxLinesSpinBox->setValue(m_settings->maxLines());
	m_autoScrollCheckBox->setChecked(m_settings->autoScroll());
	m_useMonospaceFontCheckBox->setChecked(m_settings->useMonospaceFont());
	m_wordWrapCheckBox->setChecked(m_settings->wordWrap());
}

void ccConsoleSettingsDlg::apply()
{
	if (!m_settings)
		return;

	m_settings->setThemeMode(static_cast<ccConsoleSettings::ThemeMode>(
	    m_themeModeCombo->currentData().toInt()));
	m_settings->setTimestampFormat(static_cast<ccConsoleSettings::TimestampFormat>(
	    m_timestampFormatCombo->currentData().toInt()));
	m_settings->setCustomTimestampFormat(m_customTimestampEdit->text());
	m_settings->setFormatStyle(static_cast<ccConsoleSettings::FormatStyle>(
	    m_formatStyleCombo->currentData().toInt()));
	m_settings->setFormatTemplate(m_formatTemplateEdit->text());
	m_settings->setUseIcons(m_useIconsCheckBox->isChecked());
	m_settings->setUseUnicodeIcons(m_useUnicodeIconsCheckBox->isChecked());
	m_settings->setMaxLines(m_maxLinesSpinBox->value());
	m_settings->setAutoScroll(m_autoScrollCheckBox->isChecked());
	m_settings->setUseMonospaceFont(m_useMonospaceFontCheckBox->isChecked());
	m_settings->setWordWrap(m_wordWrapCheckBox->isChecked());

	m_settings->save();
}

void ccConsoleSettingsDlg::reset()
{
	if (!m_settings)
		return;

	m_settings->reset();
	updateFromSettings();
}
