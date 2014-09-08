
// OPENCV
//#include "opencv2/core/core.hpp"
//using namespace cv;

// LIBRAW
//#include "libraw/libraw.h"

#include <string>

// NOVA
#include "rawimage.hpp"
#include "ui.hpp"



using namespace std;

// Fonction principale, executée au lancement du programme
int main() {

    int ret = 0;
    
    // création de l'interface graphique
    UI &ihm = UI::getUI();
    ihm.logInfo("NOVA", "main()", "Interface graphique créée");
    
    try {
        
        string fileName = "RAW.ARW";

        RawImage monImage(fileName);
        monImage.loadRaw();
        ihm.logInfo("NOVA", "main()", "image RAW chargée");
        

        ihm.afficher(monImage.getRawImage());
        
    }
    catch(int codeError){
        ihm.showLastError();
    }
    catch(...) {
        ret = -1 ;
        ihm.logInfo("NOVA", "main()", "Quelque chose d'inconnu n'a pas fonctionné comme prévu");
    }
    
    return ret;
}
