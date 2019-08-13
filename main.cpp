#include <iostream>
#include <thread>
#include <cassert>
#include <cstdlib>

// different OS head file
#ifdef _MSC_VER
// Windows head file
#include <conio.h>
#else
// Linux head file
#include <termio.h>
#endif  // _MSC_VER


// variable
static bool g_bBreak = false;

// function
int  scanKeyboard();
bool pressEscKey();
void mySleep(unsigned int ms);


// ����Esc�� 
#ifdef _MSC_VER  // Windows OS
// ɨ����̣���ȡ�û����µļ�ֵ��ASCIIֵ��
// ret : ��������ASCII��ֵ(����Ҫ�س�ȷ��)
// ˵��:Ҳ��ʹ��Windows.h�еġ�GetKeyState(), GetAsyncKeystate(), GetKeyboardSlate()������ʵ��
int scanKeyboard()  
{
	if (_kbhit())  // ����а������£�����true�� _kbhit()����conio.h
	{
		int ch = _getch();  // ��ȡ���µļ�ֵ
		return ch;
	}

	return -1;
}
#else  // Linux OS
// ɨ����̣���ȡ�û����µļ�ֵ��ASCIIֵ��
// ret : ��������ASCII��ֵ(����Ҫ�س�ȷ��)
int scanKeyboard()
{
	int in;
	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(0, &stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0, &stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_settings);

	in = getchar();

	tcsetattr(0, TCSANOW, &stored_settings);
	return in;
}
#endif  // _MSC_VER

// ��⵽Esc������
bool pressEscKey()
{
	while (true)
	{
		int ch = scanKeyboard();
		if (ch == 27)   // ESC���ļ�ֵ��27
		{
			return true;
		}

		// delay, reduce CPU time slice
		mySleep(20);
	}
}

// ���������߳�
void monitorKeyboardThread()
{
	using std::cout;
	using std::endl;
	g_bBreak = pressEscKey();
	cout << "monitor thread called" << endl;
}

// C++11��׼����ʱ����
// ע�⣺�����gcc��ʹ���������ʱ�����������Ҫ��-D_GLIBCXX_USE_NANOSLEEPѡ��
void mySleep(unsigned int ms)
{
	std::chrono::milliseconds dura(ms);  // ms
	std::this_thread::sleep_for(dura);
}


int main(int argc, char *argv[])
{
	std::thread monHkbThread(monitorKeyboardThread);
	monHkbThread.join();
	

	while (!g_bBreak)
	{
		mySleep(200);  // ms
	}

	return 0;
}


