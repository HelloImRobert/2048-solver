/**
 *
 * ADTF Template Project Filter.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: belkera $
 * $Date: 2011-06-30 16:51:21 +0200 (Thu, 30 Jun 2011) $
 * $Revision: 26514 $
 *
 * @remarks
 *
 */
#ifndef _SWE_TRACKCONTROL_H_
#define _SWE_TRACKCONTROL_H_

#define OID_ADTF_TEMPLATE_FILTER "adtf.swe.trackcontrol"


//*************************************************************************************************
class cSWE_TrackControl : public adtf::cFilter
{
    ADTF_FILTER(OID_ADTF_TEMPLATE_FILTER, "SWE_TrackControl", adtf::OBJCAT_DataFilter);

protected:
    cInputPin    m_VecLaneMarkings;
    cOutputPin    m_SteeringAngle;

public:
    cSWE_TrackControl(const tChar* __info);
    virtual ~cSWE_TrackControl();

protected:
    tResult Init(tInitStage eStage, __exception);
    tResult Shutdown(tInitStage eStage, __exception);

    // implements IPinEventSink
    tResult OnPinEvent(IPin* pSource,
                       tInt nEventCode,
                       tInt nParam1,
                       tInt nParam2,
                       IMediaSample* pMediaSample);

private:
	cv::Mat m_lalalu;

	tInt m_KernelWidth; /////////////////////////////////////////////////////////////

	
};

//*************************************************************************************************
#endif // _TEMPLATE_PROJECT_FILTER_H_
