#include <bsdf\DielectricBSDF.hpp>
#include <core\Frame.hpp>

NAMESPACE_BEGIN

REGISTER_CLASS(DielectricBSDF, XML_BSDF_DIELECTRIC);

DielectricBSDF::DielectricBSDF(const PropertyList & PropList)
{
	/* Interior IOR */
	m_IntIOR = PropList.GetFloat(XML_BSDF_DIELECTRIC_INT_IOR, DEFAULT_BSDF_DIELECTRIC_INT_IOR);

	/* Exterior IOR */
	m_ExtIOR = PropList.GetFloat(XML_BSDF_DIELECTRIC_EXT_IOR, DEFAULT_BSDF_DIELECTRIC_EXT_IOR);

	/* Specular reflectance */
	m_KsReflect = PropList.GetColor(XML_BSDF_DIELECTRIC_KS_REFLECT, DEFAULT_BSDF_DIELECTRIC_KS_REFLECT);

	/* Specular transmittance */
	m_KsRefract = PropList.GetColor(XML_BSDF_DIELECTRIC_KS_REFRACT, DEFAULT_BSDF_DIELECTRIC_KS_REFRACT);

	m_Eta = m_IntIOR / m_ExtIOR;
	m_InvEta = 1.0f / m_Eta;
}

Color3f DielectricBSDF::Sample(BSDFQueryRecord & Record, const Point2f & Sample) const
{
	Record.Measure = EMeasure::EDiscrete;

	float CosThetaI = Frame::CosTheta(Record.Wi);
	float CosThetaT;
	float FresnelTerm = FresnelDielectric(CosThetaI, m_Eta, m_InvEta, CosThetaT);

	// Reflection
	if (Sample.x() < FresnelTerm)
	{
		Record.Wo = Reflect(Record.Wi);
		Record.Eta = 1.0f;

		return m_KsReflect;
	}
	// Refraction
	else
	{
		Record.Wo = Refract(Record.Wi, CosThetaT, m_Eta, m_InvEta);
		Record.Eta = CosThetaT < 0.0f ? m_Eta : m_InvEta;
		float Factor = CosThetaT < 0.0f ? m_InvEta : m_Eta;
		return m_KsRefract * Factor;
	}
}

Color3f DielectricBSDF::Eval(const BSDFQueryRecord & Record) const
{
	/* Discrete BRDFs always evaluate to zero */
	return Color3f(0.0f);
}

float DielectricBSDF::Pdf(const BSDFQueryRecord & Record) const
{
	/* Discrete BRDFs always evaluate to zero */
	return 0.0f;
}

std::string DielectricBSDF::ToString() const
{
	return tfm::format("Dielectric[intIOR = %f, extIOR = %f]", m_IntIOR, m_ExtIOR);
}

NAMESPACE_END
