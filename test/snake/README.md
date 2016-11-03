## Snake

Voici le premier exemple amusant de jeu géré par notre "maker".

### Utilisation

Pour le lancer :
    python3 server.py ../Test/Snake
et
    python3 client.py ../Test/Snake

Quelques bricolages subsistent et entraînent ces limitations:
- il gère un seul joueur et le serveur doit être relancé à chaque partie
- la fin de partie (ce qui n'a aucun sens en RP) gèle simplement le jeu
- ncurses n'aime pas tellement être interrompu, fermez donc le client avec "q" même à l'arrêt pour le bien de votre console

### Implémentation

De nouveau, les Xmls utilisés sont non-contractuels mais voici le fonctionnement général, qui donne un aperçu de la façon dont je vois l'implémentation des futurs jeux:

Le fichier game.xml charge les différentes parties du monde, il chargera bientôt également chaînes, ui, images, interactions et actions.

Le jeu ne possède initialement qu'une entité (ou personnage) : la tête du serpent, ainsi qu'un unique objet : la pomme.

Un événement automatique `start` survient à la connexion du client : par le jeu d'actions (actions.xml), il place la pomme et met en place un timer pour déclencher des événements `tick`.

À chaque événement `tick` du monde, un événement `move` est transféré à la tête du serpent, celle-ci se déplace selon son paramètre `dir`, instancie un objet de type `corps` en lui donnant quelques paramètres et gère éventuellement une rencontre avec la pomme ou une case mortelle.

La création de ce corps déclenche elle un `initcorps` qui installe un timer pour détruire ce bout de corps plus tard et qui met à 1 le paramètre `mortelle` de la case sur laquelle il est.

C'est tout !
