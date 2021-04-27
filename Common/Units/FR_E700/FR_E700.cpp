#include "FR_E700.h"
#include "templates/typelist.hpp"
#include "tools_debug/DebugMess.h"
#include "App/AppBase.h"

short ToShort(char d)
{
    const char *c = "0123456789ABCDEF";
    return (c[(d >> 4) & 0xf] | (c[d & 0xf] << 8));
}
char ToHex(char i)
{
    const char* c = "0123456789ABCDEF";
    return c[i];
}

char LetterToBits(char l)
{
    if (l >= '0' && l <= '9') return l - '0';
    l = toupper(l);
    if (l >= 'A' && l <= 'F') return l - 'A' + 10;
    return 0;
}

short Sum(char* d, int len)
{
    unsigned char res = 0;
    for (int i = 0; i < len; ++i) res += (unsigned char)d[i];
    return ToShort(res);
}

void PrintBuf(char* buf, int len)
{
    static const int size = 3 * 1024;
    char buffer[size];
    char* c = buffer;
    if (len > size / 3) len = size / 3;
    for (int i = 0; i < len; ++i)
    {
        sprintf(c, "%X%X.", (buf[i] >> 4)&0xf, buf[i] & 0xf);
        c += strlen(c);
    }
    dprint("%s\n", buffer);
}

ComPortHandler noopComPortHandler;

void FR_E700::Reset::Send()
{
    char buf[] = {ENQ, 0, 0, 'F', 'D', timeout, '9', '9', '6', '6' , 0, 0, CR};
    *(short*)&buf[1] = ToShort(abonent);
    short t = *(short*)&buf[sizeof(buf) - 3] = Sum(&buf[1], sizeof(buf) - 4);
    PrintBuf(buf, sizeof(buf));
    port.Write((unsigned char *)buf, (int)sizeof(buf));
}

FR_E700::Reset::Reset()
    : port(Singleton<ComPort>::Instance())
{}

void FR_E700::Reset::operator()(unsigned char(&input)[1024], int len)
{
    if (len > 0)
    {
        if (ToShort(abonent) == *(short*)&input[1])
        {
            if (ACK == input[0])
            {
                //TODO RESET INVERTER OK Запрос на сброс инвертара прошёл успешно
                status = inverter_ok;
                dprint("inverter_ok\n");
                PrintBuf((char*)input, len);
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            else if(ENQ == input[0])
            {
               //TODO ERROR Инвертор выслал ошибку
                status = input[3];
                PrintBuf((char*)input, len);
                port.SetReceiveHandler(&noopComPortHandler);
            }
        }
        loopCount = maxLoopCount;
    }
    else
    {
        if (currentTime < GetTickCount())
        {
            if (--loopCount < 0)
            {
            //TODO EXIT ERROR RECEIVE COM PORT  Запрос на сброс инвертора не сработал
                status = inverter_reset_request_did_not_work;
                dprint("inverter_reset_request_did_not_work\n");
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            currentTime = GetTickCount() + delay;
            Send();
        }
    }
}

void FR_E700::Reset::Init()
{
    status = start_query;
    delay = 1000;
    loopCount = maxLoopCount;
    currentTime = GetTickCount() + delay;
    port.SetReceiveHandler(this);
    Send();
}

void FR_E700::SetFrequency::Send()
{
    short frequency = Singleton<ComPortTable>::Instance().items.get<InverterFrequency>().value;
    frequency *= 100;
    char buf[] = { ENQ, 0, 0, '8', '4', timeout, 0, 0, 0, 0 , 0, 0, CR };
    *(short*)&buf[1] = ToShort(abonent);
    *(short*)&buf[6] = ToShort(frequency >> 8);
    *(short*)&buf[8] = ToShort(frequency & 0xff);
    *(short*)&buf[sizeof(buf) - 3] = Sum(&buf[1], sizeof(buf) - 4);
	PrintBuf(buf, sizeof(buf));
    port.Write((unsigned char*)buf, (int)sizeof(buf));
}

FR_E700::SetFrequency::SetFrequency()
    : port(Singleton<ComPort>::Instance())
{}

void FR_E700::SetFrequency::operator()(unsigned char(&input)[1024], int len)
{
    if (len > 0)
    {
        if (ToShort(abonent) == *(short*)&input[1])
        {
            if (ACK == input[0])
            {
                //TODO RESET INVERTER OK Запрос на установку частоты вращения прошёл успешно
                status = inverter_ok;
                dprint("inverter_ok\n");
                port.SetReceiveHandler(&noopComPortHandler);
				PrintBuf((char *)input, len);
                return;
            }
            else if (ENQ == input[0])
            {
                //TODO ERROR Инвертор выслал ошибку
                status = input[3];
				port.SetReceiveHandler(&noopComPortHandler);
				PrintBuf((char*)input, len);
            }
        }
        loopCount = maxLoopCount;
    }
    else
    {
        if (currentTime < GetTickCount())
        {
            if (--loopCount < 0)
            {
                //TODO EXIT ERROR RECEIVE COM PORT  Запрос на установку частоты вращения не сработал
                status = speed_​​setting_request_did_not_work;
                dprint("speed_​​setting_request_did_not_work\n");
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            currentTime = GetTickCount() + delay;
            Send();
        }
    }
}

void FR_E700::SetFrequency::Init()
{
    status = start_query;
    delay = 1000;
    loopCount = maxLoopCount;
    currentTime = GetTickCount() + delay;
    port.SetReceiveHandler(this);
    Send();
}

void FR_E700::SetState::Send()
{
    char buf[] = { ENQ, 0, 0, 'F', 'A', timeout, 0, 0, 0, 0, CR };
    *(short*)&buf[1] = ToShort(abonent);
    *(short*)&buf[6] = ToShort((char)state);
    *(short*)&buf[sizeof(buf) - 3] = Sum(&buf[1], sizeof(buf) - 4);
	PrintBuf(buf, sizeof(buf));
    port.Write((unsigned char*)buf, (int)sizeof(buf));
}

FR_E700::SetState::SetState()
    : port(Singleton<ComPort>::Instance())
{}

void FR_E700::SetState::operator()(unsigned char(&input)[1024], int len)
{
    if (len > 0)
    {
        if (ToShort(abonent) == *(short*)&input[1])
        {
            if (ACK == input[0])
            {
                //TODO RESET INVERTER OK Запрос на установку состояния прошёл успешно
                status = inverter_ok;
                dprint("inverter_ok\n");
				PrintBuf((char *)input, len);
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            else if (ENQ == input[0])
            {
                //TODO ERROR Инвертор выслал ошибку
                status = input[3];
				PrintBuf((char*)input, len);
            }
        }
        loopCount = maxLoopCount;
    }
    else
    {
        if (currentTime < GetTickCount())
        {
            if (--loopCount < 0)
            {
                //TODO EXIT ERROR RECEIVE COM PORT  Запрос на установку состояния не сработал
                status = request_to_set_the_state_did_not_work;
                dprint("request_to_set_the_state_did_not_work\n");
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            currentTime = GetTickCount() + delay;
            Send();
        }
    }
}

void FR_E700::SetState::Init(WriteState state)
{
    status = start_query;
    this->state = state;
    delay = 300;
    loopCount = maxLoopCount;
    currentTime = GetTickCount() + delay;
    port.SetReceiveHandler(this);
    Send();
}

void FR_E700::GetState::Send()
{
    char buf[] = { ENQ, 0, 0, '7', 'A', timeout, 0, 0, CR };
    *(short*)&buf[1] = ToShort(abonent);
    *(short*)&buf[sizeof(buf) - 3] = Sum(&buf[1], sizeof(buf) - 4);
	PrintBuf(buf, sizeof(buf));
    port.Write((unsigned char*)buf, (int)sizeof(buf));
}

FR_E700::GetState::GetState()
    : port(Singleton<ComPort>::Instance())
{}

void FR_E700::GetState::operator()(unsigned char(&input)[1024], int len)
{
    if (len > 0)
    {
        if (ToShort(abonent) == *(short*)&input[1])
        {
            if (STX == input[0])
            {
				PrintBuf((char *)input, len);
                short sum = Sum((char *)&input[1], 4);
                if (sum == *(short*)&input[6])
                {
                    char bits = (LetterToBits(input[3]) << 4) | LetterToBits(input[4]);
                    if (0 != (bits & (char)ReadState::ABS))
                    {
                        if (--loopCount < 0)
                        {
                            status = ABC_bit_ON;
                            dprint("ABC_bit_ON\n");
                            port.SetReceiveHandler(&noopComPortHandler);
                            return;
                        }
                    }
					PrintBuf((char *)input, len);
                  //  Sleep(500);
                  //  Send();
                }
            }
            else if (ENQ == input[0])
            {
                //TODO ERROR Инвертор выслал ошибку
                dprint("%x  %d\n", input[3], input[3]);
                status = input[3];
            }
        }
        loopCount = maxLoopCount;
    }
    else
    {
        if (currentTime < GetTickCount())
        {
            if (--loopCount < 0)
            {
                //TODO EXIT ERROR RECEIVE COM PORT  Запрос на проверку состояния не сработал
                status = status_check_request_failed;
                dprint("status_check_request_failed\n");
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            currentTime = GetTickCount() + delay;
            Send();
        }
    }
}

void FR_E700::GetState::Init()
{
    status = start_query;
    delay = 3000;
    loopCount = maxLoopCount;
    currentTime = GetTickCount() + delay;
    port.SetReceiveHandler(this);
    Send();
}

bool FR_E700::Init()
{
    bool b = UpdateComPort();
    ComPortHandler::status = inverter_ok;
    return b;
}

void FR_E700::Destroy()
{
    Singleton<ComPort>::Instance().~ComPort();
}

bool FR_E700::UpdateComPort()
{
    auto &params = Singleton<ComPortTable>::Instance().items;
    auto &port = Singleton<ComPort>::Instance();
    ComPortHandler::abonent = params.get<Abonent>().value;
    port.Close();
    return port.Open(
        params.get<ComPortAddr>().value
        , params.get<BaudRate>().value
        , params.get<Parity>().value
        , params.get<StopBits>().value
    );

    
}

void FR_E700::Mode::Send()
{
    char buf[] = { ENQ, 0, 0, 'F', 'B', timeout, '0', '0', '0', '0' , 0, 0, CR };
    *(short*)&buf[1] = ToShort(abonent);
    short t = *(short*)&buf[sizeof(buf) - 3] = Sum(&buf[1], sizeof(buf) - 4);
    PrintBuf(buf, sizeof(buf));
    port.Write((unsigned char*)buf, (int)sizeof(buf));
}

FR_E700::Mode::Mode()
    : port(Singleton<ComPort>::Instance())
{ }

void FR_E700::Mode::operator()(unsigned char(&input)[1024], int len)
{
    if (len > 0)
    {
        if (ToShort(abonent) == *(short*)&input[1])
        {
            if (ACK == input[0])
            {
                //TODO RESET INVERTER OK Запрос на сброс инвертара прошёл успешно
                status = inverter_ok;
                dprint("inverter_ok\n");
                PrintBuf((char*)input, len);
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
			else if (ENQ == input[0])
			{
				//TODO ERROR Инвертор выслал ошибку
				status = input[3];
				PrintBuf((char*)input, len);
				port.SetReceiveHandler(&noopComPortHandler);
			}
        }
        loopCount = maxLoopCount;
    }
    else
    {
        if (currentTime < GetTickCount())
        {
            if (--loopCount < 0)
            {
                //TODO EXIT ERROR RECEIVE COM PORT  Запрос на сброс инвертора не сработал
                status = inverter_mode_did_not_work;
                dprint("inverter_mode_did_not_work\n");
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            currentTime = GetTickCount() + delay;
            Send();
        }
    }
}

void FR_E700::Mode::Init()
{
    status = start_query;
    delay = 300;
    loopCount = maxLoopCount;
    currentTime = GetTickCount() + delay;
    port.SetReceiveHandler(this);
    Send();
}

void FR_E700::GetFrequency::Send()
{
    char buf[] = { ENQ, 0, 0, '0', '4', timeout, 0, 0, CR };
    *(short*)&buf[1] = ToShort(abonent);
    short t = *(short*)&buf[sizeof(buf) - 3] = Sum(&buf[1], sizeof(buf) - 4);
    PrintBuf(buf, sizeof(buf));
    port.Write((unsigned char*)buf, (int)sizeof(buf));
}

FR_E700::GetFrequency::GetFrequency() : port(Singleton<ComPort>::Instance()) {}

void FR_E700::GetFrequency::operator()(unsigned char(&input)[1024], int len)
{
    if (len > 0)
    {
        if (ToShort(abonent) == *(short*)&input[1])
        {
            if (STX == input[0])
            {
                short sum = Sum((char*)&input[1], 6);
                if (sum == *(short*)&input[8])
                {
					for (int i = 3; i <= 6; ++i)
					{
						frequency <<= 4;
						frequency += LetterToBits(input[i]);
					}

					frequency /= 100;
                    status = inverter_ok;
                    dprint("inverter_ok\n");
                    PrintBuf((char*)input, len);
                    port.SetReceiveHandler(&noopComPortHandler);
                    return;
                }
            }
            else if (NAK == input[0])
            {
                //TODO ERROR Инвертор выслал ошибку
                status = input[3];
                PrintBuf((char*)input, len);
                port.SetReceiveHandler(&noopComPortHandler);
            }
        }
        loopCount = maxLoopCount;
    }
    else
    {
        if (currentTime < GetTickCount())
        {
            if (--loopCount < 0)
            {
                //TODO EXIT ERROR RECEIVE COM PORT  Запрос на сброс инвертора не сработал
                status = get_frequency_did_not_work;
                dprint("get_frequency_did_not_work\n");
                port.SetReceiveHandler(&noopComPortHandler);
                return;
            }
            currentTime = GetTickCount() + delay;
            Send();
        }
    }
}

void FR_E700::GetFrequency::Init()
{
    frequency = 0;
    status = start_query;
    delay = 1000;
    loopCount = maxLoopCount;
    currentTime = GetTickCount() + delay;
    port.SetReceiveHandler(this);
    Send();
}
