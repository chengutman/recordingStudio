#include <iostream>
#include "DB.h"
using namespace std;

int main(int argc, const char * argv[]) {
    bool flag = true;
    int n;
    DB& db = DB::getInstance();
    while(flag) {
        cout << "Please choose an option from the menu followed by enter:" << endl << "1. Print the number of albumes that were recoreded bewteen two dates" << endl << "2. Print the number of songs that musician recoreded between two dates" << endl << "3. Print the number of diffrenet albumes that a musician participated between two dates" << endl << "4. Print the most popular musical instrument" << endl << "5. Print list of instruments in an album" << endl << "6. Print the most productive producer between two dates" << endl << "7. Print the most popular musicial instrument manufacturer" << endl << "8. Print the number of musicians that recorded through out the years " << endl << "9. Print the musician that collaborated with the most other musicians" << endl << "10. Print the most popular ganre" << endl << "11. Print the technician that recorded the most number of songs betweeen two dates" << endl << "12. Print the first album that was recorded in the studio" << endl << "13. Print list of songs that are in two or more albumes" << endl << "14. Print list of technicians  who participated in the recording of an entire album " << endl << "15. Print the most versatile musician" << endl << "16. Exit"<<endl;
        
        cin >> n;
        switch (n) {
            case 1: {
                db.countNumberOfAlbumBetween();
                break;
            }
            case 2: {
                db.countNumberOfSongsBetween();
                break;
            }
            case 3: {
                db.countNumberOfDiffAlbumsForMusician();
                break;
            }
            case 4: {
                db.popularInstrument();
                break;
            }
            case 5: {
                db.listOfInstrumentsInAlbum();
                break;
            }
            case 6: {
                db.mostProdectiveProducer();
                break;
            }
            case 7: {
                db.mostPopularManufacturer();
                break;
            }
            case 8: {
                db.numberOfMusicians();
                break;
            }
            case 9: {
                db.musicianCollaborator();
                break;
            }
            case 10: {
                db.mostPopularGenre();
                break;
            }
            case 11: {
                db.mostProdectiveTechniacian();
                break;
            }
            case 12: {
                db.firstAlbum();
                break;
            }
            case 13: {
                db.listOfSongsInMultiAlbums();
                break;
            }
            case 14: {
                db.listOfTechInEntireAlbum();
                break;
            }
            case 15: {
                db.mostVersatileMusician();
                break;
            }
            case 16: {
                cout << "The End" << endl << endl;
                flag = false;
                break;
            }
            default:
                cout << "Wrong option number, please try again" << endl << endl;
                break;
        }
    }
    return 0;
}
