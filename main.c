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

    bson_error_t error;
    bson_t *command, reply;
    command = BCON_NEW("ping", BCON_INT32(1));
    bool retval = mongoc_client_command_simple(client, "admin", command, NULL, &reply, &error);

    if (!retval) {
        // Connection echouée
        fprintf(stderr, "%s\n", error.message);
        return EXIT_FAILURE;
    }
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
    //delete_all_documents_from_collection(client,"actiaDataBase","testCollection");

   //----------------------------------------- Application pour l'utilisateur -----------------------------------------//


    while (true) {
        printf("\n");
        printf("\n========================================================\n");
        printf("||             Application MongoDB en C               ||\n");
        printf("========================================================\n");
        printf("============== Veuillez choisir une option : ===========\n");
        printf("||                                                    ||\n");
        printf("|| 1 - Create                                         ||\n");
        printf("|| 2 - Read                                           ||\n");
        printf("|| 3 - Update                                         ||\n");
        printf("|| 4 - Delete                                         ||\n");
        printf("|| 5 - Quitter le programme                           ||\n");
        printf("||                                                    ||\n");
        printf("========================================================\n");
        int choix;
        printf("Votre choix : ");
        int scanResult = scanf("%d", &choix);
        if (scanResult < 1 || choix < 1 || choix > 5) {
            printf("Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n");
            continue;
        }
        printf("\n");

        if(choix == 1){
            printf("\n=======================================================================\n");
            printf("||                          Menu Create                              ||\n");
            printf("=======================================================================\n");
            printf("=================== Veuillez choisir une option : ====================\n");
            printf("||                                                                   ||\n");
            printf("|| 1 - Creer une collection dans la base de donnes 'actiaDataBase'   ||\n");
            printf("|| 2 - Creer un document avec un fichier XML en valeur               ||\n");
            printf("|| 3 - Stocker l'ensemble des fichiers XML d'un dossier dans MongoDB ||\n");
            printf("||     (EN NE CHARGEANT PAS LES FICHIERS EN MEMOIRE)                 ||\n");
            printf("|| 4 - Stocker l'ensemble des fichiers XML d'un dossier dans MongoDB ||\n");
            printf("||     (EN CHARGEANT LES FICHIERS EN MEMOIRE)                        ||\n");
            printf("|| 5 - Quitter le menu Create                                        ||\n");
            printf("||                                                                   ||\n");
            printf("========================================================================\n");

            int choixCreate;
            printf("Votre choix : ");
            scanResult = scanf("%d", &choixCreate);
            if (scanResult < 1 || choixCreate < 1 || choixCreate > 5) {
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n");
                continue;
            }
            printf("\n");

            if(choixCreate == 1){
                //Création d'une collection
                printf("Veuillez saisir le nom de la collection a creer : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                createCollection(client,"actiaDataBase",nomCollection);
            }
            else if(choixCreate == 2){
                //Création d'un document avec un fichier XML en valeur
                printf("Veuillez saisir le nom de la collection dans laquelle inserer le document : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                printf("Veuillez saisir le chemin du fichier XML a inserer : ");
                char cheminFichier[100];
                int resultScanFChemin = scanf("%s", cheminFichier);
                if (resultScanFChemin < 1 ) {
                    printf("Erreur lors de la saisie du chemin du fichier.\n");
                    continue;
                }
                insert_xml_into_collection(client,"actiaDataBase",nomCollection,cheminFichier);
            }
            else if(choixCreate == 3){
                //Création de plusieurs documents avec les fichiers XML d'un dossier (sans charger les fichiers en mémoire)
                printf("Veuillez saisir le nom de la collection dans laquelle inserer les documents : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                printf("Veuillez saisir le chemin du dossier contenant les fichiers XML a inserer : ");
                char cheminDossier[100];
                int resultScanFChemin = scanf("%s", cheminDossier);
                if (resultScanFChemin < 1 ) {
                    printf("Erreur lors de la saisie du chemin du dossier.\n");
                    continue;
                }
                process_files_one_by_one(client,"actiaDataBase",nomCollection,cheminDossier);
                            }
            else if(choixCreate == 4){
                //Création de plusieurs documents avec les fichiers XML d'un dossier (en chargeant les fichiers en mémoire)
                printf("Veuillez saisir le nom de la collection dans laquelle inserer les documents : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                printf("Veuillez saisir le chemin du dossier contenant les fichiers XML a inserer : ");
                char cheminDossier[100];
                int resultScanFChemin = scanf("%s", cheminDossier);
                if (resultScanFChemin < 1 ) {
                    printf("Erreur lors de la saisie du chemin du dossier.\n");
                    continue;
                }
                process_all_files_at_once(client,"actiaDataBase",nomCollection,cheminDossier);
            }
            else if(choixCreate == 5){
                //Quitter le menu Create
                continue;
            }
        }
        else if(choix == 2){
            printf("\n=======================================================================\n");
            printf("||                          Menu Read                                ||\n");
            printf("=======================================================================\n");
            printf("=================== Veuillez choisir une option : ====================\n");
            printf("||                                                                   ||\n");
            printf("|| 1 - Lire tous les documents d'une collection                      ||\n");
            printf("|| 2 - Lire tous les documents d'une collection avec le type Human   ||\n");
            printf("|| 3 - Quittez le menu Read                                          ||\n");
            printf("||                                                                   ||\n");
            printf("========================================================================\n");
            int choixRead;
            printf("Votre choix : ");
            int scanResultRead = scanf("%d", &choixRead);
            if (scanResultRead < 1 || choixRead < 1 || choixRead > 3) {
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 3.\n");
                continue;
            }
            printf("\n");
            if(choixRead == 1){
                printf("Veuillez saisir le nom de la collection a lire : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                read_all_documents(client,"actiaDataBase",nomCollection);
            }
            else if(choixRead == 2){
                printf("Veuillez saisir le nom de la collection a lire : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                read_filtered_documentsHuman(client,"actiaDataBase",nomCollection);
            }
            else if(choixRead == 3){
                //Quitter le menu Read
                continue;
            }
        }
        else if(choix == 3){
            //update_document_from_collection("actiaDataBase","testCollection","6475ad3c4a42d4efb10934f2","cha5czsfgzf4");
            //update_all_document_type(client,"chatchat");


            printf("\n=======================================================================\n");
            printf("||                          Menu Update                                ||\n");
            printf("=======================================================================\n");
            printf("=================== Veuillez choisir une option : ====================\n");
            printf("||                                                                   ||\n");
            printf("|| 1 - Modifie un document avec son ID                               ||\n");
            printf("|| 2 - Modifie tous les documents d'une collection (le type)         ||\n");
            printf("|| 3 - Quitter le menu Update                                        ||\n");
            printf("||                                                                   ||\n");
            printf("========================================================================\n");
            int choixUpdate;
            printf("Votre choix : ");
            int resultScanf = scanf("%d", &choixUpdate);
            if (resultScanf < 1 || choixUpdate < 1 || choixUpdate > 3) {
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 3.\n");
                continue;
            }
            printf("\n");

            if(choixUpdate == 1){
                printf("Veuillez saisir le nom de la collection dans laquelle modifier le document : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                printf("Veuillez saisir l'ID du document a modifier : ");
                char idDocument[100];
                int resultScanFId = scanf("%s", idDocument);
                if (resultScanFId < 1 ) {
                    printf("Erreur lors de la saisie de l'ID du document.\n");
                    continue;
                }
                printf("Veuillez saisir la nouvelle valeur : ");
                char typeDocument[100];
                int resultScanFType = scanf("%s", typeDocument);
                if (resultScanFType < 1 ) {
                    printf("Erreur lors de la saisie de la nouvelle valeur.\n");
                    continue;
                }
                update_document_from_collection("actiaDataBase",nomCollection,idDocument,typeDocument);
            }
            else if(choixUpdate == 2){
                printf("Veuillez saisir le nom de la collection dans laquelle modifier les documents : ");
                char nomCollection[100];
                int resultScanF = scanf("%s", nomCollection);
                if (resultScanF < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                printf("Veuillez saisir le nouveau type des documents : ");
                char typeDocument[100];
                int resultScanFType = scanf("%s", typeDocument);
                if (resultScanFType < 1 ) {
                    printf("Erreur lors de la saisie du nouveau type des documents.\n");
                    continue;
                }
                update_all_document_type(client,"actiaDataBase",nomCollection,typeDocument);
            }
            else if(choixUpdate == 3){
                //Quitter le menu Update
                continue;
            }
        }
        else if(choix == 4){
            printf("\n=======================================================================\n");
            printf("||                          Menu Delete                               ||\n");
            printf("=======================================================================\n");
            printf("=================== Veuillez choisir une option : ====================\n");
            printf("||                                                                   ||\n");
            printf("|| 1 - Supprimer tous les documents d'une collection                 ||\n");
            printf("|| 2 - Quitter le menu Delete                                        ||\n");
            printf("||                                                                   ||\n");
            printf("========================================================================\n");

            int choixDelete;
            printf("Votre choix : ");
            int resultScanf = scanf("%d", &choixDelete);
            if (resultScanf < 1 || choixDelete < 1 || choixDelete > 2) {
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 2.\n");
                continue;
            }
            printf("\n");

            if(choixDelete == 1){
                printf("Veuillez saisir le nom de la collection a vider : ");
                char nomCollection[100];
                int resultScanf = scanf("%s", nomCollection);
                if (resultScanf < 1 ) {
                    printf("Erreur lors de la saisie du nom de la collection.\n");
                    continue;
                }
                delete_all_documents_from_collection(client,"actiaDataBase",nomCollection);
            }
            else if(choixDelete == 2){
                //Quitter le menu Delete
                continue;
            }
        }
        else if(choix ==5){
            printf("========================================================\n");
            printf("|| Vous avez quitte le programme, a bientot !         ||\n");
            printf("========================================================\n");
            break;
        }

    }
    mongoc_cleanup(); // Libération de la mémoire


    return 0;
}
