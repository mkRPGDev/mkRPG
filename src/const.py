VERBOSE = False

# ! PATH par défaut en l'absence d'argument
PATH = "../test/snake/" # et on oublie pas le dernier /

MAXFPS = 60
UPDTIME = 1/MAXFPS

# Réseau
HOST = "localhost"
#HOST = "paquebot.ens.fr"
PORT = 12345
IPV6 = False#True
USETCP = True
BUFF = 1024
CODING = "ascii"

#sudo tcpdump -i lo -X port 13245 or 12346
#sudo netstat -apn | grep 12345

