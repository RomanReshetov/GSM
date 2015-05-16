
#ifndef VAR_H
#define	VAR_H

#include "GenericTypeDefs.h"

#define ei() {INTCON1bits.NSTDIS = 0;}
#define di() {INTCON1bits.NSTDIS = 1;}
#define reset() {__asm__ volatile ("reset");}
//#define Nop() {__asm__ volatile ("NOP");}

#define UBRAT
#define MyCode 0x0A
#define CConst_tSir 10
#define MAX_USER 10

#define len_passw 10
#define eeAdr (12 + len_passw*MAX_USER)

#define eeTSir (eeAdr + 2)
#define eeTIn  (eeTSir + 2)
#define eeTOut (eeTIn + 8)

#define eeAdrData (eeTOut + 8)
#define eeAdrData1 (eeAdrData + 2)
#define eeAdrData2 (eeAdrData1 + 2)
#define eeAdrData3 (eeAdrData2 + 2)
/*********************************************************************/
#define cKey_st     1
#define cWrOk       4
#define cErr        5
    
#define kOk     0b10000000
#define kEsc    0b01000000
#define StTamp  0b00100000
#define StRel   0b00000100
#define StBlock 0b00000010

#define NoKey   0b00100000

#define pressK  0b00000001

#define master_psw 2
#define master_set 3

#define const_time_enter_psw 100
/******************************************************************/
/***********************BELL COMMANDS******************************/
/******************************************************************/
#define codeBell			0x01
#define cBell_answ_ok       0xE5

#define cPPK_init    		0xF0
#define cPPK_state          0xE0
#define cPPK_setting		0xCB

#define cPPK_outs		   	0x31

#define cPPK_secur_on       0x11
#define cPPK_secur_half 	0x21
#define cPPK_secur_off   	0x01

#define cPPK_ds_list		0x41
#define cPPK_ds_lock		0x42
#define cPPK_ds_reg         0x43
#define cPPK_ds_unlock		0x44

#define cPPK_buzzer_on		0x51

#define cPPK_sirena_on		0x61
#define cPPK_sirena_off     0x62
#define cPPK_sirena_unlock	0x63
#define cPPK_sirena_lock	0x64
#define cPPK_buzzer_unlock	0x65
#define cPPK_buzzer_lock	0x66

#define cPPK_zone_unlock	0x71
#define cPPK_zone_lock		0x72

#define cPPK_pause_in		0x73
#define cPPK_pause_out		0x74

#define cPPK_time_sirena_on	0x75
/*********************************************************************/
#define Z_NORM      0

#define Z_ALARM   	0b00000001
//#define CFire   	0b00000010
#define Z_KZ   	    0b00001000
#define Z_TAMPER   	0b00000100
/*********************************************************************/
#define StOhr       0b10000000
#define StHalfOhr   0b01000000
#define StTamp      0b00100000
#define StTampKey   0b00010000
#define StPwrAB     0b00001000
/*********************************************************************/
#define ON  1
//#define OFF 0
/*********************************************************************/
#define AZ1 4
#define AZ2 5
#define AZ3 8//12
#define AZ4 7//11

#define AIN 6//10
/*********************************************************************/
#define OUT_REL   PORTAbits.RA6
#define OUT_SIR   PORTAbits.RA0
#define OUT_LED   PORTAbits.RA1
#define OUT_UPR   PORTBbits.RB15

#define OUT_RXTX1 PORTBbits.RB4
#define OUT_RXTX2 PORTAbits.RA4
/*********************************************************************/
#define IN_TAMP   PORTAbits.RA5
/*********************************************************************/
#define HL1_ON()  {PORTBbits.RB8 = ON;}
#define HL1_OFF() {PORTBbits.RB8 = OFF;}
#define HL2_ON()  {PORTBbits.RB9 = ON;}
#define HL2_OFF() {PORTBbits.RB9 = OFF;}
/*********************************************************************/
#define T_mig 2
/*********************************************************************/
extern char EE_PASSW_0[]; //5
extern char EE_PASSW_1[]; //5
extern char EE_PASSW_2[]; //5
extern char EE_PASSW_3[]; //5
extern char EE_PASSW_4[]; //5
extern char EE_PASSW_5[]; //5
extern char EE_PASSW_6[]; //5
extern char EE_PASSW_7[]; //5
extern char EE_PASSW_8[]; //5

extern char EE_FW[];//9
extern char EE_ST[];

extern unsigned char EE_ADR;

#define EE_PASSW_L 6
#define EE_FW_L 9
#define EE_ST_L 2

enum
{
    OFF = 0,
    GRE,
    RED,
    YEL,
    M_RED,
    M_GRE,
    M_YEL,
    M_GRE_RED
};
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
#define CLOCK_FREQ  8000000
#define BAUD_RATE2  115200//38400//
#define BAUD_RATE1  9600
#define SPBRG_VAL1   ( ((CLOCK_FREQ/BAUD_RATE1)/4) - 1)

#if SPBRG_VAL1 > 255
    #error "Calculated SPBRG value is out of range for currnet CLOCK_FREQ."
#endif

#define SPBRG_VAL2   ( ((CLOCK_FREQ/BAUD_RATE2)/4) - 1)

#if SPBRG_VAL2 > 255
    #error "Calculated SPBRG value is out of range for currnet CLOCK_FREQ."
#endif

#define const_time_end_rx1 (20000/BAUD_RATE1)+2
#define const_time_end_rx2 (20000/BAUD_RATE2)+2

#define const_time_end_tx1 2
#define const_time_end_tx2 2
/*********************************************************************/
#define cc_adc 4

#define MAX_BUFF1_LEN   13
#define MAX_BUFF1_LEN_m 10
#define MAX_BUFF2_LEN   14

#pragma pack(push, 1)
typedef union
{
    BYTE BUFF[MAX_BUFF2_LEN];
    struct
    {
        BYTE adr;
        BYTE com;
        BYTE b1;
        BYTE b2;
        BYTE b3;
        BYTE b4;
        BYTE b5;
        BYTE b6;
        BYTE b7;
        BYTE b8;
        BYTE stat;
        BYTE t0;
        BYTE t1;
        BYTE crc;
    };
}t_bu_tx;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union
{
    BYTE BUFF[MAX_BUFF1_LEN];
    struct
    {
        BYTE code;
        BYTE adr;
        BYTE stat_com;
        BYTE Z21;
        BYTE Z43;
        BYTE Z65;
        BYTE Z87;
        BYTE numUser;
        BYTE alarm;
        BYTE outs;
        BYTE t0;
        BYTE t1;
        BYTE crc;
    };
}t_bu_rx_tx_1;
#pragma pack(pop)

extern t_bu_rx_tx_1 bu1_rx, bu1_tx, *pCB1;
extern t_bu_tx bu2_rx, bu2_tx, *pCB2;

extern BYTE c_adc;
extern BYTE time_end_rx1, time_end_rx2, time_end_tx1, time_end_tx2;
extern BYTE cB1_rx, cB2_rx, cB1_tx, cB2_tx;
extern WORD adc_power, tmp_for_delay;

extern BYTE time_200ms, time_1000ms;
extern BYTE st_hl1, st_hl2;
extern BYTE in_tmpr, tmp;
extern BYTE m_hl;

extern WORD z_adc[];
extern volatile unsigned int *pBuff_adc;

extern BYTE tKeyb;
extern BYTE wait_answ, bad_answ;
extern BYTE Z12, Z34, MyAdr, curCom;

extern WORD tSir, tIn[], tOut[];
extern WORD Const_tSir, Const_tIn[], Const_tOut[];

#pragma pack(push, 1)
typedef struct
{
    BYTE num_psw;
    BYTE zone_in_psw;
    BYTE pwr;
    BYTE stPPK;
    BYTE stAlarm;
    BYTE Set;
    BYTE SubSet;
    BYTE stRelKey;
    BYTE zone_key;
    BYTE stKey;
    BYTE t0;
    BYTE t1;
    BYTE en4z;
}t_statOhr;
#pragma pack(pop)
extern t_statOhr statOhr;

extern BYTE prev_tm_bufZ[], prev_tm_buf_K[];
extern BYTE prev_req;


extern BYTE curPsw[MAX_USER][10];
extern const BYTE CcurPsw[MAX_USER][10];
extern const BYTE CConst_tIn[];
extern const BYTE CConst_tOut[];
extern BYTE cnt_tmpr;

extern BYTE CurLen;

#define Ctime_ok_set 20
extern BYTE time_ok_set;
extern BYTE mem_Zin[];

extern BYTE ZToModem[], ZToModem2[];
extern BYTE TimeGo, tms, pwr_on;


#endif	/* VAR_H */

