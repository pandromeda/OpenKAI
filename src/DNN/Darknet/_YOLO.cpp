/*
 *  Created on: Mar 7, 2018
 *      Author: yankai
 */
#include "_YOLO.h"

#ifdef USE_DARKNET

namespace kai
{

_YOLO::_YOLO()
{
	m_thresh = 0.24;
	m_hier = 0.5;
	m_nms = 0.4;
	m_nPredAvr = 3;

	m_pYoloObj = NULL;
	m_nBatch = 1;
}

_YOLO::~_YOLO()
{
	DEL(m_pYoloObj);
}

bool _YOLO::init(void* pKiss)
{
	IF_F(!this->_ObjectBase::init(pKiss));
	Kiss* pK = (Kiss*) pKiss;
	pK->m_pInst = this;

	KISSm(pK,thresh);
	KISSm(pK,hier);
	KISSm(pK,nms);
	KISSm(pK,nBatch);
	KISSm(pK,nPredAvr);

	IF_Fl(!yoloInit( m_modelFile.c_str(),
					m_trainedFile.c_str(),
					m_labelFile.c_str(),
					m_nPredAvr,
					m_nBatch), "YOLO init failed");

	m_pYoloObj = new yolo_object[OBJECT_N_OBJ];

	m_nClass = yoloNClass();
	for(int i=0; i<m_nClass; i++)
	{
		m_pClassStatis[i].init();
		m_pClassStatis[i].m_name = yoloGetClassName(i);
	}

	bSetActive(true);
	m_bReady = true;
	return true;
}

bool _YOLO::link(void)
{
	IF_F(!this->_ObjectBase::link());
	Kiss* pK = (Kiss*) m_pKiss;

	return true;
}

bool _YOLO::start(void)
{
	m_bThreadON = true;
	int retCode = pthread_create(&m_threadID, 0, getUpdateThread, this);
	if (retCode != 0)
	{
		LOG_E(retCode);
		m_bThreadON = false;
		return false;
	}

	return true;
}

void _YOLO::update(void)
{
	while (m_bThreadON)
	{
		this->autoFPSfrom();

		this->_ObjectBase::update();

		m_obj.update();
		detect();

		this->autoFPSto();
	}

}

void _YOLO::detect(void)
{
	NULL_(m_pVision);

	Frame* pBGR = m_pVision->BGR();
	NULL_(pBGR);
	IF_(pBGR->bEmpty());
	IF_(pBGR->tStamp() <= m_BGR.tStamp());
	m_BGR = *pBGR;
	Mat mBGR = *m_BGR.m();

	for(int r=0; r<m_vROI.size(); r++)
	{
		vDouble4 fRoi = m_vROI[r].m_roi;
		fRoi.x *= mBGR.cols;
		fRoi.y *= mBGR.rows;
		fRoi.z *= mBGR.cols;
		fRoi.w *= mBGR.rows;
		vInt4 iRoi;
		iRoi = fRoi;
		Rect rRoi;
		vInt42rect(iRoi,rRoi);

		IplImage ipl = mBGR(rRoi);
		int nDet = yoloUpdate(&ipl, m_pYoloObj, OBJECT_N_OBJ, (float)m_thresh, (float)m_hier, (float)m_nms);
		IF_CONT(nDet <= 0);

		OBJECT obj;
		for (int i = 0; i < nDet; i++)
		{
			yolo_object* pYO = &m_pYoloObj[i];

			obj.init();
			obj.m_tStamp = m_tStamp;
			obj.setClassMask(pYO->m_mClass);
			obj.setTopClass(pYO->m_topClass, (double)pYO->m_topProb);

			obj.m_bbox.x = rRoi.x + rRoi.width * pYO->m_l;
			obj.m_bbox.y = rRoi.y + rRoi.height * pYO->m_t;
			obj.m_bbox.z = rRoi.x + rRoi.width * pYO->m_r;
			obj.m_bbox.w = rRoi.y + rRoi.height * pYO->m_b;

			obj.m_camSize.x = mBGR.cols;
			obj.m_camSize.y = mBGR.rows;
			obj.i2fBBox();

			if(obj.m_bbox.x < 0)obj.m_bbox.x = 0;
			if(obj.m_bbox.y < 0)obj.m_bbox.y = 0;
			if(obj.m_bbox.z > obj.m_camSize.x)obj.m_bbox.z = obj.m_camSize.x;
			if(obj.m_bbox.w > obj.m_camSize.y)obj.m_bbox.w = obj.m_camSize.y;

			add(&obj);
			LOG_I("Class: "+ i2str(obj.m_topClass));
		}
	}

	m_tStamp = getTimeUsec();
}

bool _YOLO::draw(void)
{
	IF_F(!this->_ObjectBase::draw());

	return true;
}

}
#endif
