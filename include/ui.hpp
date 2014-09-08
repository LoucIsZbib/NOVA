#ifndef NOVA_UI
#define NOVA_UI

// OPENCV
#include "opencv2/core/core.hpp"

// INCLUDE GENERAUX
#include <string>

struct ligneDeLog{
    std::string classe;
    std::string methode;
    std::string msg;
};

class UI {


    
    private:
    // ATTRIBUTS
        
        std::vector<ligneDeLog> m_infoLogBook;
        
        std::vector<ligneDeLog> m_errorLogBook;
        
        // self, objet, pour acces singleton
        static UI m_UI;
        
        
    //METHODES
        
        cv::Mat mettreALEchelle(cv::Mat image);
        
        // constructeur en private --> singleton
        UI();   

        
    public:
    //ATTRIBUTS
        
        
    // METHODES
        
        // pour récupérer l'objet -- singleton
        static UI& getUI(); 
        
        void afficher(cv::Mat image); 
        
        // diffuser et stocker une information
        void logInfo(const std::string &classe, const std::string &methode, const std::string &msg); 
        
        // diffuser et stocker une erreur
        void logError(const std::string &classe, const std::string &methode, const std::string &msg); 
        
        // affiche la dernière erreur enregistrée
        void showLastError();
        

};

#endif