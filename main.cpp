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
#include <algorithm>

int boolean;									//Servira a rentrer dans les whiles
char Ligne[255]; 								//Permettra de récupérer la ligne du fichier txt ou on travaille
char CopieLigne[255]; 							//Permettra de récupérer la ligne du fichier txt ou on travaille
char* RecupereDonnerLigne;						//Permettera de récupérer une partie de la ligne séparé par des espace , \n et \t
char* RecuperationPartieDonnerLigne;			//Permettera de récupérer la partie séparé et de le séparé encore par des =,<,>
char CopieLigneQuOnEtudie[255];					//Permettera de faire la copie de la variable Ligne
double xMin;									//Permettera de récupérer la position d'un éléments
double xMin2;									//Permettera de récupérer la position d'un éléments
int isTxt = 0;                                  //Permet de savoir si on est en mode txt
int isXml = 0;                                  //Permet de savoir si on est en mode txml
double weigthPage;                              //Permet de savoir la largeur de la page d'un document pdf
double heigthPage;                              //Permet de savoir la hauteur de la page d'un document pdf
int abstractTrouver;                            //Permet de savoir si on a déja trouvé la partie abstract
int introductionTrouver;                        //Permet de savoir si on a déja trouvé la partie introduction
int corpsTrouver;                               //Permet de savoir si on a déja trouvé la partie corps
int conclusionTrouver;                          //Permet de savoir si on a déja trouvé la partie conclusion
int discussionTrouver;                          //Permet de savoir si on a déja trouvé la partie discussion
int referenceTrouver;                           //Permet de savoir si on a déja trouvé la partie reference
double dimensionsImportant;                     //Permet d'avoir la dimensions des titres des paragraphes
int compteurMotApresIntroduction;               //Ce compteur sert à ce que des mots qui suivent l'introduction qui ont une taille supérieur à égale à elle , ne déclenche pas le passement dans le corps
int detecteurRomain;                            //Permet de savoir si on utilise des chiffres romains ou pas
/**
 * @brief Perform commande pdftotext with parameters
 * @authors : Maxime Jullien,Emmanuel  Aubertin
 * @param pdf String who is the path to the pdf.
 * @param txt String who is the name of txt
 */
int ExecuterPDF(std::string pdf, std::string txt){
    int pid;
    int res;
    int ret;
    int verification_commande;
    std::string pdfChar = (  pdf);
    pid=fork();
    if (pid==0){
        std::string cmd = "pdftotext -bbox-layout \"" + pdfChar + "\" " + txt;
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

/**
 * @brief Go to the location where is the title of the document
 * @authors : Maxime Jullien
 * @param pFile File where we read information
 * @param pFile2 File where we write information
 */
void AllerVersTitreDocument(FILE * pFile,FILE* pFile2,FILE* pFile3){
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
            if (atof(RecuperationPartieDonnerLigne)< 60.0){
                fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
                RecupereDonnerLigne=strtok(Ligne," \n\t");
            }
            /*Si la position est bonne ,
             je fais 2 fgets
             pour accéder directement sur le mot*/
            else{
                fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
                fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
                RecupereDonnerLigne=strtok(Ligne," \n\t");
                boolean=0;
            }
        }
        /*Sinon je vais directement sur une autre ligne*/
        else{
            /*On va vers la prochaine ligne du fichier application.txt*/
            fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
            RecupereDonnerLigne=strtok(Ligne," \n\t");
        }
    }
}

/**
 * @brief Write all information about the title of the document
 * @authors : Maxime Jullien
 * @param pFile File where we read information
 * @param pFile2 File where we write information
 */
void EcrireLeTitreDocument(FILE * pFile,FILE* pFile2,FILE* pFile3){
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
            fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
            strcpy(CopieLigneQuOnEtudie,Ligne);
            RecupereDonnerLigne=strtok(Ligne," \t\n");
            
        }
        /*Sinon si c'est </line> ou <line
            je vais directment passer à la prochaine ligne*/
        
        else if (strcmp(RecupereDonnerLigne,"</line>")==0 || strcmp(RecupereDonnerLigne,"<line")==0 ){
            /*On va vers la prochaine ligne du fichier application.txt*/
            fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
            strcpy(CopieLigneQuOnEtudie,Ligne);
            RecupereDonnerLigne=strtok(Ligne," \t\n");
        }
        /*Si j'arrive à la fin de mon block*/
        else{
            boolean=1;
        }
    }
}
/**
 * @brief Group fonction EcrireLeTitreDocument + AllerVersTitreDocument
 * @authors : Maxime Jullien,Aubertin Emmanuel
 * @param pFile File where we read information
 * @param pFile2 File where we write information
 */
void TitreDocument(FILE * pFile,FILE* pFile2,FILE* pFile3){
    /*Recupération du block ou se trouve le titre du document .*/
    if(isTxt)
    {
        fputs("Titre :",pFile2);
    }
    if(isXml)
    {
        fputs("\t<titre>",pFile2);
    }
    AllerVersTitreDocument(pFile,pFile2,pFile3);
    EcrireLeTitreDocument(pFile,pFile2,pFile3);
    if(isTxt)
    {
        fputs("\n",pFile2);
    }
    if(isXml)
    {
        fputs("</titre>\n",pFile2);
    }    

}
/**
 * @brief Recupere une ligne dans plusieurs fichiers pour le cas ou on travaille pas en fonction des colonnes
 * @authors : Maxime Jullien
 * @param pFile File where we read information
 * @param pFile2 File where we read information
*
*/
void recupereLigneNormal(FILE* pFile,FILE* pFile3){
    fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
    strcpy(CopieLigneQuOnEtudie,Ligne);
    RecupereDonnerLigne=strtok(Ligne," \t\n");
}

/**
 * @brief Recupere une ligne dans un fichiers pour le cas ou on travaille en fonction des colonnes
 * @authors : Maxime Jullien
 * @param pFile File where we read information
 * @param pFile2 File where we read information
*
*/
void recupereLigneColonne(FILE* pFile){
    fgets(Ligne,255,pFile);
    strcpy(CopieLigneQuOnEtudie,Ligne);
    RecupereDonnerLigne=strtok(Ligne," \t\n");
}

/**
 * @brief Recupere la dimensions de l'introduction et le met dans la variable dimensionsImportant
 * @authors : Maxime Jullien
*
*/
void recuperationDimensionDesTitresParagraphes(){
    /*Dans CopieLigneQuOnEtudie on avait toute la line , je vais faire une copie
    de CopieLigneQuOnEtudie dans Ligne pour etre sur que j'ai les bonnes informations */
    strcpy(Ligne,CopieLigneQuOnEtudie);
    /*La je vais aller récupérer le yMin de la ligne */
    RecupereDonnerLigne=strtok(Ligne," \t\n");
    RecupereDonnerLigne=strtok(NULL," \t\n");
    RecupereDonnerLigne=strtok(NULL," \t\n");
    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
    RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
    double yMin = atof(RecuperationPartieDonnerLigne);

    /*Je refait une copie de CopieLigneQuOnEtudie dans Ligne */
    strcpy(Ligne,CopieLigneQuOnEtudie);
    /*Et la je vais aller récupérer le yMax de la ligne */
    RecupereDonnerLigne=strtok(Ligne," \t\n");
    RecupereDonnerLigne=strtok(NULL," \t\n");
    RecupereDonnerLigne=strtok(NULL," \t\n");
    RecupereDonnerLigne=strtok(NULL," \t\n");
    RecupereDonnerLigne=strtok(NULL," \t\n");
    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,">=\"");
    RecuperationPartieDonnerLigne=strtok(NULL,">=\"");
    /*DimensionsImportant représente la dimensions en hauteur que les paragraphes Titres ont.
    Cela permettera de reconnaitre les titres de paragraphes à ceux qui ne sont pas */
    dimensionsImportant=atof(RecuperationPartieDonnerLigne)-yMin;
}

//boolean2 =0 lorsque on fait la partie à gauche
//boolean2 =1 on fait la partie qui est à droite

/**
 * @brief Take all words in in the rigth or the left
 * @authors : Maxime Jullien
 * @param pFile File where we read information
 * @param pFile2 File where we write information
*  @param x This will be the weight of the page where we need to don't surpass him
*   @param y This will be the height of the page where we need to don't surpass him or be down
* @param boolean2 Say if we do the rigth colomn( =1 ) or the left colomn(=0)
*/
void recupererMotCelonLaColonne(FILE* pFile,FILE* pFile2,int x,int y,int boolean2){
    /*La je regarde si on n'est pas dans la fin line ou de page ou du body ou de html*/
    while(strcmp(RecupereDonnerLigne,"</page>") != 0 && strcmp(RecupereDonnerLigne,"</doc>")!=0 && strcmp(RecupereDonnerLigne,"</body>")!=0 && strcmp(RecupereDonnerLigne,"</html>")!=0){
        /*Si je récupère dans le fichier une ligne ( représentez un <line dans pdftotext)*/
        if(strcmp(RecupereDonnerLigne,"<line")==0){
            /*Je vais d'abord récupérer le xMin de la ligne */
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
            RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
            /*Après l'avoir récupérez je le met dans une variable*/
            xMin=atof(RecuperationPartieDonnerLigne);

            /*La je fais une copie de CopieLigneQuOnEtudie dans Ligne pour avoir tous les éléments correspondant au début*/
            /*La je vais aller vers yMin de la ligne*/
            strcpy(Ligne,CopieLigneQuOnEtudie);
            RecupereDonnerLigne=strtok(Ligne," \t\n");
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
            RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
            /*Pour le cas de boolean2==0 && xMin<x-20 : si je suis dans la colonne gauche et que le Xmin 
            de la ligne est inférieur à la moitièe de la largeur de la page -20 alors je rentre dedans*/

            /*Pour le cas de boolean2==1 && xMin>x-20 : si je suis dans la colonne droite et que le Xmin 
            de la ligne est supérieur à la moitièe de la largeur de la page -20 alors je rentre dedans */
            /*Avec cela on regarde si la position de la ligne par rapport à la hauteur est supérieur à 30 et inférieur à la hauteur de la page - 30*/
            if ((boolean2==0 && xMin<x-20) || (boolean2==1 && xMin>x-20) && ((atof(RecuperationPartieDonnerLigne)>30)||(atof(RecuperationPartieDonnerLigne)<y-30))){
                /*Je vais vers le premier mot (représentez par word par pdftotext)*/
                recupereLigneColonne(pFile);
                /*tant que j'arrive pas à la fin de la ligne je continue ce qu'il y a dedans*/
                while(strcmp(RecupereDonnerLigne,"</line>")!=0){
                    /*Si la donné récupérer est un mot */
                    if (strcmp(RecupereDonnerLigne,"<word")==0){
                        /*Je vais directement chercher le mot */
                        for (int i=0 ; i<4 ; i++){
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                        }
                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                        /*Je le stock dans une variable string nommé trouveerAbstract*/
                        std::string trouverAbstract = RecuperationPartieDonnerLigne;
                        /*La je regarde si j'ai pas encore trouvé la partie Abstract et que j'ai certain mots clé alors je rentre*/
                        if (abstractTrouver==0 && (trouverAbstract.find("Abstract") != std::string::npos ||
                            strcmp(RecuperationPartieDonnerLigne,"ABSTRACT") == 0 ||
                            strcmp(RecuperationPartieDonnerLigne,"This") == 0 ||
                            strcmp(RecuperationPartieDonnerLigne,"As") == 0||
                            strcmp(RecuperationPartieDonnerLigne,"In") == 0||
                            strcmp(RecuperationPartieDonnerLigne,"We") == 0))
                        {
                            /*Si je dois renvoyez un fichier txt j'écris dans le fichier de sortie Abstract :*/
                            if(isTxt)
                            {
                                fputs("Abstract :",pFile2);
                            }
                            /*Si je dois renvoyez un fichier xml j'écris dans le fichier de sortie <abstract>*/
                            if(isXml)
                            {
                                fputs("\t<abstract>",pFile2);
                            }
                            /*Je met ma variable abstarctTrouver à 1 pour plus que je rentre dedans */
                            abstractTrouver=1;
                            /*Je vais écrire dasn le document de sortie un mot clé lorsque ce n'est pas Abstract comme mot*/
                            if (strcmp(RecuperationPartieDonnerLigne,"Abstract") != 0){
                                fputs(RecuperationPartieDonnerLigne,pFile2);
                                fputs(" ",pFile2);
                            }
                        }
                        /*Si j'ai ou n'ai pas trouvé la partie de l'abstract, je regarde la partie de l'introduction si je ne l'ai pas trouvé 
                        et que le mot correspond à 1 ou 1. ou I ou I. ou introduction ou INTRODUCTION  */
                        else if(introductionTrouver == 0 && (trouverAbstract=="1" || trouverAbstract=="1." || trouverAbstract=="I" || trouverAbstract=="I." || trouverAbstract=="Introduction" || trouverAbstract=="INTRODUCTION")){
                            /*Si je suis dans le cas de 1 ou 1. ou I ou I. alors je rentre dedans*/
                            if(trouverAbstract=="1" || trouverAbstract=="1." || trouverAbstract=="I" || trouverAbstract=="I."){
                                /*Je sauvegarde au cas ou le mot qu'on a récupérer si cela ne correspond pas a l'introduction*/
                                std::string sauvegardeMot=trouverAbstract;
                                recupereLigneColonne(pFile);
                                /*Je vais directemnt chercher le mot */
                                if (strcmp(RecupereDonnerLigne,"</line>")!=0){
                                    for (int i=0 ; i<4 ; i++){
                                        RecupereDonnerLigne=strtok(NULL," \t\n");
                                    }
                                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                    RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                    /*La maintenant je compare si le mot correspond à Introduction ou INTRODUCTIOn ou ntroduction ou NTRODUCTION . Si c'est vrai je rentre dedans */
                                    if (trouverAbstract=="Introduction" || trouverAbstract=="INTRODUCTION" || trouverAbstract=="ntroduction" || trouverAbstract=="NTRODUCTION"){
                                        /*Si je dois sortir du XML j'écrit dans le ficheir les élémenst suivants*/
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                    }
                                    /*Si le mot est I ou I. alors je rentre dedans*/
                                    else if (trouverAbstract=="I" || trouverAbstract=="I."){
                                        /*J'initialise une variable detecteurRomain qui me permet de dir que les Titres des paragraphes se feront avec des chiffres romains*/
                                        detecteurRomain=1;
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                        /*Permet d'aller vers une nouvelle du fichier*/
                                        recupereLigneColonne(pFile);
                                    }
                                    /*Dans le cas ou ce n'était pas l'introduction*/
                                    else{
                                        /*J'ecrit dans le fichier de sortie les mots qu'on a trouvé*/
                                        fputs(sauvegardeMot.data(),pFile2);
                                        fputs(" ",pFile2);
                                        fputs(RecuperationPartieDonnerLigne,pFile2);
                                        fputs(" ",pFile2);
                                    }
                                }
                                /*Si le mot suivant se trouve dans une autre ligne*/
                                else{
                                    /*Je vais aller vers le prochain mot */
                                    while(strcmp(RecupereDonnerLigne,"<word")!=0){
                                        recupereLigneColonne(pFile);
                                    }
                                    /*Je vais récupérer le conten du mot */
                                    for (int i=0 ; i<4 ; i++){
                                        RecupereDonnerLigne=strtok(NULL," \t\n");
                                    }
                                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                    RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                    /*La maintenant je compare si le mot correspond à Introduction ou INTRODUCTIOn ou ntroduction ou NTRODUCTION . Si c'est vrai je rentre dedans */
                                    if (strcmp(RecuperationPartieDonnerLigne,"Introduction")!= 0 || strcmp(RecuperationPartieDonnerLigne,"INTRODUCTION")!= 0 || strcmp(RecuperationPartieDonnerLigne,"ntroduction")!= 0 || strcmp(RecuperationPartieDonnerLigne,"NTRODUCTION")!= 0){
                                        /*Si je dois sortir du XML j'écrit dans le ficheir les élémenst suivants*/
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                    }
                                    /*Si le mot est I ou I. alors je rentre dedans*/
                                    else if (strcmp(RecuperationPartieDonnerLigne,"I")!= 0 || strcmp(RecuperationPartieDonnerLigne,"I.")!= 0){
                                        /*J'initialise une variable detecteurRomain qui me permet de dir que les Titres des paragraphes se feront avec des chiffres romains*/
                                        detecteurRomain=1;
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                        /*Permet d'aller vers une nouvelle du fichier*/
                                        recupereLigneColonne(pFile);
                                    }
                                    /*Dans le cas ou ce n'était pas l'introduction*/
                                    else{
                                        /*J'ecrit dans le fichier de sortie les mots qu'on a trouvé*/
                                        fputs(sauvegardeMot.data(),pFile2);
                                        fputs(" ",pFile2);
                                        fputs(RecuperationPartieDonnerLigne,pFile2);
                                        fputs(" ",pFile2);
                                    }
                                }
                            }
                            /*Si le contenu du mot c'est Introduction ou INTRODUCTION Alors je rentre */
                            else{
                                /*Si je dois sortir du XML j'écrit dans le ficheir les élémenst suivants*/
                                if(isXml){
                                    fputs("</abstract>\n",pFile2);
                                    fputs("\t<introduction>",pFile2);
                                }
                                /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                recuperationDimensionDesTitresParagraphes();
                                /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                introductionTrouver=1;
                                
                            }
                        }
                        /*Si j'ai ou je n'ai pas trouvé l'introduction et abstract */
                        else{
                            /*Si j'ai la dimensiosn des titres du fichier pdf*/
                            if (dimensionsImportant!=0){
                                strcpy(Ligne,CopieLigneQuOnEtudie);
                                /*Je vais récupérer le yMin du mot et le mettre dans une varaible nommé yMin*/
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
                                RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
                                double yMin = atof(RecuperationPartieDonnerLigne);
                                
                                strcpy(Ligne,CopieLigneQuOnEtudie);
                                /*Je vais aller vers le yMax du mot*/
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,">=\"");
                                RecuperationPartieDonnerLigne=strtok(NULL,">=\"");
                                /*Je vais regarder maintenant si le mot corresppond aux mot clé lié à reférences et voir si 
                                c'est un titre de paragraphe*/
                                if(((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && (trouverAbstract == "References" ||
                                        trouverAbstract == "REFERENCES" || 
                                        trouverAbstract == "eferences" ||
                                        trouverAbstract == "EFERENCES" ||
                                        trouverAbstract == "R")){
                                    /*Dans le cas ou ej trouve R comme mot*/
                                    if (trouverAbstract == "R"){
                                        /*Je sauvegarde ce mot dans une variable string nommé sauvegardeMot*/
                                        std::string sauvegardeMot=trouverAbstract;
                                        recupereLigneColonne(pFile);
                                        if(strcmp(RecupereDonnerLigne,"</line>")==0){
                                            while(strcmp(RecupereDonnerLigne,"<word")!=0){
                                                recupereLigneColonne(pFile);
                                            }
                                        }
                                        /*Je vais aller chercher le contenu du mot*/
                                        for (int i=0 ; i<4 ; i++){
                                            RecupereDonnerLigne=strtok(NULL," \t\n");
                                        }
                                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                        trouverAbstract=RecuperationPartieDonnerLigne;
                                        /*Je regarde si c'est EFERENCES ou eferences*/
                                        if (trouverAbstract == "EFERENCES" || trouverAbstract == "eferences"){
                                            if(isTxt)
                                                fputs("Biblio :",pFile2);
                                                    
                                            if(isXml){
                                                /*Si je suis dans la discussion alors je la ferme*/
                                                if(discussionTrouver==1){
                                                    fputs("</discussion>\n",pFile2);
                                                }
                                                /*Si je suis dans la conclusion alors je la ferme*/
                                                else if(conclusionTrouver==1){
                                                    fputs("</conclusion>\n",pFile2);
                                                }
                                                /*Si je suis dans le corps alors je la ferme*/
                                                else if(corpsTrouver==1){
                                                    fputs("</corps>\n",pFile2);
                                                }
                                                fputs("\t<biblio>",pFile2);
                                            }
                                        }
                                        /*Sinon alors ce n'étais pas la references */
                                        else{
                                            fputs(sauvegardeMot.data(),pFile2);
                                            fputs(" ",pFile2); 
                                            fputs(trouverAbstract.data(),pFile2);
                                            fputs(" ",pFile2); 
                                        }
                                    }
                                    /*Dans le cas ou on a directement le mot reference*/
                                    else {
                                        if(isTxt)
                                            fputs("Biblio :",pFile2);
                                                
                                        if(isXml){
                                            /*Si je suis dans la discussion alors je la ferme*/
                                            if(discussionTrouver==1){
                                                fputs("</discussion>\n",pFile2);
                                            }
                                            /*Si je suis dans la conclusion alors je la ferme*/
                                            else if(conclusionTrouver==1){
                                                fputs("</conclusion>\n",pFile2);
                                            }
                                            /*Si je suis dans le corps alors je la ferme*/
                                            else if(corpsTrouver==1){
                                                fputs("</corps>\n",pFile2);
                                            }
                                            fputs("\t<biblio>",pFile2);
                                        }
                                    }
                                    /*J'initialise la variable referenceTrouver pour dire que j'ai trouver la parti reference du document*/
                                    referenceTrouver=1;
                                }
                                /*Je vais regarder maintenant si le mot corresppond aux mot clé lié à conclusions et voir si 
                                c'est un titre de paragraphe*/
                                else if(conclusionTrouver==0 && ((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && (trouverAbstract =="Conclusion" || trouverAbstract =="Conclusions" ||
                                        trouverAbstract == "CONCLUSIONS" || 
                                        trouverAbstract == "ONCLUSIONS" || 
                                        trouverAbstract == "C" )){
                                            if (trouverAbstract == "C"){
                                                std::string sauvegardeMot=trouverAbstract;
                                                recupereLigneColonne(pFile);
                                                if(strcmp(RecupereDonnerLigne,"</line>")==0){
                                                    while(strcmp(RecupereDonnerLigne,"<word")!=0){
                                                        recupereLigneColonne(pFile);
                                                    }
                                                }
                                                for (int i=0 ; i<4 ; i++){
                                                    RecupereDonnerLigne=strtok(NULL," \t\n");
                                                }
                                                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                                RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                                trouverAbstract=RecuperationPartieDonnerLigne;
                                                if (trouverAbstract == "ONCLUSIONS"){
                                                    if(corpsTrouver==1){
                                                        fputs("</corps>\n",pFile2);
                                                    }
                                                    fputs("\t<conclusion>",pFile2);
                                                }
                                                else{
                                                    fputs(sauvegardeMot.data(),pFile2);
                                                    fputs(" ",pFile2); 
                                                    fputs(trouverAbstract.data(),pFile2);
                                                    fputs(" ",pFile2); 
                                                }
                                            }
                                    else {
                                        if(isXml){
                                            if(corpsTrouver==1){
                                                fputs("</corps>\n",pFile2);
                                            }
                                            fputs("\t<conclusion>",pFile2);
                                        }
                                        conclusionTrouver=1;
                                    }
                                }
                                /*Je vais regarder maintenant si le mot corresppond aux mot clé lié à discussion et voir si 
                                c'est un titre de paragraphe*/
                                else if(discussionTrouver==0 &&((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && (trouverAbstract == "Discussion" ||
                                        trouverAbstract == "Discussion" || 
                                        trouverAbstract == "ISCUSSION" )){
                                    if(isXml){
                                        if(conclusionTrouver==1){
                                            fputs("</corps>\n",pFile2);
                                        }
                                        fputs("\t<discussion>",pFile2);
                                    }
                                    discussionTrouver=1;
                                }
                                /*Je vais regarder maintenant si le mot corresppond et un titre ( si il est après introduction alors cela veut dire qu'on est dans le corps)*/
                                else if(corpsTrouver==0 && ((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && compteurMotApresIntroduction<5){
                                    /*Si pour introduction , on avait un chiffre romain alors je regarde si le titre à comme chiffre Romain 2*/
                                    if (detecteurRomain==1 && trouverAbstract.find("II")!=std::string::npos){
                                        /*Si notre fichier de sortie est en XML alors j'écris dans le fichier de sortie c'est informations*/
                                        if(isXml){
                                            fputs("</introduction>\n",pFile2);
                                            fputs("\t<corps>",pFile2);
                                            fputs(trouverAbstract.data(),pFile2);
                                            fputs(" ",pFile2); 
                                        }
                                        /*J'initialise la variable corpsTrouver pour permettre de savoir que j'ai découvert la partie corps du pdf*/
                                        corpsTrouver=1;
                                    }
                                    /*Si pour introduction , il n'y avait pas de chiffre romain*/
                                    else if (detecteurRomain==0){
                                        /*Si notre fichier de sortie est en XML alors j'écris dans le fichier de sortie c'est informations*/
                                        if(isXml){
                                            fputs("</introduction>\n",pFile2);
                                            fputs("\t<corps>",pFile2);
                                            fputs(trouverAbstract.data(),pFile2);
                                            fputs(" ",pFile2); 
                                        }
                                        /*J'initialise la variable corpsTrouver pour permettre de savoir que j'ai découvert la partie corps du pdf*/
                                        corpsTrouver=1;
                                    }
                                }
                                /*Si le mot n'est pas un titre qu'on cherche alors j'écrit dans le fichier de sortie celon si le fichier doit etre en txt ou xml*/
                                else{
                                    /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de l'abstract pour écrire dans le fichier txt*/
                                    if (isTxt && abstractTrouver==1 && introductionTrouver==0){
                                        fputs(trouverAbstract.data(),pFile2);
                                        fputs(" ",pFile2);
                                    }
                                    /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de reference pour écrire dans le fichier txt*/
                                    else if (isTxt && abstractTrouver==1 && referenceTrouver==1){
                                        fputs(trouverAbstract.data(),pFile2);
                                        fputs(" ",pFile2);
                                    }
                                    /*Si c'est un Xml alors j'écrit toute les informations*/
                                    else if (isXml){
                                        fputs(trouverAbstract.data(),pFile2);
                                        fputs(" ",pFile2);
                                    }   
                                }
                            }
                            /*Dans le cas ou le mot n'est pas un titre de paragraphe alors j'écrit dans le fichier de sortie celon si le fichier doit etre en txt ou xml*/
                            else{
                                /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de l'abstract pour écrire dans le fichier txt*/
                                if (isTxt && abstractTrouver==1 && introductionTrouver==0){
                                    fputs(RecuperationPartieDonnerLigne,pFile2);
                                    fputs(" ",pFile2);
                                }
                                /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de reference pour écrire dans le fichier txt*/
                                else if (isTxt && abstractTrouver==1 && referenceTrouver==1){
                                    fputs(RecuperationPartieDonnerLigne,pFile2);
                                    fputs(" ",pFile2);
                                }
                                /*Si c'est un Xml alors j'écrit toute les informations*/
                                else if (isXml){
                                    fputs(RecuperationPartieDonnerLigne,pFile2);
                                    fputs(" ",pFile2);
                                }      
                            }
                        }
                        /*Après que j'ai finis mon écriture dans le fichier de sortie , je vais récupérer une nouvelle ligne*/
                        recupereLigneColonne(pFile);
                        /*Lorsque que j'ai trouver la partie du corps , je vais réduire un compteur . Ce compteur sert à éviter que les 5 premiers mots qui peuvent etre
                        écrit dans la mème police que les titres des paragraphes me fassent passer dans la partie corps */
                        if (corpsTrouver==1){
                            compteurMotApresIntroduction=compteurMotApresIntroduction-1;
                        }
                    }
                }
            }
            /*Si la ligne ne correpond pas à mes critère  de positon alors je vais la fin de la ligne*/
            else{
                while(strcmp(RecupereDonnerLigne,"</line>") != 0 && strcmp(RecupereDonnerLigne,"</page>") != 0 && strcmp(RecupereDonnerLigne,"</doc>")!=0 && strcmp(RecupereDonnerLigne,"</body>")!=0 && strcmp(RecupereDonnerLigne,"</html>")!=0){
                    recupereLigneColonne(pFile);
                }
            }
        }
        /*Tant que je n'est pas une ligne ou fin html ou fin doc ou fin page je continue a récupérer des nouvelles lignes du fichier */
        else{
            recupereLigneColonne(pFile);
        }
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
        std::string fichierpdf;							//Permettera de savoir le nom du fichier pdf mais aussi ou il se situe
        char* fichierTxt;								//Permettera de pointer ou le fichier créer va se trouver et sur quel format
        std::string appTxt = "./application.txt";				//Sa sera le fichier txt qui recevra tous les resultats de la commande pdftotext
        folder_info* workspaceInfo;						//Permet d'avoir tous les fichier pdf dans une classe .
        FILE * pFile;									//Pour lire le fichier ./application.txt
        FILE * pFile3;									//Pour lire le fichier2 ./application.txt
        FILE * pFile2;									//Pour écrire dans le fichier txt

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
            abstractTrouver=0;                  //J'initialise que je n'est pas trouver l'abstract dans le fichier pdf
            introductionTrouver=0;              //J'initialise que je n'est pas trouver l'introduction dans le fichier pdf
            corpsTrouver=0;                     //J'initialise que je n'est pas trouver le corps dans le fichier pdf
            conclusionTrouver=0;                //J'initialise que je n'est pas trouver la conclusion dans le fichier pdf
            discussionTrouver=0;                //J'initialise que je n'est pas trouver la discussion dans le fichier pdf
            referenceTrouver=0;                 //J'initialise que je n'est pas trouver la référence dans le fichier pdf
            detecteurRomain=0;                  //J'initialise que je n'est pas trouver de chiffre romain dans un titre de paragraphe dans le fichier pdf
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
            pFile3=fopen("./application.txt","r");
            /*Ecriture du Titre du fichier dans le fichier txt*/
            boolean = 1;
            fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
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
            fgets(Ligne,255,pFile);fgets(CopieLigne,255,pFile3);
            /*Titre du document*/
            while (strcmp(RecupereDonnerLigne,"<page") !=0){
                recupereLigneNormal(pFile,pFile3);
            }
            
            
            /*Permet de récupérer les dimensions de la page*/

            /*Je vais chercher la weigth de la page et le mettre dans une variable weigthPage*/
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
            RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
            weigthPage=atof(RecuperationPartieDonnerLigne);
            /*Je vais chercher la heigth de la page et le mettre dans une variable weigthPage*/
            RecupereDonnerLigne=strtok(CopieLigneQuOnEtudie," \t\n");
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,">=\"");
            RecuperationPartieDonnerLigne=strtok(NULL,">=\"");
            heigthPage=atof(RecuperationPartieDonnerLigne);
            recupereLigneNormal(pFile,pFile3);

            /*Titre du document*/
            TitreDocument(pFile,pFile2,pFile3);

            /*Auteur */
            if(isTxt)
            {
                fputs("Auteur:",pFile2);
            }
            if(isXml)
            {
                fputs("\t<auteur>",pFile2);
            }
            /*Je fais une copie de CopieLigne dans Ligne pour etre sur que dans Ligne sa soit bien la ligne compléte du fichier qu'on a récupérer*/
            strcpy(Ligne,CopieLigne);
            RecupereDonnerLigne=strtok(Ligne," \t\n");
            /*Je vais chercher la première ligne que je trouve*/
            while(strcmp(RecupereDonnerLigne,"<line")!=0){
                recupereLigneNormal(pFile,pFile3);
            }
            /*Lorsque que  j'ai la ligne , je vais aller récupérer sa position yMax et le mettre dans une variable yMax*/
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecupereDonnerLigne=strtok(NULL," \t\n");
            RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,">=\"");
            RecuperationPartieDonnerLigne=strtok(NULL,">=\"");
            double yMax=atof(RecuperationPartieDonnerLigne);
            /*Je vais vers le plus proche word*/
            recupereLigneNormal(pFile,pFile3);
            /*La variable auteur me permet de dire si j'ai finis la partie auteur */
            int auteur=0;
            while(auteur==0){
                int passer=0; //Cela me permettera lorsque je n'est pas besoin d'une ligne de passer à la suivante en passser cette variable à 1
                /*Lorsque j'ai un word*/
                while (strcmp(RecupereDonnerLigne,"<word")==0){
                    /*Si je ne dois pas passer tous les mots se trouvant dans la ligne*/
                    if (passer==0){         
                        /*Je vais aller récupérer le contenu du word ( récupérer le mot)*/
                        for (int i=0 ; i<4 ; i++){
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                        }
                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                        std::string trouveCharactèreSpécial=RecuperationPartieDonnerLigne;
                        /*Je regarde maintenant s'y il y a un @ ou un { , ces caractère définissent les adresses mails ( c'est ce que nous ne voulons pas ).
                        S'y il y en a pas j'écrit le mot dans le fichier de sorti*/
                        if(trouveCharactèreSpécial.find("@")==std::string::npos && trouveCharactèreSpécial.find("{")==std::string::npos ){
                            fputs(RecuperationPartieDonnerLigne,pFile2);
                            fputs(" ",pFile2);
                        }
                        /*Sinon je met ma variable passer à 1 donc je vais aller jusqu'a la fin de la ligne*/
                        else{
                            passer=1;
                        }
                    }
                    recupereLigneNormal(pFile,pFile3);
                }
                /*Tant que je ne suis pas dans un déut de ligne je récupère à nouveau ma ligne de mon document et je passe à la suivante*/
                while(strcmp(RecupereDonnerLigne,"<line")!=0){
                    recupereLigneNormal(pFile,pFile3);
                }
                /*Je vais aller récuperer la position yMin de la ligne pour la comparer a yMax qu'on a récupérer*/
                RecupereDonnerLigne=strtok(NULL," \t\n");
                RecupereDonnerLigne=strtok(NULL," \t\n");
                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
                RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
                /*Si cette nouvelle ligne à une poisiotn y supérieur à lancienne et qu'elle dépasse de 20 alors j'arrète ma partie auteur*/
                if(atof(RecuperationPartieDonnerLigne)>yMax && atof(RecuperationPartieDonnerLigne)-yMax>=20){
                    auteur=1;
                }
                /*Sinon je vais définir un nouveau yMax avec la nouvelle ligne */
                else{
                    /*Je fais une copie de CopieLigne dans Ligne pour etre sur d'avoir toute les information sur ma ligne*/
                    strcpy(Ligne,CopieLigne);
                    /*Je vais aller mettre le yMax de ma nouvelle ligne dans la variable yMax*/
                    RecupereDonnerLigne=strtok(Ligne," \t\n");
                    RecupereDonnerLigne=strtok(NULL," \t\n");
                    RecupereDonnerLigne=strtok(NULL," \t\n");
                    RecupereDonnerLigne=strtok(NULL," \t\n");
                    RecupereDonnerLigne=strtok(NULL," \t\n");
                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,">=\"");
                    RecuperationPartieDonnerLigne=strtok(NULL,">=\"");
                    yMax=atof(RecuperationPartieDonnerLigne);
                    recupereLigneNormal(pFile,pFile3);
                }
            }
            if(isTxt)
            {
                fputs("\n",pFile2);
            }
            if(isXml)
            {
                fputs("</auteur>\n",pFile2);
            }
            strcpy(Ligne,CopieLigneQuOnEtudie);
            RecupereDonnerLigne=strtok(Ligne," \t\n");
            dimensionsImportant=0;
            /*Tant que je ne suis pas un fin de fichier d'entré, je continue mon programme*/
            while (strcmp(RecupereDonnerLigne,"</doc>") !=0 && strcmp(RecupereDonnerLigne,"</body>")!=0 && strcmp(RecupereDonnerLigne,"</html>")!=0){
                int Modifier=0; //Cette variable me servira lorsque j'atteint une certiane posiont dans la colonne de gauche de continuer et ne pas réfléchir au cas ou il y a deux colonnes
                /*Dans le cas ou je suis pas à la fin de ma page */
                while(strcmp(RecupereDonnerLigne,"</page>") != 0 && strcmp(RecupereDonnerLigne,"</doc>")!=0 && strcmp(RecupereDonnerLigne,"</body>")!=0 && strcmp(RecupereDonnerLigne,"</html>")!=0){
                    /*Si j'ai une ligne et que mon modifier est à 0 */
                    if(strcmp(RecupereDonnerLigne,"<line")==0 && Modifier==0){
                        /*je vais regarder la position xMin de la ligne */
                        RecupereDonnerLigne=strtok(NULL," \t\n");
                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
                        RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
                        /*Si la position xMin de la ligne est inférieur à la moitiè de la largeur de la page - 10 alors je continue 
                        Cela veut dire que ma ligne se trouve dans la colonne de gauche*/
                        if(atof(RecuperationPartieDonnerLigne)<((weigthPage/2)-10)){
                            /*Je vais maintenant récupérer la position yMax de la ligne*/
                            RecupereDonnerLigne=strtok(CopieLigneQuOnEtudie," \t\n");
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                            RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,">=\"");
                            RecuperationPartieDonnerLigne=strtok(NULL,">=\"");
                            /*Si cette yMax à dépasser une certain hauteur alors je peux dire que je n'est plus besoin de traiter le cas avec les deux colonnes.
                            Les élements ce trouvant dans la colonne de droite se suiveront*/
                            if(atof(RecuperationPartieDonnerLigne)>(heigthPage-200)){
                                Modifier=1;
                            }
                            /*Si ma ligne se trouve en pied de page , je vais éviter de prendre ces éléments*/
                            else if(atof(RecuperationPartieDonnerLigne)>(heigthPage-30)){
                                /*Je vais aller à la fin de ligne*/
                                recupereLigneNormal(pFile,pFile3);
                                while(strcmp(RecupereDonnerLigne,"</line>")!=0){
                                    recupereLigneNormal(pFile,pFile3);
                                }
                            }
                            /*Si ma ligne se trouve en haut de page , je vais éviter de prendre ces éléments*/
                            else if(atof(RecuperationPartieDonnerLigne)<(50)){
                                /*Je vais aller à la fin de ligne*/
                                recupereLigneNormal(pFile,pFile3);
                                while(strcmp(RecupereDonnerLigne,"</line>")!=0){
                                    recupereLigneNormal(pFile,pFile3);
                                }
                            }
                            /*Sinon je vais à la prochaine ligne de mon fichier d'entré*/
                            else{
                                recupereLigneNormal(pFile,pFile3);
                            }
                        }
                        /*Sinon je vais d'abord récupérer tous les élements se trouvant dans la colonne de gauche 
                        Puis les éléments se trouvant dans la colonne de droite*/
                        else{
                                /*Je fais des copie entre mes variables pour etres sur que mes variables Ligne et CopieLigneQuOnEtudie et les informations
                                de la ligne qui sont completes*/
                                strcpy(Ligne,CopieLigne);
                                strcpy(CopieLigneQuOnEtudie,Ligne);
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                /*Je vais d'abord récupérer tous les élements se trouvant dans la colonne de gauche*/
                                recupererMotCelonLaColonne(pFile3,pFile2,weigthPage/2,heigthPage,0);
                                /*Je fais des copie entre mes variables pour etres sur que mes variables Ligne et CopieLigneQuOnEtudie et les informations
                                de la ligne qui sont completes*/
                                strcpy(Ligne,CopieLigne);
                                strcpy(CopieLigneQuOnEtudie,Ligne);
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                /*Je vais récupérer tous les élements se trouvant dans la colonne de droite*/
                                recupererMotCelonLaColonne(pFile,pFile2,weigthPage/2,heigthPage,1);
                            }
                        }
                    else if (strcmp(RecupereDonnerLigne,"<word")==0){
                        /*Je vais directement chercher le mot */
                        for (int i=0 ; i<4 ; i++){
                            RecupereDonnerLigne=strtok(NULL," \t\n");
                        }
                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                        /*Je le stock dans une variable string nommé trouveerAbstract*/
                        std::string trouverAbstract = RecuperationPartieDonnerLigne;
                        /*La je regarde si j'ai pas encore trouvé la partie Abstract et que j'ai certain mots clé alors je rentre*/
                        if (abstractTrouver==0 && (trouverAbstract.find("Abstract") != std::string::npos ||
                            strcmp(RecuperationPartieDonnerLigne,"ABSTRACT") == 0 ||
                            strcmp(RecuperationPartieDonnerLigne,"This") == 0 ||
                            strcmp(RecuperationPartieDonnerLigne,"As") == 0||
                            strcmp(RecuperationPartieDonnerLigne,"In") == 0||
                            strcmp(RecuperationPartieDonnerLigne,"We") == 0))
                        {
                            /*Si je dois renvoyez un fichier txt j'écris dans le fichier de sortie Abstract :*/
                            if(isTxt)
                            {
                                fputs("Abstract :",pFile2);
                            }
                            /*Si je dois renvoyez un fichier xml j'écris dans le fichier de sortie <abstract>*/
                            if(isXml)
                            {
                                fputs("\t<abstract>",pFile2);
                            }
                            /*Je met ma variable abstarctTrouver à 1 pour plus que je rentre dedans */
                            abstractTrouver=1;
                            /*Je vais écrire dasn le document de sortie un mot clé lorsque ce n'est pas Abstract comme mot*/
                            if (strcmp(RecuperationPartieDonnerLigne,"Abstract") != 0){
                                fputs(RecuperationPartieDonnerLigne,pFile2);
                                fputs(" ",pFile2);
                            }
                        }
                        /*Si j'ai ou n'ai pas trouvé la partie de l'abstract, je regarde la partie de l'introduction si je ne l'ai pas trouvé 
                        et que le mot correspond à 1 ou 1. ou I ou I. ou introduction ou INTRODUCTION  */
                        else if(introductionTrouver == 0 && (trouverAbstract=="1" || trouverAbstract=="1." || trouverAbstract=="I" || trouverAbstract=="I." || trouverAbstract=="Introduction" || trouverAbstract=="INTRODUCTION")){
                            /*Si je suis dans le cas de 1 ou 1. ou I ou I. alors je rentre dedans*/
                            if(trouverAbstract=="1" || trouverAbstract=="1." || trouverAbstract=="I" || trouverAbstract=="I."){
                                /*Je sauvegarde au cas ou le mot qu'on a récupérer si cela ne correspond pas a l'introduction*/
                                std::string sauvegardeMot=trouverAbstract;
                                recupereLigneNormal(pFile,pFile3);
                                /*Je vais directemnt chercher le mot */
                                if (strcmp(RecupereDonnerLigne,"</line>")!=0){
                                    for (int i=0 ; i<4 ; i++){
                                        RecupereDonnerLigne=strtok(NULL," \t\n");
                                    }
                                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                    RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                    /*La maintenant je compare si le mot correspond à Introduction ou INTRODUCTIOn ou ntroduction ou NTRODUCTION . Si c'est vrai je rentre dedans */
                                    if (trouverAbstract=="Introduction" || trouverAbstract=="INTRODUCTION" || trouverAbstract=="ntroduction" || trouverAbstract=="NTRODUCTION"){
                                        /*Si je dois sortir du XML j'écrit dans le ficheir les élémenst suivants*/
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                    }
                                    /*Si le mot est I ou I. alors je rentre dedans*/
                                    else if (trouverAbstract=="I" || trouverAbstract=="I."){
                                        /*J'initialise une variable detecteurRomain qui me permet de dir que les Titres des paragraphes se feront avec des chiffres romains*/
                                        detecteurRomain=1;
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                        /*Permet d'aller vers une nouvelle du fichier*/
                                        recupereLigneNormal(pFile,pFile3);
                                    }
                                    /*Dans le cas ou ce n'était pas l'introduction*/
                                    else{
                                        /*J'ecrit dans le fichier de sortie les mots qu'on a trouvé*/
                                        fputs(sauvegardeMot.data(),pFile2);
                                        fputs(" ",pFile2);
                                        fputs(RecuperationPartieDonnerLigne,pFile2);
                                        fputs(" ",pFile2);
                                    }
                                }
                                /*Si le mot suivant se trouve dans une autre ligne*/
                                else{
                                    /*Je vais aller vers le prochain mot */
                                    while(strcmp(RecupereDonnerLigne,"<word")!=0){
                                        recupereLigneNormal(pFile,pFile3);
                                    }
                                    /*Je vais récupérer le conten du mot */
                                    for (int i=0 ; i<4 ; i++){
                                        RecupereDonnerLigne=strtok(NULL," \t\n");
                                    }
                                    RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                    RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                    /*La maintenant je compare si le mot correspond à Introduction ou INTRODUCTIOn ou ntroduction ou NTRODUCTION . Si c'est vrai je rentre dedans */
                                    if (strcmp(RecuperationPartieDonnerLigne,"Introduction")!= 0 || strcmp(RecuperationPartieDonnerLigne,"INTRODUCTION")!= 0 || strcmp(RecuperationPartieDonnerLigne,"ntroduction")!= 0 || strcmp(RecuperationPartieDonnerLigne,"NTRODUCTION")!= 0){
                                        /*Si je dois sortir du XML j'écrit dans le ficheir les élémenst suivants*/
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                    }
                                    /*Si le mot est I ou I. alors je rentre dedans*/
                                    else if (strcmp(RecuperationPartieDonnerLigne,"I")!= 0 || strcmp(RecuperationPartieDonnerLigne,"I.")!= 0){
                                        /*J'initialise une variable detecteurRomain qui me permet de dir que les Titres des paragraphes se feront avec des chiffres romains*/
                                        detecteurRomain=1;
                                        if(isXml){
                                            fputs("</abstract>\n",pFile2);
                                            fputs("\t<introduction>",pFile2);
                                        }
                                        /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                        recuperationDimensionDesTitresParagraphes();
                                        /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                        introductionTrouver=1;
                                        /*Permet d'aller vers une nouvelle du fichier*/
                                        recupereLigneNormal(pFile,pFile3);
                                    }
                                    /*Dans le cas ou ce n'était pas l'introduction*/
                                    else{
                                        /*J'ecrit dans le fichier de sortie les mots qu'on a trouvé*/
                                        fputs(sauvegardeMot.data(),pFile2);
                                        fputs(" ",pFile2);
                                        fputs(RecuperationPartieDonnerLigne,pFile2);
                                        fputs(" ",pFile2);
                                    }
                                }
                            }
                            /*Si le contenu du mot c'est Introduction ou INTRODUCTION Alors je rentre */
                            else{
                                /*Si je dois sortir du XML j'écrit dans le ficheir les élémenst suivants*/
                                if(isXml){
                                    fputs("</abstract>\n",pFile2);
                                    fputs("\t<introduction>",pFile2);
                                }
                                /*Je récupère la dimensions de l'introduction que je mets dans la varaible dimensionsImportants*/
                                recuperationDimensionDesTitresParagraphes();
                                /*J'initialise la variable introductionTrouver à 1 pour me permettre de ne plus rentrer dans cette aprtie de la fonction*/
                                introductionTrouver=1;
                                
                            }
                        }
                        /*Si j'ai ou je n'ai pas trouvé l'introduction et abstract */
                        else{
                            /*Si j'ai la dimensiosn des titres du fichier pdf*/
                            if (dimensionsImportant!=0){
                                strcpy(Ligne,CopieLigne);
                                /*Je vais récupérer le yMin du mot et le mettre dans une varaible nommé yMin*/
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,"=\"");
                                RecuperationPartieDonnerLigne=strtok(NULL,"=\"");
                                double yMin = atof(RecuperationPartieDonnerLigne);
                                
                                strcpy(Ligne,CopieLigne);
                                /*Je vais aller vers le yMax du mot*/
                                RecupereDonnerLigne=strtok(Ligne," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecupereDonnerLigne=strtok(NULL," \t\n");
                                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne,">=\"");
                                RecuperationPartieDonnerLigne=strtok(NULL,">=\"");
                                /*Je vais regarder maintenant si le mot corresppond aux mot clé lié à reférences et voir si 
                                c'est un titre de paragraphe*/
                                if(((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && (trouverAbstract == "References" ||
                                        trouverAbstract == "REFERENCES" || 
                                        trouverAbstract == "eferences" ||
                                        trouverAbstract == "EFERENCES" ||
                                        trouverAbstract == "R")){
                                    /*Dans le cas ou ej trouve R comme mot*/
                                    if (trouverAbstract == "R"){
                                        /*Je sauvegarde ce mot dans une variable string nommé sauvegardeMot*/
                                        std::string sauvegardeMot=trouverAbstract;
                                        recupereLigneNormal(pFile,pFile3);
                                        /*Je vais aller chercher le contenu du mot*/
                                        if(strcmp(RecupereDonnerLigne,"</line>")==0){
                                            while(strcmp(RecupereDonnerLigne,"<word")!=0){
                                                recupereLigneNormal(pFile,pFile3);
                                            }
                                        }
                                        for (int i=0 ; i<4 ; i++){
                                            RecupereDonnerLigne=strtok(NULL," \t\n");
                                        }
                                        RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                        RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                        trouverAbstract=RecuperationPartieDonnerLigne;
                                        /*Je regarde si c'est EFERENCES ou eferences*/
                                        if (trouverAbstract == "EFERENCES" || trouverAbstract == "eferences"){
                                            if(isTxt)
                                                fputs("Biblio :",pFile2);
                                                    
                                            if(isXml){
                                                /*Si je suis dans la discussion alors je la ferme*/
                                                if(discussionTrouver==1){
                                                    fputs("</discussion>\n",pFile2);
                                                }
                                                /*Si je suis dans la conclusion alors je la ferme*/
                                                else if(conclusionTrouver==1){
                                                    fputs("</conclusion>\n",pFile2);
                                                }
                                                /*Si je suis dans le corps alors je la ferme*/
                                                else if(corpsTrouver==1){
                                                    fputs("</corps>\n",pFile2);
                                                }
                                                fputs("\t<biblio>",pFile2);
                                            }
                                        }
                                        /*Sinon alors ce n'étais pas la references */
                                        else{
                                            fputs(sauvegardeMot.data(),pFile2);
                                            fputs(" ",pFile2); 
                                            fputs(trouverAbstract.data(),pFile2);
                                            fputs(" ",pFile2); 
                                        }
                                    }
                                    /*Dans le cas ou on a directement le mot reference*/
                                    else {
                                        if(isTxt)
                                            fputs("Biblio :",pFile2);
                                                
                                        if(isXml){
                                            /*Si je suis dans la discussion alors je la ferme*/
                                            if(discussionTrouver==1){
                                                fputs("</discussion>\n",pFile2);
                                            }
                                            /*Si je suis dans la conclusion alors je la ferme*/
                                            else if(conclusionTrouver==1){
                                                fputs("</conclusion>\n",pFile2);
                                            }
                                            /*Si je suis dans le corps alors je la ferme*/
                                            else if(corpsTrouver==1){
                                                fputs("</corps>\n",pFile2);
                                            }
                                            fputs("\t<biblio>",pFile2);
                                        }
                                    }
                                    /*J'initialise la variable referenceTrouver pour dire que j'ai trouver la parti reference du document*/
                                    referenceTrouver=1;
                                }
                                /*Je vais regarder maintenant si le mot corresppond aux mot clé lié à conclusions et voir si 
                                c'est un titre de paragraphe*/
                                else if(conclusionTrouver==0 && ((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && (trouverAbstract =="Conclusion" || trouverAbstract =="Conclusions" ||
                                        trouverAbstract == "CONCLUSIONS" || 
                                        trouverAbstract == "ONCLUSIONS" || 
                                        trouverAbstract == "C" )){
                                            if (trouverAbstract == "C"){
                                                std::string sauvegardeMot=trouverAbstract;
                                                recupereLigneNormal(pFile,pFile3);
                                                if(strcmp(RecupereDonnerLigne,"</line>")==0){
                                                    while(strcmp(RecupereDonnerLigne,"<word")!=0){
                                                        recupereLigneNormal(pFile,pFile3);
                                                    }
                                                }
                                                for (int i=0 ; i<4 ; i++){
                                                    RecupereDonnerLigne=strtok(NULL," \t\n");
                                                }
                                                RecuperationPartieDonnerLigne=strtok(RecupereDonnerLigne," <>");
                                                RecuperationPartieDonnerLigne=strtok(NULL," <>");
                                                trouverAbstract=RecuperationPartieDonnerLigne;
                                                if (trouverAbstract == "ONCLUSIONS"){
                                                    if(corpsTrouver==1){
                                                        fputs("</corps>\n",pFile2);
                                                    }
                                                    fputs("\t<conclusion>",pFile2);
                                                }
                                                else{
                                                    fputs(sauvegardeMot.data(),pFile2);
                                                    fputs(" ",pFile2); 
                                                    fputs(trouverAbstract.data(),pFile2);
                                                    fputs(" ",pFile2); 
                                                }
                                            }
                                    else {
                                        if(isXml){
                                            if(corpsTrouver==1){
                                                fputs("</corps>\n",pFile2);
                                            }
                                            fputs("\t<conclusion>",pFile2);
                                        }
                                        conclusionTrouver=1;
                                    }
                                }
                                /*Je vais regarder maintenant si le mot corresppond aux mot clé lié à discussion et voir si 
                                c'est un titre de paragraphe*/
                                else if(discussionTrouver==0 &&((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && (trouverAbstract == "Discussion" ||
                                        trouverAbstract == "Discussion" || 
                                        trouverAbstract == "ISCUSSION" )){
                                    if(isXml){
                                        if(conclusionTrouver==1){
                                            fputs("</corps>\n",pFile2);
                                        }
                                        fputs("\t<discussion>",pFile2);
                                    }
                                    discussionTrouver=1;
                                }
                                /*Je vais regarder maintenant si le mot corresppond et un titre ( si il est après introduction alors cela veut dire qu'on est dans le corps)*/
                                else if(corpsTrouver==0 && ((std::to_string(atof(RecuperationPartieDonnerLigne)-yMin) == std::to_string(dimensionsImportant))) && compteurMotApresIntroduction<5){
                                    /*Si pour introduction , on avait un chiffre romain alors je regarde si le titre à comme chiffre Romain 2*/
                                    if (detecteurRomain==1 && trouverAbstract.find("II")!=std::string::npos){
                                        /*Si notre fichier de sortie est en XML alors j'écris dans le fichier de sortie c'est informations*/
                                        if(isXml){
                                            fputs("</introduction>\n",pFile2);
                                            fputs("\t<corps>",pFile2);
                                            fputs(trouverAbstract.data(),pFile2);
                                            fputs(" ",pFile2); 
                                        }
                                        /*J'initialise la variable corpsTrouver pour permettre de savoir que j'ai découvert la partie corps du pdf*/
                                        corpsTrouver=1;
                                    }
                                    /*Si pour introduction , il n'y avait pas de chiffre romain*/
                                    else if (detecteurRomain==0){
                                        /*Si notre fichier de sortie est en XML alors j'écris dans le fichier de sortie c'est informations*/
                                        if(isXml){
                                            fputs("</introduction>\n",pFile2);
                                            fputs("\t<corps>",pFile2);
                                            fputs(trouverAbstract.data(),pFile2);
                                            fputs(" ",pFile2); 
                                        }
                                        /*J'initialise la variable corpsTrouver pour permettre de savoir que j'ai découvert la partie corps du pdf*/
                                        corpsTrouver=1;
                                    }
                                }
                                /*Si le mot n'est pas un titre qu'on cherche alors j'écrit dans le fichier de sortie celon si le fichier doit etre en txt ou xml*/
                                else{
                                    /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de l'abstract pour écrire dans le fichier txt*/
                                    if (isTxt && abstractTrouver==1 && introductionTrouver==0){
                                        fputs(trouverAbstract.data(),pFile2);
                                        fputs(" ",pFile2);
                                    }
                                    /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de reference pour écrire dans le fichier txt*/
                                    else if (isTxt && abstractTrouver==1 && referenceTrouver==1){
                                        fputs(trouverAbstract.data(),pFile2);
                                        fputs(" ",pFile2);
                                    }
                                    /*Si c'est un Xml alors j'écrit toute les informations*/
                                    else if (isXml){
                                        fputs(trouverAbstract.data(),pFile2);
                                        fputs(" ",pFile2);
                                    }   
                                }
                            }
                            /*Dans le cas ou le mot n'est pas un titre de paragraphe alors j'écrit dans le fichier de sortie celon si le fichier doit etre en txt ou xml*/
                            else{
                                /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de l'abstract pour écrire dans le fichier txt*/
                                if (isTxt && abstractTrouver==1 && introductionTrouver==0){
                                    fputs(RecuperationPartieDonnerLigne,pFile2);
                                    fputs(" ",pFile2);
                                }
                                /*Si c'est un fichier txt il faut vérifier qu'on est dans la partie de reference pour écrire dans le fichier txt*/
                                else if (isTxt && abstractTrouver==1 && referenceTrouver==1){
                                    fputs(RecuperationPartieDonnerLigne,pFile2);
                                    fputs(" ",pFile2);
                                }
                                /*Si c'est un Xml alors j'écrit toute les informations*/
                                else if (isXml){
                                    fputs(RecuperationPartieDonnerLigne,pFile2);
                                    fputs(" ",pFile2);
                                }      
                            }
                        }
                        /*Après que j'ai finis mon écriture dans le fichier de sortie , je vais récupérer une nouvelle ligne*/
                        recupereLigneNormal(pFile,pFile3);
                        /*Lorsque que j'ai trouver la partie du corps , je vais réduire un compteur . Ce compteur sert à éviter que les 5 premiers mots qui peuvent etre
                        écrit dans la mème police que les titres des paragraphes me fassent passer dans la partie corps */
                        if (corpsTrouver==1){
                            compteurMotApresIntroduction=compteurMotApresIntroduction-1;
                        }
                    }

                    else{
                        recupereLigneNormal(pFile,pFile3);
                    }
                }   
                    recupereLigneNormal(pFile,pFile3);
            }
            
            if(isXml){
                fputs("</biblio>\n",pFile2);
                fputs("</article>\n",pFile2);
            }
            fclose(pFile3);
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