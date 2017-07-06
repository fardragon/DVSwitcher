#ifndef FOCUSHOOK_H
#define FOCUSHOOK_H

#include <QObject>
#include <windows.h>
#include <psapi.h>

class FocusHook : public QObject
{
	Q_OBJECT
public:
	static FocusHook& getInstance();

	FocusHook(FocusHook const&)  = delete;
	void operator=(FocusHook const&) = delete;
	~FocusHook();

private:
	explicit FocusHook(QObject *parent = nullptr);
	HWINEVENTHOOK m_hook;

	static void CALLBACK EventProc(HWINEVENTHOOK /* unused */, DWORD  /* unused */, HWND hwnd, LONG  /* unused */, LONG  /* unused */, DWORD  /* unused */, DWORD  /* unused */ );
	void emit_foreground_changed(QString new_foreground);

signals:
	void foreground_changed(QString new_foreground);

public slots:
};

#endif // FOCUSHOOK_H
