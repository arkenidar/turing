#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char* get_line(FILE *fp){
	int str_len=0;
	char*str=malloc((str_len+1)*sizeof(char));str[str_len]='\0';
	while(1){
		char ch=fgetc(fp);
		if(feof(fp)){free(str); str=NULL; break;}
		if(ch=='\n'||ch=='\0')break;
		str[str_len]=ch; str_len++;
		str=realloc(str,(str_len+1)*sizeof(char));str[str_len]='\0';
	}
	return str;
}
struct program{
	int* pa;
	int lc;
};
struct program parse_p(int argc, char* argv[]){
	struct program p; p.pa=0; p.lc=0;
        char* filename; int free_filename=0;
	if(argc==2)
        {
            filename=argv[1];
        }else{
            printf("Call with a filename FILE as argument.\n");
            printf("If FILE is '-', reads standard input.\n");
            printf("If FILE is '?', filename is asked.\n");
            printf("Current filename is '?', filename is asked: ");
            printf("Filename? ");filename=get_line(stdin); free_filename=1;
        }

        FILE* fp;
	if((strcmp(filename,"-")==0)){
            printf("Reading from stdin.\n");
            fp=stdin;
        }else{
            printf("Reading from file '%s'.\n",filename);
            fp=fopen(filename,"r");
        }
        if(!fp){printf("Error: file not opened!\n");exit(2);}
        if(free_filename){ if(filename)free(filename);filename=0;}
                
	int pa[4]; char* str;
	while(1){
		str=get_line(fp); if(!str) break;
		int r=sscanf(str,"%d %d %d %d\n",&pa[0],&pa[1],&pa[2],&pa[3]);
		printf("<< %s\n", str); if(str)free(str);
		if(r!=4){
			if(r==-1){printf("Found an end in input.\n");break;}
			else{
				printf("Bad format in line %d.\n",p.lc+1);
				free(p.pa); p.pa=0; p.lc=0;
				break;
			}
		}
		p.pa=p.pa?realloc(p.pa,(p.lc+1)*4*sizeof(int)):malloc(4*sizeof(int));
		for(int i=0;i<4;i++)p.pa[p.lc*4+i]=pa[i];
		p.lc++;
	}
	printf("Line count:%d\n",p.lc);
	if(fp!=stdin)fclose(fp);
	return p;
}
void set_bit(int* v,int b,int p){if(b)*v|=1<<p;else*v&=~(1<<p);}
int scan_bit(){
	printf("<< "); char* str;
	while(1){
		str=get_line(stdin); if(!str)continue;
		if(strcmp(str,"0")==0){ free(str); return 0;}
		if(strcmp(str,"1")==0){ free(str); return 1;}
                if(strcmp(str,"")==0){ free(str); exit(10);}
		printf("Wrong input: use 0 or 1!\n");
		printf("<< ");
	}
}
void print_bit(int bit){printf(">> %d\n",bit);}
int main(int argc, char* argv[]){
	struct program p=parse_p(argc,argv); // deallocate as necessary
	printf("The program was parsed.\n");
	if(p.lc<1){printf("No lines in program. Program is not executed.\n"); return 4;}
	int m[22+0]={0}; // minimium memory size of 22 bits (22 words 1 bit wide)
	int c=0,x,i,dst,src;m[1]=1;int px=0,pdst=0;
	while(1){
		i=c<<2;dst=p.pa[i|0];src=p.pa[i|1];
		x=src!=3?((src==0||src==1)?src:m[src]):scan_bit();
		if(dst==3)print_bit(x);else if(dst!=0&&dst!=1)m[dst]=x;
		c=p.pa[i|2|m[2]];
		if(src==4)break;//stop
		if(dst==4){//program can be modified
			for(int i=0;i<8;i++)set_bit(&px,m[5+0+i],i);
			for(int i=0;i<8;i++)set_bit(&pdst,m[5+8+i],i);
			p.pa[pdst]=px;}
	}
	return 0;
}
