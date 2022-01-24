

#include "include.h"


auto g_injection = new c_injection( );

int main( ) {
	std::cout << "Searching for target window...\n";

	HWND hwnd = NULL;

	while ( hwnd == NULL ) {
		std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
		hwnd = FindWindow( NULL, L"Calculator" );
	}

	std::cout << "Window found\n";

	DWORD process_id{};
	auto thread_id = GetWindowThreadProcessId( hwnd, &process_id );

	if ( !thread_id )
		return 1;

	std::cout << "Thread found\n";

	auto dll = LoadLibraryEx( L"sethook_dll.dll", NULL, DONT_RESOLVE_DLL_REFERENCES );
	if ( !dll )
		return 2;

	std::cout << "Dll loaded\n";

	auto hook_addr = ( HOOKPROC )GetProcAddress( dll, "CoolExport" );
	if ( !hook_addr )
		return 3;

	std::cout << "Export located\n";

	auto handle = SetWindowsHookEx( WH_GETMESSAGE, hook_addr, dll, thread_id );
	if ( !handle )
		return 4;

	std::cout << "Hook set\n";

	PostThreadMessage( thread_id, WM_USER + 432, NULL, ( LPARAM )handle );

	std::cout << "ThreadMessage issued\n";
	
	std::this_thread::sleep_for( std::chrono::seconds( 10 ) );

	//auto unhooked = UnhookWindowsHookEx( handle );
	//if ( unhooked == FALSE )
	//	return 5;

	//std::cout << "Unhooked\n";

	std::this_thread::sleep_for( std::chrono::seconds( 10 ) );

	return 0;
}

