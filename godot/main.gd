extends Node3D

var loader := PointCloudLoader.new()

func _ready() -> void:
	loader.chunk_loaded.connect(_loader_chunk_loaded)

func _loader_chunk_loaded(mesh: Mesh) -> void:
	pass

func _on_button_pressed() -> void:
	$E57FileDialog.popup_file_dialog()

func _on_e_57_file_dialog_file_selected(path: String) -> void:
	var mesh_instance = $HSplitContainer/SubViewportContainer/SubViewport/MeshInstance3D

	var point_count := 10000
	var vertices := PackedVector3Array()
	var colors := PackedColorArray()
	vertices.resize(point_count)
	colors.resize(point_count)

	var rng = RandomNumberGenerator.new()
	rng.randomize()

	for i in point_count:
		vertices[i] = Vector3(
			rng.randf_range(-5.0, 5.0),
			rng.randf_range(-5.0, 5.0),
			rng.randf_range(-5.0, 5.0)
		)
		colors[i] = Color(
			rng.randf_range(0.6, 1.0),
			rng.randf_range(0.6, 1.0),
			rng.randf_range(0.6, 1.0),
			1.0
		)

	var arrays = []
	arrays.resize(Mesh.ARRAY_MAX)
	arrays[Mesh.ARRAY_VERTEX] = vertices
	arrays[Mesh.ARRAY_COLOR] = colors

	var array_mesh = ArrayMesh.new()
	array_mesh.add_surface_from_arrays(Mesh.PRIMITIVE_TRIANGLES, arrays)

	var material = StandardMaterial3D.new()
	material.flags_use_point_size = true
	material.point_size = 4.0
	material.vertex_color_use_as_albedo = true

	array_mesh.surface_set_material(0, material)

	mesh_instance.mesh = array_mesh
