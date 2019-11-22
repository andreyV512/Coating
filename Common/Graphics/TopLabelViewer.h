#pragma once
#include "window_tool/message.h"
#include "Graphics/ColorLabel.h"

class TopLabelViewer
{
public:
	Gdiplus::Bitmap *backScreen;
public:
	ColorLabel label;
public:
	HWND hWnd;
	TopLabelViewer();
	~TopLabelViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TUser &);
	void operator()(TDestroy &);
	void SetMessage(wchar_t *);
};

