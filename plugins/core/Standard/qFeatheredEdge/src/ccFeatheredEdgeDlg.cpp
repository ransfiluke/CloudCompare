//##########################################################################
//#                                                                        #
//#                  CLOUDCOMPARE PLUGIN: qFeatheredEdge                   #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 or later of the License.      #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                     COPYRIGHT: CloudCompare Community                  #
//#                                                                        #
//##########################################################################

#include "ccFeatheredEdgeDlg.h"

//qCC_db
#include <ccBBox.h>

//Qt
#include <QMessageBox>

ccFeatheredEdgeDlg::ccFeatheredEdgeDlg(QWidget* parent)
	: QDialog(parent, Qt::Tool)
	, Ui::FeatheredEdgeDialog()
	, m_cloud(nullptr)
	, m_originPoint(0, 0, 0)
{
	setupUi(this);

	// Connect signals
	connect(pickPointButton, &QPushButton::clicked, this, &ccFeatheredEdgeDlg::onPickPointClicked);
	connect(useCloudCenterButton, &QPushButton::clicked, this, &ccFeatheredEdgeDlg::onUseCloudCenterClicked);

	connect(originXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
			this, &ccFeatheredEdgeDlg::onManualCoordsChanged);
	connect(originYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
			this, &ccFeatheredEdgeDlg::onManualCoordsChanged);
	connect(originZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
			this, &ccFeatheredEdgeDlg::onManualCoordsChanged);

	connect(falloffTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &ccFeatheredEdgeDlg::onFalloffTypeChanged);

	// Set default fall-off type to Smoothstep (index 3) as it usually gives nice results
	falloffTypeComboBox->setCurrentIndex(3);
}

void ccFeatheredEdgeDlg::setCloud(ccPointCloud* cloud)
{
	m_cloud = cloud;

	if (m_cloud)
	{
		// Use cloud center as default origin
		ccBBox bbox = m_cloud->getOwnBB();
		m_originPoint = bbox.getCenter();

		updateOriginPointDisplay();
		estimateMaxDistance();
	}
}

CCVector3 ccFeatheredEdgeDlg::getOriginPoint() const
{
	return m_originPoint;
}

double ccFeatheredEdgeDlg::getMaxDistance() const
{
	return maxDistanceSpinBox->value();
}

double ccFeatheredEdgeDlg::getStrength() const
{
	return strengthSpinBox->value();
}

int ccFeatheredEdgeDlg::getFalloffType() const
{
	return falloffTypeComboBox->currentIndex();
}

unsigned int ccFeatheredEdgeDlg::getRandomSeed() const
{
	return static_cast<unsigned int>(randomSeedSpinBox->value());
}

void ccFeatheredEdgeDlg::onPickPointClicked()
{
	// TODO: Implement interactive point picking using ccPickingHub
	// For now, show a message to use manual input
	QMessageBox::information(this,
							tr("Point Picking"),
							tr("Interactive point picking is not yet implemented.\n\n"
							   "Please use the 'Use Cloud Center' button or enter coordinates manually."),
							QMessageBox::Ok);
}

void ccFeatheredEdgeDlg::onUseCloudCenterClicked()
{
	if (!m_cloud)
	{
		QMessageBox::warning(this,
							tr("No Cloud"),
							tr("No cloud is associated with this dialog!"),
							QMessageBox::Ok);
		return;
	}

	// Get cloud bounding box center
	ccBBox bbox = m_cloud->getOwnBB();
	m_originPoint = bbox.getCenter();

	updateOriginPointDisplay();
}

void ccFeatheredEdgeDlg::onManualCoordsChanged()
{
	// Update origin point from manual input
	m_originPoint.x = static_cast<PointCoordinateType>(originXSpinBox->value());
	m_originPoint.y = static_cast<PointCoordinateType>(originYSpinBox->value());
	m_originPoint.z = static_cast<PointCoordinateType>(originZSpinBox->value());
}

void ccFeatheredEdgeDlg::onFalloffTypeChanged(int index)
{
	// Update tooltip based on selected fall-off type
	QString tooltip;
	switch (index)
	{
	case 0: // Linear
		tooltip = tr("Linear fall-off: Probability decreases linearly with distance.\n"
					"Lower strength = more gradual, Higher strength = steeper.");
		break;
	case 1: // Exponential
		tooltip = tr("Exponential fall-off: Probability decreases exponentially.\n"
					"Higher strength = faster decay.");
		break;
	case 2: // Gaussian
		tooltip = tr("Gaussian fall-off: Smooth bell curve distribution.\n"
					"Higher strength = narrower distribution.");
		break;
	case 3: // Smoothstep
		tooltip = tr("Smoothstep fall-off: Smooth cubic interpolation.\n"
					"Lower strength = more gradual, Higher strength = steeper.");
		break;
	default:
		tooltip = tr("Fall-off strength parameter.");
		break;
	}

	strengthSpinBox->setToolTip(tooltip);
}

void ccFeatheredEdgeDlg::updateOriginPointDisplay()
{
	// Block signals to avoid triggering onManualCoordsChanged
	originXSpinBox->blockSignals(true);
	originYSpinBox->blockSignals(true);
	originZSpinBox->blockSignals(true);

	originXSpinBox->setValue(m_originPoint.x);
	originYSpinBox->setValue(m_originPoint.y);
	originZSpinBox->setValue(m_originPoint.z);

	originXSpinBox->blockSignals(false);
	originYSpinBox->blockSignals(false);
	originZSpinBox->blockSignals(false);
}

void ccFeatheredEdgeDlg::estimateMaxDistance()
{
	if (!m_cloud)
		return;

	// Estimate a reasonable max distance based on cloud diagonal
	ccBBox bbox = m_cloud->getOwnBB();
	CCVector3 diag = bbox.getDiagVec();
	double diagonal = diag.norm();

	// Use 25% of diagonal as default max distance
	// This gives a reasonable feathering effect for most clouds
	double estimatedMaxDist = diagonal * 0.25;

	maxDistanceSpinBox->setValue(estimatedMaxDist);
}
