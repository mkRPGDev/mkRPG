import gui
import const

if __name__ == "__main__":

    ui = gui.ServerUI()

    while True:
        if not ui.running:
            break
        ui.update()
