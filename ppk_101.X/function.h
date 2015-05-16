/* 
 * File:   function.h
 * Author: reshetov.r
 *
 * Created on 13 ??????? 2015 ?., 11:04
 */

#ifndef FUNCTION_H
#define	FUNCTION_H

#ifdef	__cplusplus
extern "C" {
#endif

extern void init_hw(void);
extern void init_var(void);
extern void U1Init(void);
extern void U2Init(void);
extern BYTE CallCRCDallas(BYTE *SourceAdr, BYTE Len);
//extern void Print2Dec(BYTE toPrint);
//extern void Print2Char(BYTE toPrint);
//extern void U1Put(BYTE c);
//extern void U2Put(BYTE c);
//extern void Send1_str(BYTE *Com);
//extern void Send2_str(BYTE *Com);
extern void send_u1(void);
extern void send_u2(void);
extern BYTE check_z(WORD zpt/*, BYTE NZone*/);
extern void eepWr(WORD ee_addr, WORD data);
extern WORD eepRd(WORD ee_addr);
extern void eepSetup(void);
extern void eepErase(void);
extern void eepWr_buf(WORD adrs, BYTE *pBuff, BYTE Len);
extern void eepRd_buf(WORD adrs, BYTE *pBuff, BYTE Len);
extern void rd_adr(void);
extern void reset_adr(void);
extern void rd_stPPK(void);
extern void wr_stPPK(void);
extern void wr_timeSet(void);
extern void rd_timeSet(void);
extern void wr_Set(void);
extern void rd_Set(void);

extern void delay_ms(WORD tms);
extern void chek_input_key(void);
extern BYTE en_ohr(BYTE enZ);
//extern void parse_zone(BYTE *pBuff);
extern void check_alarm(void);//void check_alarm(void);
extern void ch_zone(void);


extern BYTE check_modem(BYTE Com);
extern void form_status(void);
extern void form_ohr_func(BYTE nz);
extern void time_InOut_off(void);
extern void form_stOhr_off(void);
extern void form_stOhr_on(void);


#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTION_H */

