/*
 * _FilterBase.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: yankai
 */

#include "_FilterBase.h"

namespace kai
{

_FilterBase::_FilterBase()
{
	m_nProduce = 1;
	m_progress = 0.0;
	m_bComplete = false;

	m_bgNoiseMean = 0;
	m_bgNoiseDev = 0;
	m_bgNoiseType = cv::RNG::NORMAL;

	m_pFrameIn = NULL;
	m_pFrameOut = NULL;
}

_FilterBase::~_FilterBase()
{
	reset();
}

bool _FilterBase::init(void* pKiss)
{
	IF_F(!this->_DataBase::init(pKiss));
	Kiss* pK = (Kiss*) pKiss;
	pK->m_pInst = this;

	KISSm(pK, nProduce);
	KISSm(pK, bgNoiseMean);
	KISSm(pK, bgNoiseDev);
	KISSm(pK, bgNoiseType);

	m_pFrameIn = new Frame();
	m_pFrameOut = new Frame();

	return true;
}

void _FilterBase::reset(void)
{
	this->_DataBase::reset();

	DEL(m_pFrameIn);
	DEL(m_pFrameOut);
}

bool _FilterBase::link(void)
{
	IF_F(!this->_DataBase::link());
	Kiss* pK = (Kiss*) m_pKiss;

	return true;
}

bool _FilterBase::start(void)
{
	m_bComplete = false;
	m_bThreadON = true;
	int retCode = pthread_create(&m_threadID, 0, getUpdateThread, this);
	if (retCode != 0)
	{
		m_bThreadON = false;
		return false;
	}

	return true;
}

void _FilterBase::update(void)
{
	srand(time(NULL));
}

bool _FilterBase::bComplete(void)
{
	return m_bComplete;
}

}

