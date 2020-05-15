from BPS import createBubbleModel, existsBubbleModel
from importlib import import_module

model = None

def importModel(modelName: str):
    if existsBubbleModel(modelName):
        print("Model \"%s\" has already imported." % modelName)
        return

    global model
    model = createBubbleModel(modelName)
    import_module("." + modelName, "data.bubble_models")
    print("Bubble Model \"%s\" imported successfuly!" % model.name)
    model = None

