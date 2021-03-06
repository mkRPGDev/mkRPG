README for mkRPG
==========================================
<a href='https://travis-ci.org/mkRPGDev/mkRPG'><img
 src='https://travis-ci.org/mkRPGDev/mkRPG.svg?branch=master'></a>

### Exemple minimal
Cette branche, au moyen de quelques bricolages, permet d'utiliser tous les éléments du projet.

Voici comment en obtenir un aperçu:

1. Compilez et exécutez l'éditeur (voir la branche dédiée pour les détails). Il n'est pas nécessaire de cliquer où que ce soit, un ensemble de dossier a normalement été créé dans `home/XML` dès le lancement
2. Copiez la carte "Le grand large.xml" dans `mkrpg/test/simple/maps`(si vous désirez utiliser une autre carte il est nécessaire de lire sa balise _name_ et de changer en conséquent les fichiers `simple/game.xml`, `simple/world.xml` et `simple/entities/entity.xml`).
3. Lancez un serveur en vous plaçant dans `/src` et en tapant `python3.5 server.py`.
4. Lancez un client en vous plaçant dans `/src` et en tapant `python3.5 client.py`.

### Utilisation du client
Z,Q,S et D permettent de déplacer le personnage,

F de le suivre automatiquement

mettre la souris sur les bords déplace la vue,

la roulette de la souris permet de zoomer,

U-I et J-K font varier les deux angles (à utiliser prudemment)

F11 permet de passer en plein écran mais c'est expérimental avec parfois perte de contrôle du clavier...

Des plugins affichant notamment l'aide seront ajoutés sous peu

### Salle 411
Certaines extensions de python 3 ne sont pas (ou pas correctement) installés en salle C411,
le script p411.sh permet de récupérer une version compilée des paquets suivants pour les machines :
* pygame 1.9.2b8
* numpy 1.11.2

Exécuter `$. p411.sh`

### Snake
Plus d'informations dans le [README de Snake ](https://github.com/mkRPGDev/mkRPG/tree/master/test/snake/README.md)

### Plugins
Voir le [README de Plugins ](https://github.com/mkRPGDev/mkRPG/tree/master/src/plugins/README.md)

### Documentation
[Documentation du C ](https://mkrpgdev.github.io/mkRPG/doc_c/html/) et
[documentation du Python ](https://mkrpgdev.github.io/mkRPG/doc_py/html/)

### Pylint report
[Résultat de pylint](https://mkrpgdev.github.io/mkRPG/pylint_global.html)

### Guide pour la documentation
[Guide](https://github.com/mkRPGDev/mkRPG/raw/gh-pages/doc.pdf)









