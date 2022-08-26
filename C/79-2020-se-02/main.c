//ако съм разбрала правилно, единият файл е с еди-какви си байтове, който са xor с 0х20
//целта е да се вземе какъв е бил реалния байт, преди xor и
//ако 0х55 не ни трябва
//а ако е 0х7D май също трябва да се игнорира

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

int main(const int argc,const char * argv[]){
	if(argc != 3) errx(1,"wring input");
	
	//read from file using shell--------------------------------------

	int pf[2];
	if(pipe(pf) == -1) err(2,"err creating pipe");
	
	pid_t ch = fork();
	if(ch == -1) err(3,"err forking");

	if(ch == 0){
		close(pf[0]);
		if(dup2(pf[1],1) == -1) err(3,"err dup2");
		if(execlp("cat", "cat", argv[1],NULL) == -1) err(3, "err exec");
	}
	close(pf[1]);

	//read from pipe-------------------------------------------------

	int fd=open(argv[2], O_CREAT | O_TRUNC | O_WRONLY , S_IRUSR | S_IWUSR | S_IRGRP );
	if(fd == -1) err(5, "can't create file");

	uint8_t buf;
	uint8_t decode = 0x20;
	uint8_t sym;
	int rd;
	while((rd=read(pf[0],&buf,sizeof(buf))) == sizeof(buf)){
		sym = buf ^ decode;
		if(sym == 0x55 || buf == 0x7D) continue;
		if(sym != 0xFF && sym != 0x7D && sym != 0x00) errx(1,"file is wrong format");

		if(write(fd,&sym,sizeof(sym)) != sizeof(sym) )err(6,"err writing");
	}
	if(rd != 0) err(4,"err reading");

	close(fd);
	exit(0);
}

//child
//cat = out
//child uses [1]
//
