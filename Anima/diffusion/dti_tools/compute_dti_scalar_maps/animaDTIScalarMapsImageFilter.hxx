#pragma once

#include <itkImageRegionConstIterator.h>
#include <itkNeighborhoodInnerProduct.h>
#include <itkImageRegionIterator.h>
#include <itkNeighborhoodAlgorithm.h>
#include <itkZeroFluxNeumannBoundaryCondition.h>
#include <itkOffset.h>
#include <itkProgressReporter.h>
#include <itkMatrix.h>
#include <itkSymmetricEigenAnalysis.h>

#include <animaBaseTensorTools.h>

namespace anima
{

template < unsigned int ImageDimension>
DTIScalarMapsImageFilter < ImageDimension >::DTIScalarMapsImageFilter() :
    Superclass()
{
    m_ComputeFAImage = true;
    this->SetNumberOfRequiredOutputs(2);
    this->SetNthOutput(0, this->MakeOutput(0));
    this->SetNthOutput(1, this->MakeOutput(1));
}


/**
 *   Make Ouput
 */
template < unsigned int ImageDimension>
itk::DataObject::Pointer
DTIScalarMapsImageFilter< ImageDimension >::MakeOutput(itk::ProcessObject::DataObjectPointerArraySizeType idx)
{
    itk::DataObject::Pointer output;
    switch( idx )
    {
    case 0:
        output = (OutputImageType::New()).GetPointer();
        break;
    case 1:
        output = (OutputImageType::New()).GetPointer();
        break;
    }
    return output.GetPointer();
}

template < unsigned int ImageDimension>
void
DTIScalarMapsImageFilter< ImageDimension >::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
    itk::ImageRegionConstIterator<TensorImageType> tensorIterator;
    itk::ImageRegionIterator<ADCImageType> adcIterator;

    // Allocate output
    typename  InputImageType::ConstPointer tensorImage  = this->GetInput();
    typename ADCImageType::Pointer adcImage = this->GetOutput(0);

    // support progress methods/callbacks
    itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

    TensorImageRegionType tensorRegionForThread;
    tensorRegionForThread.SetIndex(outputRegionForThread.GetIndex());
    tensorRegionForThread.SetSize(outputRegionForThread.GetSize());

    tensorIterator = itk::ImageRegionConstIterator<TensorImageType>(tensorImage, tensorRegionForThread);
    adcIterator = itk::ImageRegionIterator<OutputImageType>(adcImage, outputRegionForThread);
    tensorIterator.GoToBegin();
    adcIterator.GoToBegin();

    itk::ImageRegionIterator<OutputImageType> faIterator;
    typename FAImageType::Pointer faImage = this->GetOutput(1);
    faIterator = itk::ImageRegionIterator<OutputImageType>(faImage, outputRegionForThread);
    faIterator.GoToBegin();

    typedef vnl_matrix<double> TensorSymMatrixType;
    typedef itk::Vector<double, 3> EigenVectorType;

    EigenVectorType eigenValue;
    TensorSymMatrixType tensorSymMatrix(3,3);

    itk::SymmetricEigenAnalysis<TensorSymMatrixType, EigenVectorType> eigenAnalysis;
    eigenAnalysis.SetDimension(3);

    while (!tensorIterator.IsAtEnd())
    {
        TensorVectorType tensor = tensorIterator.Get();
        double a(tensor[0]), c(tensor[2]), f(tensor[5]), ADC(0);
        ADC = (a+c+f)/3;

        adcIterator.Set(ADC);

        if(m_ComputeFAImage)
        {
            anima::GetTensorFromVectorRepresentation(tensor, tensorSymMatrix,3,false);

            eigenAnalysis.ComputeEigenValues(tensorSymMatrix, eigenValue);

            double l1(eigenValue[0]), l2(eigenValue[1]), l3(eigenValue[2]), fa(1);
            double num = sqrt ( (l1 -l2)*(l1 -l2) + (l2 -l3)*(l2 -l3) + (l3 - l1)*(l3 - l1) );
            double den = sqrt (l1*l1 + l2*l2 + l3*l3);
            if(den == 0)
                fa = 0;
            else
                fa = sqrt(0.5) * (num / den);

            faIterator.Set(fa);
        }

        ++tensorIterator;
        ++adcIterator;
        ++faIterator;
        progress.CompletedPixel();
    }
}

/**
 * Standard "PrintSelf" method
 */
template < unsigned int ImageDimension>
void
DTIScalarMapsImageFilter< ImageDimension >::PrintSelf(
  std::ostream& os,
  itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Compute FA Image: " << m_ComputeFAImage << std::endl;
}

} // end of namespace anima
