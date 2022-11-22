# Article_handler

Article_handler est un analyseur d'articles scientifiques. Son but est de vous permettre de trouver plus facilement les informations contenues dans un article scientifique.
Actuellement, il est possible de trouver le titre et le résumé de vos articles scientifiques. Tout cela en un temps très court.a
## Prise en main:

### Compilation et dependance :

Pour compiler et installer les dépendances sur Debian/Ubuntu et MacOS, vous pouvez utiliser le makefile qui fera tout. Pour ce faire, utilisez la commande suivante :
```bash
curl https://raw.githubusercontent.com/Fliche/Parseur_Article/main/makefile && make clone && cd Parseur_Article && make
```

## Utiliser Article_handler :

Une fois compilé et installé, l'utilisation du scanner est assez simple.
Il a un argument obligatoire, qui est `-i` ; il spécifie le chemin vers le dossier contenant votre pdf.

Si vous ne spécifiez pas de dossier de sortie avec l'option `-o`, un dossier de `output/` sera créé.

Exemple d'utilisation :
```bash
./Article_handler -i /PATH/TO/PDF # La sortie sera dans output

./Article_handler -i /PATH/TO/PDF -o /PATH/TO/RESULT # La sortie sera dans /PATH/TO/RESULT
```