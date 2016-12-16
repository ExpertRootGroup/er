#include "ERLi10Reconstructor.h"

#include <vector>

#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TDatabasePDG.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include<iostream>
using namespace std;

#include "ERDetectorList.h"
#include "ERNDHit.h"
#include "ERDSRDHit.h"
#include "ERLi10EventHeader.h"


// ----------------------------------------------------------------------------
ERLi10Reconstructor::ERLi10Reconstructor()
  : FairTask("ER Li10 reconstruction scheme")
,fDSRDHits(NULL)
,fNDHits(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERLi10Reconstructor::ERLi10Reconstructor(Int_t verbose)
  : FairTask("ER Li10 reconstruction scheme", verbose)
,fDSRDHits(NULL)
,fNDHits(NULL)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERLi10Reconstructor::~ERLi10Reconstructor()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERLi10Reconstructor::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERLi10Reconstructor::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fNDHits = (TClonesArray*) ioman->GetObject("NDHit");
  if (!fNDHits)
    Fatal("Init", "Can`t find collection NDHit!"); 

  fDSRDHits = (TClonesArray*) ioman->GetObject("DSRDHit");
  if (!fDSRDHits)
    Fatal("Init", "Can`t find collection NDHit!"); 

  //todo Этот код должен умереть. Не может быть MC информации в реконструкции
  fMCHeader = (ERLi10MCEventHeader*) ioman->GetObject("MCEventHeader.");
  if (!fMCHeader)
    Fatal("Init", "Can`t find MCEventHeader!");
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERLi10Reconstructor::Exec(Option_t* opt)
{
  std::cout << "ERLi10Reconstructor: "<< std::endl;
  std::cout << "DSRD Hits: "<< std::endl;
  for (Int_t iHit = 0; iHit < fDSRDHits->GetEntriesFast(); iHit++){
    ERDSRDHit* hit = (ERDSRDHit*)fDSRDHits->At(iHit);
    std::cout << iHit << " Eloss:" << hit->Eloss() << " time:" << hit->Time() << std::endl; 
  }
  std::cout << "ND Hits: "<< std::endl;
  for (Int_t iHit = 0; iHit < fNDHits->GetEntriesFast(); iHit++){
    ERNDHit* hit = (ERNDHit*)fNDHits->At(iHit);
    std::cout << iHit << " Eloss:" << hit->LightYield() << " time:" << hit->Time() << " NeutronProb:"<< hit->NeutronProb() <<  std::endl; 
  }

  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) 
    Fatal("SetParContainers", "No analysis run");
  ERLi10EventHeader* header = (ERLi10EventHeader*)run->GetEventHeader();

  /*
  Энергия нейтрона находится по времени пролета. Для этого на 1 метр выше по пучку от мишени стоит
  пластиковый сцинтиллятор (материал как нойрад) толщиной 100 микрон. Момент прохождения пластика налетающим ионом- это старт.
  Дальше надо пересчитать время на момент взаимодействия (вычесть пролет ионом через 1 метр) , но это не сейчас.

  Сейчас скорость нейтрона равна расстояние от мишени до центра Стильбена деленная на время между моментом реакции и регистрацией нейтрона в стильбене. 
  Полная энергия нейтрона = масса нейтрона умножить на С а в квадрате и умножить на гамма-фактор, 
  куда в качестве беты входит измеренная скорость делить на С (скорость света в вакууме).
  */

  //Пропускаем событие, если количество хитов больше одного
  if (fNDHits->GetEntriesFast() == 1){
    ERNDHit* hit = (ERNDHit*)fNDHits->At(0);
    TVector3 hitPos;
    hit->Position(hitPos);
    TVector3 reactionPos(0.,0.,fMCHeader->ReactionPos());
    Float_t L = (hitPos-reactionPos).Mag(); //расстояние от рекции до хита.
    Float_t Vn = L/(hit->Time()-fMCHeader->ReactionTime());
    Float_t Mn = TDatabasePDG::Instance()->GetParticle("neutron")->Mass();
    Float_t beta = Vn/TMath::C();
    Float_t gamma = 1./TMath::Sqrt(1-beta*beta);
    Float_t NEnergy = Mn*gamma;
  }
  else {
    header->SetNEnergy(-1);
  }

   /*
  Протон в кремнии полностью поглощается. Центр сегмента говорит о направлении импульса 
  (вектор из точки взаимодействия, пока что центр мишени, потом возможно измерение координат налетающего иона).
   А энерговыделение в кремнии равно кинетической энергии протона. Все неидеальности прозже в дижитизации.
  */

  if (fDSRDHits->GetEntriesFast() == 1){
    ERDSRDHit* hit = (ERDSRDHit*)fDSRDHits->At(0);
    header->SetPEnergy(hit->Eloss());
  }
  else {
    header->SetPEnergy(-1);
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERLi10Reconstructor::Reset()
{
  if (fNDHits) {
    fNDHits->Delete();
  }
  if (fDSRDHits) {
    fDSRDHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERLi10Reconstructor::Finish()
{   

}
// ----------------------------------------------------------------------------
/*
// ----------------------------------------------------------------------------
ERNDHit* ERLi10Reconstructor::AddHit(Int_t detID, TVector3& pos, TVector3& dpos,
                       Int_t point_index, Float_t eloss, Float_t time,Float_t neutronProb)
{
  ERNDHit *hit = new((*fNDHits)[fNDHits->GetEntriesFast()])
              ERNDHit(fNDHits->GetEntriesFast(),detID, pos, dpos, point_index, eloss, time, neutronProb);
  return hit;
}
// ----------------------------------------------------------------------------
*/
//-----------------------------------------------------------------------------
ClassImp(ERLi10Reconstructor)
