// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewportSector.h"

ViewportSector::ViewportSector(int sectorID_, FString verboseName_, float xlimLower, float xlimUpper, float ylimLower, float ylimUpper)
{
	// Instantiate variables to constructor inputted
	sectorID = sectorID_; 
	verboseName = verboseName_; 
	xlims[0] = xlimLower; 
	xlims[1] = xlimUpper;
	ylims[0] = ylimLower;
	ylims[1] = ylimUpper;
}

ViewportSector::~ViewportSector()
{
}

// Sector operations 
ViewportSector* ViewportSector::checkWithinSector(float swordFocalX, float swordFocalY) {
	return this; 
}

// Getters and Setters 
float ViewportSector::getXlimLower() {
	return xlims[0];
}
float ViewportSector::getXlimUpper() {
	return xlims[1];
}
float ViewportSector::getYlimLower() {
	return ylims[0];
}
float ViewportSector::getYlimUpper() {
	return ylims[1];
}

