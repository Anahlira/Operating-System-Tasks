#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

void s(const int fd);
void w(const int fd);

int main(const int argc,const char * argv[]){
	if(argc != 3 || strlen(argv[1]) != 1 || strlen(argv[2]) != 1)
	   	errx(1,"wrong input");

	int a[2],b[2];
	if(pipe(a) == -1) err(1,"can't create pipe");
	if(pipe(b) == -1) err(1,"can't create pipe");
	
	pid_t p = fork();
	if(p == -1) err(2,"Can't create child");
	
	if(p==0){
		close(b[1]);
		close(a[0]);
		for(int i = 0; i < atoi(argv[1]); i++){
			w(b[0]);
			if(write(1,"DONG\n",5) != 5)
				err(2,"err while writing to output");
			s(a[1]);
		}	
		exit(0);
	}
	close(a[1]);
	close(b[0]);

	for(int i = 0; i < atoi(argv[1]); i++){
		if(write(1,"DING\n",5) != 5)
			err(2,"err while writing");
		s(b[1]);
		w(a[0]);
		if(sleep(atoi(argv[2]))!=0) err(4,"err while sleeping");
	}

	// parent	child
	//	a.wait	b.wait
	// ding		dong
	// b.signal	a.signal
	// a 0 in	b 0 in

	exit(0);
}

void s(const int fd){//signal
	if(write(fd,"o",1) != 1)
		err(2,"err while writing");
}
void w(const int fd){//wait
	char c;
	if(read(fd,&c,1) != 1)
		err(3,"err while reading");
}


	//works but is wrong	
	/*int status;
	for(int i = 0; i < atoi(argv[1]); i++){
		if(write(0,"DING\n",5) != 5) err(3,"err writing");

		pid_t p = fork();
		if(p == -1) err(2,"Can't create child");

		if(p == 0){
			if(write(0,"DONG\n",5) != 5) err(3,"err writing");
			exit(0);
		}
		wait(&status);
		sleep(atoi(argv[2]));
	}*/

