#include <p24F16KA101.h>
#include <xc.h>
#include "conf_bit.h"

#include "GenericTypeDefs.h"
#include "var.h"
#include "function.h"
#include "key_func.h"

/************************************************************************/
/************************************************************************/
/************************************************************************/
int main(void)
{
BYTE tm, ts;

init_hw();
delay_ms(200);
U1Init();
U2Init();
init_var();
/************************************************************************/
delay_ms(1000);

in_tmpr = IN_TAMP;
/************************WORK OR SET************************************/
tm = 0;
ts = 0;
while(tm < 100 && ts < 100)
{
    in_tmpr = IN_TAMP;
    if(in_tmpr){tm++; ts = 0;}
    else {tm = 0; ts++;}
}
if(ts == 100)statOhr.Set = 1;
/************************************************************************/
if(statOhr.Set == 0)
{
    pwr_on = 1;
    for(tm = 0; tm < 12; tm++) // 3s
    {
        delay_ms(250);
        ch_zone();
    }
    pwr_on = 0;
}
//kjadfhslugashdlg
/************************************************************************/
di();
for(tm = 0; tm < 5; tm++)z_adc[tm] = 0;
c_adc = 0;
ei();
    
rd_adr();
rd_stPPK();
rd_timeSet();
rd_all_psw();
rd_Set();
/************************************************************************/
/************************************************************************/
/************************************************************************/
/************************************************************************/
do
{
////////////////////Opros klaviatury/////////////////////////////////////
    ch_key();
/////////////////////////////////////////////////////////////////////////
///////////////////////FORM STATE ZONE///////////////////////////////////
    if(statOhr.Set == 0)
    {
        ch_zone();
/////////////////////////////////////////////////////////////////////////
////////////////////////CHECK TAMPER/////////////////////////////////////
        ch_tamper();
    }
/////////////////////////////////////////////////////////////////////////
////////////////////////SEND USART1//////////////////////////////////////
    if(time_end_rx1 == const_time_end_rx1) //rcv data from usart 1
    {
        if(bu1_rx.BUFF[cB1_rx-1] == CallCRCDallas(bu1_rx.BUFF, (cB1_rx-1)) && bu1_rx.adr == MyAdr)
        {
            if(check_modem(bu1_rx.stat_com) == 0)
            {
                pCB1 = &bu1_tx;
                send_u1();
            }
        }
        cB1_rx = 0;
        time_end_rx1++;
    }
/////////////////////////////////////////////////////////////////////////
////////////////////////END SEND USART1//////////////////////////////////
    if(time_end_tx1 == const_time_end_tx1)
    {
        time_end_tx1++;
        OUT_RXTX1 = 0;
    }
/////////////////////////////////////////////////////////////////////////
////////////////////////SEND USART2//////////////////////////////////////
    if(time_end_rx2 == const_time_end_rx2) //rcv data from usart 2
    {
        time_end_rx2++;
        wait_answ = 0;
        cB2_rx = 0;

        if(bu2_rx.crc == CallCRCDallas(bu2_rx.BUFF, (MAX_BUFF2_LEN-1)) && bu2_rx.adr == 16)
        {
            if(!(bu2_rx.stat & pressK) && prev_req != bu2_rx.stat)
            {
                if((prev_req&0b11000000) != (bu2_rx.stat&0b11000000) && (bu2_rx.stat&0b11000000))
                    chek_input_key();
                
                prev_req = bu2_rx.stat;
            }
            
            statOhr.zone_key = ((bu2_rx.stat&0b00011000) >> 3);
            if(statOhr.zone_key == 2)statOhr.zone_key = Z_KZ;
            else if(statOhr.zone_key == 3)statOhr.zone_key = Z_TAMPER;

            statOhr.t0 = bu2_rx.t0;
            statOhr.t1 = bu2_rx.t1;

            if(bu2_rx.stat&StTamp)statOhr.stKey |= (StTamp >> 3);
            else statOhr.stKey &= ~(StTamp >> 3);
            /*
            bu1_tx.Z21 = bu2_rx.b1;//Z12;
            bu1_tx.Z43 = bu2_rx.b2;//Z34;
            bu1_tx.Z65 = bu2_rx.b3;
            bu1_tx.Z87 = bu2_rx.b4;
            
            bu1_tx.numUser = statOhr.Set;
            bu1_tx.outs = statOhr.SubSet;
            bu1_tx.alarm = bu2_rx.stat;
            
            bu1_tx.stat_com = statOhr.stPPK;
            bu1_tx.t0 = bu2_rx.t0;
            bu1_tx.t1 = bu2_rx.t1;
            
            //bu1_tx.t0 = (adc_power>>8);
            //bu1_tx.t1 = adc_power;
            
            CurLen = MAX_BUFF1_LEN;
            pCB1 = &bu1_tx;
            send_u1();
            */
        }
    }
/////////////////////////////////////////////////////////////////////////
////////////////////////END SEND USART2//////////////////////////////////
    if(time_end_tx2 == const_time_end_tx2)
    {
        time_end_tx2++;
        OUT_RXTX2 = 0;
        wait_answ = 1;
    }
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
    if(time_ok_set == Ctime_ok_set && statOhr.SubSet)
    {
        statOhr.SubSet = 0;
        curCom = cWrOk;
        time_ok_set++;
    }
/////////////////////////////////////////////////////////////////////////
    if(statOhr.Set == 0)
    {
        if(tSir < Const_tSir)OUT_SIR = 1;
        else if(tSir == Const_tSir)
        {
            tSir++;
            for(ts = 0; ts < 4; ts++)
                if(prev_tm_bufZ[ts] == Z_NORM)
                    prev_tm_bufZ[ts] = Z_ALARM;
        }
        else OUT_SIR = 0;
/////////////////////////////////////////////////////////////////////////
        if(statOhr.stPPK & (StHalfOhr | StOhr))
        {
            if(statOhr.stAlarm == 0)
            {
                OUT_LED = 1;
            }
        }
        else if(statOhr.stAlarm == 0)
        {
            OUT_LED = 0;
            st_hl1 = 0;
        }
///////////////////////////////////////////////////////////////////////// 
        for(ts = 0; ts < 4; ts++)
        {
            if(tOut[ts] == Const_tOut[ts])
            {
                tOut[ts]++;
                if((statOhr.en4z & (1 << ts)) == 0)prev_tm_bufZ[ts] = Z_NORM;
                mem_Zin[ts] = 0;
                st_hl1 = GRE;
            }
            if(tIn[ts] == Const_tIn[ts])
            {
                tIn[ts]++;
                st_hl1 = M_GRE;

                if(mem_Zin[ts] == 1)
                {
                    tSir = 0;
                    mem_Zin[ts] = 2;
                    statOhr.stAlarm |= (1 << ts);
                    ZToModem[ts] = Z_ALARM;
                    if(prev_tm_bufZ[ts] == Z_ALARM) prev_tm_bufZ[ts] = Z_NORM;
                    else prev_tm_bufZ[ts] = Z_ALARM;
                }
            }
        }
    }
/////////////////////////////////////////////////////////////////////////
    if(st_hl1 == GRE)
    {
        HL1_ON();
    }
    else if(st_hl1 != M_GRE)HL1_OFF();
    
    if(st_hl2 == GRE)
    {
        HL2_ON();
    }
    else if(st_hl2 != M_GRE)HL2_OFF();
/////////////////////////////////////////////////////////////////////////
}while(1);
return 0;
}

