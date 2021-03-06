/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentDoubleSi_H
#define ERQTelescopeGeoComponentDoubleSi_H

#include "ERTelescopeGeoComponentDoubleSi.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoComponentDoubleSi : public ERTelescopeGeoComponentDoubleSi {
public:
  using ERTelescopeGeoComponentDoubleSi::ERTelescopeGeoComponentDoubleSi;
  virtual void ConstructGeometryVolume(void);
private:
  virtual void ParseXmlParameters();
  Double_t fSensX = 0.;
  Double_t fSensY = 0.;
  Double_t fSensZ = 0.;
  ClassDef(ERQTelescopeGeoComponentDoubleSi,1)
};
#endif
