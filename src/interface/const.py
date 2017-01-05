"""
    This module contains constants usefull for the GUI
"""

from math import pi

SCREEN_WIDTH = 1366
SCREEN_HEIGHT = 768
CELL_WIDTH = 32
ANGLE = 45
FLATTENING = 0
MOV_OFFSET = 40
FLAT_STEP = 0.1
ROT_STEP = 5

# Default values for the gui_loading
DEFAULT_GUI_VALUES = {
        'Container' : {
            'size' : (int(0.4*SCREEN_WIDTH),int(0.4*SCREEN_HEIGHT)),
            'background_color': (0,0,0,255),
            'background_image' : 'apple.png',
            'background_pattern' : 'repeat'
            },
        'TextField' : {
            'size' :  (int(0.4*SCREEN_WIDTH),int(0.2*SCREEN_HEIGHT)),
            'text_size' : 10,
            'text_color' : (0, 0, 0, 255),
            'text_font' : None,
            'text_align' : 'left',
            },
        'ScrollableTextField': {
            'size' :  (int(0.4*SCREEN_WIDTH),int(0.2*SCREEN_HEIGHT)),
            'text_size' : 10,
            'text_color' : (0, 0, 0, 255),
            'text_font' : None,
            'text_align' : 'left',
            'scroll_step' : 10
            },
        'Button' : {
            'size' :  (int(0.4*SCREEN_WIDTH),int(0.2*SCREEN_HEIGHT)),
            'text_size' : 10,
            'text_color' : (0, 0, 0, 255),
            'text_font' : None,
            'text_align' : 'left',
            'text_interline' : 4,
            'scroll_step' : 10,
            'mover_background_image' : 'apple.png',
            'mover_background_color' : (0, 0, 0, 255),
            'click_background_image' : 'apple.png',
            'click_background_image' : (0, 0, 0, 255),
            'action' : 'None',
            }
        }
