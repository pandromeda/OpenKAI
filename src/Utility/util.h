#ifndef OpenKAI_src_Utility_util_H_
#define OpenKAI_src_Utility_util_H_

#include "../Base/platform.h"
#include "../Base/cv.h"
#include <time.h>
#include <sys/time.h>

using namespace std;

namespace kai
{

inline string tFormat(void)
{
	time_t timer;
	char buffer[26];
	struct tm* tm_info;
	time(&timer);
	tm_info = localtime(&timer);
	strftime(buffer, 26, "%Y-%m-%d_%H-%M-%S", tm_info);
	puts(buffer);

	string str = buffer;
	return str;
}

inline string getFileDir(string file)
{
	return file.erase(file.find_last_of('/') + 1, string::npos);
}

inline string checkDirName(string& dir)
{
	if (dir.at(dir.length() - 1) != '/')
		dir.push_back('/');

	return dir;
}

inline uint32_t getTimeBootMs()
{
	// get number of milliseconds since boot
	struct timespec tFromBoot;
	clock_gettime(CLOCK_BOOTTIME, &tFromBoot);

	return tFromBoot.tv_sec * 1000 + tFromBoot.tv_nsec / 1000000;
}

inline uint64_t getTimeUsec()
{
	struct timeval tStamp;
	gettimeofday(&tStamp, NULL);
	uint64_t time = (uint64_t) tStamp.tv_sec * (uint64_t) 1000000
			+ tStamp.tv_usec;

	return time;
}

inline double NormRand(void)
{
	return ((double) rand()) / ((double) RAND_MAX);
}

inline string uuid(void)
{
	uuid_t uuid;
	uuid_generate(uuid);

	char cUuid[64];
	uuid_unparse(uuid, cUuid);
	string strUuid = cUuid;

	return strUuid;
}

inline int small(int a, int b)
{
	if (a > b)
		return b;
	return a;
}

inline int big(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

inline double small(double a, double b)
{
	if (a > b)
		return b;
	return a;
}

inline double big(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

inline double Hdg(double anyDeg)
{
	while (anyDeg > 360)
		anyDeg -= 360;

	while (anyDeg < 0)
		anyDeg += 360;

	return anyDeg;
}

inline double dHdg(double hFrom, double hTo)
{
	hFrom = Hdg(hFrom);
	hTo = Hdg(hTo);

	double d = hTo - hFrom;

	if (d > 180)
		d -= 360;
	else if (d < -180)
		d += 360;

	return d;
}

inline bool bOverlapped(vInt4* pA, vInt4* pB)
{
	if (pA->z < pB->x || pA->x > pB->z)
		return false;

	if (pA->w < pB->y || pA->y > pB->w)
		return false;

	return true;
}

inline bool bOverlapped(vDouble4* pA, vDouble4* pB)
{
	if (pA->z < pB->x || pA->x > pB->z)
		return false;

	if (pA->w < pB->y || pA->y > pB->w)
		return false;

	return true;
}

inline double overlapRatio(vInt4* pA, vInt4* pB)
{
	if (!bOverlapped(pA, pB))
		return 0.0;

	vInt4* pT;
	if (pA->area() < pB->area())
	{
		SWAP(pA, pB, pT);
	}

	//pA > pB
	double w = pA->width();
	if (pB->width() > w)
		w = pB->width();
	double h = pA->height();
	if (pB->height() > h)
		h = pB->height();

	double rX = pB->z - pA->x;
	if (pB->x > pA->x)
		rX = pA->z - pB->x;

	rX /= w;

	double rY = pB->w - pA->y;
	if (pB->y > pA->y)
		rY = pA->w - pB->y;

	rY /= h;

	return rX * rY;
}

inline double overlapRatio(vDouble4* pA, vDouble4* pB)
{
	if (!bOverlapped(pA, pB))
		return 0.0;

	vDouble4* pT;
	if (pA->area() < pB->area())
	{
		SWAP(pA, pB, pT);
	}

	//pA > pB
	double w = pA->width();
	if (pB->width() > w)
		w = pB->width();
	double h = pA->height();
	if (pB->height() > h)
		h = pB->height();

	double rX = pB->z - pA->x;
	if (pB->x > pA->x)
		rX = pA->z - pB->x;

	rX /= w;

	double rY = pB->w - pA->y;
	if (pB->y > pA->y)
		rY = pA->w - pB->y;

	rY /= h;

	return rX * rY;
}

inline void rect2vInt4(Rect r, vInt4& v)
{
	v.x = r.x;
	v.y = r.y;
	v.z = r.x + r.width;
	v.w = r.y + r.height;
}

inline void rect2vInt4(Rect2d r, vInt4& v)
{
	v.x = r.x;
	v.y = r.y;
	v.z = r.x + r.width;
	v.w = r.y + r.height;
}

inline void vInt42rect(vInt4 v, Rect& r)
{
	r.x = v.x;
	r.y = v.y;
	r.width = v.z - v.x;
	r.height = v.w - v.y;
}

inline void vInt42rect(vInt4 v, Rect2d& r)
{
	r.x = v.x;
	r.y = v.y;
	r.width = v.z - v.x;
	r.height = v.w - v.y;
}

inline int constrain(int v, int a, int b)
{
	int min, max;
	if (a < b)
	{
		min = a;
		max = b;
	}
	else
	{
		max = a;
		min = b;
	}

	if (v > max)
		v = max;
	else if (v < min)
		v = min;

	return v;
}

inline double constrain(double v, double a, double b)
{
	double min, max;
	if (a < b)
	{
		min = a;
		max = b;
	}
	else
	{
		max = a;
		min = b;
	}

	if (v > max)
		v = max;
	else if (v < min)
		v = min;

	return v;
}

inline void pack_int16(void* pB, int16_t v, bool bOrder = true)
{
	if(bOrder)
		v = (int16_t)htons((uint16_t)v);

	memcpy(pB, &v, sizeof(int16_t));
}

inline int16_t unpack_int16(const void* pB, bool bOrder = true)
{
	int16_t v = 0;
	memcpy(&v, pB, sizeof(int16_t));

	if(bOrder)
		v = (int16_t)ntohs((uint16_t)v);

	return v;
}

inline void pack_uint16(void* pB, uint16_t v, bool bOrder = true)
{
	if(bOrder)
		v = htons(v);

	memcpy(pB, &v, sizeof(uint16_t));
}

inline uint16_t unpack_uint16(const void* pB, bool bOrder = true)
{
	uint16_t v = 0;
	memcpy(&v, pB, sizeof(uint16_t));

	if(bOrder)
		v = ntohs(v);

	return v;
}

inline void pack_int32(void* pB, int32_t v, bool bOrder = true)
{
	if(bOrder)
		v = (int32_t)htonl((uint32_t)v);

	memcpy(pB, &v, sizeof(int32_t));
}

inline int32_t unpack_int32(const void* pB, bool bOrder = true)
{
	int32_t v = 0;
	memcpy(&v, pB, sizeof(int32_t));

	if(bOrder)
		v = (int32_t)ntohl((uint32_t)v);

	return v;
}

inline void pack_uint32(void* pB, uint32_t v, bool bOrder = true)
{
	if(bOrder)
		v = htonl(v);

	memcpy(pB, &v, sizeof(uint32_t));
}

inline uint32_t unpack_uint32(const void* pB, bool bOrder = true)
{
	uint32_t v = 0;
	memcpy(&v, pB, sizeof(uint32_t));

	if(bOrder)
		v = ntohl(v);

	return v;
}

#define UTIL_BUF 32

inline string i2str(int32_t val)
{
	char buf[UTIL_BUF];
	snprintf(buf, UTIL_BUF, "%d", val);
	return string(buf);
}

inline string li2str(long val)
{
	char buf[UTIL_BUF];
	snprintf(buf, UTIL_BUF, "%ld", val);
	return string(buf);
}

inline string f2str(float val)
{
	char buf[UTIL_BUF];
	snprintf(buf, UTIL_BUF, "%.3f", val);
	return string(buf);
}

inline string f2str(float val, int nDigit)
{
	char buf[UTIL_BUF];
	string format = "%."+i2str(nDigit)+"f";
	snprintf(buf, UTIL_BUF, format.c_str(), val);
	return string(buf);
}

inline string lf2str(double val)
{
	char buf[UTIL_BUF];
	snprintf(buf, UTIL_BUF, "%.3f", val);
	return string(buf);
}

inline string lf2str(double val, int nDigit)
{
	char buf[UTIL_BUF];
	string format = "%."+i2str(nDigit)+"f";
	snprintf(buf, UTIL_BUF, format.c_str(), val);
	return string(buf);
}

inline void trimJson(string* pStr)
{
	unsigned int idx = pStr->rfind('}', pStr->size()) + 1;
	if (pStr->size() > idx)
	{
		pStr->erase(idx);
	}

	idx = pStr->find('{', 0);
	if (idx != string::npos && idx > 0)
	{
		pStr->erase(0, idx - 1);
	}
}

inline vector<string> splitBy(string str, char c)
{
	vector<string> v;
	string buf = "";
	stringstream ss;

	ss << str;
	while (getline(ss, buf, c))
	{
		v.push_back(buf);
	}

	return v;
}

}
#endif
