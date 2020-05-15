#include "py.h"

#include <pybind11/operators.h>
#include <pybind11/functional.h>

#include <SFML/Graphics/Transformable.hpp>

#include "audio.h"
#include "assets.h"
#include "bubble.h"
#include "resources.h"

#define PY(name) __py__##name
#define BINDED(type) _##type
#define IF_BINDED(type, binded_var) type* binded_var = BINDED(type); if(binded_var)

#define DEF_PY_FUNC(module, name, function) (module).def((name), &__py__##function)
#define PUSH_PY_FUNC(module, function) DEF_PY_FUNC(module, #function, function)
#define PUSH_PY_FUNC_EXT(module, function, policy) (module).def(#function, &__py__##function, policy)

#define PUSH_PY_MET(py_class, _class, method) (py_class).def(#method, &_class::##method)


namespace py = pybind11;

namespace pylib
{
	static void loadResourceCache(Path cachePath)
	{
		Path filepath = cachePath + "cache.py";
		executePythonScript(filepath);
	}


	py::object executePythonScript(const std::string& filepath)
	{
		try
		{
			return py::eval_file(filepath);
		}
		catch (py::error_already_set ex)
		{
			std::cout << "Python exception occurs: " << ex.what() << std::endl;
			return py::none();
		}
		catch (std::exception& ex)
		{
			std::cout << "Unexpected exception occurs: " << ex.what() << std::endl;
			return py::none();
		}
	}

	void loadResourceCaches()
	{
		loadResourceCache(ResourcePoint::Audio);
		loadResourceCache(ResourcePoint::Textures);
		loadResourceCache(ResourcePoint::BubbleModels);
	}
}


/* PY FUNCTIONS */
namespace
{
	bool PY(loadSound) (const std::string& filename, const std::string& tag)
	{
		return SoundManager::root().load(filename, tag);
	}

	bool PY(loadTexture) (const std::string& filename, const std::string& tag)
	{
		return TextureManager::root().load(filename, tag);
	}

	bool PY(loadTextureFromPart) (const std::string& filename, const std::string& tag, int x, int y, int w, int h)
	{
		return TextureManager::root().load(filename, tag, x, y, w, h);
	}

	BubbleModel* PY(createBubbleModel) (const std::string& name)
	{
		if (BubbleModelManager::hasModel(name))
			return nullptr;

		Ref<BubbleModel> model = BubbleModelManager::createModel(name);
		return &model;
	}

	bool PY(existsBubbleModel) (const std::string& name)
	{
		return BubbleModelManager::hasModel(name);
	}
}


PYBIND11_EMBEDDED_MODULE(BPS, m) {
	/* Sound */
	PUSH_PY_FUNC(m, loadSound);


	/* Texture */
	PUSH_PY_FUNC(m, loadTexture);
	PUSH_PY_FUNC(m, loadTextureFromPart);


	/* BubbleModel */
	py::class_<BubbleModel> model{ m, "BubbleModel" };

	model.def(py::self == py::self);
	model.def(py::self != py::self);

	model.def_readonly("name", &BubbleModel::name);

	model.def_readwrite("colorType", &BubbleModel::colorType);

	model.def_readwrite("floating", &BubbleModel::floating);
	model.def_readwrite("destroyInBottom", &BubbleModel::destroyInBottom);
	model.def_readwrite("requireDestroyToClear", &BubbleModel::requireDestroyToClear);

	model.def_readwrite("resistence", &BubbleModel::resistence);

	model.def_readwrite("pointsOfTurnsToDown", &BubbleModel::pointsOfTurnsToDown);

	model.def_readwrite("localInts", &BubbleModel::localInts);
	model.def_readwrite("localFloats", &BubbleModel::localFloats);
	model.def_readwrite("localStrings", &BubbleModel::localStrings);

	model.def_readwrite("init", &BubbleModel::init);

	model.def_readwrite("onCollide", &BubbleModel::onCollide);
	model.def_readwrite("onInserted", &BubbleModel::onInserted);
	model.def_readwrite("onExplode", &BubbleModel::onExplode);
	model.def_readwrite("onNeighborInserted", &BubbleModel::onNeighborInserted);
	model.def_readwrite("onNeighborExplode", &BubbleModel::onNeighborExplode);


	PUSH_PY_FUNC_EXT(m, createBubbleModel, py::return_value_policy::reference);
	PUSH_PY_FUNC(m, existsBubbleModel);



	/* Vector2D */
	py::class_<Vec2f> v2f{ m, "Vector2" };

	v2f.def_readwrite("x", &Vec2f::x);
	v2f.def_readwrite("y", &Vec2f::y);

	v2f.def(py::init<>());
	v2f.def(py::init<float, float>());
	v2f.def(py::init<Vec2f>());

	v2f.def(-py::self);
	v2f.def(py::self += py::self);
	v2f.def(py::self -= py::self);
	v2f.def(py::self + py::self);
	v2f.def(py::self - py::self);
	v2f.def(py::self * float{});
	v2f.def(float{} * py::self);
	v2f.def(py::self *= float{});
	v2f.def(py::self / float{});
	v2f.def(py::self /= float{});
	v2f.def(py::self == py::self);
	v2f.def(py::self != py::self);



	/* Object */
	py::class_<Object> baseObj{ m, "Obj" };



	/* Transformable */
	py::class_<sf::Transformable> t{ m, "Transformable" };



	/* AnimatedSprite */
	py::class_<AnimatedSprite> as{ m, "AnimatedSprite" };

	as.def("setFrameDimensions", &AnimatedSprite::setFrameDimensions);
	as.def("setFrameCount", &AnimatedSprite::setFrameCount);

	as.def("setStaticMode", &AnimatedSprite::setStaticMode);
	as.def("setSequenceMode", &AnimatedSprite::setSequenceMode);
	as.def("setLoopMode", &AnimatedSprite::setLoopMode);
	as.def("setRandomMode", &AnimatedSprite::setRandomMode);

	as.def("isStaticMode", &AnimatedSprite::isStaticMode);
	as.def("isSequenceMode", &AnimatedSprite::isSequenceMode);
	as.def("isLoopMode", &AnimatedSprite::isLoopMode);
	as.def("isRandomMode", &AnimatedSprite::isRandomMode);

	as.def("hasEnded", &AnimatedSprite::hasEnded);

	as.def("start", &AnimatedSprite::start);
	as.def("stop", &AnimatedSprite::stop);

	as.def("setCurrentFrame", &AnimatedSprite::setCurrentFrame);
	as.def("setExactCurrentFrame", &AnimatedSprite::setExactCurrentFrame);

	as.def("getCurrentFrame", &AnimatedSprite::getCurrentFrame);
	as.def("getExactCurrentFrame", &AnimatedSprite::getExactCurrentFrame);

	as.def("rewind", &AnimatedSprite::rewind);
	as.def("fastForward", &AnimatedSprite::fastForward);

	as.def("getFrameX", &AnimatedSprite::getFrameX);
	as.def("getFrameY", &AnimatedSprite::getFrameY);
	as.def("getFrameWidth", &AnimatedSprite::getFrameWidth);
	as.def("getFrameHeight", &AnimatedSprite::getFrameHeight);
	as.def("getFrameCount", &AnimatedSprite::getFrameCount);



	/* BubbleColor */
	py::class_<BubbleColor> bc{ m, "BubbleColor" };

	bc.def(py::self == py::self);
	bc.def(py::self != py::self);
	bc.def(py::self > py::self);
	bc.def(py::self < py::self);
	bc.def(py::self >= py::self);
	bc.def(py::self <= py::self);

	bc.def_property_readonly("code", [](BubbleColor* self) { return self->code(); });
	bc.def_property_readonly("name", [](BubbleColor* self) { return self->name(); });



	/* ColorType */
	py::enum_<BubbleColorType> ct{ m, "BubbleColorType" };
	ct.value("Colorless", BubbleColorType::Colorless);
	ct.value("NormalColor", BubbleColorType::NormalColor);
	ct.value("MultiColor", BubbleColorType::MultiColor);
	ct.export_values();



	/* Bubble */
	py::class_<Bubble, Object, sf::Transformable> b{ m, "Bubble", py::multiple_inheritance() };

	b.def("getModel", [](Bubble* self) { return &(*self->getModel()); }, py::return_value_policy::reference);

	b.def("hasExploited", &Bubble::hasExploited);
	b.def("explode", &Bubble::explode);

	b.def("getPosition", &Bubble::getPosition);
	b.def("getSpeed", &Bubble::getSpeed);
	b.def("getAcceleration", &Bubble::getAcceleration);

	b.def("getColor", &Bubble::getColor);
	b.def("getColorType", &Bubble::getColorType);
	b.def("colorMatch", &Bubble::colorMatch);
	b.def("isNormalColor", [](Bubble* self) { return self->getColorType() == BubbleColorType::NormalColor; });
	b.def("isMulticolor", [](Bubble* self) { return self->getColorType() == BubbleColorType::MultiColor; });
	b.def("isColorless", [](Bubble* self) { return self->getColorType() == BubbleColorType::Colorless; });

	b.def("getSprite", static_cast<AnimatedSprite * (Bubble::*)()>(&Bubble::getSprite));

	b.def("getResistence", &Bubble::getResistence);
	b.def("isIndestructible", &Bubble::isIndestructible);
	b.def("isFloating", &Bubble::isFloating);
	b.def("destroyInBottom", &Bubble::destroyInBottom);
	b.def("requireDestroyToClear", &Bubble::requireDestroyToClear);
	b.def("getPointsOfTurnsToDown", &Bubble::getPointsOfTurnsToDown);

	b.def("getLocalInt", &Bubble::getLocalInt);
	b.def("getLocalFloat", &Bubble::getLocalFloat);
	b.def("getLocalString", &Bubble::getLocalString);

	b.def("setLocalInt", &Bubble::setLocalInt);
	b.def("setLocalFloat", &Bubble::setLocalFloat);
	b.def("setLocalString", &Bubble::setLocalString);
}
