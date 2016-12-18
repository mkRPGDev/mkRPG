from interface.trans import applyMatrix
from interface.utils import load_png
from pygame.locals import QUIT
import pygame

pygame.display.init()
sc=pygame.display.set_mode((400,400))
sc.fill((255,255,255))
path = "../../test/perso.png"#"editor/Icons/herbe.png"
mat  = [-0.5,-1,-0.5,0.7]

s=load_png(path)
s=applyMatrix(s, mat)

sc.blit(s,(50,50))
pygame.display.flip()

from PyQt4.QtGui import *
from PyQt4.QtCore import *

app = QApplication([])
lab = QLabel()
im = QImage(path)
im=im.transformed(QMatrix(*mat,0,0),Qt.SmoothTransformation)
lab.setPixmap(QPixmap(im))
lab.show()
app.exec()
