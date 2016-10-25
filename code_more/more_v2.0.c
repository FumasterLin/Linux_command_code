/*
 * Author:FumasterLin
 * version 2.0 for more
 * read and print 24 lines then pause for a few special commands
 */
#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE* fp);
int see_more(FILE* cmd);

int main(int argc, char* argv[])
{
	FILE* fp;
	if(argc == 1)
		do_more(stdin);//more 后无参数，读取stdin
	else
		while(--argc)
		{
			if( ( fp = fopen(* ++argv, "r")) != NULL)//打开文件
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
	FILE *fp_tty;
	fp_tty = fopen("/dev/tty", "r");//打开/dev/tty设备文件
	if(fp_tty == NULL)
		exit(1);
	while(fgets(line,LINELEN, fp))
	{
		if( num_of_lines == PAGELEN )
		{
			reply = see_more(fp_tty);
			if(reply == 0)
				break;
			num_of_lines -= reply;
		}
		if( fputs(line, stdout) == EOF )
			exit(1);
		num_of_lines ++;
	}
}

int see_more(FILE* cmd)
{
	int c;
	printf("\033[7m more? \033[m");
	while( (c = getc(cmd))!= EOF)//此处的getchar()是从stdin读取数据，getc(cmd)是从cmd(/dev/tty)中读入数据的
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


