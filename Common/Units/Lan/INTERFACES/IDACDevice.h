#ifndef __DAC_DEVICE_H__
#define __DAC_DEVICE_H__
#include "DAC_CONST.h"
#pragma pack(1)
/*######################################################################*/
/* LOWORD is major version, HIWORD is minor version                     */
/*                          minor|major                                 */ 
#define MAJOR_VERSION       0x0001  // essential change
#define MINOR_VERSION       0x0005  // not essential change
#define CURRENT_VERSION     (MAJOR_VERSION|(MINOR_VERSION<<16))
/*######################################################################*/
#ifndef INTERRUPT_MESSAGE_STRUCTURE
#define INTERRUPT_MESSAGE_STRUCTURE
struct  Interrupt_Message
{
    unsigned    m_hWnd;         // HWND
    unsigned    m_uMessage;     // UINT
    unsigned    m_uLParam;      // LPARAM
};
#endif
/*----------------------------------------------------------------------*/
struct  DACParametersBase
{
    int         m_nType;
    int         m_nSizeOf;
};
/*----------------------------------------------------------------------*/
struct  IDACDevice
{
    virtual unsigned __stdcall  AddRef() = 0;
    virtual unsigned __stdcall  Release() = 0;

    virtual int      __stdcall  Setup(int baseAdr, int DRQ, int IRQ, HANDLE hEvent) = 0;
    virtual int      __stdcall  Test() = 0;
    virtual int      __stdcall  Get(int mode, void* value) = 0;
    virtual int      __stdcall  PortIO(int mode, char* pAdr, int size) = 0;

    virtual int      __stdcall  Init(int mode, DACParametersBase*, float*) = 0;
    virtual int      __stdcall  Start(void) = 0;
    virtual int      __stdcall  Stop(void) = 0;
    virtual int      __stdcall  SetData(int mode, char* pAdr, int size, int offset) = 0;
    virtual int      __stdcall  ClearInterrupt() = 0;
};
/*######################################################################*/
struct  DACParametersDMA : public DACParametersBase
{
    int         m_nStartOf;
    int         m_nIntOf;
    int         m_nChannel;  
    int         m_nDMAMode;
    float       m_fFreqStart;
    int         m_nTimerStart;
    int         m_nBlockSize;
    float       m_fFreqCutoff;
    float       m_fComp;
    float       m_fDelta;
    
    DACParametersDMA()
    {
        m_nType   = DAC_DMA_TYPE;
        m_nSizeOf = sizeof(DACParametersDMA);
        m_nChannel = 0;
        m_nBlockSize = 0;
    }
};
/*######################################################################*/
struct  DACParametersMemory : public DACParametersBase
{
    int         m_nStartOf;
    int         m_nIntOf;
    int         m_nChannel;  
    float       m_fFreqStart;
    float       m_fFreqCutoff;
    float       m_fComp;
    float       m_fDelta;
    int         m_nBlockSize;
  
    DACParametersMemory()
    {
        m_nType = DAC_MEMORY_TYPE;
        m_nSizeOf = sizeof(DACParametersMemory);
        m_nBlockSize = 0;
        m_fComp = 0;
        m_fDelta = 0;
    }
};
/*#####################################################################*/
struct  DACParametersTimer : public DACParametersBase
{
    int         m_nTimer;
    int         m_nTimerDelay;
    int         m_nTimerMode;

    DACParametersTimer()
    {
        m_nType   = DAC_TIMER_TYPE;
        m_nSizeOf = sizeof(DACParametersTimer);
    }
};      
/*######################################################################*/
#pragma pack()
#endif
