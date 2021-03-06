#pragma once

#include <iostream>
#include <itkImageToImageFilter.h>
#include <itkImage.h>

namespace anima
{

template <class TScalarType, unsigned int NDegreesOfFreedom, unsigned int NDimensions = 3>
class MEstimateSVFImageFilter :
public itk::ImageToImageFilter< itk::Image < itk::Vector <TScalarType,NDegreesOfFreedom>, NDimensions > , itk::Image < itk::Vector <TScalarType,NDegreesOfFreedom>, NDimensions > >
{
public:
    /** Standard class typedefs. */
    typedef MEstimateSVFImageFilter Self;
    typedef itk::Image < TScalarType, NDimensions > WeightImageType;
    typedef typename WeightImageType::Pointer WeightImagePointer;
    typedef itk::Image < itk::Vector <TScalarType,NDegreesOfFreedom>, NDimensions > TInputImage;
    typedef itk::Image < itk::Vector <TScalarType,NDegreesOfFreedom>, NDimensions > TOutputImage;
    typedef itk::ImageToImageFilter< TInputImage, TOutputImage > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods) */
    itkTypeMacro(MEstimateSVFImageFilter, ImageToImageFilter);

    typedef typename TOutputImage::PixelType OutputPixelType;
    typedef typename TInputImage::PixelType InputPixelType;
    typedef typename TInputImage::IndexType InputIndexType;
    typedef typename TInputImage::PointType InputPointType;

    /** Image typedef support */
    typedef typename TInputImage::Pointer InputImagePointer;
    typedef typename TOutputImage::Pointer OutputImagePointer;

    /** Superclass typedefs. */
    typedef typename Superclass::OutputImageRegionType OutputImageRegionType;

    itkSetMacro(WeightImage, WeightImagePointer);

    itkSetMacro(FluidSigma, double);
    itkSetMacro(MEstimateFactor, double);
    itkSetMacro(NeighborhoodHalfSize, unsigned int);
    void SetDistanceBoundary (double num) {m_SqrDistanceBoundary = num * num;}

    itkSetMacro(ConvergenceThreshold, double);
    itkSetMacro(MaxNumIterations, unsigned int);

protected:
    MEstimateSVFImageFilter()
    {
        m_FluidSigma = 4.0;
        m_MEstimateFactor = 1.0;
        m_AverageResidualValue = 1.0;
        m_NeighborhoodHalfSize = (unsigned int)floor(3.0 * m_FluidSigma);
        m_SqrDistanceBoundary = 9.0 * m_FluidSigma * m_FluidSigma;
    }

    virtual ~MEstimateSVFImageFilter() {}

    void BeforeThreadedGenerateData(void);
    void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread, itk::ThreadIdType threadId);

private:
    MEstimateSVFImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    bool checkConvergenceThreshold (OutputPixelType &outValOld, OutputPixelType &outVal);

    WeightImagePointer m_WeightImage, m_InternalSpatialWeight;

    double m_FluidSigma, m_MEstimateFactor;
    double m_SqrDistanceBoundary;
    unsigned int m_NeighborhoodHalfSize;
    unsigned int m_MaxNumIterations;
    double m_ConvergenceThreshold;

    //Internal parameter
    double m_AverageResidualValue;
};

} // end namespace anima


#include "animaMEstimateSVFImageFilter.hxx"
