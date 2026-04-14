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

#include <random>
#include <vector>

using namespace godot;

class PointCloudLoader : public RefCounted
{
	GDCLASS(PointCloudLoader, RefCounted)

protected:
	static void _bind_methods()
	{
		godot::ClassDB::bind_method(D_METHOD("load_random", "point_count"), &PointCloudLoader::load_random);
		godot::ClassDB::bind_method(D_METHOD("load_random_range", "point_count", "min_pos", "max_pos"), &PointCloudLoader::load_random_range);
		ADD_SIGNAL(MethodInfo("chunk_loaded", PropertyInfo(Variant::Type::OBJECT, "mesh")));
	}

public:
	PointCloudLoader() = default;
	~PointCloudLoader() override = default;

	// Generate random point cloud with default range (-100 to 100)
	void load_random(int point_count)
	{
		load_random_range(point_count, -100.0f, 100.0f);
	}

	// Generate random point cloud with custom range
	void load_random_range(int point_count, float min_pos, float max_pos)
	{
		if (point_count <= 0)
		{
			return;
		}

		// Random number generators
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> position_dist(min_pos, max_pos);
		std::uniform_real_distribution<float> color_dist(0.0f, 1.0f);

		// Create arrays for mesh data
		PackedVector3Array positions;
		PackedColorArray colors;

		positions.resize(point_count);
		colors.resize(point_count);

		// Generate random points
		for (int i = 0; i < point_count; i++)
		{
			positions[i] = Vector3(
				position_dist(gen),
				position_dist(gen),
				position_dist(gen)
			);

			colors[i] = Color(
				color_dist(gen),
				color_dist(gen),
				color_dist(gen),
				1.0f  // Alpha
			);
		}

		// Create mesh
		Ref<ArrayMesh> mesh;
		mesh.instantiate();

		Array array;
		array.resize(Mesh::ARRAY_MAX);
		array[Mesh::ARRAY_VERTEX] = positions;
		array[Mesh::ARRAY_COLOR] = colors;

		mesh->add_surface_from_arrays(Mesh::PrimitiveType::PRIMITIVE_POINTS, array);
		emit_signal("chunk_loaded", mesh);
	}
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

extern "C"
{
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
