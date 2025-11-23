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

#ifndef Q_FEATHERED_EDGE_PLUGIN_HEADER
#define Q_FEATHERED_EDGE_PLUGIN_HEADER

#include "ccStdPluginInterface.h"

//CCCoreLib
#include <ReferenceCloud.h>

//! Plugin for creating feathered edges on point clouds using distance-based random reduction with fall-off
/** This plugin allows users to create aesthetically pleasing feathered edges on point clouds
	that have hard cutoffs from terrestrial scanning or photogrammetry. It works by:
	1. Allowing user to select an origin point
	2. Calculating distance from origin to all points
	3. Applying distance-based random reduction with various fall-off curves (Linear, Exponential, Gaussian)
	4. Creating a new point cloud with the feathered edge effect
**/
class qFeatheredEdge : public QObject, public ccStdPluginInterface
{
	Q_OBJECT
	Q_INTERFACES( ccPluginInterface ccStdPluginInterface )

	Q_PLUGIN_METADATA( IID "cccorp.cloudcompare.plugin.qFeatheredEdge" FILE "../info.json" )

public:
	//! Fall-off curve types for controlling the edge effect
	enum class FalloffType
	{
		LINEAR = 0,      //! Linear fall-off: probability decreases linearly with distance
		EXPONENTIAL = 1, //! Exponential fall-off: probability decreases exponentially
		GAUSSIAN = 2,    //! Gaussian fall-off: smooth bell curve distribution
		SMOOTHSTEP = 3   //! Smoothstep fall-off: smooth cubic interpolation
	};

	//! Default constructor
	explicit qFeatheredEdge(QObject* parent = nullptr);

	virtual ~qFeatheredEdge() = default;

	//inherited from ccStdPluginInterface
	virtual void onNewSelection(const ccHObject::Container& selectedEntities) override;
	virtual QList<QAction *> getActions() override;

protected:

	//! Slot called when associated action is triggered
	void doAction();

	//! Calculate fall-off probability based on distance and fall-off type
	/** @param distance Distance from origin point
		@param maxDistance Maximum distance for fall-off
		@param strength Fall-off strength parameter (affects steepness)
		@param falloffType Type of fall-off curve to use
		@return Probability value between 0.0 and 1.0
	**/
	double calculateFalloffProbability(double distance,
									   double maxDistance,
									   double strength,
									   FalloffType falloffType) const;

	//! Apply distance-based random reduction with fall-off
	/** @param cloud Input point cloud
		@param originPoint Origin point for distance calculation
		@param maxDistance Maximum distance for fall-off effect
		@param strength Fall-off strength parameter
		@param falloffType Type of fall-off curve
		@param randomSeed Random seed for reproducibility (0 = use random)
		@return Reference cloud containing sampled points, or nullptr on failure
	**/
	CCCoreLib::ReferenceCloud* applyFeatheredEdge(ccPointCloud* cloud,
												  const CCVector3& originPoint,
												  double maxDistance,
												  double strength,
												  FalloffType falloffType,
												  unsigned int randomSeed = 0);

protected:

	//! Associated action
	QAction* m_action;
};

#endif
