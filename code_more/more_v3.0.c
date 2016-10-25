/*File Name: more_v3.0
 *Author: FumasterLin
 *添加了键入字符立即响应程序
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define PAGELEN 24
#define LINELEN 512

void do_more(FILE* fp);
int see_more(FILE* cmd);

int main(int argc, char* argv[])
{
	FILE* fp;
	if(argc == 1)
		do_more(stdin);
	else
		while(--argc)
		{
			if( ( fp = fopen(* ++argv, "r")) != NULL)
			{
				do_more(fp);
				fclose(fp);
			}
			else
				exit(1);
		}
	return 0;
}

void do_more(FILE* fp)
{
	char line[LINELEN];
	int num_of_lines=0;
	int see_more(FILE *cmd),reply;
	FILE *fp_tty_in, *fp_tty_out;
	fp_tty_in = fopen("/dev/tty", "r");
	fp_tty_out = fopen("/dev/tty", "w");
	struct termios initial_settings, new_settings;
	tcgetattr(fileno(fp_tty_in), &initial_settings);//获取当前终端的属性
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;//设置终端为非标准模式
	//设置读入一个字符，立即返回字符
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	//重新配置终端接口
	if(tcsetattr(fileno(fp_tty_in), TCSANOW, &new_settings)!= 0)
		fprintf(stderr,"could not set attributes\n");
//	if(fp_tty == NULL)
//		exit(1);
	while(fgets(line,LINELEN, fp))
	{
		if( num_of_lines == PAGELEN )
		{
			reply = see_more(fp_tty_in);
			if(reply == 0)
				break;
			num_of_lines -= reply;
		}
		if( fputs(line, fp_tty_out) == EOF )
		{
			//恢复终端接口的配置
			tcsetattr(fileno(fp_tty_in), TCSANOW, &initial_settings);
			exit(1);
		}
		num_of_lines ++;
	}
	//恢复终端接口的配置
	tcsetattr(fileno(fp_tty_in), TCSANOW, &initial_settings);
}

int see_more(FILE* cmd)
{
	int c;
	printf("\033[7m more? \033[m");
	while( (c = getc(cmd))!= EOF)
	{
		if( c == 'q')
			return 0;
		if( c == ' ' )
			return PAGELEN;
		if( c == '\n')
			return 1;
	}

	return 0;
}


