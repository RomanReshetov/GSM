#include <p24F16KA101.h>

#include "GenericTypeDefs.h"
#include "var.h"
#include "function.h"
/**********************************************************************************************************/
/**********************************************************************************************************/
/**********************************************************************************************************/
BYTE check_modem(BYTE Com)
{
    st_hl2 = OFF;
    HL2_OFF();

    bu1_tx.code = MyCode;
    bu1_tx.adr = MyAdr;

    switch(Com)
    {
        case cPPK_state:
        {
            form_status();
            return 0;
        }
        case cPPK_secur_on:
        {
            statOhr.stPPK &= 0b00111111;
            statOhr.stPPK |= StOhr;
            statOhr.num_psw = bu1_rx.BUFF[3];
            
            bu1_tx.stat_com = Com;
            bu1_tx.Z21 = statOhr.num_psw;
            bu1_tx.Z43 = bu1_tx.stat_com;
            bu1_tx.Z65 = bu1_tx.Z21;
            
            CurLen = MAX_BUFF1_LEN_m;
            return 0;
        }
        case cPPK_secur_off:
        {
            statOhr.stPPK &= 0b00111111;
            statOhr.num_psw = bu1_rx.BUFF[3];
            
            bu1_tx.stat_com = Com;
            bu1_tx.Z21 = statOhr.num_psw;
            bu1_tx.Z43 = bu1_tx.stat_com;
            bu1_tx.Z65 = bu1_tx.Z21;

            CurLen = MAX_BUFF1_LEN_m;
            return 0;
        }
        default: 
        {
            return 1;
        }
    };
    return 1;
}
/**********************************************************************************************************/
void form_status(void)
{
    bu1_tx.Z21 = (ZToModem[1]<<4) + ZToModem[0];//Z12;
    bu1_tx.Z43 = (ZToModem[3]<<4) + ZToModem[2];//Z34;
    bu1_tx.Z65 = statOhr.zone_key;
    bu1_tx.Z87 = 0;
    
    bu1_tx.stat_com = 0;
    if(statOhr.stPPK&StHalfOhr)bu1_tx.stat_com |= StHalfOhr;
    else if(statOhr.stPPK&StOhr)bu1_tx.stat_com |= StOhr;
    
    bu1_tx.numUser = statOhr.num_psw;
    
    bu1_tx.alarm = statOhr.stKey;//0;
    
    if(statOhr.stPPK&StTamp)bu1_tx.alarm |= 0x80;

    bu1_tx.outs = (OUT_REL | (OUT_UPR<<1) | (statOhr.stRelKey<<2));

    bu1_tx.t0 = statOhr.t0;
    bu1_tx.t1 = statOhr.t1;
    CurLen = MAX_BUFF1_LEN;
}
/**********************************************************************************************************/


/**********************************************************************************************************/

/**********************************************************************************************************/

/**********************************************************************************************************/


