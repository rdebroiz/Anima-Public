# This defines useful cmake variables for ITK libs
set(ITK_TRANSFORM_LIBRARIES
  ITKIOTransformBase
  ITKIOTransformInsightLegacy
  ITKIOTransformHDF5
  ITKIOTransformMatlab
)

set(ITKIO_LIBRARIES
  ITKIOImageBase
  ITKIOBMP
  ITKIOBioRad
  ITKIOGDCM
  ITKIOGIPL
  ITKIOGE
  ITKIOHDF5
  ITKIOJPEG
  ITKIOLSM
  ITKIOMeta
  ITKIONIFTI
  ITKIONRRD
  ITKIOPNG
  ${ITKIOPhilipsREC_LIBRARIES}
  ITKIOStimulate
  ITKIOTIFF
  ITKIOVTK
  ITKIOMRC
)

