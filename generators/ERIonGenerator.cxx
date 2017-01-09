/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                   ERIonGenerator source file                 -----
// -----          Created 09/07/04  by V. Friese / D.Bertini           -----
// -------------------------------------------------------------------------
#include "ERIonGenerator.h"

#include "FairIon.h"                    // for FairIon
#include "FairParticle.h"               // for FairParticle
#include "FairPrimaryGenerator.h"       // for FairPrimaryGenerator
#include "FairRunSim.h"                 // for FairRunSim
#include "FairLogger.h"                 // for logging

#include "Riosfwd.h"                    // for ostream
#include "TDatabasePDG.h"               // for TDatabasePDG
#include "TObjArray.h"                  // for TObjArray
#include "TParticle.h"                  // for TParticle
#include "TParticlePDG.h"               // for TParticlePDG
#include "TRandom.h"

#include <stdio.h>                      // for NULL, sprintf

// -----   Default constructor   ------------------------------------------
ERIonGenerator::ERIonGenerator()
  :FairGenerator(),
   fMult(0),
   fIon(NULL),  fQ(0)
{
//  LOG(WARNING) << "ERIonGenerator: "
//               << " Please do not use the default constructor! " 
//               << FairLogger::endl;
}
// ------------------------------------------------------------------------

ERIonGenerator::ERIonGenerator(const Char_t* ionName, Int_t mult)
  :FairGenerator(),
   fMult(mult),
   fIon(NULL),  fQ(0)

{
  FairRunSim* fRun=FairRunSim::Instance();
  TObjArray* UserIons=fRun->GetUserDefIons();
  TObjArray* UserParticles=fRun->GetUserDefParticles();
  FairParticle* part=0;
  fIon =static_cast<FairIon*>(UserIons->FindObject(ionName));
  if(fIon) {
    fMult = mult;

  } else {
    part= static_cast<FairParticle*>(UserParticles->FindObject(ionName));
    if(part) {
      TParticle* particle=part->GetParticle();
      fMult = mult;
    }
  }
  if(fIon==0 && part==0 ) {
    LOG(FATAL) << "Ion or Particle is not defined !" 
	       << FairLogger::endl;
  }
  SetPhiRange();
}
// ------------------------------------------------------------------------



// -----   Default constructor   ------------------------------------------
ERIonGenerator::ERIonGenerator(TString name, Int_t z, Int_t a, Int_t q, Int_t mult)
  :FairGenerator(),
  fPDGType(-1),fMult(mult),fPDGMass(0),fPtMin(0),fPtMax(0),
  fPhiMin(0),fPhiMax(0),fEtaMin(0),fEtaMax(0),fYMin(0),fYMax(0),
  fPMin(0),fPMax(0),fThetaMin(0),fThetaMax(0),fX(0),fY(0),fZ(0),
  fX1(0),fY1(0),fX2(0),fY2(0),
  fEtaRangeIsSet(0), fYRangeIsSet(0),fThetaRangeIsSet(0),
  fCosThetaIsSet(0), fPtRangeIsSet(0), fPRangeIsSet(0),
  fPointVtxIsSet(0),fBoxVtxIsSet(0),fDebug(0),fIon(NULL), fName(name)

{
  SetPhiRange();
  fIon= new FairIon(fName, z, a, q);
  FairRunSim* run = FairRunSim::Instance();
  if ( ! run ) {
    LOG(ERROR) << "No FairRun instantised!" 
	       << FairLogger::endl;
  } else {
    run->AddNewIon(fIon);
  }
}
//_________________________________________________________________________



// -----   Destructor   ---------------------------------------------------
ERIonGenerator::~ERIonGenerator()
{
// if (fIon) delete fIon;
}
//_________________________________________________________________________



// -----   Public method SetExcitationEnergy   ----------------------------
void ERIonGenerator::SetExcitationEnergy(Double_t eExc)
{
  fIon->SetExcEnergy(eExc);
}
//_________________________________________________________________________



// -----   Public method SetMass   ----------------------------------------
void ERIonGenerator::SetMass(Double_t mass)
{
  fIon->SetMass(mass);
}
//_________________________________________________________________________



// -----   Public method ReadEvent   --------------------------------------
Bool_t ERIonGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{

// if ( ! fIon ) {
//   LOG(WARNING) << "ERIonGenerator: No ion defined! " 
//                << FairLogger::endl;
//   return kFALSE;
// }
    Double32_t pabs=0, phi, pt=0, theta=0, eta, y, mt, px, py, pz=0;

  // Generate particles
  for (Int_t k = 0; k < fMult; k++) {
    phi = gRandom->Uniform(fPhiMin,fPhiMax) * TMath::DegToRad();

    if      (fPRangeIsSet ) { pabs = gRandom->Uniform(fPMin,fPMax); }
    else if (fPtRangeIsSet) { pt   = gRandom->Uniform(fPtMin,fPtMax); }

    if      (fThetaRangeIsSet) {
      if (fCosThetaIsSet)
        theta = acos(gRandom->Uniform(cos(fThetaMin* TMath::DegToRad()),
                                      cos(fThetaMax* TMath::DegToRad())));
      else {
        theta = gRandom->Uniform(fThetaMin,fThetaMax) * TMath::DegToRad();
      }
    } else if (fEtaRangeIsSet) {
      eta   = gRandom->Uniform(fEtaMin,fEtaMax);
      theta = 2*TMath::ATan(TMath::Exp(-eta));
    } else if (fYRangeIsSet) {
      y     = gRandom->Uniform(fYMin,fYMax);
      mt = TMath::Sqrt(fPDGMass*fPDGMass + pt*pt);
      pz = mt * TMath::SinH(y);
    }

    if (fThetaRangeIsSet || fEtaRangeIsSet) {
      if      (fPRangeIsSet ) {
        pz = pabs*TMath::Cos(theta);
        pt = pabs*TMath::Sin(theta);
      } else if (fPtRangeIsSet) {
        pz = pt/TMath::Tan(theta);
      }
    }

    px = pt*TMath::Cos(phi);
    py = pt*TMath::Sin(phi);

    if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
    }

    TParticlePDG* thisPart =
      TDatabasePDG::Instance()->GetParticle(fIon->GetName());
    if ( ! thisPart ) {
      LOG(WARNING) << "ERIonGenerator: Ion " << fIon->GetName()
  		 << " not found in database!" << FairLogger::endl;
      return kFALSE;
    }

    int pdgType = thisPart->PdgCode();

    std::cout << "ERIonGenerator: Generating " << fMult << " ions of type "
  	    << fIon->GetName() << " (PDG code " << pdgType << ")" 
  	    << std::endl;
    std::cout << "    Momentum (" << px << ", " << py << ", " << pz
  	    << ") Gev from vertex (" << fX << ", " << fY
  	    << ", " << fZ << ") cm" << std::endl;
    primGen->AddTrack(pdgType, px, py, pz, fX, fY, fZ);
  }

  return kTRUE;

}

//_____________________________________________________________________________

ClassImp(ERIonGenerator)