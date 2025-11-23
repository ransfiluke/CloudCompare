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

#include "ccConsoleSearchWidget.h"

// Qt
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

ccConsoleSearchWidget::ccConsoleSearchWidget(QWidget* parent)
    : QWidget(parent)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(4, 4, 4, 4);
	layout->setSpacing(4);

	// Close button
	m_closeButton = new QPushButton(QString::fromUtf8("\u2715"), this); // X symbol
	m_closeButton->setToolTip("Close search");
	m_closeButton->setMaximumWidth(24);
	layout->addWidget(m_closeButton);
	connect(m_closeButton, &QPushButton::clicked, this, &ccConsoleSearchWidget::closeRequested);

	// Search field
	m_searchField = new QLineEdit(this);
	m_searchField->setPlaceholderText("Search...");
	m_searchField->setClearButtonEnabled(true);
	layout->addWidget(m_searchField);
	connect(m_searchField, &QLineEdit::textChanged, this, &ccConsoleSearchWidget::onSearchTextChanged);
	connect(m_searchField, &QLineEdit::returnPressed, this, &ccConsoleSearchWidget::findNext);

	// Previous button
	m_prevButton = new QPushButton(QString::fromUtf8("\u25B2"), this); // Up arrow
	m_prevButton->setToolTip("Find previous (Shift+F3)");
	m_prevButton->setMaximumWidth(32);
	layout->addWidget(m_prevButton);
	connect(m_prevButton, &QPushButton::clicked, this, &ccConsoleSearchWidget::findPrevious);

	// Next button
	m_nextButton = new QPushButton(QString::fromUtf8("\u25BC"), this); // Down arrow
	m_nextButton->setToolTip("Find next (F3)");
	m_nextButton->setMaximumWidth(32);
	layout->addWidget(m_nextButton);
	connect(m_nextButton, &QPushButton::clicked, this, &ccConsoleSearchWidget::findNext);

	// Match counter label
	m_matchLabel = new QLabel(this);
	m_matchLabel->setMinimumWidth(60);
	layout->addWidget(m_matchLabel);

	// Case sensitive checkbox
	m_caseSensitiveCheckBox = new QCheckBox("Case", this);
	m_caseSensitiveCheckBox->setToolTip("Case sensitive search");
	layout->addWidget(m_caseSensitiveCheckBox);
	connect(m_caseSensitiveCheckBox, &QCheckBox::toggled, this, &ccConsoleSearchWidget::caseSensitiveChanged);
	connect(m_caseSensitiveCheckBox, &QCheckBox::toggled, this, &ccConsoleSearchWidget::onSearchTextChanged);

	// Regex checkbox
	m_regexCheckBox = new QCheckBox("Regex", this);
	m_regexCheckBox->setToolTip("Use regular expressions");
	layout->addWidget(m_regexCheckBox);
	connect(m_regexCheckBox, &QCheckBox::toggled, this, &ccConsoleSearchWidget::regexChanged);
	connect(m_regexCheckBox, &QCheckBox::toggled, this, &ccConsoleSearchWidget::onSearchTextChanged);

	layout->addStretch();

	// Initially hidden
	hide();
}

QString ccConsoleSearchWidget::searchText() const
{
	return m_searchField->text();
}

bool ccConsoleSearchWidget::caseSensitive() const
{
	return m_caseSensitiveCheckBox->isChecked();
}

bool ccConsoleSearchWidget::useRegex() const
{
	return m_regexCheckBox->isChecked();
}

void ccConsoleSearchWidget::focusSearchField()
{
	m_searchField->setFocus();
	m_searchField->selectAll();
}

void ccConsoleSearchWidget::updateMatchCount(int current, int total)
{
	if (total == 0)
	{
		m_matchLabel->setText("No matches");
	}
	else
	{
		m_matchLabel->setText(QString("%1/%2").arg(current).arg(total));
	}
}

void ccConsoleSearchWidget::onSearchTextChanged()
{
	emit searchChanged(searchText());
}
