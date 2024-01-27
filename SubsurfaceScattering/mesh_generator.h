#pragma once

#include "mesh.h"
#include "textured_mesh.h"

class MeshGenerator {
  public:
	static void generate_grid(LineMesh &mesh, unsigned int half_x_count,
							  unsigned int half_z_count, float x_length,
							  float z_length);
	static void load_from_file(TriMesh &mesh, const char *filename,
							   bool normalize = false);
	static void load_from_common_file(TriMesh &mesh, const char *filename);
	static void load_from_common_file(TexturedTriMesh &mesh,
									  const char *filename);
	static void load_textures(TexturedTriMesh &mesh, const char *color_texture,
							  const char *normal_texture);
	static void generate_cube(TriMesh &mesh);
	static void generate_inverted_cube(TriMesh &mesh);
	static void generate_cylinder(TriMesh &mesh, float radius, float height,
								  unsigned int circle_divisions);
};