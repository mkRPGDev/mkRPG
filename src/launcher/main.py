import gui
import const

if __name__ == "__main__":

    ui = gui.MainUI()

    while True:
        if not ui.running:
            break
        ui.update()
