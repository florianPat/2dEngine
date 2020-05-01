#include "Camera.h"

eg::Camera::Camera(const Vector3f& dir, Vector3f pos) : dir(dir), pos(pos)
{
	update();
}

bool eg::Camera::hasUpdated()
{
	bool result = updated;
	updated = false;
	return result;
}

void eg::Camera::update()
{
	updated = true;

	// NOTE: Lets create a new basis relative to the direction where the camera is looking
	Vector3f up = { 0.0f, 1.0f, 0.0f };
	// NOTE: If the vectors are parallel, then the up vector will be the zero vector. No good.
	assert(up.dotProduct(dir) != 1.0f);
	up = up - dir * up.dotProduct(dir);
	up.normalize();
	Vector3f right = dir.crossProduct(up);

	worldToView = Mat4x4::identity();

	// NOTE: This is a view to world transform.
	worldToView.matrix[0 * 4 + 0] = right.x;
	worldToView.matrix[0 * 4 + 1] = right.y;
	worldToView.matrix[0 * 4 + 2] = right.z;
	worldToView.matrix[1 * 4 + 0] = up.x;
	worldToView.matrix[1 * 4 + 1] = up.y;
	worldToView.matrix[1 * 4 + 2] = up.z;
	worldToView.matrix[2 * 4 + 0] = dir.x;
	worldToView.matrix[2 * 4 + 1] = dir.y;
	worldToView.matrix[2 * 4 + 2] = dir.z;
	// NOTE: The inverse, which is the transpose, is the world to view transform.
	worldToView.transpose();
	// NOTE: The position of the camera also needs to be multiplied (because TRS) and then inversed (for v2w to w2v)
	pos = worldToView * pos;
	pos *= -1;

	worldToView *= Mat4x4::translate(pos);
}

const eg::Mat4x4& eg::Camera::getWorldToViewMatrix() const
{
	return worldToView;
}
