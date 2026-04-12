#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <gdextension_interface.h>

using namespace godot;

class PointCloudLoader : public RefCounted {
	GDCLASS(PointCloudLoader, RefCounted)

protected:
	static void _bind_methods()
	{
		godot::ClassDB::bind_method(D_METHOD("load", "path"), &PointCloudLoader::load);
		ADD_SIGNAL(MethodInfo("chunk_loaded", PropertyInfo(Variant::Type::OBJECT, "mesh")));
	}

public:
	PointCloudLoader() = default;
	~PointCloudLoader() override = default;

	void load(const String &path)
	{
		Ref<ArrayMesh> mesh;
		mesh->surface_get_arrays(0);
		mesh.instantiate();

		auto array = Array();
		array[Mesh::ARRAY_VERTEX] = PackedVector3Array({ Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0) });

		mesh->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_POINTS, array);
		emit_signal("chunk_loaded", mesh);
	}

	//void print_type(const Variant& p_variant) const
	//{
	//	print_line(vformat("Type: %d", p_variant.get_type()));
	//}
};

void initialize_example_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
	{
		return;
	}
	GDREGISTER_CLASS(PointCloudLoader);
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
	GDExtensionBool GDE_EXPORT native_init(
		GDExtensionInterfaceGetProcAddress p_get_proc_address,
		GDExtensionClassLibraryPtr p_library,
		GDExtensionInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
