#ifndef __WIRED_HART_APP_H_
#define __WIRED_HART_APP_H_


uint8 AppHART_ProcessLocalCmd(uint8 * p_pucCmdData);
uint8 AppHART_ComposeLocalCmd(void *p_pucInBuffer, uint8 *p_pComposedCmd, uint16 p_unCmdId, uint8 p_ucRespCode, uint16 p_ucIndex, uint16 *p_unSize);
uint8 AppHART_ParseLocalCmd(uint8 *p_pucInBuffer, void *p_pParsedCmd, uint16 *p_punCmdId, uint16 *p_pIndex);
uint8 AppHART_IsLocalCmd(uint16 p_unCmdId);
uint8 AppHART_Rx_Indicate ( const uint8 * p_pucMsgBuf, uint8 p_unSize );


#endif // __WIRED_HART_APP_H_
