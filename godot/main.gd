extends Node3D

var loader := PointCloudLoader.new()
var mesh_instance: MeshInstance3D

func _ready() -> void:
	loader.chunk_loaded.connect(_loader_chunk_loaded)
	mesh_instance = $HSplitContainer/SubViewportContainer/SubViewport/MeshInstance3D

func _loader_chunk_loaded(mesh: Mesh) -> void:
	var material = StandardMaterial3D.new()
	material.flags_use_point_size = true
	material.point_size = 4.0
	material.vertex_color_use_as_albedo = true

	mesh.surface_set_material(0, material)
	mesh_instance.mesh = mesh

func _on_button_pressed() -> void:
	$E57FileDialog.show()

func _on_e_57_file_dialog_file_selected(path: String) -> void:
	var point_count := 10000
	var min_pos := -5.0
	var max_pos := 5.0

	loader.load_random_range(point_count, min_pos, max_pos)
