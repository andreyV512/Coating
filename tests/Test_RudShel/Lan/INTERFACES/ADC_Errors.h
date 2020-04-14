#ifndef __ADC_ERRORS_H__
#define __ADC_ERRORS_H__
/* ####################################################################### */
#include "adc_const.h"
/* ----------------------------------------------------------------------- */
#define ADC_ERROR_INVALIDERRORCODE 0xFFFFFFFF
#ifndef DIM
#define DIM(__x) (sizeof(__x)/sizeof(__x[0]))
#endif
/* ----------------------------------------------------------------------- */
typedef struct _ADCError
{
    int         m_nErrorCode;    // ��� ������
    char*       m_nErrorStr;     // �����������
} ADCError, *PADCError;
/* ----------------------------------------------------------------------- */
const _ADCError  ADCErrorStrings[] =
    { ADC_ERROR_NOTSUPPORTED,        "������� �� ��������������",
      ADC_ERROR_NOTINITIALIZE,       "������� �� ��������������� (�� ������ ������� �����)",
      ADC_ERROR_INVALIDPARAMETERS,   "������������ �������� ����������",
      ADC_ERROR_INVALIDBASE,         "������������ ������� �����",
      ADC_ERROR_INVALIDIRQ,          "������������ ����� IRQ",
      ADC_ERROR_INVALIDDRQ,          "������������ ����� DRQ",
      ADC_ERROR_DRQ_ALLREADY_USED,   "������������� DRQ �����",
      ADC_ERROR_IRQ_ALLREADY_USED,   "������������� IRQ �����",
      ADC_ERROR_VXD_NOTLOADED,       "��� ������������ ��������",
      ADC_ERROR_VXD_INVALIDVERSION,  "������� ����, �� ������������ ������",
      ADC_ERROR_LOADXILINX,          "�� ������� ��������� ����",
      ADC_ERROR_DRQ_NOTSETUP,        "�� ������ ��������� ���������",
      ADC_ERROR_NO_CONFIG_INFO,      "������� �� ���������������",
      ADC_ERROR_CANTLOADXILINXFILES, "�� ������� ��������� ���� ������������ ����",
      ADC_ERROR_LOAD_ADSP,           "�� ������� ��������� ��������� � ADSP ���������",
      ADC_ERROR_LOAD_ADSP_FILE,      "������ ��� ����������� ���� � ���������� ��� ADSP ����������",
      1,                             "�� ������������� ������������!",
      ADC_ERROR_INVALIDERRORCODE,    "�� ����������� ��� ������"};

const _ADCError  ADCErrorStringsEng[] =
    { ADC_ERROR_NOTSUPPORTED,        "Function is not supported",
      ADC_ERROR_NOTINITIALIZE,       "Driver is not initialized (possibly wrong base address)",
      ADC_ERROR_INVALIDPARAMETERS,   "Invalid parameters",
      ADC_ERROR_INVALIDBASE,         "Invalid base address",
      ADC_ERROR_INVALIDIRQ,          "Invalid IRQ",
      ADC_ERROR_INVALIDDRQ,          "Invalid DRQ",
      ADC_ERROR_DRQ_ALLREADY_USED,   "Requested DRQ is already used",
      ADC_ERROR_IRQ_ALLREADY_USED,   "Requested IRQ is already used",
      ADC_ERROR_VXD_NOTLOADED,       "No virtual driver",
      ADC_ERROR_VXD_INVALIDVERSION,  "Virtual driver has incorrect version",
      ADC_ERROR_LOADXILINX,          "Failed to configure FPGA chip",
      ADC_ERROR_DRQ_NOTSETUP,        "Parameters of mesurement are not specified",
      ADC_ERROR_NO_CONFIG_INFO,      "Driver is not configured",
      ADC_ERROR_CANTLOADXILINXFILES, "Failed to load file with configuration for FPGA chip",
      ADC_ERROR_LOAD_ADSP,           "Failed to upload program to ADSP",
      ADC_ERROR_LOAD_ADSP_FILE,      "Bad or missing file with program for ADSP",
      1,                             "No errors (Success)",
      ADC_ERROR_INVALIDERRORCODE,    "Unknown error"};
/* ----------------------------------------------------------------------- */

#endif
