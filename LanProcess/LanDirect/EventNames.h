#pragma once
extern const wchar_t *wStart;
extern const wchar_t *wStop;
extern const wchar_t *wExit;
extern const wchar_t *wLanParams;
static const unsigned ID_GET_PACKET_SIZE = 0xc0ffe;
static const unsigned ID_NO_DATA_COLLECTION_FROM_LAN_BOARD = ID_GET_PACKET_SIZE + 1;
static const unsigned ID_ERROR_INIT_LAN_BOARD              = ID_GET_PACKET_SIZE + 2;