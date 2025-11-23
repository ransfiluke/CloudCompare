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

#ifndef CC_FEATHERED_EDGE_DLG_HEADER
#define CC_FEATHERED_EDGE_DLG_HEADER

#include "ui_featheredEdgeDlg.h"

//qCC_db
#include <ccPointCloud.h>

//! Dialog for the qFeatheredEdge plugin
class ccFeatheredEdgeDlg : public QDialog, public Ui::FeatheredEdgeDialog
{
	Q_OBJECT

public:

	//! Default constructor
	explicit ccFeatheredEdgeDlg(QWidget* parent = nullptr);

	//! Set the cloud for point picking
	void setCloud(ccPointCloud* cloud);

	//! Get the origin point
	CCVector3 getOriginPoint() const;

	//! Get the maximum distance
	double getMaxDistance() const;

	//! Get the strength parameter
	double getStrength() const;

	//! Get the fall-off type (as integer index)
	int getFalloffType() const;

	//! Get the random seed (0 = random)
	unsigned int getRandomSeed() const;

protected slots:

	//! Called when the "Pick Point" button is clicked
	void onPickPointClicked();

	//! Called when the "Use Cloud Center" button is clicked
	void onUseCloudCenterClicked();

	//! Called when manual coordinates are changed
	void onManualCoordsChanged();

	//! Called when fall-off type changes
	void onFalloffTypeChanged(int index);

protected:

	//! Update the origin point display
	void updateOriginPointDisplay();

	//! Estimate a good default max distance based on cloud size
	void estimateMaxDistance();

protected:

	//! Associated cloud
	ccPointCloud* m_cloud;

	//! Current origin point
	CCVector3 m_originPoint;
};

#endif
