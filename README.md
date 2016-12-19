# Branche refonteClient
### Objectif
Afin de faire parvenir le client pygame à des performances suffisantes pour le projet, il est ici reconstruit progressivement en profilant à chaque étape.
La version actuelle ne contient pas de chuncks, de layers, d'updates partiels de l'écran, ni de cache. Elle atteint cependant des performances raisonnables sur une carte de dizaines de miliers de cellules, en affichant des images déformées selon des angles variables. Les animations demeurent envisageables.

### Minimal Working Example
Cette branche, au moyen de quelques bricolages, permet d'utiliser tous les éléments du projet.

Voici comment en obtenir un aperçu:

1. Compilez et exécutez l'éditeur (voir la branche dédiée pour les détails). Il n'est pas nécessaire de cliquer où que ce soit, un ensemble de dossier a normalement été créé dans `home/XML` dès le lancement
2. Copiez la carte "Le grand large.xml" dans `mkrpg/test/simple/maps`(si vous désirez utiliser une autre carte il est nécessaire de lire sa balise _name_ et de changer en conséquent les fichiers `simple/game.xml`, `simple/world.xml` et `simple/entities/entity.xml`).
3. Lancez un serveur en vous plaçant dans `/src` et en tapant `python3 server.py`.
4. Lancez un client en vous plaçant dans `/src` et en tapant `python3 client.py`.

### Utilisation du client
Z,Q,S et D permettent de déplacer le personnage,

les flèches du clavier déplacent la vue,

pageup et pagedown permettent de zoomer,

U<->I et J<->K font varier les deux angles (à utiliser prudemment)

Le déplacement à la souris va être rétabli sous peu, ainsi que des plugins affichant notamment l'aide.
