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
bool ViewportSector::checkWithinSector(float swordFocalX, float swordFocalY, ViewportSector*& sectorObj) {
	
	// Check within limits. Always using <= >= as to avoid any "Dead spots" which would result 
	// in no sector being returned when checking the SF point position. (i.e. <0.33 or 0.33> would result in 0.33 returning nothing)
	if (xlims[0] >= swordFocalX && swordFocalX <= xlims[1]) {

		if (ylims[0] >= swordFocalY && swordFocalY <= ylims[1]) {

			// If within limits
			// Set viewportsector pointer reference to this object and return true
			sectorObj = this;
			return true;
		}
	}
	return false; 
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

