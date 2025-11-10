
#ifndef KNPCDEATHCALCEXP_H
#define KNPCDEATHCALCEXP_H

#ifdef _SERVER

#define		defMAX_CALC_EXP_NUM				3
#define		defMAX_CALC_EXP_TIME			1200

typedef struct
{
	int		m_nAttackIdx;
	int		m_nTotalDamage;
	int		m_nTime;
} KCalcExpInfo;

class KNpcDeathCalcExp
{
	friend class KNpc;
private:
	int				m_nNpcIdx;
	KCalcExpInfo	m_sCalcInfo[defMAX_CALC_EXP_NUM];
public:
	void			Init(int nNpcIdx);
	void			Active();
	void			AddDamage(int nPlayerIdx, int nDamage);
	int 			CalcExp(int nAttacker);
	
	int				GetDamage() const;
	
	void			Clear();
};

#endif // _SERVER

#endif // KNPCDEATHCALCEXP_H
