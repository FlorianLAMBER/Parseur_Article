#include <string>
#include <string.h>
#include <iostream>
#include "folder_info.hpp"
#include "folder_info.cpp"



#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <malloc.h>
#include <sys/wait.h>

int Executer(char* pdf,char* txt){
    int pid;
    int res;
    int ret;
    int verification_commande;
    pid=fork();
    if (pid==0){
        verification_commande=execlp("pdftotext","pdftotext","-bbox-layout",pdf,txt,NULL);
        if (verification_commande == -1) {
            exit(254);
        }
        exit(0);
    }
    else if (pid>0){
        res=wait(&ret);
        return WEXITSTATUS(ret);
    }
    else{
        return 255;
    }
}

int main()
{
	int xMin;
	int xMin2;
	int compteurWord=0;
	std::string essai;
	char* essai2;
	int savin;
	FILE * pFile;
	savin=dup(0);
	folder_info* test = new folder_info("./test");
	std::cout << "test : " << test->get_pdfFolder() << std::endl;
	test->update_pdfList();
	std::vector<std::string> pdf_vect = test->get_pdfList();
	for (auto it = pdf_vect.begin(); it != pdf_vect.end(); it++)    
	{
		essai="./test/"+*it;
		essai2=strdup(essai.data());
		essai2[essai.size()-3]='t';
		essai2[essai.size()-2]='x';
		essai2[essai.size()-1]='t';
		Executer(essai.data(),essai2);
		int t,s,cpt;
		std::cout<<essai2<<std::endl;
		pFile=fopen(essai2,"r");
		char ligne[255];
		char* ligne2;
		char* ligne3;
		char ligne4[255];
		cpt = 1;
		fgets(ligne,255,pFile);
		ligne2=strtok(ligne," \n\t");
		while (cpt == 1){
			if (strcmp(ligne2,"<block")==0){
				for (int i=0 ; i<2 ; i++){
					ligne2=strtok(NULL," \t\n");
				}
				ligne3=strtok(ligne2,"=\"");
				ligne3=strtok(NULL,"=\"");
				if (atol(ligne3)< 60.0){
					fgets(ligne,255,pFile);
					ligne2=strtok(ligne," \n\t");
				}
				else{
					fgets(ligne,255,pFile);
					fgets(ligne,255,pFile);
					ligne2=strtok(ligne," \n\t");
					cpt=0;
				}
			}
			else{
				fgets(ligne,255,pFile);
				ligne2=strtok(ligne," \n\t");
			}
		}
		while (cpt==0){
			if (strcmp(ligne2,"<word")==0){
				for (int i=0 ; i<4 ; i++){
					ligne2=strtok(NULL," \t\n");
				}
				ligne3=strtok(ligne2," <>");
				ligne3=strtok(NULL," <>");
				printf("%s ",ligne3);
				fgets(ligne,255,pFile);
				ligne2=strtok(ligne," \t\n");

			}
			else if (strcmp(ligne2,"</line>")==0 || strcmp(ligne2,"<line")==0 ){
				fgets(ligne,255,pFile);
				ligne2=strtok(ligne," \t\n");
			}
			else{
				cpt=1;
			}
		}
		std::cout<<std::endl;
		while (cpt == 1){
			if (strcmp(ligne2,"<block")==0){
				fgets(ligne,255,pFile);
				fgets(ligne,255,pFile);
				strcpy(ligne4,ligne);
				ligne2=strtok(ligne4," \t\n");
				ligne2=strtok(NULL," \t\n");
				ligne3=strtok(ligne2,"=\"");
				ligne3=strtok(NULL,"=\"");
				xMin=atoi(ligne3);
				if(xMin<300){
					ligne2=strtok(ligne," \t\n");
					ligne2=strtok(NULL," \t\n");
					ligne2=strtok(NULL," \t\n");
					ligne2=strtok(NULL," \t\n");
					ligne2=strtok(NULL," \t\n");
					ligne3=strtok(ligne2," <>");
					ligne3=strtok(NULL," <>");
					if (strcmp(ligne3,"Abstract") == 0 || strcmp(ligne3,"Abstract—We") == 0 || strcmp(ligne3,"This") == 0 || strcmp(ligne3,"As") == 0|| strcmp(ligne3,"In") == 0|| strcmp(ligne3,"We") == 0){
						cpt=0;
						if (strcmp(ligne3,"Abstract") != 0){
							printf("%s ",ligne3);
						}
					}
				}
			}
			else{
				fgets(ligne,255,pFile);
				ligne2=strtok(ligne," \t\n");
				
			}
		}
		cpt=1;
		fgets(ligne,255,pFile);
		ligne2=strtok(ligne," \t\n");
		while(strcmp(ligne2,"<word")!=0){
			fgets(ligne,255,pFile);
			ligne2=strtok(ligne," \t\n");
		}
		ligne2=strtok(NULL," \t\n");
		ligne3=strtok(ligne2,"=\"");
		ligne3=strtok(NULL,"=\"");
		xMin2=atoi(ligne3);
		while (xMin2 > (xMin+70)){
			if (strcmp(ligne2,"<line")==0 || strcmp(ligne2,"<word")==0){
				ligne2=strtok(NULL," \t\n");
				ligne3=strtok(ligne2,"=\"");
				ligne3=strtok(NULL,"=\"");
				xMin2=atoi(ligne3);
			}
			else{
				fgets(ligne,255,pFile);
				ligne2=strtok(ligne," \t\n");
			}
		}
		ligne2=strtok(ligne," \t\n");
		while(cpt==1){
			if (strcmp(ligne2,"<word")==0){
			for (int i=0 ; i<4 ; i++){
				ligne2=strtok(NULL," \t\n");
			}
			ligne3=strtok(ligne2," <>");
			ligne3=strtok(NULL," <>");
			printf("%s ",ligne3);
			fgets(ligne,255,pFile);
			ligne2=strtok(ligne," \t\n");
			}
			else if (strcmp(ligne2,"</line>")==0 || strcmp(ligne2,"<line")==0 ){
				fgets(ligne,255,pFile);
				ligne2=strtok(ligne," \t\n");
			}
			else{
				cpt=0;
			}
		}
		std::cout<<std::endl;
		std::cout<<std::endl;
	}
	return 0;
}
