#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

int main(const int argc,const char * argv[]){
	//open file-----------------------------------------
	int fd;
	if((fd = open("foo.log", O_CREAT| O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		err(1,"can't open file");
	}

	//time----------------------------------------------

	struct timeval tv; //second and microseconds
	if(gettimeofday(&tv, NULL) == -1) err(2,"err geting microseconds");

	time_t t = time(NULL);
	struct tm * lt = localtime(&t);
	
	if(lt == NULL) err(2,"err geting date");

	char s[21];
	if(strftime(s,21,"%Y-%m-%d %H:%M:%S.",lt) == 21)	err(2,"err geting date");

	//user----------------------------------------------
	
	uid_t usrID=getuid();
	struct passwd * usr = getpwuid(usrID); //.pw_name
	if(usr == NULL) err(3,"err geting user data");
	
	//--------------------------------------------------
	//write all data------------------------------------
	
	pid_t c = fork();
	if(c == -1) err(4,"err geting user data");
	if(c == 0){
	dup2(fd,1);
	setbuf(stdout,NULL);
	printf("time: %s%ld %s",s,tv.tv_usec,usr->pw_name);

	for(int i=0; i< argc; i++){
		printf(" %s",argv[i]);
	}
	printf("\n");
		exit(0);
	}
	
	close(fd);

	//block user and kill all prosses-----------------------
	//	passwd -l usr->pw_name
		
	pid_t p = fork();
	if(p == -1) err(4,"err geting user data");
	
	if(p == 0){
		//execlp("passwd", "passwd","-l",usr->pw_name, NULL);
		exit(0);
	}

	//	ps h -U usr->pw_name -o pid | xargs -n echo "kill"
	//							a[1] a[0]
	
	int pf[2];
	if(pipe(pf) == -1) err(5, "err creating pipe");

	p = fork();
	if(p == -1) err(4,"err geting user data");

	if(p == 0){
		close(pf[0]);
		dup2(pf[1],1);
		if(execlp("ps","ps","h","-U",usr->pw_name,"-o","pid",NULL) == -1){
		err(6,"err in exec");
	}

	}
	close(pf[1]);

	dup2(pf[0],0);
	if(execlp("xargs","xargs","-n","1","echo", "kill",NULL) == -1){
		err(6,"err in exec");
	}	
}
