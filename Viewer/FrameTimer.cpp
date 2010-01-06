/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

float g_frametime=0.0001f;

float g_fStartOfFrame, g_fEndOfFrame, g_fRateInv;

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>

__int64 StartClk;

void  InitFrameTimer()
{
	__int64 Rate;
	g_fRateInv = 1.0f/(float)CLOCKS_PER_SEC;

	if( !QueryPerformanceFrequency( ( LARGE_INTEGER* )&Rate) )
		return;

	if(!Rate)
		return;

	g_fRateInv=1.0f/(float)Rate;
	if(!QueryPerformanceCounter( ( LARGE_INTEGER* )&StartClk) )
		return;

}

float CurrentTime_()
{
	__int64 EndClk;
	QueryPerformanceCounter(( LARGE_INTEGER* )&EndClk);
	return  ( EndClk - StartClk )*g_fRateInv;
}

#else
#include <sys/time.h>

timeval StartTime;

/*
 *	Initialise the frame counter
 */
void  InitFrameTimer()
{
	gettimeofday(&StartTime, NULL);
}

/*
 *	Get the current time
 */
float CurrentTime_()
{
	timeval currentTime;
	gettimeofday(&currentTime, NULL);
	
	timeval elapsedtp; 
	timersub(&currentTime, &StartTime, &elapsedtp);
	
	float elapsedSec = (double)elapsedtp.tv_sec + (double)elapsedtp.tv_usec/1000000.0;
	return elapsedSec;
}

#endif

void  SortFrameTimer()
{
	do
	{
		g_fEndOfFrame= CurrentTime_();
	}
	while(g_fEndOfFrame==g_fStartOfFrame);

	g_frametime       =  g_fEndOfFrame - g_fStartOfFrame ;
	g_fStartOfFrame  =  g_fEndOfFrame ;
}

float FrameTime()
{
	return g_frametime;
}

