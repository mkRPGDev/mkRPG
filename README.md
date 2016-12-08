README for mkRPG
==========================================
<a href='https://travis-ci.org/mkRPGDev/mkRPG'><img
 src='https://travis-ci.org/mkRPGDev/mkRPG.svg?branch=master'></a>

# Utilisation:
(Se placer dans src)


### Serveur
Generic game server.

usage: server.py [-h] [-p PATH]

### Client
Generic game client.

usage: client.py [-h] [-p PATH] [-c | -g | -n] [-d]


Un sytème de plugins est disponible (et en amélioration) cf plugins/ ...


### Lancement
lancer le serveur puis le nombre requis de clients avec python3.5 client.py -g


### En salle 411
Pygame n'est pas installé en salle C411, le script pygame411.sh permet de récupérer une version compilée pour ces machines. Exécuter `$. p411.sh`


# Snake

Plus d'informations dans Test/Snake/README.md...

# Exemple minimal

L'exemple ne marche pas, cela est probablement dû à de petits bugs que nous n'avons pas eu le temps de corriger. Le réseau fonctionne (voir fenêtre terminal server pour voir les messages reçus).
Version ncurses qui fonctionne : cf commit 94039af (ou branche server actuelle)

# Documentation
[Documentation du C ](https://mkrpgdev.github.io/mkRPG/doc_c/html/) et
[documentation du Python ](https://mkrpgdev.github.io/mkRPG/doc_py/html/)

# Guide pour la documentation
[Guide](https://github.com/mkRPGDev/mkRPG/raw/gh-pages/doc.pdf)
