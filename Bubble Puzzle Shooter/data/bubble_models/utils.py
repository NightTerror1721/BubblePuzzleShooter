from BPS import createBubbleModel, existsBubbleModel
from importlib import import_module

model = None

def importModel(modelName: str):
    if existsBubbleModel(modelName):
        print("Model \"%s\" has already imported." % modelName)
        return

    global model
    model = createBubbleModel(modelName)
    module = import_module("." + modelName, "data.bubble_models")

    # Set imported model properties #
    model.colorType = module.colorType

    model.floating = module.floating
    model.destroyInBottom = module.destroyInBottom
    model.requireDestroyToClear = module.requireDestroyToClear
    model.onlyBoardColorInArrowGen = module.onlyBoardColorInArrowGen

    model.resistence = module.resistence

    model.pointsOfTurnsToDown = module.pointsOfTurnsToDown

    model.localInts = module.localInts
    model.localFloats = module.localFloats
    model.localStrings = module.localStrings

    model.init = module.init
    model.onCollide = module.onCollide
    model.onInserted = module.onInserted
    model.onExplode = module.onExplode
    model.onNeighborInserted = module.onNeighborInserted
    model.onNeighborExplode = module.onNeighborExplode

    print("Bubble Model \"%s\" imported successfuly!" % model.name)
    model = None

