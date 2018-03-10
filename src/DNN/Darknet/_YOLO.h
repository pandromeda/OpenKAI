/*
 * _YOLO.h
 *
 *  Created on: Mar 7, 2018
 *      Author: yankai
 */

#ifndef OpenKAI_src_DNN_Darknet__YOLO_H_
#define OpenKAI_src_DNN_Darknet__YOLO_H_

#include "../../Base/common.h"
#include "../../Vision/_VisionBase.h"
#include "../../Detector/_DetectorBase.h"

#ifdef USE_DARKNET

extern "C" {
#include "api.h"
}

namespace kai
{

class _YOLO: public _DetectorBase
{
public:
	_YOLO();
	~_YOLO();

	bool init(void* pKiss);
	bool link(void);
	bool start(void);
	bool draw(void);

private:
	void detect(void);
	void update(void);
	static void* getUpdateThread(void* This)
	{
		((_YOLO*) This)->update();
		return NULL;
	}

public:
	double m_thresh;			//0.24
	double m_hier;			//0.5
	double m_nms; 			//0.4

	yolo_object* m_pYoloObj;
	int m_nMaxDetect;
	int m_nBatch;
	Frame* m_pBGR;
};

}

#endif
#endif