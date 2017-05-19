/*********************************************************************************/
/*!
@file           timemanager

@brief         c++11 time tools class.

@author         StevenZhang

Copyright (c)   2014-2016, zhangliutong&zhengsutec, all rights reserved

This file is C++11 default tools class
 

this is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PianoBooster.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef _TIMEMANAGER_timemanager_H_
#define _TIMEMANAGER_timemanager_H_
 
#include <ratio>
#include <chrono>
 
class CTimeManager 
{
public:
	static CTimeManager* getInstance() { static CTimeManager* p = new CTimeManager; return p; }
	/*
		1.开始记录时间,返回当前微秒时间
	*/
	double	time_begin(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		m_beginTime = (long long)timeNow.time_since_epoch().count();

		return m_beginTime;
	}

	/*
		2,记录结束时间返回当前微秒时间 结束时间-开始时间等于, 本段时间差额
	*/
	double time_end(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		m_endTime = (long long)timeNow.time_since_epoch().count();

		return m_endTime;
	}
	/*
		更新时间,初始化，下一次调用时间更新,将返回时间update的插值
	*/
	void init_time_update(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		long long _timenow = (long long)timeNow.time_since_epoch().count();
		m_lastRecordTime = _timenow;
	}
	double time_update(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		long long _timenow = (long long)timeNow.time_since_epoch().count();
		m_intervalsTime = _timenow - m_lastRecordTime;
		m_lastRecordTime = _timenow;
		return m_intervalsTime;
	}


	/*
		获得时间间隔函数，需要调用一次timebegin,开始进行计数和调用update函数进行
	*/
	double getIntervalsTime() { return (double)m_intervalsTime; }

	/*
		获得当前的时间
	*/
	double getCurrentTime(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		return (long long)timeNow.time_since_epoch().count();
	}
protected:
	/*记录开始时间系统以毫秒进行计数*/
	long long m_beginTime;
	long long m_endTime;
	/*上一次记录时间 以毫秒进行计数*/
	long long m_lastRecordTime;
	 
	/*间隔时间数，以毫秒进行计数 elapse time*/
	int m_intervalsTime;
};
#endif /*_TIMEMANAGER_timemanager_H_*/