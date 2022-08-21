#include<stdlib.h>
#include<stdint.h>
#include<err.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

comp(const void* first, const void* second){
uint32_t x = *(const uint32_t *)first;
uint32_t y = *(const uint32_t *)second;
return x - y;
}

int main(const int argc,const char * argv[]){
	if(argc != 1){
		errx(1,"wring input");
	}
	int fd;
	fd=open(argv[1],O_RDWR);
	if(fd==-1){
		err(2,"err opening file");
	}

	struct stat st;
	fstat(fd,&st);
	if(st.st_size > (long int)(100000000 * sizeof(uint32_t))){
		errx(3,"File is too big");
	}
	if(st.st_size % sizeof(uint32_t) != 0){
		errx(3,"file is wrong format");
	}
	
	//400 000 000 maximum big file, so
	//256*1 000 000 is 256 000 000, so
	//it needs to be in two files
	

	//temporary files used for sorting	
	int fd2,fd3;
	char file1[] = "/tmp/fisthalfXXXXXX";
	char file2[] = "/tmp/secondhalfXXXXXX";

	fd2=mkstemp(file1);
	if(fd2 == -1){
		err(4,"Coudn't create temporary file");
	}
	if(unlink(file1) == -1){
		err(4,"can't delete temp file");
	}
	fd3=mkstemp(file2);
        if(fd3 == -1){
                err(4,"Coudn't create temporary file");
        }
	if(unlink(file2) == -1){
                err(4,"can't delete temp file");
        }
	//calculating the half elements
	int half[2];
	half[0] = (st.st_size / sizeof(uint32_t)) / 2;
	half[1] = (st.st_size / sizeof(uint32_t)) - half[0];
	
	for(int i=0;i<2;i++){
		uint32_t *buf = malloc(half[i]*sizeof(uint32_t));
		if(buff == NULL){
			err(5,"can't alocate memory");
		}
		
		//nz if work
		if(read(fd,&buf,sizeof(uint32_t)*half[i]) == sizeof(uint32_t)*half[i]){
			err(6,"cant read file");
		}

		qsort(&buf,half[i],comp);
		
		for(int j=0; j < half[i]; j++){
                        if(write(fd3,&buf[j],sizeof(uint32_t)) == sizeof(uint32_t)){
                                err(6,"cant write file");
                        }
                }

		free(buf);
	}

	//furst to return the offset to the begining
	int i=0,j=0;
        if(lseek(fd3,j,SEEK_SET) == -1){
     	   err(8,"Can't set offset.");
        }

    	//now is merge sort time
	uint32_t A,B;
	while((i < half[0]) && (j < half[1])){
	if(lseek(fd2,i,SEEK_SET) == -1){
            err(8,"Can't set offset.");
        }
        if(lseek(fd3,j,SEEK_SET) == -1){
            err(8,"Can't set offset.");
        }
        if(read(fd2,&A,sizeof(uint32_t)) == -1){
            err(6,"Couldn't read file.");
        }
        if(read(fd3,&B,sizeof(uint32_t)) == -1){
            err(6,"Couldn't read file.");
        }

        if(A<=B){
            if(write(fd,&A,sizeof(uint32_t)) == -1){
                err(7,"Couldn't read file.");
            }
            i++;
        }
        else{
            if(write(fd,&B,sizeof(uint32_t)) == -1){
                err(7,"Couldn't read file.");
            }
            j++;
        }
	}
	
	while(i<half[0]){
	if(write(fd1,&A,sizeof(uint32_t)) == -1){
            err(7,"Couldn't read file.");
        }
        i++;
	}
	while(j<half[1]){
	if(write(fd1,&B,sizeof(uint32_t)) == -1){
            err(7,"Couldn't read file.");
        }
        j++;
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
