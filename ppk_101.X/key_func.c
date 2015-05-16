#include <p24F16KA101.h>

#include "GenericTypeDefs.h"
#include "var.h"
#include "function.h"
#include "key_func.h"
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
void ReturnAll(void)
{
BYTE tm, ts;

    for(tm = 0; tm < MAX_USER; tm++)
    {
        for(ts = 0; ts < len_passw; ts++)curPsw[tm][ts] = CcurPsw[tm][ts];
    }
    wr_all_psw();
    for(tm = 0; tm < 4; tm++)
    {
        Const_tIn[tm] = CConst_tIn[tm];
        Const_tOut[tm] = CConst_tOut[tm];
    }
    Const_tSir = CConst_tSir;
    statOhr.en4z = 0b00001000;
    
    wr_Set();
    wr_timeSet();
    rd_timeSet();
}
/**********************************************************************************************************/

/**********************************************************************************************************/
void rd_all_psw(void)
{
BYTE tm;

    for(tm = 0; tm < MAX_USER; tm++)eepRd_buf((tm*len_passw), curPsw[tm], len_passw);
}
/**********************************************************************************************************/
void wr_all_psw(void)
{
BYTE tm;

    for(tm = 0; tm < MAX_USER; tm++)eepWr_buf((tm*len_passw), curPsw[tm], len_passw);
}
/**********************************************************************************************************/
void ch_key(void)
{
//    return;
    
    if(tKeyb > 99)
    {
        tKeyb = 0;

        if(wait_answ)
        {
            if(bad_answ < 3)bad_answ++;
        }
        else 
        {
            bad_answ = 0;
            statOhr.stKey &= ~NoKey;
        }
        if(bad_answ == 3)
        {
           bad_answ++;//No keyboard!!! 
           statOhr.stKey |= NoKey;
        }

        bu2_tx.adr = 16;//AdrKey;

        if(curCom)
        {
            bu2_tx.com = curCom;
            curCom = 0;
        }
        else bu2_tx.com = cKey_st;
        
        bu2_tx.b1 = TimeGo;
        //bu2_tx.b1 = tIn;
        //bu2_tx.b2 = Const_tIn;
        //bu2_tx.b3 = tOut;
        //bu2_tx.b4 = Const_tOut;
        
        bu2_tx.b5 = statOhr.stAlarm;
        bu2_tx.b6 = statOhr.stRelKey;
        bu2_tx.stat = statOhr.stPPK;
        
        if(statOhr.Set == 0)bu2_tx.t0 = statOhr.pwr;
        else
        {
            bu2_tx.t0 = statOhr.SubSet;
            bu2_tx.b8 = statOhr.num_psw;
        }
        bu2_tx.t1 = statOhr.Set;
        
        
        
        pCB2 = &bu2_tx;        
        send_u2();
    }
}
/**********************************************************************************************************/
void form_ohr_func(BYTE nz)
{
BYTE tm;
    if(en_ohr(curPsw[nz][9]) == 0)
    {
        if((statOhr.stPPK & curPsw[nz][9]) == 0)return; //hoteli postavit (mogno tolko snyat)
    }
    statOhr.stAlarm = 0;
    time_InOut_off(); // otkl. - time in/out
    
    if(statOhr.stPPK & curPsw[nz][9])
    {
        statOhr.stPPK &= (~(curPsw[nz][9])); // Снять с охраны
        statOhr.stPPK &= (~StOhr);
    }
    else 
    {
        if(tSir <= Const_tSir)
        {
            tSir = Const_tSir + 1;
            return;
        }
        statOhr.stPPK |= curPsw[nz][9];
        statOhr.stPPK |= StOhr;
    }
/////////////////////////////////////////////////////////////////////////////////////////
    tSir = Const_tSir + 1;

    statOhr.zone_in_psw = curPsw[nz][9];
    statOhr.num_psw = nz + 1;

//    if(statOhr.stPPK & 0x0F)statOhr.stPPK |= StOhr;
//    else statOhr.stPPK &= (~StOhr);

    wr_stPPK();

    for(tm = 0; tm < 4; tm++)
    {
        if(statOhr.stPPK & (1 << tm) ) //time for out
        {
            tOut[tm] = 0;
            st_hl1 = M_GRE;
        }
        mem_Zin[tm] = 0;
    }
}
/**********************************************************************************************************/
void time_InOut_off(void)
{
BYTE tm;
    for(tm = 0; tm < 4; tm++)
    {
        tIn[tm] = Const_tIn[tm] + 1;
        tOut[tm] = Const_tOut[tm] + 1;
        if(prev_tm_bufZ[tm] == Z_ALARM)ZToModem[tm] = Z_NORM;
        mem_Zin[tm] = 0;
//        if(ZToModem2[tm] == Z_ALARM)prev_tm_bufZ[tm] = ZToModem2[tm] = ZToModem[tm] = Z_NORM;
    }
}
/**********************************************************************************************************/
void chek_input_key(void)
{
BYTE tm, ts, ta;

    curCom = cErr;
    if(statOhr.Set == 0)
    {
        if(bu2_rx.stat&kOk)
        {
            for(tm = 0; tm < (MAX_USER-2); tm++)
            {
                ta = 0;
                for(ts = 0; ts < 8; ts++)
                {
                    if(curPsw[tm][ts] == bu2_rx.BUFF[2 + ts])ta++;
                }
                if(ta == 8)
                {
                    curCom = cWrOk;
//                    ts = (1 << tm);
                    form_ohr_func(tm);
                    return;
                }
            }
        }
    }
    else
    {
        if(bu2_rx.stat&kEsc)
        {
            if(statOhr.SubSet == 3 && statOhr.Set == master_psw)statOhr.SubSet = 1;
            else if(statOhr.SubSet)statOhr.SubSet--;
            else
            {
                statOhr.Set = 1;
            }
            curCom = cWrOk;
            return;
        }
        else if(bu2_rx.stat&kOk)
        {
            
            if(statOhr.Set == 1)
            {
                for(tm = (MAX_USER-2); tm < MAX_USER; tm++)
                {
                    ta = 0;
                    for(ts = 0; ts < 8; ts++)
                    {
                        if(curPsw[tm][ts] == bu2_rx.BUFF[2 + ts])ta++;
                    }
                    if(ta == 8)
                    {
                        curCom = cWrOk;
                        statOhr.SubSet = 0;
                        if(tm == (MAX_USER-2))
                        {
                            statOhr.Set = master_psw;
                        }
                        else
                        {
                            statOhr.Set = master_set;
                        }
                    }
                }
            }
            else if(statOhr.Set == master_set)
            {
                if(statOhr.SubSet == 0)
                {
                    if(bu2_rx.BUFF[4])return;
                    tm = conv_2_in_1(bu2_rx.BUFF[2], bu2_rx.BUFF[3]);
                    if(tm > 10 && tm < 15)
                    {
                        statOhr.SubSet = 1; // 11, 12, 13 or 14
                        statOhr.num_psw = tm;
                        curCom = cWrOk;
                    }
                }
                else if(statOhr.SubSet == 1)
                {
                    statOhr.SubSet = 0;
                    if(bu2_rx.BUFF[4])return;
                    tm = conv_2_in_1(bu2_rx.BUFF[2], bu2_rx.BUFF[3]);
                    
                    if(tm > 99)return;

                    if(statOhr.num_psw == 11)
                    {
                        if(statOhr.num_psw == 11)Const_tSir = tm;
//                        else if(statOhr.num_psw == 12)Const_tIn = tm;
//                        else if(statOhr.num_psw == 13)Const_tOut = tm;
                        wr_timeSet();
                        //rd_timeSet();
                    }
                    else if(statOhr.num_psw == 12 || statOhr.num_psw == 13)
                    {
                        statOhr.SubSet = 0;
                        
                        tm = conv_2_in_1(bu2_rx.BUFF[2], bu2_rx.BUFF[3]);
                        if(tm && tm < 5)
                        {
                            statOhr.zone_in_psw = tm - 1;
                            //statOhr.num_psw = tm - 1;
                            statOhr.SubSet = 2;
                            curCom = cWrOk;
                            return;
                        }
                    }
                    else if(statOhr.num_psw == 14)
                    {
                        if(tm && tm <= 4)
                        {
                            tm--;
                            statOhr.en4z = (1 << tm);
                            wr_Set();
                        }
                        else return;
                    }
                   
                    statOhr.SubSet = 2;
                    curCom = cWrOk;
                    time_ok_set = 0;
                }
                else if(statOhr.SubSet == 2)
                {
                    statOhr.SubSet = 0;
                    tm = conv_2_in_1(bu2_rx.BUFF[2], bu2_rx.BUFF[3]);
                    if(tm > 99)return;
                    if(statOhr.num_psw == 12)
                    {
                        Const_tIn[statOhr.zone_in_psw] = tm;
                    }
                    else if(statOhr.num_psw == 13)
                    {
                        Const_tOut[statOhr.zone_in_psw] = tm;
                    }
                    else return;
                    wr_timeSet();
                    curCom = cWrOk;
                    time_ok_set = 0;
                    statOhr.SubSet = 3;
                }
            }
            else if(statOhr.Set == master_psw)
            {
                if(statOhr.SubSet == 0) // Num password 1 data
                {
                    if(bu2_rx.BUFF[4])return;
                    tm = conv_2_in_1(bu2_rx.BUFF[2], bu2_rx.BUFF[3]);

                    if(tm != 0 && tm <= 10)
                    {
                        statOhr.SubSet = 1; // Num password OK
                        statOhr.num_psw = tm - 1;
                        curCom = cWrOk;
                    }
                }
                else if(statOhr.SubSet == 1) // Get password
                {
                    if(bu2_rx.BUFF[5])
                    {
                        for(tm = 0; tm < 8; tm++)curPsw[statOhr.num_psw][tm] = bu2_rx.BUFF[2 + tm];
                        curCom = cWrOk;
                        statOhr.SubSet = 2;
                    }
                    
                }
                else if(statOhr.SubSet == 2) // Equal password
                {
                    ta = 0;
                    for(tm = 0; tm < 8; tm++)
                    {
                        if(curPsw[statOhr.num_psw][tm] == bu2_rx.BUFF[2 + tm])ta++;
                    }
                    if(ta == 8)
                    {
                        eepWr_buf((statOhr.num_psw*len_passw), curPsw[statOhr.num_psw], len_passw);
                        curCom = cWrOk;
                        time_ok_set = 0;
                        statOhr.SubSet = 3;
                    }
                    rd_all_psw();
                }
            }
        }
    }
}
/**********************************************************************************************************/
void ch_tamper(void)
{
    if(IN_TAMP != in_tmpr)
    {
        if(cnt_tmpr == 100)
        {
            cnt_tmpr = 0;
            in_tmpr = IN_TAMP;
            if(in_tmpr)
            {
                statOhr.stPPK |= StTamp;
                tSir = 0;
            }
            else
            {
                statOhr.stPPK &= ~StTamp;
            }
        }
        else cnt_tmpr++;
    }
}
/**********************************************************************************************************/
BYTE conv_2_in_1(BYTE data1, BYTE data2)
{
BYTE tm;

    if(data2 == 0 || data1 == 0)return 0xFF;

    if(data1 > 0x09)data1 = 0;
    if(data2 > 0x09)data2 = 0;
    tm = data1*10;
    tm += data2;
/*
    tm = (data>>4)&0x0F;
    if(tm > 0x09)tm = 0;
    tm *= 10;
    if((data&0x0F) > 0x09) data &= 0xF0;
    tm += (data&0x0F);
    
    */
    return tm;
}
/**********************************************************************************************************/

