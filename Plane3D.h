//
// Plane3D.h
//

#pragma once
#include <SimpleMath.h>
#include <PrimitiveBatch.h>
#include "VertexTypes.h"
#include <SpriteBatch.h>

using namespace DirectX::SimpleMath;

class Plane3D {
public:
	// Default: the plane has the same size as the window
	Plane3D() noexcept :
		point1(DirectX::VertexPositionTexture(Vector3(1., 1., 0.), Vector2(1, 1))),
		point2(DirectX::VertexPositionTexture(Vector3(1., -1., 0.), Vector2(1, -1))),
		point3(DirectX::VertexPositionTexture(Vector3(-1., -1., 0.), Vector2(-1, -1))),
		point4(DirectX::VertexPositionTexture(Vector3(-1., 1., 0.), Vector2(-1, 1)))
	{}

	// The plane's size is a bit larger/smaller than the window in order to give additional effect (e.g. screen shake, etc.)
	/*Plane3D(float scale) noexcept :
		point1(DirectX::VertexPositionColor(Vector3(1., 1., 0.) * scale, DirectX::Colors::White)),
		point2(DirectX::VertexPositionColor(Vector3(1., -1., 0.) * scale, DirectX::Colors::White)),
		point3(DirectX::VertexPositionColor(Vector3(-1., -1., 0.) * scale, DirectX::Colors::White)),
		point4(DirectX::VertexPositionColor(Vector3(-1., 1., 0.) * scale, DirectX::Colors::White))
	{}*/

	/*Plane3D(DirectX::VertexPositionColor p1, DirectX::VertexPositionColor p2,
		DirectX::VertexPositionColor p3, DirectX::VertexPositionColor p4) noexcept :
		point1(p1),
		point2(p2),
		point3(p3),
		point4(p4)
	{}*/

	// Load 4 vertices in clockwise order
	void loadPlane(DirectX::VertexPositionTexture p1,
		DirectX::VertexPositionTexture p2,
		DirectX::VertexPositionTexture p3,
		DirectX::VertexPositionTexture p4)
	{
		this->point1 = p1;
		this->point2 = p2;
		this->point3 = p3;
		this->point4 = p4;
	}

	void loadBackgroundPlane(float scale) {
		point1 = (DirectX::VertexPositionTexture(Vector3(1., 1., 0.) * scale, Vector2(1, 1)));
		point2 = (DirectX::VertexPositionTexture(Vector3(1., -1., 0.) * scale, Vector2(1, -1)));
		point3 = (DirectX::VertexPositionTexture(Vector3(-1., -1., 0.) * scale, Vector2(-1, -1)));
		point4 = (DirectX::VertexPositionTexture(Vector3(-1., 1., 0.) * scale, Vector2(-1, 1)));
	}

	void draw(std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>& m_batch) {
		m_batch->DrawTriangle(point1, point2, point3);
		m_batch->DrawTriangle(point3, point4, point1);
	}

private:
	DirectX::VertexPositionTexture point1;
	DirectX::VertexPositionTexture point2;
	DirectX::VertexPositionTexture point3;
	DirectX::VertexPositionTexture point4;
};
