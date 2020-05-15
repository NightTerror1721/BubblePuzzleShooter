from BPS import loadTexture
from BPS import loadTextureFromPart
from importlib import import_module
import os
import pathlib

def importTexture(path: str, tag: str, bounds: set = None):
    state = False
    if bounds:
        state = loadTextureFromPart(path, tag, int(bounds[0]), int(bounds[1]), int(bounds[2]), int(bounds[3]))
    else:
        state = loadTexture(path, tag)
    if state:
        print("Texture \"%s\" from file \"%s\" loaded!" % (tag, path))

def importTextureFolder(folderName: str):
    path = "data.textures." + folderName
    import_module(".cache", path)

def currentFolder():
    return pathlib.Path(__file__).parent.name

class TextureImporter:
    def __init__(self, *currentPath):
        self.__path = ""
        for p in currentPath:
            self.__path = os.path.join(self.__path, p)
    
    def importTexture(self, path: str, tag: str, bounds: set = None):
        path = os.path.join(self.__path, path)
        importTexture(path, tag, bounds)
