#include "DB.h"
#include <iostream>
#include <string>

DB *DB::instance = 0;

DB::DB() : driver(get_driver_instance()) {
    connection_properties["hostName"] = DB_HOST;
    connection_properties["port"] = DB_PORT;
    connection_properties["userName"] = DB_USER;
    connection_properties["password"] = DB_PASS;
    connection_properties["OPT_RECONNECT"] = true;
    // use database
    try {
        Connection *connection = driver->connect(connection_properties);
        try {
            connection->setSchema(DB_NAME);
        }
        catch (SQLException &e) {
            cout << "ERROR";
        }
        
        delete connection;
    }
    catch (SQLException &e) {
        cout << e.getErrorCode() << " " << e.what() << " " << e.getSQLState();
        throw e;
    }
}

DB & DB::getInstance() {
    if (DB::instance == 0) {
        instance = new DB();
    }
    return *instance;
}
Connection * DB::getConnection() {
    try {
        Connection *connection = driver->connect(connection_properties);
        connection->setSchema(DB_NAME);
        return connection;
    } catch (SQLException &e) {
        cout << e.what();
    }
    return 0;
}

void DB::countNumberOfAlbumBetween() {
    string Y1, Y2;
    cout << "Please enter start date (yyyy-mm-dd)" << endl;
    cin >> Y1;
    cout << "Please enter end date (yyyy-mm-dd)" << endl;
    cin >> Y2;
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    PreparedStatement *pstmt = connection->prepareStatement("SELECT COUNT(distinct idalbum) AS counter FROM album WHERE album_sdate>=? AND album_edate<=?");
    pstmt->setString(1, Y1);
    pstmt->setString(2, Y2);
    ResultSet *res = pstmt->executeQuery();
    res->first();
    cout<<"The number of albumes between "<<Y1<<" and "<<Y2<< " are: "<<res->getUInt("counter");
    cout << endl<<endl;
    delete connection;
    delete pstmt;
    delete res;
}

void DB::countNumberOfSongsBetween () {
    string fName,lName,Y1,Y2;
    cout<< "Enter musician first name:";
    cin>> fName;
    cout<< "Enter musician Last name:";
    cin>> lName;
    cout << "Please enter start date (yyyy-mm-dd)" << endl;
    cin >> Y1;
    cout << "Please enter end date (yyyy-mm-dd)" << endl;
    cin >> Y2;
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    
    PreparedStatement *pstmt = connection->prepareStatement("SELECT COUNT(distinct idsong) AS counter FROM song WHERE song_rdate BETWEEN ? AND ? AND idsong IN (SELECT songid FROM musician_in_song WHERE musician_in_song_id = (SELECT idmusician FROM musician WHERE musician_fname = ? AND musician_lname = ?))");
    pstmt->setString(1, Y1);
    pstmt->setString(2, Y2);
    pstmt->setString(3, fName);
    pstmt->setString(4, lName);
    ResultSet *res = pstmt->executeQuery();
    res->first();
    cout<<"The number of song between "<<Y1<<" and "<<Y2<< " that "<<fName <<" " <<lName <<" recorded: " <<res->getUInt("counter");
    cout << endl<<endl;
    delete connection;
    delete pstmt;
    delete res;
}

void DB::countNumberOfDiffAlbumsForMusician () {
    string fName,lName,Y1,Y2;
    cout<< "Enter musician first name:";
    cin>> fName;
    cout<< "Enter musician Last name:";
    cin>> lName;
    cout << "Please enter start date (yyyy-mm-dd)" << endl;
    cin >> Y1;
    cout << "Please enter end date (yyyy-mm-dd)" << endl;
    cin >> Y2;
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    
    PreparedStatement *pstmt = connection->prepareStatement(" SELECT COUNT(distinct albumid) AS counter FROM songs_in_album WHERE song_id IN(SELECT  idsong FROM song WHERE song_rdate BETWEEN ? AND ? AND idsong IN (SELECT songid FROM musician_in_song WHERE musician_in_song_id = (SELECT idmusician FROM musician WHERE musician_fname = ? AND musician_lname = ?)))");
    pstmt->setString(1, Y1);
    pstmt->setString(2, Y2);
    pstmt->setString(3, fName);
    pstmt->setString(4, lName);
    ResultSet *res = pstmt->executeQuery();
    res->first();
    cout<<"The number of albums between "<<Y1<<" and "<<Y2<< " that "<<fName <<" " <<lName <<" recorded: " <<res->getUInt("counter");
    cout << endl<<endl;
    delete connection;
    delete pstmt;
    delete res;
}

void DB::popularInstrument () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT instrument_type FROM instrument WHERE idinstrument =(SELECT im.instrument_id FROM player AS p, instru_manu AS im WHERE p.player_im_id = im.idminstru_manu GROUP BY (im.instrument_id) ORDER BY COUNT(im.instrument_id) DESC LIMIT 1)");
    res->first();
    cout<<"The most popular instrument: "<< res->getString("instrument_type");
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

void DB::listOfInstrumentsInAlbum () {
    string aName;
    cout<< "Enter Album Name:";
    getline(cin,aName);
    getline(cin,aName);

    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    
    PreparedStatement *pstmt = connection->prepareStatement("SELECT instrument_type FROM instrument WHERE idinstrument IN(SELECT instrument_id FROM instru_manu WHERE idminstru_manu IN(SELECT player_im_id FROM player WHERE player_sid IN(SELECT song_id FROM songs_in_album WHERE albumid =(SELECT idalbum FROM album WHERE album_name = ?))))");
    pstmt->setString(1, aName);
    ResultSet *res = pstmt->executeQuery();
    res->beforeFirst();
    int count = 1;
    cout<<"List of all instruments in album " << aName <<" :"<<endl;
    while(res->next()){
        cout<<count<<". " << res->getString("instrument_type")<<endl;
        count++;
    }
    delete connection;
    delete pstmt;
    delete res;
    
}


void DB::mostProdectiveProducer () {
    string Y1, Y2;
    cout << "Please enter start date (yyyy-mm-dd)" << endl;
    cin >> Y1;
    cout << "Please enter end date (yyyy-mm-dd)" << endl;
    cin >> Y2;
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    PreparedStatement *pstmt = connection->prepareStatement("SELECT producer_fname,producer_lname FROM producer WHERE idproducer=(SELECT producer_id FROM(SELECT producer_id FROM produced_by WHERE album_id IN(SELECT idalbum FROM album WHERE album_sdate>=? AND album_edate<=?))AS temp GROUP BY temp.producer_id ORDER BY COUNT(*)DESC LIMIT 1)");
    pstmt->setString(1, Y1);
    pstmt->setString(2, Y2);
    ResultSet *res = pstmt->executeQuery();
    res->first();
    cout<<"The most Prodective Producer between "<<Y1<<" and "<<Y2<< " is: "<<res->getString("producer_fname")<<" "<< res->getString("producer_lname");
    cout<< endl<<endl;
    delete connection;
    delete pstmt;
    delete res;
}

void DB::mostPopularManufacturer () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT manufacturer_name FROM manufacturer WHERE idmanufacturer =(SELECT im.manufacturer_id FROM player AS p, instru_manu AS im WHERE p.player_im_id = im.idminstru_manu GROUP BY (im.manufacturer_id) ORDER BY COUNT(im.manufacturer_id) DESC LIMIT 1)");
    res->first();
    cout<<"The most popular manufacturer: "<< res->getString("manufacturer_name");
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

void DB::numberOfMusicians () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT COUNT(distinct idmusician) AS counter FROM musician");
    res->first();
    cout<<"The number of Musician: "<< res->getUInt("counter");
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

void DB::musicianCollaborator () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT musician_fname,musician_lname FROM musician WHERE idmusician=(SELECT musician_in_song_id FROM(SELECT musician_in_song_id FROM musician_in_song WHERE songid IN(SELECT songid FROM musician_in_song GROUP BY songid HAVING COUNT(*)>1 ORDER BY COUNT(*) DESC))AS temp GROUP BY temp.musician_in_song_id ORDER BY COUNT(*) DESC LIMIT 1)");
    res->first();
    cout<<"The collaborative musician: "<< res->getString("musician_fname")<<" " <<res->getString("musician_lname");
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
    }

void DB::mostPopularGenre () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT song_genre FROM song GROUP BY song_genre ORDER BY COUNT(*)DESC LIMIT 1");
    res->first();
    cout<<"The most popular genre is: "<< res->getString("song_genre");
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

void DB::mostProdectiveTechniacian () {
    string Y1, Y2;
    cout << "Please enter start date (yyyy-mm-dd)" << endl;
    cin >> Y1;
    cout << "Please enter end date (yyyy-mm-dd)" << endl;
    cin >> Y2;
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    PreparedStatement *pstmt = connection->prepareStatement("SELECT technician_fname,technicianc_lname FROM technician WHERE idtechnician=(SELECT song_techid FROM song WHERE idsong IN(SELECT idsong FROM song WHERE song_rdate between ? and ?) GROUP BY song_techid ORDER BY COUNT(*)DESC LIMIT 1)");
    pstmt->setString(1, Y1);
    pstmt->setString(2, Y2);
    ResultSet *res = pstmt->executeQuery();
    res->first();
    cout<<"The most Prodective technician between "<<Y1<<" and "<<Y2<< " is: "<<res->getString("technician_fname")<<" "<< res->getString("technicianc_lname");
    cout<< endl<<endl;
    delete connection;
    delete pstmt;
    delete res;
}

void DB::firstAlbum () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT album_name FROM album ORDER BY album_sdate ASC LIMIT 1");
    res->first();
    cout<<"The first album recorded: "<< res->getString("album_name");
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

void DB::listOfSongsInMultiAlbums () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT song_name FROM song WHERE idsong IN(SELECT song_id FROM songs_in_album GROUP BY song_id HAVING COUNT(*)>1 ORDER BY COUNT(*) DESC)");
    res->beforeFirst();
    int count = 1;
    cout<<"List of all song that are in more than one album:"<<endl;
    while(res->next()){
        cout<<count<<". " << res->getString("song_name")<<endl;
        count++;
    }
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

void DB::listOfTechInEntireAlbum () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT technician_fname,technicianc_lname FROM technician WHERE idtechnician IN( SELECT song_techid FROM song WHERE idsong IN(SELECT song_id FROM songs_IN_album WHERE albumid IN(SELECT tia.idalbum FROM (SELECT distinct a.idalbum,s.song_techid FROM song AS s,album AS a WHERE a.idalbum IN(SELECT albumid FROM songs_in_album WHERE song_id=s.idsong))AS tia GROUP BY tia.idalbum HAVING COUNT(tia.idalbum)=1)))");
    res->beforeFirst();
    int count = 1;
    cout<<"list of technicians  who participated in the recording of an entire album:"<<endl;
    while(res->next()){
        cout<<count<<". " << res->getString("technician_fname")<<" " <<res->getString("technicianc_lname")<<endl;
        count++;
    }
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

void DB::mostVersatileMusician () {
    Connection *connection = driver->connect(connection_properties);
    connection->setSchema(DB_NAME);
    Statement *stmt = connection->createStatement();
    ResultSet *res = stmt->executeQuery("SELECT musician_fname,musician_lname FROM musician WHERE idmusician =(SELECT i.musician_in_song_id FROM musician_in_song AS i,song AS s WHERE i.songid=s.idsong GROUP BY i.musician_in_song_id ORDER BY COUNT(distinct s.song_genre) DESC LIMIT 1)");
    res->first();
    cout<<"The most versatile musician: "<< res->getString("musician_fname")<<" " <<res->getString("musician_lname");
    cout<< endl<<endl;
    delete connection;
    delete stmt;
    delete res;
}

