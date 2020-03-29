#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc,char *argv[])
{
    char InFile[80];
    char OutFile[80];
    char key[16];           //密码
    int choose;

    printf("（1）加密or（2）解密？");
    scanf("%d",&choose);
    getchar();
    if(argc!=4)
    {
        printf("被选文件名称：");
        gets(InFile);
        printf("输出文件名称：");
        gets(OutFile);
        printf("请输入密钥：");
        gets(key);
    }
    else
    {
        strcpy(InFile,argv[1]);
        strcpy(key,argv[2]);
        strcpy(OutFile,argv[3]);
    }

    FILE *fp1,*fp2;
	register char ch;
	int j=0;
	int j0=0;
	fp1=fopen(InFile,"r");
	if(fp1==NULL){
		printf("ERROR!\n");
		exit(1);
	}
	fp2=fopen(OutFile,"w");
	if(fp2==NULL){
		printf("ERROR!/n");
		exit(1);
	}
	while(key[++j0]);
	ch=fgetc(fp1);

	if(choose==1)
    {
        while(!feof(fp1))
        {
            if(j0>7)
                j0 = 0;
            ch += key[j0++];
            fputc(ch,fp2);
            ch=fgetc(fp1);
        }
	fclose(fp1);
	fclose(fp2);
    }
    else if(choose==2)
    {
        while(!feof(fp1))
        {
            if(j0 >7)
                j0 = 0;
            ch -= key[j0++];
            fputc(ch,fp2);
            ch=fgetc(fp1);
        }
	fclose(fp1);
	fclose(fp2);
    }

	system("pause");
	return 0;
}



