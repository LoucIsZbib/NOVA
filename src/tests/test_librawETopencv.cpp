
// OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
//using namespace cv;

#include <stdio.h>
//#include <string>
//using namespace std;


// LIBRAW
#include "libraw/libraw.h"

/*#ifdef WIN32
#define snprintf _snprintf
#endif*/


/*
*   A330 => 12 bits / px
*
*  NEX 6 => 12 bits / px
*
*
*/

int main() {

    int ret;
//int black_subtraction=1;
   // char outfn[1024]; 

    LibRaw RawProcessor;

//#define P1 RawProcessor.imgdata.idata
#define S RawProcessor.imgdata.sizes
//#define C RawProcessor.imgdata.color
//#define T RawProcessor.imgdata.thumbnail
//#define P2 RawProcessor.imgdata.other
/*#define OUT RawProcessor.imgdata.params

    OUT.output_bps=16;
    OUT.output_tiff=1;
    OUT.user_flip=0;
    OUT.no_auto_bright = 1;
    OUT.half_size=1;
*/
//    int c;
    const char * fileName = "RAW.ARW";
    
    printf("Processing file %s\n", fileName);
    if( (ret = RawProcessor.open_file(fileName)) != LIBRAW_SUCCESS) {
        fprintf(stderr,"Cannot open %s: %s\n",fileName,libraw_strerror(ret));
    }
    
    printf("longueur raw_image = %d\n",sizeof(RawProcessor.imgdata.rawdata.raw_image));
    
    
    if( (ret = RawProcessor.unpack() ) != LIBRAW_SUCCESS) {
        fprintf(stderr,"Cannot unpack %s: %s\n",fileName,libraw_strerror(ret));
    }
    
    
    
    RawProcessor.raw2image();
    
    printf("Image size: %dx%d\nRaw size: %dx%d\n",S.width,S.height,S.raw_width,S.raw_height);
    printf("Margins: top=%d, left=%d\n", S.top_margin,S.left_margin);

    printf("longueur raw_image = %d\n",sizeof(RawProcessor.imgdata.rawdata.raw_image));

/****************
    
*****/
    cv::Mat image(S.iheight,S.iwidth,CV_16U);// new blank image
    
    for( int row=0 ; row < S.iheight ; row++ ) {
        for ( int col=0 ; col < S.iwidth ; col++ ) {
            image.at<unsigned short>(row,col) = RawProcessor.imgdata.image[row*S.width+col][0];
        }
    }
    
    
    cv::namedWindow( "Display window", cv::WINDOW_NORMAL );// create a window for display.
    cv::Mat scaled(S.iheight/2,S.iwidth/2,CV_16U);
    unsigned short max = 0;
    unsigned short min = 65535;
    for( int row=0; row < S.iheight/2 ; row++ ) {
        for( int col=0 ; col < S.iwidth/2 ; col++ ) {
            scaled.at<unsigned short>(row,col) = (cv::sum( image( cv::Range(row*2,row*2+1) , cv::Range(col*2,col*2+1) ) )[0]);
            if ( scaled.at<unsigned short>(row,col) > max ) {
                max = scaled.at<unsigned short>(row,col) ; 
            }
            else if ( scaled.at<unsigned short>(row,col) < min ) {
                min = scaled.at<unsigned short>(row,col);
            }
        }
    }
    
    for( int row=0; row < 4 ; row++ ) {
        for( int col=0 ; col < 4 ; col++ ) {
            printf( "%4d", scaled.at<unsigned short>(row,col) );
        }
        printf(" - ");
        for( int col=0 ; col < 4 ; col++ ) {
            printf( "%4d", image.at<unsigned short>(row,col) );
        }
        printf(" - ");
        for( int col=0 ; col < 4 ; col++ ) {
            printf( "%4d", RawProcessor.imgdata.image[row*S.width+col][0] );
        }
        printf(" - ");
        for( int col=0 ; col < 4 ; col++ ) {
            printf("%4d", RawProcessor.imgdata.rawdata.raw_image[row*S.width+col] );
        }
        
        printf("\n\n");
    }
    
   // int mult = 65535/(max-min);
   // printf("max = %d - min =%d - mult = %d\n",max, min, mult);
   // printf("16 bits = %d - 14 bits = %d - 12 bits = %d\n",65535, 16384, 4096);
   // scaled = mult*(scaled-min);
    int mult = 65535/max;
    printf("max = %d - min =%d - mult = %d\n",max, min, mult);
    printf("16 bits = %d - 14 bits = %d - 12 bits = %d\n",65536, 16384, 4096);
    scaled = mult*scaled;
    
    
 /************/   
 /***************
 
    cv::Mat image(S.iheight,S.iwidth,CV_16U,RawProcessor.imgdata.image[0] );// new blank image
    
   // for( int row=0 ; row < S.iheight ; row++ ) {
    //    for ( int col=0 ; col < S.iwidth ; col++ ) {
     //       image.at<unsigned short>(row,col) = RawProcessor.imgdata.image[row*S.width+col][0];
     //   }
    //}
    
    
    cv::namedWindow( "Display window", cv::WINDOW_NORMAL );// create a window for display.
    cv::Mat scaled(S.iheight/2,S.iwidth/2,CV_16U);
    unsigned short max = 0;
    unsigned short min = 65535;
    for( int row=0; row < S.iheight/2 ; row++ ){
        for( int col=0 ; col < S.iwidth/2 ; col++ ) {
            scaled.at<unsigned short>(row,col) = (cv::sum( image( cv::Range(row*2,row*2+1) , cv::Range(col*2,col*2+1) ) )[0]);
            if ( scaled.at<unsigned short>(row,col) > max ) {
                max = scaled.at<unsigned short>(row,col) ; 
            }
            else if ( scaled.at<unsigned short>(row,col) < min ) {
                min = scaled.at<unsigned short>(row,col);
            }
        }
    }
    
    for( int row=0; row < 4 ; row++ ) {
        for( int col=0 ; col < 4 ; col++ ) {
            printf( "%4d", scaled.at<unsigned short>(row,col) );
        }
        printf(" - ");
        for( int col=0 ; col < 4 ; col++ ) {
            printf( "%4d", image.at<unsigned short>(row,col) );
        }
        printf(" - ");
        for( int col=0 ; col < 4 ; col++ ) {
            printf( "%4d", RawProcessor.imgdata.image[row*S.width+col][0] );
        }
        printf(" - ");
        for( int col=0 ; col < 4 ; col++ ) {
            printf("%4d", RawProcessor.imgdata.rawdata.raw_image[row*S.width+col] );
        }
        
        printf("\n\n");
    }
    
    int mult = 65535/(max-min);
    printf("max = %d - min =%d - mult = %d\n",max, min, mult);
    scaled = mult*(scaled-min);
  

/**************/  
    
    cv::imshow( "Display window", scaled );// show our image inside it.
    cv::waitKey(0);// wait for a keystroke in the window
    
    return 0;
    
}
