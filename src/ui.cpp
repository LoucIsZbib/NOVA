#include "ui.hpp"

// OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;


#include <vector>
using namespace std;

// pour affichage en console
//#include <stdio.h>
#include <iostream>

// création initiale de l'objet
UI UI::m_UI = UI();

// constructeur
UI::UI(){

}

// récupérer l'instance du singleton
UI& UI::getUI(){
    return m_UI;
}

void UI::afficher(Mat image) {
/*
    // Fonctionne !
    namedWindow( "Gav", WINDOW_NORMAL );// create a window for display.
    printf("fenetre créée\n");

    Mat gav = imread("gavroche.png", 3);
    imshow( "Gav", gav);// show our image inside it.
    waitKey(0);


    vector<Mat> multLayer;
    printf("vecteur créé\n");


    split(gav, multLayer);
    printf("image splitée\n");

    namedWindow( "blob", WINDOW_NORMAL );// create a window for display.
    printf("fenetre blob créée\n");
    imshow( "blob", multLayer[0]);// show our image inside it.
    printf("image affichée\n");
    waitKey(0);// wait for a keystroke in the window
*/

/*  // Ne plante pas, affiche du noir
    namedWindow( "blob", WINDOW_NORMAL );// create a window for display.
    printf("fenetre blob créée\n");
    
    imshow( "blob", mettreALEchelle(image));// show our image inside it.
    printf("image affichée\n");
    
    waitKey(0);// wait for a keystroke in the window
/**/

/**/
    vector<Mat> multLayer;
    printf("vecteur créé\n");
    
    split(image, multLayer);
    printf("image splitée\n");

    namedWindow( "blob", WINDOW_NORMAL );// create a window for display.
    printf("fenetre blob créée\n");
    for (int i=0; i< 4 ; i++) {
        imshow( "blob", mettreALEchelle(multLayer[1]));// show our image inside it.
        printf("image affichée\n");
    
        waitKey(0);// wait for a keystroke in the window
    }

/**/

}

Mat UI::mettreALEchelle(Mat image) {

    Mat scaled;
    
    //scaled = (unsigned short) 65536/4096 * image ;
    scaled = 16 * image ;
    
    return scaled;
}

// diffuser et stocker une information
void UI::logInfo(const string &classe, const string &methode, const string &msg){
    //affichage
    cout << classe << " " << methode << " " << msg << endl;
    //stockage
    ligneDeLog monLog = {classe, methode, msg};
    m_infoLogBook.push_back(monLog);
}

// diffuser et stocker une information
void UI::logError(const string &classe, const string &methode, const string &msg){
    //affichage
    cout << classe << " - " << methode << " - " << msg << endl;
    //stockage
    ligneDeLog monLog = {classe, methode, msg};
    m_errorLogBook.push_back(monLog);
}

void UI::showLastError(){
    if (!m_errorLogBook.empty()) {
       ligneDeLog dernierElement = m_errorLogBook.back();
       cout << dernierElement.classe << " - " << dernierElement.methode << " - " << dernierElement.msg << endl;
    }
}