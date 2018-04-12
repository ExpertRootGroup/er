/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERRunAna_H
#define ERRunAna_H

#include "TH1.h"
#include "TCut.h"
#include "TString.h"

#include "FairRunAna.h"

class ERRunAna : public FairRunAna
{

public:
  static ERRunAna* Instance();
  virtual ~ERRunAna(){}
  ERRunAna();

  /*Modifiers*/
  void SetUserCut(TCut cut) {fUserCut = cut;}
  /*Accessors*/

  void Init();
public:
  bool ContentForAnalysis();
private:
  static ERRunAna* fInstance;

  static TCut fUserCut;
  static TH1I* fEventsForProcessing;

  ClassDef(ERRunAna ,1)
};

#endif
