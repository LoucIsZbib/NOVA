// NOVA
#include "rawimage.hpp"
#include "ui.hpp"

#include <string>
#include <vector>
using namespace std;

// OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

RawImage::RawImage(string fullFileName) {

    this->m_fileName = fullFileName ;

}


void RawImage::loadRaw() {

    UI &ihm = UI::getUI();

// import du fichier dans les méandres de LibRaw

    int ret;
    LibRaw RawProcessor;
    #define Sizes RawProcessor.imgdata.sizes
    const char * fileName4libRaw = m_fileName.c_str();
    ihm.logInfo("rawimage", "loadRaw()", "Processing file " + m_fileName);

    // ouverture du fichier
    ret = RawProcessor.open_file(fileName4libRaw) ;
    if( ret != LIBRAW_SUCCESS) {
        ihm.logError("rawimage", "loadRaw()", "Cannot open " + m_fileName + " : " + libraw_strerror(ret));
        throw -1 ;
    }

    // unpack du fichier
    ret = RawProcessor.unpack();
    if( ret != LIBRAW_SUCCESS) {
        ihm.logError("rawimage", "loadRaw()", "Cannot unpack " + m_fileName + " : " +libraw_strerror(ret));
        throw -2 ;
    }

    // décodage de la matrice de bayer en couleur
    ret = RawProcessor.raw2image();
    if( ret != LIBRAW_SUCCESS) {
        ihm.logError("rawimage", "loadRaw()", "raw2image failed on " + m_fileName + " : " + libraw_strerror(ret));
        throw -3 ;
    }
    
    ihm.logInfo("rawimage", "loadRaw()", "LibRaw job done on " + m_fileName );


// recopie des données de l'image dans la structure Mat d'opencv

    // recopie des valeurs dans un Mat tampon
    vector<Mat> image;
    for( int layer=0 ; layer < 4 ; layer++ ) {
        image.push_back(Mat(Sizes.iheight,Sizes.iwidth,CV_16U));
        for( int row=0 ; row < Sizes.iheight ; row++ ) {
            for ( int col=0 ; col < Sizes.iwidth ; col++ ) {
                image[layer].at<unsigned short>(row,col) = RawProcessor.imgdata.image[row*Sizes.iwidth+col][layer];
            }
        }
    }
    
    // séparation des couleurs
    unsigned short pixelValue;
    for( int layer=0 ; layer < 4 ; layer++ ) {
        m_rawImage.push_back(Mat(Sizes.iheight/2,Sizes.iwidth/2,CV_16U));
        for( int row=0; row < Sizes.iheight/2 ; row++ ) {
            for( int col=0 ; col < Sizes.iwidth/2 ; col++ ) {
                pixelValue = 
                      image[layer].at<unsigned short>(2*row,2*col)
                    + image[layer].at<unsigned short>(2*row,2*col+1)
                    + image[layer].at<unsigned short>(2*row+1,2*col)
                    + image[layer].at<unsigned short>(2*row+1,2*col+1);
                m_rawImage[layer].at<unsigned short>(row,col) = pixelValue;
            }
        }
    }

    ihm.logInfo("rawImage", "loadRaw()", "m_rawImage créé et rempli on " + m_fileName);
}
 
// récupérer un Mat multichannel des données brutes
Mat RawImage::getRawImage(){
    Mat rawMultiChannel;
    merge(m_rawImage,rawMultiChannel);
    return rawMultiChannel;
}

