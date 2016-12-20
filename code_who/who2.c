/* who1.c —— a first version of the who program
 *           open,read UTMP file,and show results
*/ 



#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST	/* include remote machine on output*/
void show_info(struct utmp *utbufp);
void showTime(long timevalue);
int main()
{
	struct utmp current_record;	/* read info into here */	
	int utmpfd;					/* read from this descriptor */
	int reclen = sizeof(current_record);

	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE);   /* UTMP_FILE is in utmp.h */
		exit(1);
	}
	while(read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;   /* went ok*/
}
/* show info()
 * displays contents of utmp struct in human readable form
 * *note* these sizes should not be hardwired
 */
void show_info(struct utmp *utbufp)
{
	if(utbufp->ut_type != USER_PROCESS)// remove blamk users
		return;
	printf("%-11.11s", utbufp->ut_user);// the logname
	printf(" ");
	printf("%-11.11s", utbufp->ut_line);// the tty
	printf(" ");
//	printf("% 10ld", utbufp->ut_time);// login time
//	printf(" ");
	showTime(utbufp->ut_time);
#ifdef SHOWHOST
	if(utbufp->ut_host[0] != '\0')
		printf("(%s)", utbufp->ut_host);//the host
#endif
	printf("\n");//new line

}
/* displays time in a format fit for human consumption
 * uses ctimr() to build a string then picks parts out of it
 * Note: %24.24s printfs a strings 24 chars wide and LIMITS
 * it to 24chars
 */
void showTime(long timevalue)
{
	char *cp;		//to hold address of time
	cp = ctime(&timevalue);/*convert time to string 
							string looks like
							Mon Feb 4 00:12:23 EST 1991
							*/
	printf("%4.4s", cp+20);//pick 24 chars from pos 0
	printf(" ");
	printf("%20.20s", cp);//pick 24 chars from pos 0


}
