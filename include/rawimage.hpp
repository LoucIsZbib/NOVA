#ifndef NOVA_RAWIMAGE_H
#define NOVA_RAWIMAGE_H

// OPENCV
#include "opencv2/core/core.hpp"

// LIBRAW
#include "libraw/libraw.h"

#include <string>


/*
*   Profondeurs des couleurs de mes appareils
*
*   A330 => 12 bits / px
*
*  NEX 6 => 12 bits / px
*
*/

class RawImage {

    private:
    // ATTRIBUTS
        // chemin et nom du fichier
        std::string m_fileName;
        
        // Stockage de chacune des couches R, G, B, G ; valeurs brutes ;
        std::vector<cv::Mat> m_rawImage;
        
        
    // METHODES
    
    
    public:
    // ATTRIBUTS
        
        
        
        
    // METHODES 
        // Constructeur
        RawImage(std::string fullFileName);
        
        // Charger le fichier et stocker l'image dans une structure exploitable 
        // (std::vector<cv::Mat> en RGBG)
        void loadRaw();
        
        // récupérer un Mat multichannel des données brutes
        cv::Mat getRawImage();
        

        
        
        
};

#endif
