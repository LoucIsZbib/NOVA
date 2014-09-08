// NOVA
#include "RawImage.hpp"
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
        throw -1 ;
    }

    // décodage de la matrice de bayer en couleur
    ret = RawProcessor.raw2image();
    if( ret != LIBRAW_SUCCESS) {
        ihm.logError("rawimage", "loadRaw()", "raw2image failed on " + m_fileName + " : " + libraw_strerror(ret));
        throw -1 ;
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
    
    
    
    

    
    
    namedWindow( "blob", WINDOW_NORMAL );// create a window for display.
    
    /*
    printf("recopie dans tampon 1 dimension fait\n");
    
    for (int layer=0 ; layer < 4; layer ++) {
    
        imshow( "blob", 16*image[layer]);// show our image inside it.
        printf("layer = %d -- value = %d -- value = %d -- value = %d -- value = %d\n ",layer, image[layer].at<unsigned short>(0,0),image[layer].at<unsigned short>(0,1),image[layer].at<unsigned short>(1,0),image[layer].at<unsigned short>(1,1));
        
        waitKey(0);// wait for a keystroke in the window
    }
    */
   

    
    // séparation des couleurs
    unsigned short pixelValue;
    // redéfinition de la structure Mat de stockage
    //_rawImage.assign(4,Mat(Sizes.iheight/2,Sizes.iwidth/2,CV_16U));
    for( int layer=0 ; layer < 4 ; layer++ ) {
        _rawImage.push_back(Mat(Sizes.iheight/2,Sizes.iwidth/2,CV_16U));
        for( int row=0; row < Sizes.iheight/2 ; row++ ) {
            for( int col=0 ; col < Sizes.iwidth/2 ; col++ ) {
                pixelValue = 
                      image[layer].at<unsigned short>(2*row,2*col)
                    + image[layer].at<unsigned short>(2*row,2*col+1)
                    + image[layer].at<unsigned short>(2*row+1,2*col)
                    + image[layer].at<unsigned short>(2*row+1,2*col+1);

                

                //Scalar monScalaire = sum( image[layer]( Range(row*2,row*2+1) , Range(col*2,col*2+1) ) );

                
                _rawImage[layer].at<unsigned short>(row,col) = pixelValue;
            }
        }
    }

    
    
printf(" in loadRaw : fin unpack, début visu pour debug\n");

/*
    printf("_rawImage affichée\n");
    for (int layer=0 ; layer < 4; layer ++) {
    
        imshow( "blob", 16*_rawImage[layer]);// show our image inside it.

        printf("layer = %d -- value = %d\n",layer, _rawImage[layer].at<unsigned short>(5,5));
        
        waitKey(0);// wait for a keystroke in the window
    }
    */
    
    // tentative de remise des canaux dans l'ordre
    Mat R = _rawImage[0];
    Mat G = _rawImage[1];
    Mat B = _rawImage[2];
    
    
    // TROUVER MIN MAX MOY
    printf("trouver max, min\n");
    double maxR, minR, maxG, minG, maxB, minB ;
    minMaxLoc(G, &minR, &maxR);
    printf("trouver max, min R Ok\n");
    minMaxLoc(G, &minG, &maxG);
    printf("trouver max, min G Ok\n");
    minMaxLoc(B, &minB, &maxB);
    printf("trouver max, min B Ok\n");
    
    printf("max min trouvés\n");
    
    printf("R, max = %f -- min = %f\n",maxR, minR);
    printf("G, max = %f -- min = %f\n",maxG, minG);
    printf("B, max = %f -- min = %f\n",maxB, minB);
    
    
    Mat enCouleur;
    
    merge(_rawImage,enCouleur);
    
    Scalar_<unsigned short> moy = mean(enCouleur);
    printf("moy trouvé(s)\n");
    for (int i=0 ; i <4 ; i++)
        printf("moy(%d) = %d\n",i,moy[i]);
    
    // essayer une balance de blanc ?
    double multR, multB;
    
    multR = (double)moy[1]/moy[0];
    multB = (double)moy[1]/moy[2];
    printf("multR = %f -- multB = %f\n",multR, multB);
    printf("Libraw => cam white balance %f %f %f %f\n",RawProcessor.imgdata.color.cam_mul[0],RawProcessor.imgdata.color.cam_mul[1],RawProcessor.imgdata.color.cam_mul[2],RawProcessor.imgdata.color.cam_mul[3]);
    multR = RawProcessor.imgdata.color.cam_mul[0]/RawProcessor.imgdata.color.cam_mul[1];
    multB = RawProcessor.imgdata.color.cam_mul[2]/RawProcessor.imgdata.color.cam_mul[1];
    
    vector<Mat> balanced;
    balanced.push_back(B*multB);
    balanced.push_back(G);
    balanced.push_back(R*multR);
    
    merge(balanced, enCouleur);
    imshow("blob",16*enCouleur);
    waitKey(0);
    
    
    
    vector<Mat> multi;
    multi.push_back(B);
    multi.push_back(G);
    multi.push_back(R);
    
    Mat A;
    merge(multi, A);
    
    printf("A.channels() %d\n",A.channels());
    
    imshow( "blob", 16*A);// show our image inside it.
    printf("_rawImage affichée\n");
        
    waitKey(0);// wait for a keystroke in the window
    
    
    printf(" libraw..raw_image    libraw..image[layer]   image[layer]    _rawImage[layer]\n\n");
    int start = 16;
    for (int layer=0 ; layer < 4 ; layer++) {
    
        for( int row=start; row < start + 4 ; row++ ) {
        
            for( int col=start ; col < start + 4 ; col++ ) {
                printf( "%4d",  RawProcessor.imgdata.rawdata.raw_image[row*Sizes.width+col] );
            }
            // LibRaw..raw_image = matrice de bayer, ordre inconnu
            
            printf(" - ");
            for( int col=0 ; col < 4 ; col++ ) {
                printf( "%4d", RawProcessor.imgdata.image[row*Sizes.width+col][layer] );
            }
            //RGBG ?
            
            printf(" - ");
            for( int col=0 ; col < 4 ; col++ ) {
                printf( "%4d", image[layer].at<unsigned short>(row,col) );
            }
            //
            
            printf(" - ");
            for( int col=0 ; col < 4 ; col++ ) {
                printf( "%4d", _rawImage[layer].at<unsigned short>(row,col) );
            }
            //
            
            // opencv BGR 
            
            printf("\n\n");
        }
        
        printf("\n\n");
    }
    
    
    
    
    

    
    
}
