/* ls_v1.0.c
 * purpose list content of directory or directories
 * action if no args,use.else list files in args
 */


#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char[]);
void main(int ac, char *av[])
{
	if(ac==1)
		do_ls(".");
	else if(ac==2)
		do_ls(av[1]);
	else
		while(--ac)
		{
			printf("%s:\n",*++av);
			do_ls(*av);
	//		printf("\n");
		}
}
/*
 * list files in directory called dirname
 *
 */
void do_ls(char dirname[])
{
	DIR *dir_ptr;
	struct dirent *direntp;
	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1: canot open %s\n",dirname);
	else
	{
		while((direntp = readdir(dir_ptr)) != NULL)
			printf("%s    ",direntp->d_name);
		closedir(dir_ptr);
		printf("\n");
	}
}
