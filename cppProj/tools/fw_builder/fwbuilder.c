#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//const char *searchSymbol="#define BUILD_VERSION ";

#define VER_FILE	argv[1]//"./ioctl_vendor.h"
#define BUILD_SYMBOL	argv[2]
#define BUFFER_SIZE	256
int 
main(int argc, char *argv[])
{
	int line=-1;
	int result=0;	
	int cmpLength;
	FILE *fptr;
	char str[BUFFER_SIZE];
	char *cpyStr;
	
	
	if(argc<3)
	{
		printf("Usage : %s file symbol!!\n", argv[0]);
		return -1;
	}
	
	
	fptr = fopen(VER_FILE, "r+");
	cmpLength = strlen(BUILD_SYMBOL);
	printf("cmpSymbol length = %d\n", cmpLength);	
	cpyStr = (char *)malloc(cmpLength);
	if(fptr == NULL)
	{
		printf("file %c%s%c can not open!!\n", '"', VER_FILE, '"');
		result = -1;
	}	
	else
	{
		fseek(fptr , 0, SEEK_SET);
		while(!feof(fptr))
		{
			fgets(str, BUFFER_SIZE, fptr);
			line++;
			strncpy(cpyStr, str, cmpLength);
			if(0==strcmp(cpyStr, BUILD_SYMBOL))
			{//found symbol				
				char build[5];
				int i;
				//seek back to build number	//4 digital + __EOS
				fseek(fptr, -5, SEEK_CUR);
				
				for(i=0;i<4;i++)
				{
					build[i]=fgetc(fptr);
				}
				build[4]=0;
				result = atoi(build) + 1;
				sprintf(build, "%4d\n", result);
				//printf("array=%s, int = %d\n", build, result);
				printf("new build = %s", build);
				//update back to file				
				fseek(fptr, -4, SEEK_CUR);
				fwrite(build, 1, 4, fptr);
				break;
			}
		}
		fclose(fptr);
	}		
	free(cpyStr);
	return result;
}
