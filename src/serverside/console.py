""" Manages the server console """

# XXX en faire une classe avec un unique attribut server pour éviter le partial ?

def invite():
    print(">> ", end='', flush=True)

def welcomeMessage(server):
    """ Print server hello """
    print("Server started; waiting for", len(server.world.entities), "clients")
    invite()

def inputReady(server):
    """ Treat server interactive commands """
    st = input()
    if st in ("h", "help"):
        print("""Liste des commandes disponibles:
(vous pouvez aussi taper uniquement la première lettre)
    clients - affiche la liste des clients connectés
    dump - enregistre l'état actuel du monde dans un ensemble de Xml
    exec:c - exécute le code c à vos risques et périls
    help - affiche cette page
    log - affiche/masque des informations du serveur
    plugins - liste les plugins chargés côté serveur
    restart - redémarre le serveur
    send:ev - envoie l'événement ev au serveur appliqué au monde
    quit - arrête le serveur
              """)
    elif st in ("c", "clients"):
        for co in server.net.connections:
            print(co.writer.transport.get_extra_info("peername"),
                  *(("on entity", co.entity.ident) if co.entity else ("with no entity",)))
    elif st in ("d", "dump", "r", "restart"):
        print("Non implémenté")
#            for t in asyncio.Task.all_tasks(): t.cancel()
#            self.loop.stop()
#            self.__init__(args.path)
#            self.run()
    elif st.startswith("e:"):
        exec(st[2:])
    elif st.startswith("exec:"):
        exec(st[5:])
    elif st in ("exit", "q", "quit", "w"):
        exit(0)
    elif st in ("p", "plugins"):
        for p in server.plugins:
            print(p)
    elif st.startswith("s:"):
        server.events.put_nowait((server.world, st[2:]))
    elif st.startswith("send:"):
        server.events.put_nowait((server.world, st[2:]))
    elif not st or st.isspace():
        pass
    else:
        print("Commande inconnue : '"+st+"'. Tapez help pour afficher la liste des commandes.")
    invite()
