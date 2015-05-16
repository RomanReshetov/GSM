#include <p24F16KA101.h>
#include <xc.h>

#include "var.h"
#include "function.h"

char EE_PASSW_0[] __attribute__ ((space(eedata), address(0x00 + 0x7FFE00)))  = {0x01, 0x01, 0x01, 0x01, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_1[] __attribute__ ((space(eedata), address(0x00 + len_passw + 0x7FFE00)))  = {0x02, 0x02, 0x02, 0x02, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_2[] __attribute__ ((space(eedata), address(0x00 + len_passw*2 + 0x7FFE00)))  = {0x03, 0x03, 0x03, 0x03, 0x05, 0x06, 0x07, 0x08, 0, 0b00001111}; //5
char EE_PASSW_3[] __attribute__ ((space(eedata), address(0x00 + len_passw*3 + 0x7FFE00)))  = {0x04, 0x04, 0x04, 0x04, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_4[] __attribute__ ((space(eedata), address(0x00 + len_passw*4 + 0x7FFE00)))  = {0x05, 0x05, 0x05, 0x05, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_5[] __attribute__ ((space(eedata), address(0x00 + len_passw*5 + 0x7FFE00)))  = {0x06, 0x06, 0x06, 0x06, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_6[] __attribute__ ((space(eedata), address(0x00 + len_passw*6 + 0x7FFE00)))  = {0x07, 0x07, 0x07, 0x07, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_7[] __attribute__ ((space(eedata), address(0x00 + len_passw*7 + 0x7FFE00)))  = {0x08, 0x08, 0x08, 0x08, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_8[] __attribute__ ((space(eedata), address(0x00 + len_passw*8 + 0x7FFE00)))  = {0x09, 0x09, 0x09, 0x09, 0, 0, 0, 0, 0, 0b00001111}; //5
char EE_PASSW_9[] __attribute__ ((space(eedata), address(0x00 + len_passw*9 + 0x7FFE00)))  = {0x0A, 0x0A, 0x0A, 0x0A, 0, 0, 0, 0, 0, 0b00001111}; //5

char EE_FW[] __attribute__ ((space(eedata), address((len_passw*MAX_USER) + 0x7FFE00)))  = {"pv1.04.15"};//9
char EE_ST[] __attribute__ ((space(eedata), address((10 + len_passw*MAX_USER) + 0x7FFE00)))  = {0xDD, 0xAA};//2

unsigned char EE_ADR __attribute__ ((space(eedata), address(eeAdr + 0x7FFE00)))  = {16};

unsigned char EE_T_SIR __attribute__ ((space(eedata), address(eeTSir + 0x7FFE00)))  = {10};
unsigned char EE_T_IN[] __attribute__ ((space(eedata), address(eeTIn + 0x7FFE00)))  = {5, 0, 0, 0, 0, 0, 0, 0};
unsigned char EE_T_OUT[] __attribute__ ((space(eedata), address(eeTOut + 0x7FFE00)))  = {5, 0, 0, 0, 0, 0, 0, 0};
unsigned char EE_ST_PPK __attribute__ ((space(eedata), address(eeAdrData + 0x7FFE00)))  = {0};

unsigned char EE_ST_NumPPK __attribute__ ((space(eedata), address(eeAdrData1 + 0x7FFE00)))  = {1};
unsigned char EE_ST_ZonePPK __attribute__ ((space(eedata), address(eeAdrData2 + 0x7FFE00)))  = {0};

unsigned char EE_ST_4hZPPK __attribute__ ((space(eedata), address(eeAdrData3 + 0x7FFE00)))  = {0b00001000};
/**********************************************************************************************************/
/**********************************************************************************************************/
void wr_timeSet(void)
{
BYTE tm;
    eepWr(eeTSir, Const_tSir);
    for(tm = 0; tm < 4; tm++)
    {
        eepWr(eeTIn + (tm<<1), Const_tIn[tm]);
        eepWr(eeTOut + (tm<<1), Const_tOut[tm]);
    }
}
/**********************************************************************************************************/
void rd_timeSet(void)
{
BYTE tm;

    Const_tSir = eepRd(eeTSir);
    tSir = Const_tSir + 1;
    
    for(tm = 0; tm < 4; tm++)
    {
        Const_tIn[tm] = eepRd(eeTIn + (tm<<1));
        Const_tOut[tm] = eepRd(eeTOut + (tm<<1));

        tIn[tm] = Const_tIn[tm] + 1;
        tOut[tm] = Const_tOut[tm] + 1;
    }
}
/**********************************************************************************************************/
void wr_Set(void)
{
    eepWr(eeAdrData3, statOhr.en4z);
}
/**********************************************************************************************************/
void rd_Set(void)
{
BYTE tm, ta = 0;

    statOhr.en4z = (BYTE)eepRd(eeAdrData3);
    for(tm = 0; tm < 4; tm++)
    {
        if(statOhr.en4z & (1 << tm))ta++;
    }
    if(ta > 1)
    {
        statOhr.en4z = 0b00001000;
        wr_Set();
    }
}
/**********************************************************************************************************/
void rd_stPPK(void)
{
    statOhr.stPPK = (BYTE)eepRd(eeAdrData);
    statOhr.num_psw = (BYTE)eepRd(eeAdrData1);
    statOhr.zone_in_psw = (BYTE)eepRd(eeAdrData2);
}
/**********************************************************************************************************/
void wr_stPPK(void)
{
    eepWr(eeAdrData, statOhr.stPPK);
    eepWr(eeAdrData1, statOhr.num_psw);
    eepWr(eeAdrData2, statOhr.zone_in_psw);
    
#ifndef UBRAT
    bu1_tx.BUFF[0] = 0x88;
    bu1_tx.BUFF[1] = statOhr.stPPK;
    bu1_tx.BUFF[2] = 0x88;

    pCB1 = &bu1_tx;
    send_u1();
    while(cB1_tx < (MAX_BUFF1_LEN-1));
    delay_ms(100);
#endif
}
/**********************************************************************************************************/
void reset_adr(void)
{
    MyAdr = 1;
    eepWr(eeAdr, MyAdr);
}
/**********************************************************************************************************/
void rd_adr(void)
{
    MyAdr = (BYTE)eepRd(eeAdr);
    if(MyAdr == 0 || MyAdr > 8)reset_adr();
}
/**********************************************************************************************************/
void eepWr(WORD ee_addr, WORD data)
{
//WORD offset;

    // Set up NVMCON to write one word of data EEPROM
    NVMCON = 0x4004;
    // Set up a pointer to the EEPROM location to be written
//    TBLPAG = __builtin_tblpage(&ee_addr);
//    offset = __builtin_tbloffset(&ee_addr);
    TBLPAG = 0x7F;
//    offset = 0xFE00 + ee_addr;
    // Write Data Value To Holding Latch
    __builtin_tblwtl((0xFE00 + ee_addr), data);
    // Disable Interrupts For 5 Instructions
    asm volatile ("disi #5");
    
    
    // Issue Unlock Sequence & Start Write Cycle
    __builtin_write_NVM();
//    Nop();Nop();Nop();Nop();
    while(NVMCONbits.WR);
}
/**********************************************************************************************************/
void eepWr_buf(WORD adrs, BYTE *pBuff, BYTE Len)
{
WORD dat;

//    Len >>= 1;
    while(Len)
    {
        //dat = (*pBuff)<<8;
        dat = *pBuff;
        pBuff++;
        //dat += *pBuff;
        dat += (*pBuff)<<8;
        pBuff++;
        eepWr(adrs, dat);
        adrs+=2;
        Len -= 2;
    }
}
/**********************************************************************************************************/
WORD eepRd(WORD ee_addr)
{
WORD data;

    TBLPAG = 0x7F;
    
    data = __builtin_tblrdl((0xFE00 + ee_addr));
    Nop();
//    Nop();
    
/*
    // Set up a pointer to the EEPROM location to be read
    TBLPAG = __builtin_tblpage(&ee_addr);
    offset = __builtin_tbloffset(&ee_addr);
    // Read the EEPROM data
    data = __builtin_tblrdl(offset);
*/
return data;
}
/**********************************************************************************************************/
void eepRd_buf(WORD adrs, BYTE *pBuff, BYTE Len)
{
WORD dat;

//    Len >>= 1;
    while(Len)
    {
        dat = eepRd(adrs);
        *pBuff = (BYTE)dat;
//        *pBuff = (dat>>8);
        pBuff++;
        //*pBuff = (BYTE)dat;
        *pBuff = (dat>>8);
        pBuff++;
        adrs+=2;
        Len -= 2;
    }
}
/**********************************************************************************************************/
void eepSetup(void)
{
    //Disable Interrupts For 5 instructions
    asm volatile("disi #5");
    //Issue Unlock Sequence
    asm volatile("mov #0x55, W0   \n"
     "mov W0, NVMKEY  \n"
     "mov #0xAA, W1   \n"
     "mov W1, NVMKEY  \n");
}
/**********************************************************************************************************/
void eepErase(void)
{
    NVMCON = 0x4050;            // Set up NVMCON to bulk erase the data EEPROM
    asm volatile ("disi #5");   // Disable Interrupts For 5 Instructions
    __builtin_write_NVM();      // Issue Unlock Sequence and Start Erase Cycle
    while(_WR);
}

