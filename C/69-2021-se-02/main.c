#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

uint16_t encode(const uint8_t c);

int main(const int argc,const char * argv[]){
	if(argc != 3){
		errx(1,"invalid input");
	}
	
	//open files--------------------------------------------
	int fd1,fd2;
	if((fd1 = open(argv[1],O_RDONLY)) == -1){
		err(2,"err opening file");
	}
	if((fd2 = open(argv[2],O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		err(2,"err opening file");
	}

	//arrays------------------------------------------------
	
	uint8_t valid[65536];
	for(int j = 0; j < 65536; j++){
		valid[j]=0; //за всеки случай
	}
	for(int i = 0; i < 256; i++){
		valid[encode(i)] = i;
	}

	//read file--------------------------------------------
	
	uint16_t c;
	int rd;
	while((rd=read(fd1,&c,sizeof(c))) == sizeof(c)){
		if(valid[c] != 0){
			uint8_t buf = valid[c];
			if(write(fd2,&buf,sizeof(buf)) != sizeof(buf)) err(4,"err writing");
		}
	}
	if(rd != 0) err(3,"err while reading");

	//close files------------------------------------------
	close(fd1);
	close(fd2);
	exit(0);
}

uint16_t encode(const uint8_t c){
	uint8_t m_bit[8]={
	0x80,
	0x40,
	0x20,
	0x10,
	0x08,
	0x04,
	0x02,
	0x01
	};

	uint16_t m_true[8]={
	0x8000,
	0x2000,
	0x0800,
	0x0200,
	0x0080,
	0x0020,
	0x0008,
	0x0002
	};

	uint16_t m_false[8]={
	0x4000,
	0x1000,
	0x0400,
	0x0100,
	0x0040,
	0x0010,
	0x0004,
	0x0001
	};
	
	uint16_t buf = 0;
	for(int i = 0; i < 8; i++){
		if((c & m_bit[i]) != 0){
			buf = buf | m_true[i];
		}
		else buf = buf | m_false[i];
	}

	return buf;
}

