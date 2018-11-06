#pragma once

#include <core\Common.hpp>
#include <core\Object.hpp>
#include <core\Mesh.hpp>

NAMESPACE_BEGIN

/**
* \brief Acceleration data structure for ray intersection queries
*
* The current implementation falls back to a brute force loop
* through the geometry.
*/
class Acceleration : public Object
{
public:
	Acceleration(const PropertyList & PropList);

	/**
	* \brief Register a triangle mesh for inclusion in the acceleration
	* data structure
	*
	* This function can only be used before \ref Build() is called
	*/
	void AddMesh(Mesh * pMesh);

	/// Build the acceleration data structure (currently a no-op)
	virtual void Build();

	/// Return an axis-aligned box that bounds the scene
	const BoundingBox3f & GetBoundingBox() const;

	/**
	* \brief Intersect a ray against all triangles stored in the scene and
	* return detailed intersection information
	*
	* \param Ray
	*    A 3-dimensional ray data structure with minimum/maximum extent
	*    information
	*
	* \param Isect
	*    A detailed intersection record, which will be filled by the
	*    intersection query
	*
	* \param bShadowRay
	*    \c true if this is a shadow ray query, i.e. a query that only aims to
	*    find out whether the ray is blocked or not without returning detailed
	*    intersection information.
	*
	* \return \c true if an intersection was found
	*/
	virtual bool RayIntersect(const Ray3f & Ray, Intersection & Isect, bool bShadowRay) const;

	/**
	* \brief Return the type of object (i.e. Mesh/BSDF/etc.)
	* provided by this instance
	* */
	virtual EClassType GetClassType() const override;

	/// Return a brief string summary of the instance (for debugging purposes)
	virtual std::string ToString() const override;

protected:
	/**
	* \brief Obtain the mesh index and its triangle index by the total 
	* index of triangle 
	* */
	void GetIndex(size_t TotalTriangleIdx, size_t & MeshIdx, size_t & TriangleIdx) const;

protected:
	std::vector<Mesh*> m_pMeshes;
	std::vector<uint32_t> m_AccumulateMeshFacet;
	BoundingBox3f m_BBox;
};

NAMESPACE_END