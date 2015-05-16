#include <p24F16KA101.h>
#include "var.h"
/**********************************************************************************************************/
/**********************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt (void) // 100ms
{
/*********************************************/    
    if(c_adc < cc_adc)AD1CON1bits.ASAM = 1;
/*********************************************/
    if((++time_200ms)>=2)
    {
        time_200ms = 0;
        if(++time_1000ms > 4)
        {
            time_1000ms = 0;
            if(tSir < Const_tSir)tSir++;
            for(tms = 0; tms < 4; tms++)
            {
                if(tIn[tms] < Const_tIn[tms])tIn[tms]++;
                if(tOut[tms] < Const_tOut[tms])tOut[tms]++;
            }
        }
/*********************************************/
        if(m_hl)
        {
            m_hl = 0;
            if(st_hl2 == M_GRE)HL2_OFF();
            if(st_hl1 == M_GRE)HL1_OFF();
            
            if(statOhr.stAlarm)OUT_LED = 0;
        }
        else
        {
            m_hl++;
            if(st_hl2 == M_GRE)HL2_ON();
            if(st_hl1 == M_GRE)HL1_ON();
            
            if(statOhr.stAlarm)OUT_LED = 1;
        }
/*********************************************/
    }
    if(time_ok_set < Ctime_ok_set)time_ok_set++;

IFS0bits.T2IF = 0;
}
/**********************************************************************************************************/
/**********************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt (void) // 1ms
{
    if(time_end_rx1<const_time_end_rx1)time_end_rx1++;
    if(time_end_rx2<const_time_end_rx2)time_end_rx2++;

    if(time_end_tx1<const_time_end_tx1)time_end_tx1++;
    if(time_end_tx2<const_time_end_tx2)time_end_tx2++;

    tmp_for_delay++;

    tKeyb++;

IFS0bits.T1IF = 0;
}
/**********************************************************************************************************/
/**********************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
//    st_hl2 = GRE;
//    HL2_ON();
   if(U2STAbits.URXDA)
    {
        if(cB2_rx >= MAX_BUFF2_LEN)cB2_rx = 0;
        bu2_rx.BUFF[cB2_rx] = U2RXREG;
        cB2_rx++;
        time_end_rx2 = 0;
    }
    if(U2STAbits.OERR)while(U2STAbits.OERR)U2STAbits.OERR=0;
    IFS1bits.U2RXIF = 0;
}
/**********************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void)
{
//    st_hl2 = GRE;
//    HL2_ON();
    
    if(cB2_tx < (MAX_BUFF2_LEN-1))
    {
        cB2_tx++;
        U2TXREG = pCB2->BUFF[cB2_tx];
    }
    else time_end_tx2 = 0;

    if(U2STAbits.OERR)while(U2STAbits.OERR)U2STAbits.OERR=0;
    IFS1bits.U2TXIF = 0;
}
/**********************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    st_hl2 = GRE;
    HL2_ON();
    
    if(U1STAbits.URXDA)
    {
        if(cB1_rx >= MAX_BUFF1_LEN)cB1_rx = 0;
        bu1_rx.BUFF[cB1_rx] = U1RXREG;
        cB1_rx++;
        time_end_rx1 = 0;
    }
    if(U1STAbits.OERR)while(U1STAbits.OERR)U1STAbits.OERR=0;
    IFS0bits.U1RXIF = 0;
}
/**********************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    if(cB1_tx < (CurLen - 1))
    {
        cB1_tx++;
        U1TXREG = pCB1->BUFF[cB1_tx];
    }
    else time_end_tx1 = 0;

    if(U1STAbits.OERR)while(U1STAbits.OERR)U1STAbits.OERR=0;
    IFS0bits.U1TXIF = 0;
}
/**********************************************************************************************************/
void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{
    AD1CON1bits.ASAM = 0;
    c_adc++;

    for(tmp = 4; tmp < 9; tmp++) z_adc[tmp-4] += (*(pBuff_adc+tmp));

    IFS0bits.AD1IF = 0;
}
/**********************************************************************************************************/

