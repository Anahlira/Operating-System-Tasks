#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(void){
	int a[2],b[2],c[2];
	if(pipe(a) == -1) err(2,"can't pipe");
	
	pid_t p = fork();
	if(p == -1) err(1,"can't fork");
	
	if(p == 0){
		close(a[0]);
		dup2(a[1],1);
		if(execlp("cut","cut", "-d",":","-f","7","/etc/passwd",NULL) == -1) err(3,"can't exec");
	}
	close(a[1]);
	
	dup2(a[0],0);
	if(pipe(b) == -1) err(2,"can't pipe");
	p = fork();
	if(p == -1) err(1,"can't fork");
	
	if(p==0){
		close(b[0]);
		dup2(b[1],1);
		if(execlp("sort","sort",NULL) == -1) err(3,"can't exec");
	}
	close(b[1]);
	close(a[0]); // no more need
	dup2(b[0],0);

	if(pipe(c) == -1) err(2,"can't pipe");
	p = fork();
	if(p == -1) err(1,"can't fork");
	if(p == 0){
		close(c[0]);
		dup2(c[1],1);
		if(execlp("uniq","uniq","-c",NULL) == -1) err(3,"can't exec");
	}
	close(c[1]);
	close(b[0]);
	dup2(c[0],0);

	if(execlp("sort","sort","-n",NULL) == -1) err(3,"can't exec");
}
