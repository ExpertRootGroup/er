/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeGeoComponentPassive.h"

//--------------------------------------------------------------------------------
ERQTelescopeGeoComponentPassive::ERQTelescopeGeoComponentPassive(
    const TString& fileName, const TVector3& position, const TVector3& rotation)
    : ERGeoComponent("None", fileName, position, rotation), fFileName(fileName) {
  fGeoFile = new TFile(fileName, "READ");
  if (!fGeoFile->IsOpen())
    LOG(FATAL) << "Can not open file: " << fileName << FairLogger::endl;
}
//--------------------------------------------------------------------------------
void ERQTelescopeGeoComponentPassive::ConstructGeometryVolume() {
  fVolume = static_cast<TGeoVolume*>(fGeoFile->Get("PassiveElement"));
  if (!fVolume)
    LOG(FATAL) << "PassiveElement volume not found in file " << fFileName 
               << FairLogger::endl;
}
//--------------------------------------------------------------------------------
ClassImp(ERQTelescopeGeoComponentPassive)