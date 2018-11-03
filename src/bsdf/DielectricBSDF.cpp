#include <bsdf\DielectricBSDF.hpp>

NAMESPACE_BEGIN

REGISTER_CLASS(DielectricBSDF, XML_BSDF_DIELECTRIC);

DielectricBSDF::DielectricBSDF(const PropertyList & PropList)
{
	/* Interior IOR (default: BK7 borosilicate optical glass) */
	m_IntIOR = PropList.GetFloat(XML_BSDF_DIELECTRIC_INT_IOR, 1.5046f);

	/* Exterior IOR (default: air) */
	m_ExtIOR = PropList.GetFloat(XML_BSDF_DIELECTRIC_EXT_IOR, 1.000277f);
}

Color3f DielectricBSDF::Sample(BSDFQueryRecord & Record, const Point2f & Sample) const
{
	throw HikariException("DielectricBSDF::Sample(const BSDFQueryRecord & Record, const Point2f & Sample) const: not implemented!");
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
