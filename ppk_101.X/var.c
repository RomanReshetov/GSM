#include "var.h"

const BYTE CcurPsw[MAX_USER][10] = {
    {0x01, 0x01, 0x01, 0x01, 0, 0, 0, 0, 0, 0x0F},
    {0x02, 0x02, 0x02, 0x02, 0, 0, 0, 0, 0, 0x0F},
    {0x03, 0x03, 0x03, 0x03, 0, 0, 0, 0, 0, 0x0F},
    {0x04, 0x04, 0x04, 0x04, 0, 0, 0, 0, 0, 0x0F},
    {0x05, 0x05, 0x05, 0x05, 0, 0, 0, 0, 0, 0x0F},
    {0x06, 0x06, 0x06, 0x06, 0, 0, 0, 0, 0, 0x0F},
    {0x07, 0x07, 0x07, 0x07, 0, 0, 0, 0, 0, 0x0F},
    {0x08, 0x08, 0x08, 0x08, 0, 0, 0, 0, 0, 0x0F},
    
    {0x09, 0x09, 0x09, 0x09, 0, 0, 0, 0, 0, 0x0F},
    {0x0A, 0x0A, 0x0A, 0x0A, 0, 0, 0, 0, 0, 0x0F}
};

const BYTE CConst_tIn[4] =  {10, 0, 0, 0};
const BYTE CConst_tOut[4] = {10, 0, 0, 0};

t_bu_rx_tx_1 bu1_rx, bu1_tx, *pCB1;
t_bu_tx bu2_rx, bu2_tx, *pCB2;

BYTE c_adc;
BYTE time_end_rx1, time_end_rx2, time_end_tx1, time_end_tx2;
BYTE cB1_rx, cB2_rx, cB1_tx, cB2_tx;
WORD adc_power, tmp_for_delay;

BYTE time_200ms, time_1000ms;
BYTE st_hl1, st_hl2;
BYTE in_tmpr, tmp;
BYTE m_hl;

WORD z_adc[5];
volatile unsigned int *pBuff_adc;
BYTE *pt;

BYTE tKeyb;
BYTE wait_answ, bad_answ;

BYTE Z12, Z34, MyAdr, curCom;

WORD tSir, tIn[4], tOut[4];
WORD Const_tSir, Const_tIn[4], Const_tOut[4];

t_statOhr statOhr;

BYTE prev_tm_bufZ[4], prev_tm_buf_K[4];
BYTE prev_req;
BYTE curPsw[MAX_USER][10];
BYTE cnt_tmpr;

BYTE CurLen;
BYTE time_ok_set;
BYTE mem_Zin[4];
BYTE ZToModem[5], ZToModem2[5];
BYTE TimeGo, tms, pwr_on;







