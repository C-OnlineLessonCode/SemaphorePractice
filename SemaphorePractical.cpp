#include <iostream>
#include <windows.h>
#include <sstream>
HANDLE g_semaphore;

void Wharf(const std::string& shipName) {
	for (int i = 0; i < 10; ++i) {
		Sleep(200);
		std::cout << shipName << " wharfed amount: " << i << std::endl;
	}
}

DWORD WINAPI ThreadMain(LPVOID threadParameter) {
	
	WaitForSingleObject(g_semaphore, INFINITE);
	std::stringstream ss;
	ss << "name" << reinterpret_cast<int>(threadParameter);
	Wharf(ss.str());
	LONG priviousCount = 0;
	ReleaseSemaphore(g_semaphore, 1, &priviousCount);
	return 0;
}

int main()
{
	g_semaphore = CreateSemaphoreW(
		NULL,
		3,
		3,
		NULL
	);

	HANDLE threads[10] = {};

	for (int i = 0; i < std::size(threads); ++i) {
		DWORD threadId = 0;
		threads[i] = CreateThread(
			0,
			0,
			ThreadMain,
			reinterpret_cast<void*>(i + 1),
			0,
			&threadId
		);
	}

	WaitForMultipleObjects(std::size(threads), threads, TRUE, INFINITE);

	for (int i = 0; i < std::size(threads); ++i) {
		CloseHandle(threads[i]);
	}
	CloseHandle(g_semaphore);
}