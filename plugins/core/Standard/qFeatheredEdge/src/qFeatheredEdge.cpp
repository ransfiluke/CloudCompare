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

#include "qFeatheredEdge.h"
#include "ccFeatheredEdgeDlg.h"

//Qt
#include <QtGui>
#include <QMainWindow>
#include <QElapsedTimer>
#include <QtMath>

//qCC_db
#include <ccPointCloud.h>
#include <ccOctree.h>
#include <ccOctreeProxy.h>
#include <ccProgressDialog.h>
#include <ccScalarField.h>

//CCCoreLib
#include <CloudSamplingTools.h>
#include <GenericProgressCallback.h>

//System
#include <random>
#include <algorithm>

qFeatheredEdge::qFeatheredEdge(QObject* parent)
	: QObject(parent)
	, ccStdPluginInterface(":/CC/plugin/qFeatheredEdge/info.json")
	, m_action(nullptr)
{
}

QList<QAction *> qFeatheredEdge::getActions()
{
	//default action
	if (!m_action)
	{
		m_action = new QAction(getName(), this);
		m_action->setToolTip(getDescription());
		m_action->setIcon(getIcon());
		//connect signal
		connect(m_action, &QAction::triggered, this, &qFeatheredEdge::doAction);
	}

	return QList<QAction *>{ m_action };
}

void qFeatheredEdge::onNewSelection(const ccHObject::Container& selectedEntities)
{
	if (m_action)
	{
		//a single point cloud must be selected
		m_action->setEnabled(selectedEntities.size() == 1 && selectedEntities.front()->isA(CC_TYPES::POINT_CLOUD));
	}
}

double qFeatheredEdge::calculateFalloffProbability(double distance,
												   double maxDistance,
												   double strength,
												   FalloffType falloffType) const
{
	if (distance >= maxDistance)
	{
		return 0.0;
	}

	if (distance <= 0.0)
	{
		return 1.0;
	}

	// Normalize distance to [0, 1]
	double normalizedDist = distance / maxDistance;

	double probability = 1.0;

	switch (falloffType)
	{
	case FalloffType::LINEAR:
		// Linear fall-off: probability = 1 - (distance / maxDistance)
		// Strength affects the rate of decrease
		probability = 1.0 - std::pow(normalizedDist, 1.0 / strength);
		break;

	case FalloffType::EXPONENTIAL:
		// Exponential fall-off: probability = exp(-distance * strength / maxDistance)
		probability = std::exp(-normalizedDist * strength);
		break;

	case FalloffType::GAUSSIAN:
		// Gaussian fall-off: probability = exp(-(distance^2) / (2 * sigma^2))
		// sigma is derived from maxDistance and strength
		{
			double sigma = maxDistance / (strength * 2.0);
			double distSq = distance * distance;
			double sigmaSq = sigma * sigma;
			probability = std::exp(-distSq / (2.0 * sigmaSq));
		}
		break;

	case FalloffType::SMOOTHSTEP:
		// Smoothstep fall-off: smooth cubic interpolation
		// Apply strength to control the curve
		{
			double t = std::pow(normalizedDist, 1.0 / strength);
			t = std::max(0.0, std::min(1.0, t));
			// Smoothstep formula: 3t^2 - 2t^3
			double smoothValue = t * t * (3.0 - 2.0 * t);
			probability = 1.0 - smoothValue;
		}
		break;

	default:
		probability = 1.0 - normalizedDist;
		break;
	}

	// Clamp to [0, 1]
	return std::max(0.0, std::min(1.0, probability));
}

CCCoreLib::ReferenceCloud* qFeatheredEdge::applyFeatheredEdge(ccPointCloud* cloud,
															  const CCVector3& originPoint,
															  double maxDistance,
															  double strength,
															  FalloffType falloffType,
															  unsigned int randomSeed)
{
	assert(cloud);
	if (!cloud)
		return nullptr;

	unsigned nbPoints = cloud->size();
	if (nbPoints == 0)
		return nullptr;

	// Initialize random number generator
	std::mt19937 rng;
	if (randomSeed == 0)
	{
		std::random_device rd;
		rng.seed(rd());
	}
	else
	{
		rng.seed(randomSeed);
	}
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	// Create reference cloud for result
	CCCoreLib::ReferenceCloud* sampledCloud = new CCCoreLib::ReferenceCloud(cloud);

	// Reserve space (estimate: at least half the points will be kept)
	if (!sampledCloud->reserve(nbPoints / 2))
	{
		delete sampledCloud;
		return nullptr;
	}

	// Process each point
	unsigned keptPoints = 0;
	for (unsigned i = 0; i < nbPoints; ++i)
	{
		const CCVector3* P = cloud->getPoint(i);

		// Calculate distance from origin
		double distance = (*P - originPoint).norm();

		// Calculate keep probability based on distance and fall-off type
		double keepProbability = calculateFalloffProbability(distance, maxDistance, strength, falloffType);

		// Random decision based on probability
		double randomValue = dist(rng);
		if (randomValue < keepProbability)
		{
			if (!sampledCloud->addPointIndex(i))
			{
				// Memory allocation failed
				delete sampledCloud;
				return nullptr;
			}
			++keptPoints;
		}
	}

	// Shrink to fit actual size
	if (!sampledCloud->resize(keptPoints))
	{
		// This shouldn't fail as we're reducing size, but handle it anyway
		delete sampledCloud;
		return nullptr;
	}

	return sampledCloud;
}

void qFeatheredEdge::doAction()
{
	assert(m_app);
	if (!m_app)
		return;

	const ccHObject::Container& selectedEntities = m_app->getSelectedEntities();

	if (!m_app->haveOneSelection() || !selectedEntities.front()->isA(CC_TYPES::POINT_CLOUD))
	{
		m_app->dispToConsole("Select only one cloud!", ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		return;
	}

	ccPointCloud* cloud = static_cast<ccPointCloud*>(selectedEntities[0]);

	// Show dialog to get parameters
	ccFeatheredEdgeDlg dlg(m_app->getMainWindow());

	// Set cloud for point picking
	dlg.setCloud(cloud);

	if (!dlg.exec())
		return;

	// Get parameters from dialog
	CCVector3 originPoint = dlg.getOriginPoint();
	double maxDistance = dlg.getMaxDistance();
	double strength = dlg.getStrength();
	int falloffTypeIndex = dlg.getFalloffType();
	unsigned int randomSeed = dlg.getRandomSeed();

	FalloffType falloffType = static_cast<FalloffType>(falloffTypeIndex);

	// Validate parameters
	if (maxDistance <= 0.0)
	{
		m_app->dispToConsole("Invalid maximum distance! Must be greater than 0.", ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		return;
	}

	if (strength <= 0.0)
	{
		m_app->dispToConsole("Invalid strength parameter! Must be greater than 0.", ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		return;
	}

	// Progress dialog
	ccProgressDialog progressCb(false, m_app->getMainWindow());
	progressCb.setMethodTitle(QObject::tr("Feathered Edge"));
	progressCb.setInfo(QObject::tr("Applying distance-based reduction..."));
	progressCb.start();

	QElapsedTimer eTimer;
	eTimer.start();

	// Apply feathered edge algorithm
	QScopedPointer<CCCoreLib::ReferenceCloud> sampledCloud(
		applyFeatheredEdge(cloud, originPoint, maxDistance, strength, falloffType, randomSeed)
	);

	progressCb.stop();

	if (!sampledCloud || sampledCloud->size() == 0)
	{
		m_app->dispToConsole("Feathered edge operation failed or resulted in empty cloud!", ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		return;
	}

	m_app->dispToConsole(QString("[Feathered Edge] Original: %1 points - Result: %2 points - Time: %3 s")
						.arg(cloud->size())
						.arg(sampledCloud->size())
						.arg(eTimer.elapsed() / 1000.0, 0, 'f', 3),
						ccMainAppInterface::STD_CONSOLE_MESSAGE);

	// Check if all points were kept
	if (sampledCloud->size() == cloud->size())
	{
		m_app->dispToConsole("No points were removed! Try increasing the maximum distance or adjusting the strength parameter.",
							ccMainAppInterface::WRN_CONSOLE_MESSAGE);
		return;
	}

	// Create new point cloud from sampled points
	ccPointCloud* newCloud = cloud->partialClone(sampledCloud.data());
	if (!newCloud)
	{
		m_app->dispToConsole("Not enough memory to create result cloud!", ccMainAppInterface::ERR_CONSOLE_MESSAGE);
		return;
	}

	// Set up new cloud properties
	newCloud->setName(cloud->getName() + QString(".feathered"));
	newCloud->setDisplay(cloud->getDisplay());
	newCloud->setVisible(true);
	newCloud->prepareDisplayForRefresh();

	// Disable original cloud
	cloud->setEnabled(false);

	// Add to database tree
	if (cloud->getParent())
	{
		cloud->getParent()->addChild(newCloud);
	}
	m_app->addToDB(newCloud);

	// Refresh display
	m_app->refreshAll();

	m_app->dispToConsole(QString("[Feathered Edge] Successfully created feathered cloud with %1 points")
						.arg(newCloud->size()),
						ccMainAppInterface::STD_CONSOLE_MESSAGE);
}
