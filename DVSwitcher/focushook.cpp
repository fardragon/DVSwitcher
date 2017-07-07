#include "focushook.h"

FocusHook::FocusHook(QObject *parent) : QObject(parent), m_hook(nullptr)
{
	this->hook();
}

FocusHook &FocusHook::getInstance()
{
	static FocusHook instance;
	return instance;
}

void FocusHook::emit_foreground_changed(QString new_foreground)
{
	emit foreground_changed(new_foreground);
}

void FocusHook::hook()
{
	m_hook = SetWinEventHook(EVENT_OBJECT_FOCUS,EVENT_OBJECT_FOCUS,NULL,FocusHook::EventProc,0,0,WINEVENT_OUTOFCONTEXT);
}

void FocusHook::unhook()
{
	UnhookWinEvent(m_hook);
}

void FocusHook::toggle_active(bool inactive)
{
	if (!inactive)
	{
		this->hook();
		this->emit_exec(GetActiveWindow());
	}
	else
	{
		this->unhook();
		emit foreground_changed("Disabled");
	}
}

FocusHook::~FocusHook()
{
	this->unhook();
}

void FocusHook::EventProc(HWINEVENTHOOK /* unused */, DWORD  /* unused */, HWND hwnd, LONG  /* unused */, LONG  /* unused */, DWORD  /* unused */, DWORD  /* unused */ )
{
	FocusHook::getInstance().emit_exec(hwnd);
}

void FocusHook::emit_exec(HWND hwnd)
{
	int size = 100;
	int length = 0;
	LPWSTR name = new WCHAR[size];
	LPDWORD pid = new DWORD;

	GetWindowThreadProcessId(hwnd,pid);
	auto handle = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_OPERATION,0,*pid);
	length = GetModuleFileNameExW(handle,NULL,name,size);

	while (length == size - 1)
	{
		delete[] name;
		size += 50;
		name = new WCHAR[size];
		length = GetModuleFileNameExW(handle,NULL,name,size);
	}


	QString text = QString::fromWCharArray(name,length);
	std::replace(text.begin(),text.end(),QChar('\\'),QChar('/'));
	FocusHook::getInstance().emit_foreground_changed(text);

	CloseHandle(handle);
	delete pid;
	delete[] name;
}
