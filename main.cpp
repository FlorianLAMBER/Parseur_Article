#include <string>
#include <iostream>
#include "utils/folder_info.hpp"
#include "utils/appInfo.hpp"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int ExecuterPDF(const char* pdf, char* txt){
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

int main(int argc,char** argv)
{
	int hasOutput = 0;
	int hasInput = 0;
	std::string inputFolder, outputFolder;
	#ifdef DEBUG
		std::cout << "argc = " << argc << std::endl;
	#endif 
	if (argc >= 1 )
	{
		for(int i=1; i < argc; i++)
		{
			std::string currentArg = std::string(argv[i]);
			#ifdef DEBUG
				std::cout << "\targv[" << i <<"] = " << argv[i] << std::endl;
			#endif
			if(currentArg == "-v" || currentArg == "--version")
			{
				appInfo::print_release();
				continue;
			}
			if(currentArg == "-h" || currentArg == "--help")
			{
				appInfo::print_help();
				continue;
			}
			if(currentArg == "-o" || currentArg == "--output")
			{
				outputFolder = argv[i+1];
				hasOutput = 1;
				#ifdef DEBUG
					std::cout << "Your output folder is " << outputFolder << std::endl;
				#endif
				i++;
				continue;
			}

			if(currentArg == "-i" || currentArg == "--input")
			{
				inputFolder = argv[i+1];
				hasInput = 1;
				#ifdef DEBUG
					std::cout << "Your input folder is " << inputFolder << std::endl;
				#endif
				i++;
				continue;
			}

			std::cout << "Unknow argument " << argv[i] << ", see usage below for more detail." << std::endl;
			appInfo::print_usage();
			return 0;
		}
		if(!hasInput)
		{
			std::cout << "\033[1;31mArgument -i is missing !\033[0m" << std::endl << "See usage below for more detail." << std::endl;
			appInfo::print_usage();
			return 0;
		}
	} else {
		appInfo::print_usage();
		return 0;
	}


	if (argc > 1 ){
		int t,s,cpt;
		char ligne[255];
		char* ligne2;
		char* ligne3;
		char ligne4[255];
		int xMin;
		int xMin2;
		int compteurWord=0;
		std::string essai;
		char* essai2;
		int savin;
		FILE * pFile;
		FILE * pFile2;
		folder_info* test ;
		if (hasInput){
			test = new folder_info(inputFolder);
		}
		else if (hasInput && hasOutput){
			test = new folder_info(inputFolder, outputFolder);
		}
		test->update_pdfList();
		std::vector<std::string> pdf_vect = test->get_pdfList();
		for (auto it = pdf_vect.begin(); it != pdf_vect.end(); it++)    
		{
			essai=test->get_outputFolder()+*it;
			mkdir(test->get_outputFolder().data(),S_IRWXU |S_IRGRP | S_IXGRP |S_IROTH | S_IXOTH );
			essai2=strdup(essai.data());
			essai2[essai.size()-3]='t';
			essai2[essai.size()-2]='x';
			essai2[essai.size()-1]='t';
			essai=test->get_pdfFolder()+*it;
			char* appTxt = "./application.txt";
			ExecuterPDF(essai.data(), appTxt);
			pFile=fopen("./application.txt","r");
			pFile2=fopen(essai2,"w+a");
			cpt = 1;
			fgets(ligne,255,pFile);
			ligne2=strtok(ligne," \n\t");
			fputs("Titre du fichier : ",pFile2);
			essai=*it;
			fputs(essai.data(),pFile2);
			fputs("\n",pFile2);
			fputs("Titre :",pFile2);
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
					fputs(ligne3,pFile2);
					fputs(" ",pFile2);
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
			fputs("\n",pFile2);
			fputs("Abstract :",pFile2);
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
								fputs(ligne3,pFile2);
								fputs(" ",pFile2);
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
			strcpy(ligne4,ligne);
			ligne2=strtok(ligne4," \t\n");
			while(strcmp(ligne2,"<word")!=0){
				fgets(ligne,255,pFile);
				strcpy(ligne4,ligne);
				ligne2=strtok(ligne4," \t\n");
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
					strcpy(ligne4,ligne);
					ligne2=strtok(ligne4," \t\n");
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
				fputs(ligne3,pFile2);
				fputs(" ",pFile2);
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
			fputs("\n",pFile2);
		}
		remove("./application.txt");
	}
	else{
		std::cout<<"Erreur le nombre argument donné n'est pas sufficent. Il faut donner le dossier ou on va chercher les pdf .On peut rajouter le dossier de sortie des fichiers txt"<<std::endl;
	}
	return 0;
}

