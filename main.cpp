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
#include <filesystem>

/*Fonction permettant d'exécuter la fonction pdftotext en c++*/
int ExecuterPDF(std::string pdf, std::string txt){
    std::cout << pdf << std::endl;
    int pid;
    int res;
    int ret;
    int verification_commande;
    std::string pdfChar = (  pdf);
    std::cout << pdfChar << std::endl;
    pid=fork();
    if (pid==0){
        std::string cmd = "pdftotext -bbox-layout \"" + pdfChar + "\" " + txt;
        std::cout << cmd << std::endl;
        verification_commande= std::system((cmd).data());
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

// accepts a string, checks for a number, if true, returns 1
bool isNumber(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](const char& ch) { return isdigit(ch); });
}

// accepts a string with double quotes, extracts a number between quotes
float extractFromQuotes(std::string str) {
    std::string tempStr = str;

    auto pos = tempStr.find_last_of('"');
    if (pos != std::string::npos)
        tempStr.erase(pos, std::numeric_limits<std::string::size_type>::max());
        
    pos = tempStr.find_last_of('"');
    if (pos != std::string::npos)
        tempStr.erase(0, pos+1);

    return std::stof(tempStr);
}


int main(int argc,char** argv)
{
    int hasOutput = 0;
    int hasInput = 0;
    int isTxt = 0;
    int isXml = 0;
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
            if(currentArg == "-t" || currentArg == "--txt")
            {
                if(isXml)
                {
                    std::cout << "\033[1;31mCan't take -x and -t !\033[0m" << std::endl;
                    appInfo::print_usage();
                    return 0;
                }
                isTxt = 1;
                continue;
            }
            if(currentArg == "-x" || currentArg == "--xml")
            {
                if(isTxt)
                {
                    std::cout << "\033[1;31mCan't take -x and -t !\033[0m" << std::endl;
                    appInfo::print_usage();
                    return 0;
                }
                isXml = 1;
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
        if(!isTxt && !isXml)
        {
            std::cout << "\033[1;31mArgument -x or -t is missing !\033[0m" << std::endl << "See usage below for more detail." << std::endl;
            appInfo::print_usage();
            return 0;
        }
    } else {
        appInfo::print_usage();
        return 0;
    }
    
    
    if (argc > 1 ){
        int boolean;									//Servira a rentrer dans les whiles
        char Ligne[255]; 								//Permettra de récupérer la ligne du fichier txt ou on travaille
        char* RecupereDonnerLigne;						//Permettera de récupérer une partie de la ligne séparé par des espace , \n et \t
        char* RecuperationPartieDonnerLigne;			//Permettera de récupérer la partie séparé et de le séparé encore par des =,<,>
        char CopieLigneQuOnEtudie[255];					//Permettera de faire la copie de la variable Ligne
        double xMin;									//Permettera de récupérer la position d'un éléments
        double xMin2;									//Permettera de récupérer la position d'un éléments
        std::string fichierpdf;							//Permettera de savoir le nom du fichier pdf mais aussi ou il se situe
        char* fichierTxt;								//Permettera de pointer ou le fichier créer va se trouver et sur quel format
        std::string appTxt = "./application.txt";				//Sa sera le fichier txt qui recevra tous les resultats de la commande pdftotext
        FILE * pFile;									//Pour lire le fichier ./application.txt
        FILE * pFile2;									//Pour écrire dans le fichier txt
        folder_info* workspaceInfo;						//Permet d'avoir tous les fichier pdf dans une classe .
        
        if (hasInput && !hasOutput)
        {
            workspaceInfo = new folder_info(inputFolder);
        }
        else if (hasInput && hasOutput){
            workspaceInfo = new folder_info(inputFolder, outputFolder);
        }
        
        workspaceInfo->update_pdfList();
        std::vector<std::string> pdf_vect = workspaceInfo->get_pdfList();
        for (auto it = pdf_vect.begin(); it != pdf_vect.end(); it++)
        {
            /*Récupération nom du fichier et du dossier .
             Creation du workspaceInfo de sortie .
             Création application.txt et cration du fichier txt liéer au pdf*/
            fichierpdf=workspaceInfo->get_outputFolder()+*it;
            if (!std::filesystem::exists(workspaceInfo->get_outputFolder())){
                mkdir(workspaceInfo->get_outputFolder().data(),S_IRWXU |S_IRGRP | S_IXGRP |S_IROTH | S_IXOTH );//Création du workspaceInfo
            }
            while(fichierpdf.find(" ") != std::string::npos){
                fichierpdf=fichierpdf.replace(fichierpdf.find(" "),1,"_");
            }
            
            fichierTxt=strdup(fichierpdf.data());
            if(isTxt)
            {
                fichierTxt[fichierpdf.size()-3]='t';
                fichierTxt[fichierpdf.size()-2]='x';
                fichierTxt[fichierpdf.size()-1]='t';
            }
            if(isXml)
            {
                fichierTxt[fichierpdf.size()-3]='x';
                fichierTxt[fichierpdf.size()-2]='m';
                fichierTxt[fichierpdf.size()-1]='l';
            }
            fichierpdf=workspaceInfo->get_pdfFolder()+*it;
            ExecuterPDF(fichierpdf.data(), appTxt);
            pFile=fopen("./application.txt","r");
            pFile2=fopen(fichierTxt,"w+a");
            
            /*Ecriture du Titre du fichier dans le fichier txt*/
            boolean = 1;
            fgets(Ligne,255,pFile);
            RecupereDonnerLigne=strtok(Ligne," \n\t");
            if(isTxt)
            {
                fputs("Titre du fichier : ",pFile2);
            }
            if(isXml)
            {
                fputs("<article>\n",pFile2);
                fputs("\t<preambule>",pFile2);
            }
            
            fichierpdf=*it;
            while(fichierpdf.find(" ") != std::string::npos){
                fichierpdf=fichierpdf.replace(fichierpdf.find(" "),1,"_");
            }
            fputs(fichierpdf.data(),pFile2);
            // Writre with different format
            if(isTxt)
            {
                fputs("\n",pFile2);
            }
            if(isXml)
            {
                fputs("</preambule>\n",pFile2);
            }
            
            /*Recupération du block ou se trouve le titre du document .*/
            if(isTxt)
            {
                fputs("Titre :",pFile2);
            }
            if(isXml)
            {
                fputs("\t<titre>",pFile2);
            }
            
            while (boolean == 1){
                
                /*Si dans RecupereDonnerLigne il ressemble à <block
                 alors on rentre*/
                if (strcmp(RecupereDonnerLigne,"<block")==0){
                    for (int i=0 ; i<2 ; i++){
                        RecupereDonnerLigne=strtok(NULL," \t\n");
                    }
                    /*Récupération de la position du block .
                     S'y il est trop haut (je vais regarder sa position yMin)
                     je vais dans un autre block*/
                    
                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
                    RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
                    if (atol(RecuperationPartieDonnerLigne)< 60.0){
                        fgets(Ligne,255,pFile);
                        RecupereDonnerLigne=strtok(Ligne," \n\t");
                    }
                    /*Si la position est bonne ,
                     je fais 2 fgets
                     pour accéder directement sur le mot*/
                    else{
                        fgets(Ligne,255,pFile);
                        fgets(Ligne,255,pFile);
                        RecupereDonnerLigne=strtok(Ligne," \n\t");
                        boolean=0;
                    }
                }
                /*Sinon je vais directement sur une autre ligne*/
                else{
                    /*On va vers la prochaine ligne du fichier application.txt*/
                    fgets(Ligne,255,pFile);
                    RecupereDonnerLigne=strtok(Ligne," \n\t");
                }
            }
            /*Ecriture du Titre dans le fichier txt*/
            while (boolean==0){
                /*Si RecupereDonnerLigne est <word alors
                 je vais ecrire le mot dans le fichier txt*/
                if (strcmp(RecupereDonnerLigne,"<word")==0){
                    /*Pour accéder à la partie ou se trouve le mot*/
                    for (int i=0 ; i<4 ; i++){
                        RecupereDonnerLigne=strtok(NULL," \t\n");
                    }
                    /*Récupération su mot dans la partie ou il se trouve*/
                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                    RecuperationPartieDonnerLigne=strtok(NULL," <>");
                    /*Ecriture du mot + d'un espace dan sle fichier txt*/
                    fputs(RecuperationPartieDonnerLigne,pFile2);
                    fputs(" ",pFile2);
                    /*On va vers la prochaine ligne du fichier application.txt*/
                    fgets(Ligne,255,pFile);
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                    
                }
                /*Sinon si c'est </line> ou <line
                 je vais directment passer à la prochaine ligne*/
                
                else if (strcmp(RecupereDonnerLigne,"</line>")==0 || strcmp(RecupereDonnerLigne,"<line")==0 ){
                    /*On va vers la prochaine ligne du fichier application.txt*/
                    fgets(Ligne,255,pFile);
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                }
                /*Si j'arrive à la fin de mon block*/
                else{
                    boolean=1;
                }
            }
            if(isTxt)
            {
                fputs("\n",pFile2);
            }
            if(isXml)
            {
                fputs("</titre>\n",pFile2);
            }
            
            
            
            
            
            /*Recuperation des infos des auteurs : nom, prenom, adresse*///--------------PARTIE AUTEUR
            
            
            
            if(isTxt)
            {
                fputs("Auteur :",pFile2);
            }
            if(isXml)
            {
                fputs("\t<auteur>",pFile2);
            }
            
            
            
            
            
            
            while (boolean == 1){
                
                /*Si RecupereDonnerLigne est <block alors
                 je vais vers le premier mot pour voir sa position.
                 S'y il est à plus de 300 cela veut dire que se mot provient
                 de la colonne qui est à droit du document
                 (S'y il y en a une ) */
                /*Recupération du block ou se trouve Abstract ou //--------------------------PARTIE ABSTRACT
                 We,This,As,In si Abstract n'est pas présent dans le fichier*/
                
                if (strcmp(RecupereDonnerLigne,"<block")==0){
                    
                    
                    /*Je fais 2 fgets pour aller*/
                    
                    fgets(Ligne,255,pFile);
                    fgets(Ligne,255,pFile);
                    /*Je fais copie de la ligne au cas si la position
                     est inférieur à 300*/
                    
                    strcpy(CopieLigneQuOnEtudie,Ligne);
                    RecupereDonnerLigne=strtok(CopieLigneQuOnEtudie," \t\n");
                    RecupereDonnerLigne=strtok(NULL," \t\n");
                    
                    /*J'accède  à la position xMin de mon mot*/
                    
                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
                    RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
                    
                    /*atof transforme contenu d'un char* en double*/
                    
                    xMin=atof(RecuperationPartieDonnerLigne);
                    if(xMin<300.0){
                        RecupereDonnerLigne=strtok(Ligne," \t\n");
                        
                        /*Pour accéder à la partie ou se trouve le mot*/
                        
                        for (int i=0 ; i<4 ; i++){
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                        }
                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                        
                        /*Si le mot c'est Abstract ou We,This,As,In
                         si Abstract n'est pas présent*/
                        
                        
                        std::string trouverAbstract = RecuperationPartieDonnerLigne;
                        if (trouverAbstract.find("Abstract") != std::string::npos ||
                            strcmp(RecuperationPartieDonnerLigne,"ABSTRACT") == 0 ||
                            strcmp(RecuperationPartieDonnerLigne,"This") == 0 ||
                            strcmp(RecuperationPartieDonnerLigne,"As") == 0||
                            strcmp(RecuperationPartieDonnerLigne,"In") == 0||
                            strcmp(RecuperationPartieDonnerLigne,"We") == 0){
                            if(isTxt)
                            {
                                fputs("\nAbstract :",pFile2);
                            }
                            if(isXml)
                            {
                                fputs("</auteur>\n\t<abstract>",pFile2);
                            }
                            boolean=0;
                            if (strcmp(RecuperationPartieDonnerLigne,"Abstract") != 0){
                                fputs(RecuperationPartieDonnerLigne,pFile2);
                                fputs(" ",pFile2);
                            }
                        }
                        else{//----------------------------------------------PARTIE AUTEUR
                            int mail=0;
                            boolean=0;
                            while(RecuperationPartieDonnerLigne[boolean]!='\0'){
                                if(RecuperationPartieDonnerLigne[boolean]=='@'){
                                    mail=1;
                                }
                                boolean++;
                            }
                            boolean=1;
                            if( mail == 0 ){
                                fputs(RecuperationPartieDonnerLigne,pFile2);
                                fputs(" ",pFile2);
                            }
                            while (strcmp(RecupereDonnerLigne,"</block>")!=0){
                                if(strcmp(RecupereDonnerLigne,"<word")==0){
                                    
                                    
                                    /*Pour accéder à la partie ou se trouve le mot*/
                                    
                                    int mail=0;
                                    
                                    for (int i=0 ; i<4 ; i++){
                                        RecupereDonnerLigne=strtok(NULL," \t\n");
                                    }
                                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                    RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                    boolean=0;
                                    while(RecuperationPartieDonnerLigne[boolean]!='\0'){
                                        if(RecuperationPartieDonnerLigne[boolean]=='@'){
                                            mail=1;
                                        }
                                        boolean++;
                                    }
                                    boolean=1;
                                    if( mail != 0 ){
                                        boolean=0;
                                    }
                                    else if(strcmp(RecuperationPartieDonnerLigne,"\\")!=0){
                                        fputs(RecuperationPartieDonnerLigne,pFile2);
                                        fputs(" ",pFile2);
                                    }
                                    fgets(Ligne,255,pFile);
                                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                                    
                                    
                                }
                                else{
                                    fgets(Ligne,255,pFile);
                                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                                    
                                }
                            }
                            boolean=1;
                        }
                    }
                    else{//---------AUTEUR QUAND ON EST A DROITE DE LA PAGE (xmin > 300)
                        
                        RecupereDonnerLigne=strtok(Ligne," \t\n");
                        
                        /*Pour accéder à la partie ou se trouve le mot*/
                        
                        for (int i=0 ; i<4 ; i++){
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                        }
                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                        
                        
                        int mail=0;
                        boolean=0;
                        while(RecuperationPartieDonnerLigne[boolean]!='\0'){
                            if(RecuperationPartieDonnerLigne[boolean]=='@'){
                                mail=1;
                            }
                            boolean++;
                        }
                        boolean=1;
                        if( mail == 0 ){
                            fputs(RecuperationPartieDonnerLigne,pFile2);
                            fputs(" ",pFile2);
                        }
                        
                        
                        while (strcmp(RecupereDonnerLigne,"</block>")!=0){
                            if(strcmp(RecupereDonnerLigne,"<word")==0){
                                
                                
                                /*Pour accéder à la partie ou se trouve le mot*/
                                
                                int mail=0;
                                
                                for (int i=0 ; i<4 ; i++){
                                    RecupereDonnerLigne=strtok(NULL," \t\n");
                                }
                                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                boolean=0;
                                while(RecuperationPartieDonnerLigne[boolean]!='\0'){
                                    if(RecuperationPartieDonnerLigne[boolean]=='@'){
                                        mail=1;
                                    }
                                    boolean++;
                                }
                                boolean=1;
                                if( mail != 0 ){
                                    boolean=0;
                                }
                                else if(strcmp(RecuperationPartieDonnerLigne,"\\")!=0){
                                    fputs(RecuperationPartieDonnerLigne,pFile2);
                                    fputs(" ",pFile2);
                                }
                                fgets(Ligne,255,pFile);
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                
                                
                            }
                            else{
                                fgets(Ligne,255,pFile);
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                
                            }
                        }
                        boolean=1;
                    }
                }
                
                /*Sinon je vais à la prochain ligne de mon application.txt*/
                
                else{
                    fgets(Ligne,255,pFile);
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                    
                }
            }//---------------------------------------------------LA ON PRENDS VRAIMENT ABSTRACT
            boolean=1;
            
            fgets(Ligne,255,pFile);
            /*Dans le cas ou le mot suivant Abstarct,We,In,This,As
             n'est pas à la suite on va vers le prochain <word*/
            
            strcpy(CopieLigneQuOnEtudie,Ligne);
            RecupereDonnerLigne=strtok(CopieLigneQuOnEtudie," \t\n");
            while(strcmp(RecupereDonnerLigne,"<word")!=0){
                fgets(Ligne,255,pFile);
                strcpy(CopieLigneQuOnEtudie,Ligne);
                RecupereDonnerLigne=strtok(CopieLigneQuOnEtudie," \t\n");
            }
            
            /*La on verifie ensuite le cas ou le block se trouvant
             dans une autre colonne */
            
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
            RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
            xMin2=atof(RecuperationPartieDonnerLigne);
            
            /*Je  regarde la position du mot Abstract,In,This,As,We
             et le mot ou on se trouve .S'il y a une trop grosse différence
             alors on chercher la ligne ou le mot ou la différence est négatif */
            
            while (xMin2 > (xMin+70)){
                if (strcmp(RecupereDonnerLigne,"<line")==0 || strcmp(RecupereDonnerLigne,"<word")==0){
                    RecupereDonnerLigne=strtok(NULL," \t\n");
                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
                    RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
                    xMin2=atof(RecuperationPartieDonnerLigne);
                }
                else{
                    fgets(Ligne,255,pFile);
                    strcpy(CopieLigneQuOnEtudie,Ligne);
                    RecupereDonnerLigne=strtok(CopieLigneQuOnEtudie," \t\n");
                }
            }
            RecupereDonnerLigne=strtok(Ligne," \t\n");
            /*Récupération donné du block ou se trouve tous les infos de l'abstract*/
            while(boolean==1){
                if (strcmp(RecupereDonnerLigne,"<word")==0){
                    for (int i=0 ; i<4 ; i++){
                        RecupereDonnerLigne=strtok(NULL," \t\n");
                    }
                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                    RecuperationPartieDonnerLigne=strtok(NULL," <>");
                    fputs(RecuperationPartieDonnerLigne,pFile2);
                    fputs(" ",pFile2);
                    fgets(Ligne,255,pFile);
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                }
                else if (strcmp(RecupereDonnerLigne,"</line>")==0 || strcmp(RecupereDonnerLigne,"<line")==0 ){
                    fgets(Ligne,255,pFile);
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                }
                else{
                    boolean=0;
                }
            }
            if(isTxt)
            {
                fputs("\n",pFile2);
            }
            if(isXml)
            {
                fputs("\t</abstract>\n",pFile2);
            }
            
            
//          References part (biblio)
            
            if(isTxt)
                fputs("Biblio :",pFile2);
            
            if(isXml)
                fputs("\t<biblio>",pFile2);
            
            boolean = 1;
            bool whitRef = true;
            
            while (boolean == 1){
                if (strcmp(RecupereDonnerLigne,"<block") == 0){
                    
                    fgets(Ligne,255,pFile);
                    fgets(Ligne,255,pFile);
                    
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                    
                    for (int i=0; i<4; i++) {
                        RecupereDonnerLigne=strtok(NULL," \t\n");
                    }
                    
                    RecuperationPartieDonnerLigne = strtok(RecupereDonnerLigne," <>");
                    RecuperationPartieDonnerLigne = strtok(NULL," <>");
                    
                    std::string trouverAbstract = RecuperationPartieDonnerLigne;
                    if (trouverAbstract == "References" ||
                        trouverAbstract == "REFERENCES" ) {
                        boolean = 0;
                    }
                    
                    
//  For an exception, if "R" and "eferences" are separate
                    if (boolean == 1) {
                        if (trouverAbstract == "R") {
                            fgets(Ligne,255,pFile);
                            RecupereDonnerLigne=strtok(Ligne," \t\n");
                            
                            if (strcmp(RecupereDonnerLigne,"<word")==0){
                                for (int i=0; i<4; i++) {
                                    RecupereDonnerLigne=strtok(NULL," \t\n");
                                }
                                
                                RecuperationPartieDonnerLigne = strtok(RecupereDonnerLigne," <>");
                                RecuperationPartieDonnerLigne = strtok(NULL," <>");

                                std::string trouverAbstract = RecuperationPartieDonnerLigne;
                                if (trouverAbstract == "eferences" ||
                                    trouverAbstract == "EFERENCES" ) {
                                    boolean = 0;
                                }
                            }
                        }
                    }
                } else{
                    fgets(Ligne,255,pFile);
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                    
                }
                
                if (strcmp(RecupereDonnerLigne,"</doc>")==0
                    || strcmp(RecupereDonnerLigne,"</body>")==0) {
                    boolean = 0;
                    whitRef = false;
                }
            }
            
            boolean=1;
            
            if (whitRef) {
                
                fgets(Ligne,255,pFile);
                
                strcpy(CopieLigneQuOnEtudie,Ligne);
                RecupereDonnerLigne=strtok(CopieLigneQuOnEtudie," \t\n");
                
                RecupereDonnerLigne=strtok(Ligne," \t\n");
                
                while(boolean==1){
                    if (strcmp(RecupereDonnerLigne,"<word") == 0){
                        for (int i=0; i<4; i++) {
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                            
                            if (i == 0) {
                                strcpy(CopieLigneQuOnEtudie,RecupereDonnerLigne);
                            }
                        }
                        
                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                        
// centering check
                        if (!(extractFromQuotes(CopieLigneQuOnEtudie) > 293
                              && extractFromQuotes(CopieLigneQuOnEtudie) < 304
                            && isNumber(RecuperationPartieDonnerLigne)) == 1) {
                            
                            fputs(RecuperationPartieDonnerLigne,pFile2);
                            fputs(" ",pFile2);
                            fgets(Ligne,255,pFile);
                            RecupereDonnerLigne=strtok(Ligne," \t\n");
                        }
                    }
                    else if (strcmp(RecupereDonnerLigne,"</line>")==0
                             || strcmp(RecupereDonnerLigne,"<line")==0 ){
                        fgets(Ligne,255,pFile);
                        RecupereDonnerLigne=strtok(Ligne," \t\n");
                    }
                    else{
                        fgets(Ligne,255,pFile);
                        RecupereDonnerLigne=strtok(Ligne," \t\n");
                        
                        if (strcmp(RecupereDonnerLigne,"</flow>") == 0 ) {
                            fgets(Ligne,255,pFile);
                        }
                        
                        if (strcmp(RecupereDonnerLigne,"</doc>") == 0 || strcmp(RecupereDonnerLigne,"</body>") == 0) {
                            boolean = 0;
                        }
                    }
                }
            } else {
                fputs("\t Couldn't read 'References'", pFile2);
            }
            
            if(isTxt)
                fputs("\n",pFile2);
            
            if(isXml)
                fputs("</biblio>\n",pFile2);
            
            
            
            if(isXml)
                fputs("</article>\n",pFile2);
            
            fclose(pFile2);
            fclose(pFile);
        }
        remove("./application.txt");
    }
    else{
        std::cout<<"Erreur le nombre argument donné n'est pas sufficent. Il faut donner le dossier ou on va chercher les pdf .On peut rajouter le dossier de sortie des fichiers txt"<<std::endl;
    }
    return 0;
}



