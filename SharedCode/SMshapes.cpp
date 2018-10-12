#include "SMshapes.h"
#ifdef __WINDOWS__
#include <cmath>
#else
#include <math.h>
#endif

#include <algorithm>


#define PI 3.14159268f
#define DEPTH2D -10.f

namespace SMshapes {


	float length2D(vec2f v) { return sqrt(v.x*v.x + v.y*v.y); }

	float angleBetweenTwoVectors2D(vec2f v1, vec2f v2)
	{
		double prod = v1.x*v2.y - v1.y*v2.x;
		return fabs((prod < 0 ? -1.0f : 1.0f)*acosf((v1.x*v2.x + v1.y*v2.y) / (length2D(v1)*length2D(v2))));
	}

	vec2f dot(vec2f p1, vec2f p2)
	{
		//returns a normalized dot product ...
		float a = p2.x - p1.x;
		float b = p1.y - p2.y;
		float s = sqrt(a*a + b*b);
		if (s > 0.0f) return vec2f(b / s, a / s);
		return vec2f(0.0f, 0.0f);
	}

	SMmesh plane(vec3f pos, vec3f size, vec2f uvsize)
	{
		SMmesh plane("Plane");
		float *cols = nullptr;

		vec3f n(0, -1.f, 0);
		plane.addPackedVert2(pos + vec3f(size.x, size.y, 0), n, uvsize, cols);
		plane.addPackedVert2(pos + vec3f(size.x*-1.f, size.y*-1.f, 0), n, vec2f(0, 0), cols);
		plane.addPackedVert2(pos + vec3f(size.x*-1.f, size.y, 0), n, vec2f(0, uvsize.y), cols);

		plane.addPackedVert2(pos + vec3f(size.x, size.y*-1.f, 0), n, vec2f(uvsize.x, 0), cols);
		plane.addPackedVert2(pos + vec3f(size.x*-1.f, size.y*-1.f, 0), n, vec2f(0, 0), cols);
		plane.addPackedVert2(pos + vec3f(size.x, size.y, 0), n, uvsize, cols);

		plane.bbox.bboxFromVerts(plane.verts, 0, plane.vc, plane.stride);
		plane.materialRef = 0;
		return plane;
	}

	SMmesh sprite(vec2f pos, vec2f size, vec2f uvsize)
	{
		SMmesh sprite("sprite");
		float *cols = nullptr;

		vec3f n(0, -1.f, 0);
		vec3f pos3(pos.x, pos.y, 0);
		sprite.addPackedVert2(pos3 + vec3f(size.x, size.y, DEPTH2D), n, uvsize, cols);
		sprite.addPackedVert2(pos3 + vec3f(0, 0, DEPTH2D), n, vec2f(0, 0), cols);
		sprite.addPackedVert2(pos3 + vec3f(0, size.y, DEPTH2D), n, vec2f(0, uvsize.y), cols);

		sprite.addPackedVert2(pos3 + vec3f(size.x, 0, DEPTH2D), n, vec2f(uvsize.x, 0), cols);
		sprite.addPackedVert2(pos3 + vec3f(0, 0, DEPTH2D), n, vec2f(0, 0), cols);
		sprite.addPackedVert2(pos3 + vec3f(size.x, size.y, DEPTH2D), n, uvsize, cols);

		sprite.bbox.bboxFromVerts(sprite.verts, 0, sprite.vc, sprite.stride);
		sprite.materialRef = 0;
		return sprite;
	}

	float cubeOffs[72] = { -1,1,-1, 1,1,-1, 1,-1,-1, -1,-1,-1,
		1,1,-1, 1,1,1, 1,-1,1, 1,-1,-1,
		1,1,1, -1,1,1, -1,-1,1, 1,-1,1,
		-1,1,1, -1,1,-1, -1,-1,-1, -1,-1,1,
		-1,-1,-1, 1,-1,-1, 1,-1,1, -1,-1,1,
		1,1,-1, -1,1,-1, -1,1,1, 1,1,1 };

	float cnorms[18] = { 0,0,-1.0f, 1.0f,0,0, 0,0,1.0f, -1.0f,0,0, 0,-1.0f,0, 0,1.0f,0 };

	SMmesh cuboid(vec3f pos, vec3f size)
	{
		SMmesh cube("Cuboid");
		float *cols = nullptr;

		for (int i = 0; i < 6; i++)
		{
			int v = i * 12;
			vec3f n = vec3f(cnorms[i * 3], cnorms[i * 3 + 1], cnorms[i * 3 + 2]);
			cube.addPackedVert2(pos + vec3f(size.x*cubeOffs[v + 6], size.y*cubeOffs[v + 7], size.z*cubeOffs[v + 8]), n, vec2f(1, 1), cols);
			cube.addPackedVert2(pos + vec3f(size.x*cubeOffs[v + 3], size.y*cubeOffs[v + 4], size.z*cubeOffs[v + 5]), n, vec2f(0, 1), cols);
			cube.addPackedVert2(pos + vec3f(size.x*cubeOffs[v], size.y*cubeOffs[v + 1], size.z*cubeOffs[v + 2]), n, vec2f(0, 0), cols);

			cube.addPackedVert2(pos + vec3f(size.x*cubeOffs[v + 9], size.y*cubeOffs[v + 10], size.z*cubeOffs[v + 11]), n, vec2f(1, 0), cols);
			cube.addPackedVert2(pos + vec3f(size.x*cubeOffs[v + 6], size.y*cubeOffs[v + 7], size.z*cubeOffs[v + 8]), n, vec2f(1, 1), cols);
			cube.addPackedVert2(pos + vec3f(size.x*cubeOffs[v], size.y*cubeOffs[v + 1], size.z*cubeOffs[v + 2]), n, vec2f(0, 0), cols);
		}

		cube.bbox.bboxFromVerts(cube.verts, 0, cube.vc, cube.stride);
		cube.materialRef = 0;
		return cube;
	}
	
	void calcPathNormals(std::vector<vec2f> &path, std::vector<vec2f> &normals, float creaseAngle, bool joined)
	{
		//Splits a path by creasAngle threshold
		//If the angle between two lines is greater than the creaseAngle, then split them.

		uint32_t sz = path.size();
		if (sz < 2) return;

		std::vector<vec2f> newPath;
		vec2f p1;
		vec2f p2 = path[0];
		vec2f p3 = path[1];
		vec2f norm;

		//work out first point in path
		newPath.push_back(p2);
		if (joined && sz>2) {
			p1 = path[sz - 2]; 
			if (angleBetweenTwoVectors2D(p2 - p1, p3 - p2) > 0.5f) {
				normals.push_back(dot(p2, p3));
			}
			else {
				normals.push_back(dot(p1 - p2, p3 - p2));
			}
			p1 = path[0];
		}
		else
		{
			p1 = path[0];
			normals.push_back(dot(p2, p3));
		}
		

		//work out normals and points between end and last points
		if (sz > 2) {
			for (size_t i = 1; i < sz - 1; i++) {
				p2 = path[i]; p3 = path[i + 1];
				newPath.push_back(p2);
				if (angleBetweenTwoVectors2D(p2 - p1, p3 - p2) > 0.5f) {
					normals.push_back(dot(p1, p2));
					newPath.push_back(p2);
					normals.push_back(dot(p2, p3));
				}
				else {
					normals.push_back(dot(p1 - p2, p3 - p2));
				}
				p1 = p2;
			}

			//work out last point in path
			p2 = path[sz - 1];
			newPath.push_back(p2);

			if (joined) {
				p3 = path[1];
				if (angleBetweenTwoVectors2D(p2 - p1, p3 - p2) > 0.5f) {
					normals.push_back(dot(p1, p2));
				}
				else {
					normals.push_back(dot(p1 - p2, p3 - p2));
				}
			}
			else
			{
				normals.push_back(normals[normals.size() - 1]);
			}
		}
		else
		{
			normals.push_back(normals[0]);
			newPath.push_back(p3);
		}

		path = newPath;
	}

	void copyVerts(std::vector<float> &srcverts, std::vector<float> &destverts, uint32_t stride, uint32_t &vc, uint32_t ref)
	{
		if (vc + stride > destverts.size()) destverts.resize(vc + 1000);
		size_t p = ref*stride;
		for (size_t r = 0; r < stride; r++) destverts[vc++] = srcverts[p + r];
	}

	SMmesh lathe(std::string name, vec3f pos, std::vector<vec2f> &path, int sides, float rise, float coils, bool invert, float creaseAngle, vec2f prevPoint, vec2f lastPoint, uint32_t uvtype)
	{
		SMmesh model(name);
		
				/*
		float cols[4];
		cols[0] = (col & 255) / 255.f;
		cols[1] = ((col >> 8) & 255) / 255.f;
		cols[2] = ((col >> 16) & 255) / 255.f;
		cols[3] = ((col >> 24) & 255) / 255.f;
		*/

		float *cols = nullptr;

		size_t sz = path.size();
		if (sz < 2) return model;

		std::vector<vec2f> normals;
		calcPathNormals(path, normals, 60, true); // path[0] == path[sz - 1]);
		sz = path.size();

		int32_t rl = (int32_t)(sides*coils);
		float inv = (invert) ? -1.0f : 1.0f;
		float tcy = 0.0f;
		float tcx = 1.0f / (float)sides;
		float pr = (PI / (float)sides) * 2.f;
		float rdiv = rise / (float)rl;

		//Find min y / max y of path
		float miny = path[0].y; float maxy = miny;
		for (size_t p = 0; p < sz; p++) {
			if (path[p].y < miny) miny = path[p].y;
			if (path[p].y > maxy) maxy = path[p].y;
		}
		float cy = (maxy + miny) / 2.f;

		std::vector<float> vertTemp;
		uint32_t tc = 0;

		for (size_t p = 0; p < sz; p++) {

			//Apply UV mapping ...
			switch (uvtype) {
			case UVMAP_CYLINDER: //Cylinder UV mapping
				tcy = 1.0f - ((path[p].y - miny) / (maxy - miny));
				break;
			case UVMAP_SPHERE: //Spherical UV mapping
				tcy = atan2f(path[p].x, path[p].y - cy) / PI;
				if (tcy < 0.0f) tcy = 0.9999f;
				break;
			}

			float risey = path[p].y;
			for (int r = 0; r < rl; r++) {
				float sinr = sin(pr*r*inv);
				float cosr = cos(pr*r*inv);
				model.addPackedVert(vertTemp, tc, pos + vec3f(path[p].x*sinr, risey, path[p].x*cosr), vec3f(sinr*normals[p].x, normals[p].y, cosr*normals[p].x), vec2f(tcx*r, tcy), cols);
				risey += rdiv;
			}

			model.addPackedVert(vertTemp, tc, pos + vec3f(0, path[p].y, path[p].x), vec3f(0, normals[p].y, normals[p].x), vec2f(0.9999f, tcy), cols);
		}

		model.stride = 8;

		int pn = 0;
		int pp = 0;
		for (size_t p = 0; p < (sz-1); p++) {
			pn += (rl + 1);
			if ((path[p + 1] != path[p]) || p==(sz-2)) {
				for (int32_t r = 0; r < rl; r++)
				{
					//GL_TRIANGLES
					copyVerts(vertTemp, model.verts, model.stride, model.vc, pn + r);
					copyVerts(vertTemp, model.verts, model.stride, model.vc, pp + r + 1);
					copyVerts(vertTemp, model.verts, model.stride, model.vc, pp + r);

					copyVerts(vertTemp, model.verts, model.stride, model.vc, pn + r + 1);
					copyVerts(vertTemp, model.verts, model.stride, model.vc, pp + r + 1);
					copyVerts(vertTemp, model.verts, model.stride, model.vc, pn + r);
				}
			}
			pp += (rl + 1);
		}

		model.bbox.bboxFromVerts(model.verts, 0, model.vc, model.stride);
		model.mode = GL_TRIANGLES;
		return model;
	}

	SMmesh sphere(vec3f pos, float radius, float hemi, int slices, int sides, bool invert)
	{
		std::vector<vec2f> path;
		float st = (PI*(1.0f - hemi)) / (float)slices;
		float inv = invert ? -1.0f : 1.0f;
		for (int r = 0; r < (slices + 1); r++) {
			path.push_back(vec2f(radius*-sin(r*st)*inv, radius*-cos(r*st)));
		}
		return lathe("Sphere", pos, path, sides, 0.0f, 1.0f, false, 90, vec2f(1, path[0].y), vec2f(-1, path[path.size() - 1].y),UVMAP_SPHERE);
	}

	SMmesh torus(vec3f pos, float radius, float ringradius, int ringrots, int sides)
	{
		std::vector<vec2f> path;
		float st = (PI *2.f) / (float)ringrots;
		for (int r = 0; r < (ringrots + 1); r++) {
			path.push_back(vec2f(radius + ringradius*sin(r*st), ringradius*cos(r*st)));
		}
		return lathe("Torus", pos, path, sides, 0.0f, 1.0f, true);
	}

	SMmesh spring(vec3f pos, float radius, float length, int coils, float coilradius, int ringrots, int sides)
	{
		std::vector<vec2f> path;
		//float rise = length / coils;
		float st = (PI * 2.f) / (float)ringrots;
		for (int r = 0; r < (ringrots + 1); r++) {
			path.push_back(vec2f(radius + coilradius*sin(r*st), coilradius*cos(r*st)));
		}
		return lathe("Spring",pos, path, sides, length, (float)coils);
	}

	SMmesh cone(vec3f pos, float radius, float height, int sides)
	{
		std::vector<vec2f> path;
		path.push_back(vec2f(0, height));
		path.push_back(vec2f(radius, 0));
		path.push_back(vec2f(0, 0));
		return lathe("Cone",pos, path, sides, 0.0f, 1.0f, true, 60.0f, vec2f(-radius, height*2), vec2f(-radius, 0));
	}

	SMmesh tcone(vec3f pos, float botradius, float topradius, float height, int sides)
	{
		std::vector<vec2f> path;
		path.push_back(vec2f(0, height));
		path.push_back(vec2f(topradius, height));
		path.push_back(vec2f(botradius, 0));
		path.push_back(vec2f(0, 0));
		return lathe("Truncated Cone", pos, path, sides, 0.0f, 1.0f, true, 60.0f, vec2f(-1.0f,height));
	}

	SMmesh cylinder(vec3f pos, float radius, float height, int sides)
	{
		std::vector<vec2f> path;
		path.push_back(vec2f(0, height));
		path.push_back(vec2f(radius, height));
		path.push_back(vec2f(radius, 0));
		path.push_back(vec2f(0, 0));
		return lathe("Cylinder", pos, path, sides, 0.0f, 1.0f, true, 60.0f, vec2f(-1.0f, height));
	}

	SMmesh tube( vec3f pos, float radius1, float radius2, float height, int sides)
	{
		std::vector<vec2f> path;
		path.push_back(vec2f(radius1, height));
		path.push_back(vec2f(radius2, height));
		path.push_back(vec2f(radius2, 0));
		path.push_back(vec2f(radius1, 0));
		path.push_back(vec2f(radius1, height));
		return lathe( "Tube", pos, path, sides, 0.0f, 1.0f, true);
	}
	
	void texMap(std::vector<float> &vertBuffer, uint32_t vc, uint32_t stride, uint32_t uvoffset, uint8_t x, uint8_t y)
	{
		//Get X,Y bounds of vertices ...
		float minx = 1e20f; float maxx = -1e20f;
		float miny = 1e20f;float maxy = -1e20f;		
		for (size_t i = 0; i < vc; i += stride) {
			if (vertBuffer[i+x] < minx) minx = vertBuffer[i+x];
			if (vertBuffer[i+x] > maxx) maxx = vertBuffer[i+x];
			if (vertBuffer[i+y] < miny) miny = vertBuffer[i+y];
			if (vertBuffer[i+y] > maxy) maxy = vertBuffer[i+y];
		}

		float left = minx;
		float top = miny;
		float w = 1.f / (maxx-minx);
		float h = 1.f / (maxy-miny);

		for (size_t i = 0; i < vc; i += stride) {
			vertBuffer[i + uvoffset] = (vertBuffer[i+x] - left)*w;
			vertBuffer[i + uvoffset + 1] = (vertBuffer[i+y] - top)*h;
		}
	}


	SMmesh extrude(std::vector<float> &vertBuffer, uint32_t &vc, vec3f pos, std::vector<std::vector<float>> contours, SMbbox3d &modelbox, uint32_t col, float depth, uint16_t divs)
	{
		//Find leftmost polygon (always an outside polygon)
		float leftx = 1e20;
		int f = -1;
		for (size_t c = 0; c < contours.size(); c++) {
			std::vector<float> &contour = contours[c];
			for (size_t p = 0; p < contour.size(); p += 2) {
				if (contour[p] < leftx) {
					leftx = contour[p];
					f = c;
				}
			}
		}

		//Work out CW/CCW of leftmost polygon ...
		std::vector<float> &contour = contours[f];
		float a = 0.f; size_t p = contour.size() - 2;
		for (size_t q = 0; q < contour.size() - 1; q += 2) {
			a += contour[p] * contour[q + 1] - contour[q] * contour[p + 1];
			p = q;
		}

		//reverse contours if inverted
		if (a > 0)
		{
			for (size_t a = 0; a < contours.size(); a++) {
				std::vector<float> &contour = contours[a];
				uint32_t vs = contour.size();
				for (size_t j = 0; j < (vs - 1) / 2; j += 2) {
					std::swap(contour[j], contour[vs - j - 2]);
					std::swap(contour[j + 1], contour[vs - j - 1]);
				}
			}
		}

		std::vector<float> tris;
		std::vector<std::vector<float>> edges;
		//tessellate(&contours, NULL, &edges, true);	//Get outer edges
		//tessellate(&contours, &tris, NULL, false);	//Get triangles
		int triCount = tris.size() / 9;

		SMmesh extrusion("Extrude");

		float cols[4];
		cols[0] = (col & 255) / 255.f;
		cols[1] = ((col >> 8) & 255) / 255.f;
		cols[2] = ((col >> 16) & 255) / 255.f;
		cols[3] = ((col >> 24) & 255) / 255.f;

		extrusion.stride = 12;
		extrusion.vertOffset = vc / extrusion.stride;
		vec3f n = vec3f(0.f, 1.f, 0.f);
		for (int i = 0; i < triCount; i++) {  //Top faces
			int t = i * 9;
			extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(tris[t], depth, tris[t + 1]), n, vec2f(0, 0), cols);
			extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(tris[t + 3], depth, tris[t + 4]), n, vec2f(0, 0), cols);
			extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(tris[t + 6], depth, tris[t + 7]), n, vec2f(0, 0), cols);
		}
		n = vec3f(0.f, -1.f, 0.f);
		for (int i = 0; i < triCount; i++) {  //Bottom faces
			int t = i * 9;
			extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(tris[t], 0, tris[t + 1]), n, vec2f(0, 0), cols);
			extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(tris[t + 6], 0, tris[t + 7]), n, vec2f(0, 0), cols);
			extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(tris[t + 3], 0, tris[t + 4]), n, vec2f(0, 0), cols);
		}

		for (size_t i = 0; i < edges.size(); i++) {
			std::vector<float> &contour = edges[i];
			for (size_t j = 0; j < contour.size(); j+=2) {
				size_t k = (j + 2) % contour.size();
				vec2f norm = dot(vec2f(contour[j], contour[j + 1]), vec2f(contour[k], contour[k + 1]));
				extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(contour[j], 0, contour[j + 1]), vec3f(norm.x, 0, norm.y), vec2f(0, 0), cols);
				extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(contour[k], 0, contour[k + 1]), vec3f(norm.x, 0, norm.y), vec2f(0, 0), cols);
				extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(contour[k], depth, contour[k + 1]), vec3f(norm.x, 0, norm.y), vec2f(0, 0), cols);

				extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(contour[k], depth, contour[k + 1]), vec3f(norm.x, 0, norm.y), vec2f(0, 0), cols);
				extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(contour[j], depth, contour[j + 1]), vec3f(norm.x, 0, norm.y), vec2f(0, 0), cols);
				extrusion.addPackedVert(vertBuffer, vc, pos + vec3f(contour[j], 0, contour[j + 1]), vec3f(norm.x, 0, norm.y), vec2f(0, 0), cols);
			}
		}

		extrusion.vertSize = vc / extrusion.stride - extrusion.vertOffset;
		extrusion.bbox.bboxFromVerts(vertBuffer, extrusion.vertOffset, extrusion.vertSize, extrusion.stride);
		extrusion.mode = GL_TRIANGLES;
		extrusion.offset(-extrusion.bbox.center(), vertBuffer, extrusion.vertOffset, extrusion.vertSize, extrusion.stride);
		extrusion.bbox.translate(-extrusion.bbox.center());
		modelbox = extrusion.bbox;

		texMap(extrusion.verts, extrusion.vc, extrusion.stride, 6,0,2);

		return extrusion;
	}


}
