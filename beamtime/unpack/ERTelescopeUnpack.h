#ifndef ERTelescopeUnpack_H
#define ERTelescopeUnpack_H

#include <map>
#include <vector>

#include "TString.h"

#include "DetEventDetector.h"

#include "ERUnpack.h"

class ERTelescopeUnpack : public ERUnpack
{
  public:
    ERTelescopeUnpack(TString detName);
    virtual ~ERTelescopeUnpack();

    virtual Bool_t Init(SetupConfiguration* setupConf);
    virtual Bool_t DoUnpack(Int_t* data, Int_t size);

  protected:
    void AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, Int_t stripNb, TString digiBranchName);
    void AddCsIDigi(Float_t edep, Double_t time, Int_t wallNb, Int_t blockNb, TString digiBranchName);
    TString FormBranchName(TString type, Int_t sideCount, TString stName, TString XY, TString XYside);
    std::map<TString,TString> fBnames;
    std::map<TString,TString> fSiAmpTimeStations;
    std::vector<TString> fCsIStations;
  public:
    ClassDef(ERTelescopeUnpack, 0)
};


#endif
