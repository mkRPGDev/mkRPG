import sys
from os import path as ospath

try:
    sys.path.append(ospath.dirname(ospath.abspath(ospath.dirname(__file__))))
    import src.path as path
except OSError:
    sys.exit(-1)


def test_dir_methods():
    print("Unimplemented")

def test_files_methods():
    print("Unimplemented")


if __name__=="__main__":
    archi = path.Archi()
    print(archi.main_directory)

