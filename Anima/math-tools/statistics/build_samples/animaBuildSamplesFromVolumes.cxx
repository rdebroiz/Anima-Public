#include <iostream>
#include <tclap/CmdLine.h>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkTimeProbe.h>
#include <itkImageRegionIteratorWithIndex.h>

#include <animaReadWriteFunctions.h>
#include <animaRetrieveImageTypeMacros.h>

struct arguments
{
    itk::ImageRegionIteratorWithIndex<itk::Image<unsigned char, 3> > maskIt;
    unsigned int nbImgToSample;
    std::vector< std::vector<std::string> > samples;
    std::vector<std::string> labels;
    std::vector<unsigned int> x;  std::vector<unsigned int> y;  std::vector<unsigned int> z;
};

template <typename ImageType>
void
buildSample(std::string inputfileName, arguments& arg)
{



    typename ImageType::Pointer image = anima::readImage<ImageType>(inputfileName);
    itk::ImageRegionIteratorWithIndex<ImageType> imageIt(image, image->GetRequestedRegion());

    arg.maskIt.GoToBegin();

    std::vector<std::string> sample;
    while(!imageIt.IsAtEnd())
    {
        if(arg.maskIt.Value() != 0)
        {
            std::ostringstream sstream;
            sstream << imageIt.Get();
            std::string varAsString = sstream.str();

            sample.push_back(varAsString);

            if(arg.nbImgToSample == 0)
            {
                const unsigned int x = imageIt.GetIndex()[0];
                const unsigned int y = imageIt.GetIndex()[1];
                const unsigned int z = imageIt.GetIndex()[2];
                arg.x.push_back(x);
                arg.y.push_back(y);
                arg.z.push_back(z);
            }
        }
        ++arg.maskIt;
        ++imageIt;
    }
    arg.samples.push_back(sample);
    ++arg.nbImgToSample;
}

int main(int argc, char **argv)
{
    TCLAP::CmdLine cmd("Given a mask creates and a list of volume data, populates a csv file where each colmuns "
                       "represents one of the volume data and each line a given voxel. The voxel for which the mask "
                       "is 0 are not set into the csv file. \\ Note: the voxel location are lost into the csv file.", ' ',"1.0");
    TCLAP::ValueArg<std::string> dataListArg("i","database","Image List",true,"","image list",cmd);
    TCLAP::ValueArg<std::string> maskArg("m","maskname","Mask",true,"","mask",cmd);
    TCLAP::ValueArg<std::string> resNameArg("o","outputname","CSV file name",true,"","file name for the output csv file",cmd);
    try
    {
        cmd.parse(argc,argv);
    }
    catch (TCLAP::ArgException& e)
    {
        std::cerr << "Error: " << e.error() << "for argument " << e.argId() << std::endl;
        return(1);
    }

    std::string dataListName = dataListArg.getValue();
    std::string maskName = maskArg.getValue();

    arguments arg;

    // load mask file
    typedef itk::Image <unsigned char, 3> MaskType;
    MaskType::Pointer mask = anima::readImage<MaskType>(maskName.c_str());

    typedef itk::ImageRegionIteratorWithIndex<MaskType>  MaskIterator;
    arg.maskIt = MaskIterator(mask, mask->GetRequestedRegion());

    // load filenames file
    std::ifstream inputs(dataListName.c_str());
    std::string oneLine;

    while (std::getline(inputs, oneLine))
    {
        std::size_t pos = oneLine.find(":");                   // position of ":" in str
        std::string inputFileName = oneLine.substr(pos+1);     // get from ":" to the end
        arg.labels.push_back(oneLine.substr(0,pos));

        // Find out the type of the image we're dealing with
        itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(inputFileName.c_str(),
                                                                               itk::ImageIOFactory::ReadMode);
        // Now that we found the appropriate ImageIO class, ask it to read the meta data from the image file.
        imageIO->SetFileName(inputFileName);
        imageIO->ReadImageInformation();

        if(!imageIO)
        {
            std::cerr << "Itk could not find suitable IO factory for the input" << std::endl;
            return EXIT_FAILURE;
        }
        try
        {
            ANIMA_CHECK_IF_COMPONENTS_ARE_VECTORS(imageIO, double, 3, buildSample, inputFileName, arg)
        }
        catch ( itk::ExceptionObject & err )
        {
            std::cerr << "Itk cannot build samples from" << inputFileName
                      << ", be sure to use valid filenames..." << std::endl;
            std::cerr << err << std::endl;
            return EXIT_FAILURE;
        }

    }

    if (resNameArg.getValue() != "")
    {
        // save csv
        std::ofstream file(resNameArg.getValue().c_str());

        for(unsigned int line = 0; line < arg.samples.size(); ++line)
        {
            file << arg.labels[line];
            if(line < (arg.samples.size() - 1))
                file << ",";
        }

        file << ",X, Y, Z";
        file << std::endl;

        for(unsigned int indexJ = 0; indexJ < arg.samples[0].size(); ++indexJ)
        {
            for(unsigned int indexI = 0; indexI < arg.samples.size(); ++indexI)
            {
                file << "\"" << arg.samples[indexI][indexJ] << "\"";

                if(indexI < (arg.samples.size() - 1))
                    file<<",";
            }

            file << ",\"" << arg.x[indexJ] <<"\",\"" << arg.y[indexJ] << "\",\"" << arg.z[indexJ] << "\"";
            file<<std::endl;
        }

        file.close();
    }

    return EXIT_SUCCESS;
}

