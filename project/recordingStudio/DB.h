// Chen Gutman 205616147
//Avital Kahani 205688666

#ifndef DB_h
#define DB_h
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
using namespace sql;
using namespace std;
#define DB_NAME "recordingstudio"
#define DB_HOST "tcp://127.0.0.1/"
#define DB_PORT 3306
#define DB_USER "root"
#define DB_PASS "yourpassword"

class DB {
private:
    static DB *instance;
    
    Driver *driver;
    ConnectOptionsMap connection_properties;
  
    DB();
public:
    static DB& getInstance();
    
    Connection *getConnection();
    
    void countNumberOfAlbumBetween ();
    void countNumberOfSongsBetween  ();
    void countNumberOfDiffAlbumsForMusician ();
    void popularInstrument ();
    void listOfInstrumentsInAlbum ();
    void mostProdectiveProducer ();
    void mostPopularManufacturer ();
    void numberOfMusicians ();
    void musicianCollaborator ();
    void mostPopularGenre ();
    void mostProdectiveTechniacian ();
    void firstAlbum ();
    void listOfSongsInMultiAlbums ();
    void listOfTechInEntireAlbum  ();
    void mostVersatileMusician ();
    virtual ~DB() {}
};
#endif
