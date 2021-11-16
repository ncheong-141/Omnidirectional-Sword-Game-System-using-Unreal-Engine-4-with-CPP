// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewportSector.h"


void UViewportSector::populate(int sectorID_, float xlimLower, float xlimUpper, float ylimLower, float ylimUpper) {
	
	// Instantiate variables to constructor inputted
	sectorID = sectorID_;
	xlims[0] = xlimLower;
	xlims[1] = xlimUpper;
	ylims[0] = ylimLower;
	ylims[1] = ylimUpper;
}

// Sector operations 
bool UViewportSector::checkWithinSector(float swordFocalX, float swordFocalY, UViewportSector*& sectorObj) {

	// Check within limits. Always using <= >= as to avoid any "Dead spots" which would result 
	// in no sector being returned when checking the SF point position. (i.e. <0.33 or 0.33> would result in 0.33 returning nothing)
	if (xlims[0] <= swordFocalX && swordFocalX <= xlims[1]) {

		if (ylims[0] <= swordFocalY && swordFocalY <= ylims[1]) {

			// If within limits
			// Set viewportsector pointer reference to this object (if object isnt already) and return true
			if (sectorObj != this) {

				sectorObj = this;

				// Print debug info
				printInfoToLog();
			}

			return true;
		}
	}
	return false;
}

// Getters and Setters 
int UViewportSector::getSectorID() {
	return sectorID;
}
float UViewportSector::getXlimLower() {
	return xlims[0];
}
float UViewportSector::getXlimUpper() {
	return xlims[1];
}
float UViewportSector::getYlimLower() {
	return ylims[0];
}
float UViewportSector::getYlimUpper() {
	return ylims[1];
}


void UViewportSector::printInfoToLog() {
	UE_LOG(LogTemp, Display, TEXT("Sector ID: %d | xlim/ylim: (%f, %f) (%f, %f)"), sectorID, xlims[0], xlims[1], ylims[0], ylims[1]);
}