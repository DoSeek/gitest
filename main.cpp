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


// 监听Esc键 
#ifdef _MSC_VER  // Windows OS
// 扫描键盘，获取用户按下的键值（ASCII值）
// ret : 被按键的ASCII码值(不需要回车确认)
// 说明:也可使用Windows.h中的“GetKeyState(), GetAsyncKeystate(), GetKeyboardSlate()”函数实现
int scanKeyboard()  
{
	if (_kbhit())  // 如果有按键按下，返回true。 _kbhit()归属conio.h
	{
		int ch = _getch();  // 获取按下的键值
		return ch;
	}

	return -1;
}
#else  // Linux OS
// 扫描键盘，获取用户按下的键值（ASCII值）
// ret : 被按键的ASCII码值(不需要回车确认)
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

// 检测到Esc键被按
bool pressEscKey()
{
	while (true)
	{
		int ch = scanKeyboard();
		if (ch == 27)   // ESC键的键值是27
		{
			return true;
		}

		// delay, reduce CPU time slice
		mySleep(20);
	}
}

// 监听键盘线程
void monitorKeyboardThread()
{
	using std::cout;
	using std::endl;
	g_bBreak = pressEscKey();
	cout << "monitor thread called" << endl;
}

// C++11标准的延时函数
// 注意：如果在gcc中使用这个函数时报编译错误，需要加-D_GLIBCXX_USE_NANOSLEEP选项
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


