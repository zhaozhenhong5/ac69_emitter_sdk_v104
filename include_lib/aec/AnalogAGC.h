#ifndef _ANALOGAGC_H_
#define _ANALOGAGC_H_

typedef struct
{
	int sup_cnt;    //dec
	int lenv;
	int lgain;      //Q10
	int Target;     //Q10
	int Const_sup;  //Q16
	int Const_pro;  //Q16
	int FIX_dec_cnt;
	int MAX_GainLevel;
	int MIN_GainLevel;
}Analog_AGC;


int MicLevel_2_Parm(char gain);
char GainLeverl_2_Gain(int Gain);
void Analog_AGC_Set_Hold_Time(Analog_AGC *Aagc,int tms);
unsigned char Analog_AGC_Process(Analog_AGC *Aagc,short *in,int npoint);
void Analog_AGC_Init(Analog_AGC *Aagc,int Target,int TimeConstant,int MAX_GainLevel,int MIN_GainLevel);
#endif
