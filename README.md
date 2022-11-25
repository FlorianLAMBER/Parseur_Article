# Article_handler

Article_handler est un analyseur d'articles scientifiques. Son but est de vous permettre de trouver plus facilement les informations contenues dans un article scientifique.
Actuellement, il est possible de trouver le titre et le résumé de vos articles scientifiques ainsi que la bibliothèque des réferences et les informations des auteurs (adresse postal). Tout cela en un temps suffisamment court.
## Prise en main:

### Compilation et dependance :

Pour compiler et installer les dépendances sur Debian/Ubuntu et MacOS, vous pouvez utiliser le makefile qui fera tout. Pour ce faire, utilisez la commande suivante :
```bash
curl https://raw.githubusercontent.com/Fliche/Parseur_Article/main/makefile && make clone && cd Parseur_Article && make
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
