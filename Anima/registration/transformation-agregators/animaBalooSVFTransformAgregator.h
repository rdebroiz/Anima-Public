#pragma once

#include <animaBaseTransformAgregator.h>
#include <itkStationaryVelocityFieldTransform.h>


namespace anima
{

template <unsigned int NDimensions = 3>
class BalooSVFTransformAgregator :
public BaseTransformAgregator <NDimensions>
{
public:
    typedef BaseTransformAgregator <NDimensions> Superclass;
    typedef typename Superclass::PointType PointType;
    typedef typename Superclass::BaseInputTransformType BaseInputTransformType;
    typedef typename Superclass::ScalarType ScalarType;
    typedef typename itk::StationaryVelocityFieldTransform<ScalarType,NDimensions> BaseOutputTransformType;
    typedef typename Superclass::InternalScalarType InternalScalarType;

    typedef itk::Image <ScalarType,NDimensions> WeightImageType;
    typedef typename WeightImageType::IndexType IndexType;
    typedef typename WeightImageType::Pointer WeightImagePointer;

    typedef itk::MatrixOffsetTransformBase <InternalScalarType, NDimensions, NDimensions> BaseMatrixTransformType;
    typedef typename BaseMatrixTransformType::ParametersType ParametersType;

    typedef typename BaseOutputTransformType::VectorFieldType VelocityFieldType;
    typedef typename VelocityFieldType::Pointer VelocityFieldPointer;
    typedef typename VelocityFieldType::IndexType VelocityFieldIndexType;
    typedef typename VelocityFieldType::PixelType VelocityFieldPixelType;
    typedef typename VelocityFieldType::PointType VelocityFieldPointType;
    typedef typename VelocityFieldType::RegionType VelocityFieldRegionType;
    typedef typename VelocityFieldType::SpacingType VelocityFieldSpacingType;
    typedef typename VelocityFieldType::DirectionType VelocityFieldDirectionType;

    BalooSVFTransformAgregator();
    virtual ~BalooSVFTransformAgregator() {}

    virtual bool Update();

    void SetExtrapolationSigma(double sigma) {m_ExtrapolationSigma = sigma;}
    double GetExtrapolationSigma() {return m_ExtrapolationSigma;}

    void SetOutlierRejectionSigma(double sigma) {m_OutlierRejectionSigma = sigma;}
    double GetOutlierRejectionSigma() {return m_OutlierRejectionSigma;}

    void SetNumberOfThreads(unsigned int num) {m_NumberOfThreads = num;}
    unsigned int GetNumberOfThreads() {return m_NumberOfThreads;}

    void SetDamIndexes(std::vector <IndexType> &indexes) {m_DamIndexes = indexes;}

    template <class TInputImageType> void SetGeometryInformation(const TInputImageType *geomImage)
    {
        if (geomImage == NULL)
            return;

        m_Origin = geomImage->GetOrigin();
        m_LargestRegion = geomImage->GetLargestPossibleRegion();
        m_Spacing = geomImage->GetSpacing();
        m_Direction = geomImage->GetDirection();
    }

protected:
    double m_ExtrapolationSigma;
    double m_OutlierRejectionSigma;

    VelocityFieldPointType m_Origin;
    VelocityFieldRegionType m_LargestRegion;
    VelocityFieldSpacingType m_Spacing;
    VelocityFieldDirectionType m_Direction;

    unsigned int m_NumberOfThreads;
    std::vector <IndexType> m_DamIndexes;

private:
    void estimateSVFFromTranslations();
    void estimateSVFFromRigidTransforms();
    void estimateSVFFromAffineTransforms();
};


const double zeroWeight = 0;

template <class ScalarType, unsigned int NDimensions, unsigned int NDegreesOfFreedom>
void
filterInputs(itk::Image <ScalarType,NDimensions> *weights,
             typename itk::Image < itk::Vector <ScalarType, NDegreesOfFreedom>, NDimensions >::Pointer &output,
             std::vector < itk::Vector <ScalarType, NDegreesOfFreedom> > &curTrsfs,
             std::vector < typename itk::Image < itk::Vector <ScalarType, NDegreesOfFreedom>, NDimensions >::IndexType > &posIndexes,
             BalooSVFTransformAgregator <NDimensions> *filterPtr);

} // end of namespace anima

#include "animaBalooSVFTransformAgregator.hxx"
