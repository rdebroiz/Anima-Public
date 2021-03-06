#pragma once

#include <animaBaseTransformAgregator.h>
#include <itkAffineTransform.h>

namespace anima
{

template <unsigned int NDimensions = 3>
class LSWTransformAgregator :
public BaseTransformAgregator <NDimensions>
{
public:
    typedef BaseTransformAgregator <NDimensions> Superclass;
    typedef typename Superclass::PointType PointType;
    typedef typename Superclass::BaseInputTransformType BaseInputTransformType;
    typedef typename Superclass::ScalarType ScalarType;
    typedef typename itk::AffineTransform <ScalarType,NDimensions> BaseOutputTransformType;
    typedef typename Superclass::InternalScalarType InternalScalarType;

    LSWTransformAgregator();
    virtual ~LSWTransformAgregator() {}

    virtual bool Update();

private:
    void lswEstimateTranslationsToTranslation();
    void lswEstimateTranslationsToRigid();
    void lswEstimateTranslationsToAffine();
    void lswEstimateAnyToAffine();
};

}// end of namespace anima

#include "animaLSWTransformAgregator.hxx"
