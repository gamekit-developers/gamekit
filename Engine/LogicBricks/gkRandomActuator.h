/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/

#ifndef GKRANDOMACTUATOR_H
#define GKRANDOMACTUATOR_H

#include "gkLogicActuator.h"
#include "utRandom.h"

class gkRandomActuator : public gkLogicActuator
{
public:

	enum Distribution
	{
		RA_BOOL_CONSTANT,
		RA_BOOL_UNIFORM,
		RA_BOOL_BERNOUILLI,
		RA_INT_CONSTANT,
		RA_INT_UNIFORM,
		RA_INT_POISSON,
		RA_FLOAT_CONSTANT,
		RA_FLOAT_UNIFORM,
		RA_FLOAT_NORMAL,
		RA_FLOAT_NEGEXP,
	};

private:
	utRandomNumberGenerator* m_randGen;
	int m_distribution;
	int m_seed;
	gkString m_prop;
	float m_min;
	float m_max;
	float m_constant;
	float m_mean;
	float m_deviation;
	float m_halflife;
	int m_current, m_count;

public:
	gkRandomActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkRandomActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);
	void execute(void);

	void                      setSeed(int v);
	GK_INLINE void            setDistribution(int v)         {m_distribution = v;}
	GK_INLINE void            setProperty(const gkString& v) {m_prop = v;}
	GK_INLINE void            setMin(float v)                {m_min = v;}
	GK_INLINE void            setMax(float v)                {m_max = v;}
	GK_INLINE void            setConstant(float v)           {m_constant = v;}
	GK_INLINE void            setMean(float v)               {m_mean = v;}
	GK_INLINE void            setDeviation(float v)          {m_deviation = v;}
	GK_INLINE void            setHalfLife(float v)           {m_halflife = v;}

	GK_INLINE int             getSeed(void)                  const {return m_seed;}
	GK_INLINE int             getDistribution(void)          const {return m_distribution;}
	GK_INLINE const gkString& getProperty(void)              const {return m_prop;}
	GK_INLINE float           getMin(void)                   const {return m_min;}
	GK_INLINE float           getMax(void)                   const {return m_max;}
	GK_INLINE float           getConstant(void)              const {return m_constant;}
	GK_INLINE float           getMean(void)                  const {return m_mean;}
	GK_INLINE float           getDeviation(void)             const {return m_deviation;}
	GK_INLINE float           getHalfLife(void)              const {return m_halflife;}
};

#endif // GKRANDOMACTUATOR_H
