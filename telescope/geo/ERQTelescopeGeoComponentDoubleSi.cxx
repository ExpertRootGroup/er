/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeGeoComponentDoubleSi.h"

#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairLogger.h"

#include "ERTelescopeSetup.h"

//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentDoubleSi::ConstructGeometryVolume(void) {
  ParseXmlParameters();
  auto* media = CreateMaterial(fMedia); 
  // ----------------- DoubleSi -----------------------------------------------
  TGeoVolume* shell;
  TGeoVolume* strip;
  TGeoVolume* box;
  fVolume =  gGeoManager->MakeBox(this->GetVolumeName(), media, fSizeX / 2, fSizeY / 2,  fSizeZ / 2);
  //------------------ Silicon strip   ---------------------------------------
  Double_t stripX = fSensX / fStripCountX;
  Double_t stripY = fSensY;
  Double_t stripZ = fSensZ - fDeadLayerThicknessFrontSide - fDeadLayerThicknessBackSide;
  strip = gGeoManager->MakeBox("doubleSiStrip" + fOrientAroundZ, media, stripX / 2, 
                                                                       stripY / 2, 
                                                                       stripZ / 2);
  //------------------ Silicon box   -----------------------------------------
  Double_t boxX = stripX;  
  Double_t boxY = stripY / fStripCountY; 
  Double_t boxZ = stripZ; 
  box = gGeoManager->MakeBox("SensitiveDoubleSiBox"+fOrientAroundZ, media, boxX / 2, 
                                                                          boxY / 2, 
                                                                          boxZ / 2);
  //------------------ STRUCTURE  ---------------------------------------------
  //----------------------- Double Si structure -------------------------------
  //------------------ Add fibers to station  along x -----------------------
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  for (Int_t iBox = 0; iBox < fStripCountY ; ++iBox ) {
    Double_t translateY = (fSensY / 2) - boxY / 2 - boxY * iBox ;
    strip->AddNode(box, iBox, new TGeoCombiTrans(0, translateY, 0, fZeroRotation));
  }

  for (Int_t iStripX = 0; iStripX < fStripCountX; iStripX++) {
    Double_t translateX = fSensX / 2 - stripX *(iStripX)-(stripX / 2);
    Double_t translateZ = (fDeadLayerThicknessFrontSide - fDeadLayerThicknessBackSide)/2. ;
    fVolume->AddNode(strip, iStripX, new TGeoCombiTrans(translateX, 0, translateZ, fZeroRotation));
  }
  // TGeoRotation *rotation = new TGeoRotation();
  // rotation->RotateX(fRotation->X());
  // rotation->RotateY(fRotation->Y());
  // rotation->RotateZ(fRotation->Z() + deltaRotAroundZ);
  // fVolume->AddNode(shell, fConstructedObjCount++, 
  //                  new TGeoCombiTrans(fPosition->X(), fPosition->Y(), fPosition->Z(), fRotation));
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentDoubleSi::ParseXmlParameters() {
  TString xmlFile = ERTelescopeSetup::Instance()->GetXMLParametersFile();
  TDOMParser *domParser;//
  //gROOT->ProcessLine(".O 0");
  domParser = new TDOMParser;
  domParser->SetValidate(false); // do not validate with DTD

  Int_t parsecode = domParser->ParseFile(xmlFile);
  if (parsecode < 0) {
     LOG(FATAL) << domParser->GetParseCodeMessage(parsecode) << FairLogger::FairLogger::endl;
    return ;
  }
  TXMLNode *rootNode = domParser->GetXMLDocument()->GetRootNode();
  TXMLNode *detPartNode = rootNode->GetChildren();
  TXMLNode *SiTypeNodes;
  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) { // detector's part
    if (!strcasecmp(detPartNode->GetNodeName(), "SiTypes")) {
      SiTypeNodes = detPartNode->GetChildren();
      for ( ; SiTypeNodes; SiTypeNodes = SiTypeNodes->GetNextNode()) {
        if (!strcasecmp(SiTypeNodes->GetNodeName(), "doubleSiTypes")) {
          TXMLNode* curNode = SiTypeNodes->GetChildren()->GetNextNode();
          for(; curNode; curNode = curNode->GetNextNode()) {
            TList *attrList;
            TXMLAttr *attr = 0;
            if (curNode->HasAttributes()){
              attrList = curNode->GetAttributes();
              TIter next(attrList);
              while ((attr=(TXMLAttr*)next())) {
                if (!strcasecmp("id", attr->GetName())) { 
                  break;
                }
              }
            }
            else {
              continue;
            }
            if(!strcasecmp(fComponentId, attr->GetValue())) {
              FillTwoSidedChannelAttribute(curNode->GetAttributes());
              TXMLNode* curNode2 = curNode->GetChildren();
              for(; curNode2; curNode2 = curNode2->GetNextNode()) {
                if(!strcasecmp(curNode2->GetNodeName(), "doubleSiSize")) {
                  attrList = curNode2->GetAttributes();
                  attr = 0;
                  TIter nextSizeAttr(attrList);
                  while ((attr=(TXMLAttr*)nextSizeAttr())) {
                    if (!strcasecmp("doubleSiX", attr->GetName())) {
                      fSizeX = atof(attr->GetValue());
                    }
                    if (!strcasecmp("doubleSiY", attr->GetName())) {
                      fSizeY = atof(attr->GetValue());
                    }
                    if (!strcasecmp("doubleSiZ", attr->GetName())) {
                      fSizeZ = atof(attr->GetValue());
                    }
                  }
                }
                if(!strcasecmp(curNode2->GetNodeName(), "doubleSiSensAreaSize")) {
                  attrList = curNode2->GetAttributes();
                  attr = 0;
                  TIter nextSensSizeAttr(attrList);
                  while ((attr=(TXMLAttr*)nextSensSizeAttr())) {
                    if (!strcasecmp("doubleSiSensX", attr->GetName())) {
                      fSensX = atof(attr->GetValue());
                    }
                    if (!strcasecmp("doubleSiSensY", attr->GetName())) {
                      fSensY = atof(attr->GetValue());
                    }
                    if (!strcasecmp("doubleSiSensZ", attr->GetName())) {
                      fSensZ = atof(attr->GetValue());
                    }
                  }
                }
                if(!strcasecmp(curNode2->GetNodeName(), "deadLayerThicknessFrontSide")) {
                  fDeadLayerThicknessFrontSide = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "deadLayerThicknessBackSide")) {
                  fDeadLayerThicknessBackSide = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "stripCountX")) {
                  fStripCountX = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "stripCountY")) {
                  fStripCountY = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "doubleSiMedia")) {
                  fMedia = curNode2->GetText();
                }
              }
            }
          }
        }
      }
    }
  }
}
ClassImp(ERQTelescopeGeoComponentDoubleSi)

