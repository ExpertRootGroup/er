// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal source file            -----
// -------------------------------------------------------------------------

#include "ERNeuRadPMTSignal.h"

#include "FairLogger.h"

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
  Double_t t2 = fpoint->AnodeTime() + 12.8/7. * (fpoint->AnodeTime() - fpoint->CathodeTime());
  Double_t A2 = fpoint->Amplitude();
  Double_t t3 = fpoint->AnodeTime() + 15.2/7. * (fpoint->AnodeTime() - fpoint->CathodeTime());
  Double_t A3 = fpoint->Amplitude();
  Double_t t4 = fpoint->AnodeTime() + 23./7. * (fpoint->AnodeTime() - fpoint->CathodeTime());
  Double_t A4 = 0.;
  
  SignalPointsMap separateSignal;
  separateSignal.insert(SignalPoint(t1,A1));
  separateSignal.insert(SignalPoint(t2,A2));
  separateSignal.insert(SignalPoint(t3,A3));
  separateSignal.insert(SignalPoint(t4,A4));
  fSeparateSignals.push_back(separateSignal);
  //if first fiber point in signal
  if (fSignalPoints.size() == 0){
    LOG(INFO) << "First fiber point" << FairLogger::endl;
    LOG(INFO) << t1 << " " << A1 << FairLogger::endl;
    LOG(INFO) << t2 << " " << A2 << FairLogger::endl;
    LOG(INFO) << t3 << " " << A3 << FairLogger::endl;
    LOG(INFO) << t4 << " " << A4 << FairLogger::endl;
    fSignalPoints.insert(SignalPoint(t1,A1));
    fSignalPoints.insert(SignalPoint(t2,A2));
    fSignalPoints.insert(SignalPoint(t3,A3));
    fSignalPoints.insert(SignalPoint(t4,A4));
    return;
  }
  LOG(INFO) << "Next fiber point" << FairLogger::endl;
  LOG(INFO) << t1 << " " << A1 << FairLogger::endl;
  LOG(INFO) << t2 << " " << A2 << FairLogger::endl;
  LOG(INFO) << t3 << " " << A3 << FairLogger::endl;
  LOG(INFO) << t4 << " " << A4 << FairLogger::endl; 

  /*
  ���������� ��������� ����� �������� ������� � ������� �� ������ FiberPoint
  ���������� �� ������ �������� ����� ����� ������ �� ��� ������� � ��������� � ��� �������������� ��������
  �� �����, ���� ��� ���������� ����������� ��������� � ��� ������, ��� ��� ���� �� ���.
  ��� ����� ������� ������ map ���������, � ����� ���� ���������� �������� ���
  */
  struct Change{SignalPointsMap::iterator it; Double_t value;};
  vector<Change> changes;

  SignalPointsMap::iterator itFirstSPoint,itSecSPoint, itSPrevPoint,itSNextPoint;
  
  pair<SignalPointsMap::iterator,bool> ret;
  ret = fSignalPoints.insert(SignalPoint(t1,A1));
  
  itFirstSPoint= ret.first;
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
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  //�������� �������� ������ �����, ���� ��� �������� �� ������� � ������������ ��������
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    if (itSPrevPoint == itFirstSPoint && itFirstSPoint != fSignalPoints.begin())
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
  //�������� �������� ������� �����, ���� ��� �������� �� ������� � ������������ ��������
  itFirstSPoint = itSecSPoint;
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    if (itSPrevPoint == itFirstSPoint && itFirstSPoint != fSignalPoints.begin())
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
  //�������� �������� ��������� �����, ���� ��� �������� �� ������� � ������������ ��������
  itFirstSPoint = itSecSPoint;
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    if (itSPrevPoint == itFirstSPoint && itFirstSPoint != fSignalPoints.begin())
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

ClassImp(ERNeuRadPMTSignal);