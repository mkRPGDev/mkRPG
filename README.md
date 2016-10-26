## Serveur&co

Cette branche contient le serveur, son monde et de quoi le tester (client console, communication naïve, monde xml dans Test/).

Aucun module supplémentaire n'est requis.

L'utilisation de ncurses empêche le fonctionnement sous Windows® -- difficile de concilier les gens sans interface graphique et les gens sans OS...

L'organisation du Xml a été repensée depuis, elle n'est pas contractuelle.

Il arrive que des threads réseau survivent aux massacres et il est alors bon de savoir que le numéro de port se change dans const.py.

La majorité des fichiers pour lequels cela a du sens peuvent être exécutés indépendamment pour les tester en attendant la mise en place d'une infrastructure plus sérieuse.

### Utilisation:

    python3 server.py

puis

    python3 client.py

### À faire ou à revoir:
- protocole d'initialisation
- structure de données events
- destruction d'objets
- vrai réseau
- distinction grille/cellules
