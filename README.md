# Parseur_Article

Explication du système :
Fonctionnement avec argument :


Fonctionnement normal :
Notre programme marche que sur des fichiers pdf qui sont écrits en Anglais .
Avec le dossier donné en argument , on va créer une classe folder_info ( classe créeer par Emmanuel AUBERTIN) qui nous donnera tous les documents pdf se trouvant dedans .Mais aussi le nom du dossier de sortie qui peut etre donner en argument ou créer automatiquement ( on prend le dossier d'entré et on rajoute out) et celui d'entré .
Avec Cela pour chaque fichier pdf :
  Je vais d'abord faire une copie du nom du fichier pdf ( en comptant le .pdf ) et modifier le .pdf en .txt .
  Après je vais executer la commande pdftotext -bbox-layout dossier/nomfichierpdf ./application.txt ( application.txt sera le fichier on en aura la conversion par le  pdftotext de tous les pdf)
  J'ouvre maintenant en lecture ./application.txt et j'ouvre en écriture + append dossiersortie/nomfichiertxt .
  j'écrit "Titre fichier pdf " ensuite le titre du fichier pdf dans le fichiertxt.
  J'écrit un saut à la ligne .
  J'écrit "Titre" dans le fichier txt.
  La après je vais aller dans le fichier ./application.txt et je vais accéder au premier bloc , si la position du bloc dans une page est inférieur à 60 en position y alors je vais aux bloc suivant (la page fait max ... en position y et max ... en position x ).Cela permet d'enlever les cas ou on a du textes sur les bordures haut du documents .
  Après avoir touvé le bloc se trouvant le titre , je vais pour chaque mot jusqu'a la fin du block , ecrire ces mot dans le fichier txt .
    J'écrit un saut à la ligne puis "Abstrat :" dans le fichier txt.
    Après je vais voir pour chaque block qui suit si ce qui se trouve dans le premier mot (premier ligne <word dans le bloc ),s'y il trouve Abstract ou This , In , We , As . Et j'écrit dans le document si il trouve This,In,We,As.
  Dans le cas ou on  le mot Abstract tous seul dans un block ( qu'il y a un seul <word dans le <block ) alors je vais au block suivant et je regarde si le block suivant si sa position x est bien inférieur à +60 de celle ou se trouve le mot Abstract sinon je vais dans un autre block.
  Après etre dans le bon block je vais etre tous les mots jusqu'a la fin du block dans le fichier txt .
  Je ferme le fichier txt et application.txt et on recommence pour les autres fichiers pdf.
  A la fin du programme on supprime application.txt .
  
  Procédure nécessaire pour avoir Application :
  g++ -o ..... main.cpp pour compiler le programmme
  
  Pour éxécuter que sur Linux :
 ./...... -i dossier_entré -o dossier_sortie
  
