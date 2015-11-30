// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal source file            -----
// -------------------------------------------------------------------------

#include "ERNeuRadPMTSignal.h"

#include "FairLogger.h"
#include <iostream>
#include <algorithm>
using namespace std;

// -----   Default constructor   -------------------------------------------
ERNeuRadPMTSignal::ERNeuRadPMTSignal(){

}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
ERNeuRadPMTSignal::~ERNeuRadPMTSignal(){

}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void ERNeuRadPMTSignal::AddFiberPoint(ERNeuRadFiberPoint* fpoint){
  //���������� ����� ����� ����������� ��������� ����� �� ��������� ���
  Double_t t1 = fpoint->AnodeTime();
  Double_t A1 = 0.;
  Double_t t2 = t1 + 0.4;
  Double_t A2 = fpoint->Amplitude();
  Double_t t3 = t2 + 0.1;
  Double_t A3 = fpoint->Amplitude();
  Double_t t4 = t3 + 0.8;
  Double_t A4 = 0.;
  SignalPointsMap separateSignal;
  separateSignal.insert(SignalPoint(t1,A1));
  separateSignal.insert(SignalPoint(t2,A2));
  separateSignal.insert(SignalPoint(t3,A3));
  separateSignal.insert(SignalPoint(t4,A4));
  fSeparateSignals.push_back(separateSignal);
  //if first fiber point in signal
  if (fSignalPoints.size() == 0){
    //LOG(INFO) << "First fiber point" << FairLogger::endl;
    //LOG(INFO) << t1 << " " << A1 << FairLogger::endl;
    //LOG(INFO) << t2 << " " << A2 << FairLogger::endl;
    //LOG(INFO) << t3 << " " << A3 << FairLogger::endl;
    //LOG(INFO) << t4 << " " << A4 << FairLogger::endl;
    fSignalPoints.insert(SignalPoint(t1,A1));
    fSignalPoints.insert(SignalPoint(t2,A2));
    fSignalPoints.insert(SignalPoint(t3,A3));
    fSignalPoints.insert(SignalPoint(t4,A4));
    return;
  }
  //LOG(INFO) << "Next fiber point" << FairLogger::endl;
  //LOG(INFO) << t1 << " " << A1 << FairLogger::endl;
  //LOG(INFO) << t2 << " " << A2 << FairLogger::endl;
  //LOG(INFO) << t3 << " " << A3 << FairLogger::endl;
  //LOG(INFO) << t4 << " " << A4 << FairLogger::endl; 

  /*
  ���������� ��������� ����� �������� ������� � ������� �� ������ FiberPoint
  ���������� �� ������ �������� ����� ����� ������ �� ��� ������� � ��������� � ��� �������������� ��������
  �� �����, ���� ��� ���������� ����������� ��������� � ��� ������, ��� ��� ���� �� ���.
  ��� ����� ������� ������ map ���������, � ����� ���� ���������� �������� ���
  */
  
  struct Change{SignalPointsMap::iterator it; Double_t value;};
  vector<Change> changes;
  vector<Double_t> fpTimes;
  
  SignalPointsMap::iterator itFirstSPoint,itSecSPoint, itSPrevPoint,itSNextPoint;
  
  pair<SignalPointsMap::iterator,bool> ret;
  ret = fSignalPoints.insert(SignalPoint(t1,A1));
  itFirstSPoint= ret.first;
  fpTimes.push_back(t1);
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  //�������� �������� ������ �����, ���� ��� �������� �� ������� � ������������ ��������
  if (!(itFirstSPoint == fSignalPoints.begin() || itSNextPoint == fSignalPoints.end())){
    //������� ���������� � ��������� ����� �������� �������, ���������� ��������� ������
    itSPrevPoint = itFirstSPoint;
    itSPrevPoint--;
    Double_t newValue = itFirstSPoint->second +(itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itFirstSPoint->first-itSPrevPoint->first) + itSPrevPoint->second; 
    Change change = Change{itFirstSPoint,newValue};
    changes.push_back(change);
  }

  ret = fSignalPoints.insert(SignalPoint(t2,A2));
  fpTimes.push_back(t2);
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  //�������� �������� ������ �����, ���� ��� �������� �� ������� � ������������ ��������
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    while (find(fpTimes.begin(), fpTimes.end(), itSPrevPoint->first) != fpTimes.end())
      itSPrevPoint--;
    Double_t newValue = itSecSPoint->second + (itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itSecSPoint->first-itSPrevPoint->first) + itSPrevPoint->second;
    Change change = Change{itSecSPoint,newValue};
    changes.push_back(change);
  }
  
  //�������� �������� ����� ����� ������������
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  for (SignalPointsMap::iterator it = itSNextPoint; it != itSecSPoint; ++it){
    Double_t newValue = it->second + (itSecSPoint->second - itFirstSPoint->second)/(itSecSPoint->first - itFirstSPoint->first)
            *(it->first-itFirstSPoint->first) + itFirstSPoint->second;
    Change change = Change{it,newValue};
    changes.push_back(change);
  }

  ret = fSignalPoints.insert(SignalPoint(t3,A3));
  fpTimes.push_back(t3);
  //�������� �������� ������� �����, ���� ��� �������� �� ������� � ������������ ��������
  itFirstSPoint = itSecSPoint;
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    while (find(fpTimes.begin(), fpTimes.end(), itSPrevPoint->first) != fpTimes.end())
      itSPrevPoint--;
    Double_t newValue = itSecSPoint->second + (itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itSecSPoint->first-itSPrevPoint->first) + itSPrevPoint->second;
    Change change = Change{itSecSPoint,newValue};
    changes.push_back(change);
  }

  //�������� �������� ����� ����� ������������
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  for (SignalPointsMap::iterator it = itSNextPoint; it != itSecSPoint; ++it){
    Double_t newValue = it->second + (itSecSPoint->second - itFirstSPoint->second)/(itSecSPoint->first - itFirstSPoint->first)
            *(it->first-itFirstSPoint->first) + itFirstSPoint->second;
    Change change = Change{it,newValue};
    changes.push_back(change);
  }

  ret = fSignalPoints.insert(SignalPoint(t4,A4));
  fpTimes.push_back(t4);
  //�������� �������� ��������� �����, ���� ��� �������� �� ������� � ������������ ��������
  itFirstSPoint = itSecSPoint;
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    while (find(fpTimes.begin(), fpTimes.end(), itSPrevPoint->first) != fpTimes.end())
      itSPrevPoint--;
    Double_t newValue = itSecSPoint->second + (itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itSecSPoint->first-itSPrevPoint->first) + itSPrevPoint->second;
    Change change = Change{itSecSPoint,newValue};
    changes.push_back(change);
  }

  //�������� �������� ����� ����� ������������
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  for (SignalPointsMap::iterator it = itSNextPoint; it != itSecSPoint; ++it){
    Double_t newValue = it->second + (itSecSPoint->second - itFirstSPoint->second)/(itSecSPoint->first - itFirstSPoint->first)
            *(it->first-itFirstSPoint->first) + itFirstSPoint->second;
    Change change = Change{it,newValue};
    changes.push_back(change);
  }
  
  //��������� ��������� � �������
  for (vector<Change>::iterator it = changes.begin(); it != changes.end(); ++it){
    (*it).it->second = (*it).value;
  }
  
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
vector<Double_t> ERNeuRadPMTSignal::GetIntersections(Double_t discriminatorThreshold){
  vector<Double_t> intersections;

  SignalPointsMap::iterator itSp= fSignalPoints.begin();
  itSp++;
  SignalPointsMap::iterator itPrevSp;
  bool inSignal = false;
  for (; itSp != fSignalPoints.end(); itSp++){
    itPrevSp= itSp;
    itPrevSp--;
    if (!inSignal){
      if(itSp->second < discriminatorThreshold){
        continue;
      }else{
        inSignal = true;
        Double_t intersect = (discriminatorThreshold - itPrevSp->second)/(itSp->second - itPrevSp->second)
                      *(itSp->first - itPrevSp->first) + itPrevSp->first;
                      
        intersections.push_back(intersect);
      }
    }
    else{
      if(itSp->second > discriminatorThreshold){
        continue;
      }else{
        inSignal = false;
        Double_t intersect = (discriminatorThreshold - itPrevSp->second)/(itSp->second - itPrevSp->second)
                      *(itSp->first - itPrevSp->first) + itPrevSp->first;
                      
        intersections.push_back(intersect);
      }  
    }
  }
  return intersections;
}
// -------------------------------------------------------------------------

ClassImp(ERNeuRadPMTSignal);