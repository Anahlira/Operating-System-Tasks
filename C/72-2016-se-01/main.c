#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(const int argc,const char * argv[]){
	if(argc != 2){
		errx(1,"invalid input");
	}
	
	//cat "file" | sort
	//		a[1]   a[0]
	//да сложи на 1 | да чете

	int pf[2];
	
	//create pipe
	if(pipe(pf) == -1) err(2,"Could not create pipe");

	//need child
	const pid_t child_pid = fork();
	if (child_pid == -1) err(3,"Could not fork");

	//work child!!!
	if(child_pid == 0){ //child are always = 0
		close(pf[0]);
		dup2(pf[1],1); // we want the thing cat returns, to put it in the pipe
		if(execlp("cat", "cat", argv[1] ,NULL) == -1) err(4,"Could not exec");
		//execlp nothing happes after
	}
	close(pf[1]);

	dup2(pf[0],0);
	if(execlp("sort", "sort",NULL) == -1) err(4,"Could not exec");

	//no need for exit
}
