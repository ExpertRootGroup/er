// -------------------------------------------------------------------------
// -----                      ERQTelescopeSiPoint source file                -----
// -------------------------------------------------------------------------

#include "ERQTelescopeSiPoint.h"
#include <iostream>
using namespace std;
// -----   Default constructor   -------------------------------------------
ERQTelescopeSiPoint::ERQTelescopeSiPoint()
  : FairMCPoint(),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.),
    fN_Station(-1), fX_Strip(-1), fY_Strip(-1)
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ERQTelescopeSiPoint::ERQTelescopeSiPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Double_t mass,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss,Int_t N_Station ,Int_t X_Strip, Int_t Y_Strip)
  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
    fEventID(eventID),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fN_Station(N_Station), fX_Strip(X_Strip), fY_Strip(Y_Strip)
{
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
ERQTelescopeSiPoint::ERQTelescopeSiPoint(const ERQTelescopeSiPoint& right)
  : FairMCPoint(right),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fSector(right.fSector), fSensor(right.fSensor)
    fN_Station(right.fN_Station), fX_Strip(right.fX_Strip), fY_Strip(right.fY_Strip)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERQTelescopeSiPoint::~ERQTelescopeSiPoint()
{
}
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void ERQTelescopeSiPoint::Print(const Option_t* opt /* = 0*/) const
{
  cout << "-I- ERQTelescopeSiPoint: track " << fTrackID << " mother track = " << fMot0TrackID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength << " cm" << endl;
  cout << "    Energy loss " << fELoss << " keV "<< endl;
}
// -------------------------------------------------------------------------



// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ERQTelescopeSiPoint::GetX(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------



// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ERQTelescopeSiPoint::GetY(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------



// -----   Public method IsUsable   ----------------------------------------
Bool_t ERQTelescopeSiPoint::IsUsable() const
{
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------



ClassImp(ERQTelescopeSiPoint)
