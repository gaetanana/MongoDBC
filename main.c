#include <stdio.h>
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include "READ\READ.c"
#include "CREATE\CREATE.c"
#include "DELETE\DELETE.c"
#include "UPDATE\UPDATE.c"
#include "libxml/parser.h"

int main() {

    //-----------------------Connection au serveur-----------------------//

    mongoc_client_t *client;
    mongoc_init();
    //client = mongoc_client_new("mongodb://root:examplepassword@localhost:27017"); //Connection à la base de données Docker
    client = mongoc_client_new("mongodb://localhost:27017"); //Connection au serveur local

    //---------------------- Fonctions disponibles ----------------------//

    /**Partie création de documents*/
    //insert_document_into_collection(client,"actiaDataBase","testCollection","chat2"); //Insertion d'un document dans une collection
    //insert_xml_into_collection(client,"actiaDataBase","testCollection","C:\\Users\\g.gonfiantini\\Desktop\\data\\FichiersXML\\FichiersXML8000\\Onvif_Metadata_C1000_2023-04-21_16-48-58.629.xml");
    //process_files_one_by_one(client,"actiaDataBase","testCollection","C:\\Users\\g.gonfiantini\\Desktop\\data\\FichiersXML\\FichiersXML104295");
    //process_all_files_at_once(client,"actiaDataBase","testCollection","C:\\Users\\g.gonfiantini\\Desktop\\data\\FichiersXML\\FichiersXML8000"); //Chargement des fichiers en mémoire

    /**Partie lecture de documents*/
    //read_all_documents(client,"actiaDataBase","testCollection");
    //read_filtered_documentsHuman(client,"actiaDataBase","testCollection");

    /**Partie mise à jour de documents*/
    //update_document_from_collection("actiaDataBase","testCollection","6475ad3c4a42d4efb10934f2","cha5czsfgzf4");
    //update_all_document_type(client,"chatchat");

    /**Partie suppression documents*/
    delete_all_documents_from_collection(client,"actiaDataBase","testCollection");

   //----------------------------------------- Application pour l'utilisateur -----------------------------------------//


   //Todo faire une application intuitive pour l'utilisateur

    return 0;
}
