from data.bubble_models.utils import model
from BPS import BubbleColorType

def init(bubble, color, editorMode):
    pass
    
def onCollide(self, other):
    pass
    
def onInserted(self):
    pass

def onExplode(self):
    pass
    
def onNeighborInserted(bubble, color, editorMode):
    pass

def onNeighborExplode(bubble, color, editorMode):
    pass

model.colorType = BubbleColorType.NormalColor

model.floating = False
model.destroyInBottom = False
model.requireDestroyToClear = False

model.resistence = 0

model.pointsOfTurnsToDown = 1.0

model.localInts = 0
model.localFloats = 0
model.localStrings = 0

model.init = init
model.onCollide = onCollide
model.onInserted = onInserted
model.onExplode = onExplode
model.onNeighborInserted = onNeighborInserted
model.onNeighborExplode = onNeighborExplode
    
