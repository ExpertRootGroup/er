/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentPassive_H
#define ERQTelescopeGeoComponentPassive_H

#include "TString.h"
#include "TVector3.h"
#include "TFile.h"

#include "ERGeoComponent.h"

class ERQTelescopeGeoComponentPassive : public ERGeoComponent {
  public:
    ERQTelescopeGeoComponentPassive() = default;
    ERQTelescopeGeoComponentPassive(const TString& fileName, const TString& topVolumeName,
                                    const TVector3& position, const TVector3& rotation);
    virtual void ConstructGeometryVolume();
    virtual std::list<TString> GetBranchNames(ERDataObjectType object) const {return {};}
  protected:
    virtual void ParseXmlParameters() {}
    TFile* fGeoFile = nullptr;
    TString fFileName;
    TString fTopVolumeName;
    
    ClassDef(ERQTelescopeGeoComponentPassive,1)                            
};
#endif 
