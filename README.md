# Article_handler

Article_handler est un analyseur d'articles scientifiques. Son but est de vous permettre de trouver plus facilement les informations contenues dans un article scientifique.
Actuellement, il est possible de trouver le titre et le résumé de vos articles scientifiques ainsi que la bibliothèque des réferences et les informations des auteurs (adresse postal). Tout cela en un temps suffisamment court.
## Prise en main:

./Article_handler -i chemin_dossier_des_pdf/ -o chemin_dossier_de_sortie -t

	-t peut être changé par -x, selon l'envie d'avoir des fichiers au format txt ou au format xml.
	
./Article_handler_menu

	une interface graphique permet de séléctionner, au clavier, le dossier d'entrée (celui des fichiers PDF), le dossier de sortie, les PDF du dossier que l'on souhaite convertir et finalement Get XML ou Get Txt.
	
	La selection se fait avec les flèches puis entrer pour confirmer.
	
	si besoin, il est possible de reset les sélections.

### Compilation et dependance :

Pour compiler et installer les dépendances sur Debian/Ubuntu et MacOS, vous pouvez utiliser le makefile qui fera tout. Pour ce faire, utilisez la commande suivante :
```bash
curl https://raw.githubusercontent.com/Fliche/Parseur_Article/sprint5/makefile && make clone 
```
Si vous n'avez pas la commande curl vous pouvez le faire comme ceci : 
```bash
wget https://raw.githubusercontent.com/Fliche/Parseur_Article/sprint5/makefile && make clone 
```

### Installation :
Accéder dans le dossier Parseur_Article qu'il vient d'etre créer, est réaliser la commande suivant : 
```bash
make
```
## Utiliser Article_handler :

Une fois compilé et installé, l'utilisation du scanner est assez simple.
Il a deux arguments obligatoires : 
  -`-i` ; il spécifie le chemin vers le dossier contenant votre pdf.
  -Au choix : `-x` ou `-t`. Le premier sortira dans l'output les fichiers sous format .xml et le second sortira les fichiers sous format .txt

Si vous ne spécifiez pas de dossier de sortie avec l'option `-o`, un dossier de `output/` sera créé dans la même racine que celui d'input (d'entrée).



Exemple d'utilisation :
```bash
./Article_handler -i /PATH/TO/PDF -t # La sortie sera dans output sous format .txt

./Article_handler -i /PATH/TO/PDF -o /PATH/TO/RESULT -x # La sortie sera dans /PATH/TO/RESULT.xml
```


## Utiliser l'interface graphique :

L'utilisateur à la possibilité d'utiliser une interface graphique pour notre programme.

```bash
./Article_handler_menu
```

Après cela, vous arriverez à une interface graphique qui vous permettra d'utiliser la souris ou les touches: "haut", "bas", "tabulation"

<img width="581" alt="Снимок экрана 2022-12-06 в 22 11 33" src="https://user-images.githubusercontent.com/32616842/208298024-eddbe29a-3de2-42fd-ab03-4e5dc11393d9.png">

Sélectionnez le dossier dans lequel vous souhaitez placer vos fichiers txt / xml

<img width="1025" alt="Снимок экрана 2022-12-06 в 22 15 00" src="https://user-images.githubusercontent.com/32616842/208298136-56d3ad5b-230c-4c48-9c23-ef4b1343be14.png">

Dans l'onglet "select pdf", vous pouvez sélectionner tout ou plusieurs documents à analyser.

<img width="1014" alt="Снимок экрана 2022-12-06 в 22 15 19" src="https://user-images.githubusercontent.com/32616842/208298883-9584b209-74b1-4e38-8254-0e96a63a8435.png">

Cliquez sur l'onglet "Output" adjacent.

Sélectionnez le dossier dans lequel se trouvent vos fichiers pdf.

<img width="1023" alt="Снимок экрана 2022-12-06 в 22 15 06" src="https://user-images.githubusercontent.com/32616842/208298461-ee81331a-b7e7-48f6-a976-cab79ee7cef6.png">

Appuyez sur la touche "Get txt/xml"

Le résultat se trouve dans le dossier sélectionné précédemment

<img width="834" alt="Снимок экрана 2022-12-06 в 22 15 28" src="https://user-images.githubusercontent.com/32616842/208298551-0ab7989e-5401-4d0f-b47e-079f91651e32.png">

