#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>

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

	//read bits--------------------------------------------
		
		//0000 0001 - 0000 0000 0000 0010 -0x0002
		//0000 0010 - 0000 0000 0000 1000 -0x0008
		//...
		//1000 0000 - 1000 0000 0000 0000 -0x8000
		//
		//0000 0000 - 0000 0000 0000 0001 -0x0001
		//			- 0000 0000 0000 0100 -0x0004
		//...

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
	
	uint8_t c;
	int rd;
	while((rd=read(fd1,&c,sizeof(c))) == sizeof(c))	{
		uint16_t buf = 0;
		for(int i = 0; i < 8; i++){
			if((c & m_bit[i]) != 0){
				buf = buf | m_true[i];
			}
			else buf = buf | m_false[i];
		}

		if(write() != sizeof(buf)) err(4,"err writng");
	}
	if(rd != 0) err(3,"err reading file");

	//close files------------------------------------------
	close(fd1);
	close(fd2);
	exit(0);
}
