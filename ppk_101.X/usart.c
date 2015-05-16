#include <p24F16KA101.h>

#include "var.h"
#include "function.h"

/**********************************************************************************************************/
/**********************************************************************************************************/
void init_hw(void)
{
    AD1PCFG = 0b1110001111001111;

    TRISA = 0b10101100;
    TRISB = 0b0111110011101111;

    AD1CON1=0x00E0;
    AD1CON2=0x043C;//0x0004;
    AD1CON3=0x0F00;
//    AD1CHS=(ADC_CH);
    AD1CSSL=0xFFFF;//0;
    IFS0bits.AD1IF=0;
    IPC3bits.AD1IP=6;
    IEC0bits.AD1IE=1;
    
    IPC0bits.T1IP=5;
    PR1=8000;//15000;//15625;//15600;
    T1CON=0x8000;//0x8020;//0x8060;//
    IEC0bits.T1IE=1;

    IPC1bits.T2IP=7;
    PR2=12500;
    T2CON=(0x8000 | 0b00100000);//0x8020;//0x8060;//
    IEC0bits.T2IE=1;

    AD1CON1bits.ADON = 1;
    AD1CON1bits.ASAM = 1;
}
/********************************************************************/
void init_var(void)
{
    statOhr.Set = 0;
    
    time_end_tx1 = const_time_end_tx1+1;
    time_end_tx2 = const_time_end_tx2+1;
    time_end_rx1 = const_time_end_rx1+1;
    time_end_rx2 = const_time_end_rx2+1;

    st_hl2 = GRE;
    st_hl1 = GRE;

    pBuff_adc = &ADC1BUF0;
    
    cB1_tx = MAX_BUFF1_LEN;
    cB2_tx = MAX_BUFF2_LEN;
    cB1_rx = 0;
    cB2_rx = 0;
    
    bad_answ = 0;
    
    curCom = 0;
    cnt_tmpr = 0;
    
    time_ok_set = Ctime_ok_set + 1;
}
/********************************************************************/
void delay_ms(WORD tms)
{
    tmp_for_delay = 0;
    while(tmp_for_delay < tms);
}
/**********************************************************************************************************/
//const unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
/**********************************************************************************************************/
void U2Init(void)
{
    TRISBbits.TRISB0 = 0;
    Nop();
    TRISBbits.TRISB1 = 1;

    U2MODE=0;U2STA=0;
    U2MODEbits.BRGH=1;

    U2BRG = SPBRG_VAL2;

    IPC7bits.U2RXIP=1;
    IPC7bits.U2TXIP=2;

    U2STAbits.URXISEL=0;

    IEC1bits.U2RXIE=1;
    IEC1bits.U2TXIE=1;

    U2STAbits.UTXISEL0=0;
    U2STAbits.UTXISEL1=0;//1;

    U2MODEbits.UARTEN=1;

    U2STAbits.UTXEN=1;

    Nop();Nop();Nop();Nop();Nop();

    IFS1bits.U2TXIF = 0;
}
/********************************************************************/
void U1Init(void)
{
    TRISBbits.TRISB7 = 0;
    Nop();
    TRISBbits.TRISB2 = 1;

    U1MODE=0;U1STA=0;
    U1MODEbits.BRGH=1;

    U1BRG = SPBRG_VAL1;

    IPC2bits.U1RXIP=4;
    IPC3bits.U1TXIP=3;

    U1STAbits.URXISEL=0;

    IEC0bits.U1RXIE=1;

    IEC0bits.U1TXIE=1;

    U1STAbits.UTXISEL0=0;
    U1STAbits.UTXISEL1=0;//1;

    U1MODEbits.UARTEN=1;

    U1STAbits.UTXEN=1;

    Nop();Nop();Nop();Nop();Nop();

    IFS0bits.U1TXIF = 0;
}
//******************************************************************
/*
void Send1_str(BYTE *Com)
{
BYTE n,j,*cs;

   for(cs=Com;*cs!='\0';cs++);
   n=cs-Com;
   for(j=0;j<n;j++)U1Put(Com[j]);
}
*/
//******************************************************************
/*
void Send2_str(BYTE *Com)
{
BYTE n,j,*cs;

   for(cs=Com;*cs!='\0';cs++);
   n=cs-Com;
   for(j=0;j<n;j++)U2Put(Com[j]);
}
*/
/********************************************************************/
/*
void U2Put(BYTE c)
{
    while(!U2STAbits.TRMT);
    U2TXREG = c;
}
*/
/********************************************************************/
/*
void U1Put(BYTE c)
{
    while(!U1STAbits.TRMT);
    U1TXREG = c;
}
*/
/********************************************************************/
/*
void Print2Char(BYTE toPrint)
{
    BYTE PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint>>4)&0x0F;
    U2Put(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    U2Put(CharacterArray[toPrint]);
    return;
}
*/
/********************************************************************/
/*
void Print2Dec(BYTE toPrint)
{
    if( toPrint >= 100 )
        U2Put(CharacterArray[toPrint/100]);
    if( toPrint >= 10 )
        U2Put(CharacterArray[(toPrint%100)/10]);
    U2Put(CharacterArray[toPrint%10]);
}
*/
/******************************************************************/
BYTE CallCRCDallas(BYTE *SourceAdr, BYTE Len)
{
   BYTE i,k,In,CRC=0;

   for(i=0;i<Len;i++)
   {
      In=*SourceAdr;
      for(k=0;k<8;k++)
      {
         if((In^CRC)&1)CRC=((CRC^0x18)>>1)|0x80;
         else CRC=CRC>>1;
         In>>=1;
      }
      SourceAdr++;
   }
   return CRC;
}
/**********************************************************************************************************/
void send_u1(void)
{
    pCB1->crc = CallCRCDallas(pCB1->BUFF, (CurLen-1));//(MAX_BUFF1_LEN-1));
    OUT_RXTX1 = 1;
    cB1_tx = 0;
    U1TXREG = pCB1->BUFF[cB1_tx];
}
/**********************************************************************************************************/
void send_u2(void)
{
    pCB2->crc = CallCRCDallas(pCB2->BUFF, (MAX_BUFF2_LEN-1));
    OUT_RXTX2 = 1;
    cB2_tx = 0;
    U2TXREG = pCB2->BUFF[cB2_tx];
}
/**********************************************************************************************************/
BYTE check_z(WORD zpt/*, BYTE NZone*/)
{
    if(zpt<0xDD)return Z_KZ;//return 'K';
    if(zpt<0x12B)return Z_NORM;//return 'N';
    if(zpt<0x190)return Z_ALARM;//return 'A';
    return Z_TAMPER;//return 'T';
}
/**********************************************************************************************************/
BYTE en_ohr(BYTE enZ)
{
BYTE tm;

    for(tm = 0; tm < 4; tm++)
    {
        if(enZ&(1<<tm))
        {
            if(ZToModem[tm] == Z_TAMPER || ZToModem[tm] == Z_KZ)return 0;
            else if(ZToModem[tm] == Z_ALARM && Const_tOut[tm] == 0)return 0; //Check time out
        }
    }
    if((enZ&0x0F) == 0)return 0;
    return 1;
}
/**********************************************************************************************************/
void ch_time_go(BYTE cnt)
{
    if(tOut[cnt] < Const_tOut[cnt] || tIn[cnt] < Const_tIn[cnt])TimeGo |= (1 << cnt);
}
/**********************************************************************************************************/
void check_alarm(void)
{
BYTE tm, ts;

    TimeGo = 0;
//    statOhr.stAlarm = 0;
    
    for(tm = 0; tm < 4; tm++)
    {
        if(pwr_on == 0)
        {
            ch_time_go(tm);
            if(prev_tm_bufZ[tm] != ZToModem2[tm]) // izmenenie v zonah
            {
                ts = (1 << tm);
                if(curPsw[tm][9] & ts)//zona obslugivaetsa
                {
                    if(!(statOhr.en4z & ts))// NE "postanovka knopkoj"
                    {
                        if(curPsw[tm][8]&ts)//zona bez rezistora
                        {
                            if(ZToModem2[tm] == Z_KZ)
                            {
                                if(tSir > Const_tSir)ZToModem[tm] = Z_NORM;
                            }
                            else if(ZToModem2[tm] == Z_TAMPER)
                            {
                                ZToModem[tm] = Z_NORM;
                                if((TimeGo & ts) == 0)
                                {
                                    if(mem_Zin[tm] == 0 && (statOhr.stPPK & ts))
                                    {
                                        mem_Zin[tm] = 1;
                                        tIn[tm] = 0;
                                    }
                                    else if(mem_Zin[tm] == 2)
                                    {
                                        ZToModem[tm] = Z_ALARM; // net zadergki

                                        statOhr.stAlarm |= ts;
                                        tSir = 0;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(ZToModem2[tm] == Z_ALARM)
                            {
                                ZToModem[tm] = Z_NORM;
                                if((TimeGo & ts) == 0)
                                {
                                    if(mem_Zin[tm] == 0 && (statOhr.stPPK & ts))
                                    {
                                        mem_Zin[tm] = 1;
                                        tIn[tm] = 0;
                                    }
                                    else if(mem_Zin[tm] == 2)
                                    {
                                        ZToModem[tm] = Z_ALARM; // net zadergki

                                        statOhr.stAlarm |= ts;
                                        tSir = 0;
                                    }
                                }
                            }
                            else
                            {
                                if(ZToModem2[tm] == Z_NORM)
                                {
                                    if(tSir > Const_tSir)ZToModem[tm] = ZToModem2[tm];
                                }
                                else 
                                {
                                    ZToModem[tm] = ZToModem2[tm];
                                    statOhr.stAlarm |= ts;
                                    tSir = 0;
                                }
                            }
                        }
                    }
                    else// "postanovka knopkoj"
                    {
                        time_InOut_off(); // otkl. - time in/out
                        if(ZToModem2[tm] == Z_KZ && !(statOhr.stPPK & 0x0F))//Knopka zamknuta
                        {
                            ZToModem[tm] = Z_NORM;
                            form_stOhr_on();
                            //postavit pod ohranu
                        }
                        else if(ZToModem2[tm] == Z_TAMPER && (statOhr.stPPK & 0x0F))//knopka razomknuta
                        {
                            ZToModem[tm] = Z_NORM;
                            form_stOhr_off();
                            //snyat s ohrany
                        }
                        statOhr.stAlarm = 0;

                        tSir = Const_tSir + 1;
                        statOhr.zone_in_psw = 0x0F;
                        statOhr.num_psw = 0xE0;
                        wr_stPPK();
                    }
                }
                else ZToModem[tm] = Z_NORM;
            }
        }
        prev_tm_bufZ[tm] = ZToModem2[tm];
    }
}
/**********************************************************************************************************/
void form_stOhr_off(void)
{
    statOhr.stPPK &= 0xF0;
    statOhr.stPPK &= (~StOhr);
}
/**********************************************************************************************************/
void form_stOhr_on(void)
{
BYTE tm;

    statOhr.stPPK |= 0x0F;
    for(tm = 0; tm < 4; tm++)
    {
        tOut[tm] = 0;
        mem_Zin[tm] = 0;
    }
    statOhr.stPPK |= StOhr;
}
/**********************************************************************************************************/
void ch_zone(void)
{
BYTE tm;

    if(c_adc == cc_adc)
    {
//////////////////////////////////////////////////////////////////////////
        z_adc[2] >>= 2;
        if(adc_power > z_adc[2])
        {
            if((adc_power - z_adc[2]) > 2)
            {
                statOhr.pwr = StPwrAB;
            }
        }
        else
        {
            if((z_adc[2] - adc_power) > 2)
            {
                statOhr.pwr = 0;
            }
        }
        adc_power = z_adc[2];
//////////////////////////////////////////////////////////////////////////  
        ZToModem2[0] = check_z(z_adc[0]>>2);
        ZToModem2[1] = check_z(z_adc[1]>>2);
        ZToModem2[2] = check_z(z_adc[3]>>2);
        ZToModem2[3] = check_z(z_adc[4]>>2);
        
        for(tm = 0; tm<5; tm++)z_adc[tm] = 0;
        c_adc = 0;

        check_alarm();
    }
}
/**********************************************************************************************************/
