/*
 * WCalHit.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: vratik
 */

#include "WCalHit.h"

WCalHit::WCalHit() :
//fTimestamp(0),
fChannel(0),
//fRawAdcVal(0),
//fAdcVal(0),
fAdcCalibrated(0.)
{
	// TODO Auto-generated constructor stub

}

WCalHit::WCalHit(UChar_t p_ch, Float_t p_adcCalib) :
//		fTimestamp(p_ts),
		fChannel(p_ch),
//		fRawAdcVal(p_rawAdc),
//		fAdcVal(p_adc),
		fAdcCalibrated(p_adcCalib)
{

}

WCalHit::~WCalHit() {
	// TODO Auto-generated destructor stub
	Clear();
}

void WCalHit::Clear(Option_t * /*option*/)
{
   TObject::Clear();
}

WCalHit &WCalHit::operator=(const WCalHit &orig)
{
   TObject::operator=(orig);

//   fTimestamp = orig.fTimestamp;
   fChannel = orig.fChannel;
//   fRawAdcVal = orig.fRawAdcVal;
//   fAdcVal = orig.fAdcVal;
   fAdcCalibrated = orig.fAdcCalibrated;

   return *this;
}