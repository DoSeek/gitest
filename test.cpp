#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <termio.h>

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


bool pressEscKey()
{
	while (true)
	{
		int ch = scanKeyboard();
		if (ch == 27)   // ESCŒüµÄŒüÖµÊÇ27
		{
			return true;
		}

		// delay, reduce CPU time slice
		//mySleep(20);
	}
}

int main(int argc, char *argv[])
{
	bool ret = pressEscKey();
	if(ret)
	{
		printf("Pressed Esc key, will exit!\n");	
	}
	return 0;
}
