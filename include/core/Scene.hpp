#pragma once

#include <core\Common.hpp>
#include <core\Object.hpp>

NAMESPACE_BEGIN

class Scene : public Object
{
public:
	/// Construct a new scene object
	Scene(const PropertyList & PropList);

	/// Release all memory
	virtual ~Scene();

	/// Return a pointer to the scene's acceleration structure
	const Acceleration * GetAccel() const;

	/// Return a pointer to the scene's integrator
	const Integrator * GetIntegrator() const;

	/// Return a pointer to the scene's integrator
	Integrator * GetIntegrator();

	/// Return a pointer to the scene's camera
	const Camera * GetCamera() const;

	/// Return a pointer to the scene's sample generator (const version)
	const Sampler * GetSampler() const;

	/// Return a pointer to the scene's sample generator
	Sampler * GetSampler();

	/// Return a reference to an array containing all meshes
	const std::vector<Mesh*> & GetMeshes() const;

	/**
	* \brief Intersect a ray against all triangles stored in the scene
	* and return detailed intersection information
	*
	* \param Ray
	*    A 3-dimensional ray data structure with minimum/maximum
	*    extent information
	*
	* \param Isect
	*    A detailed intersection record, which will be filled by the
	*    intersection query
	*
	* \return \c true if an intersection was found
	*/
	bool RayIntersect(const Ray3f & Ray, Intersection & Isect) const;

	/**
	* \brief Inherited from \ref Object::Activate()
	*
	* Initializes the internal data structures (kd-tree,
	* bvh, emitter sampling data structures, etc.)
	*/
	virtual void Activate() override;

	/// Add a child object to the scene (meshes, integrators etc.)
	virtual void AddChild(Object * pChildObj) override;

	/// Return a string summary of the scene (for debugging purposes)
	virtual std::string ToString() const override;

	virtual EClassType GetClassType() const override;

protected:
	std::vector<Mesh*> m_pMeshes;
	std::unique_ptr<Integrator> m_pIntegrator = nullptr;
	std::unique_ptr<Sampler> m_pSampler = nullptr;
	std::unique_ptr<Camera> m_pCamera = nullptr;
	std::unique_ptr<Acceleration> m_pAcceleration = nullptr;
};

NAMESPACE_END