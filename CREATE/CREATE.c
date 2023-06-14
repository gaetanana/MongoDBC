#include <windows.h>
#include <stdio.h>
#include <bson.h>
#include <mongoc.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <jansson.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

//-------------------- Fonctions auxiliaires --------------------

/**
 * Cette fonction permet de convertir un nœud XML en un objet JSON.
 * @param node
 * @return
 */
json_t *xmlNodeToJson(xmlNodePtr node) { // Définition de la fonction xmlNodeToJson qui prend un pointeur vers un nœud XML en entrée et renvoie un pointeur vers un objet json_t.

    json_t *jsonNode = json_object(); // Création d'un nouvel objet JSON.

    // Parcours de tous les attributs du nœud XML.
    for (xmlAttr *attr = node->properties; attr; attr = attr->next) {
        xmlChar *value = attr->children->content; // Obtention de la valeur de l'attribut.
        // Ajout de l'attribut et de sa valeur à l'objet JSON.
        json_object_set_new(jsonNode, (const char *)attr->name, json_string((const char *)value));
    }

    // Parcours de tous les nœuds enfants du nœud XML.
    for (xmlNodePtr child = node->children; child; child = child->next) {
        if (child->type == XML_TEXT_NODE) { // Vérification si le nœud enfant est un nœud de texte.
            xmlChar *content = xmlNodeGetContent(child); // Obtention du contenu du nœud texte.
            if (content && *content && !isspace(*content)) { // Vérification si le contenu n'est pas vide ou un espace.
                // Ajout du contenu du nœud texte à l'objet JSON.
                json_object_set_new(jsonNode, (const char *)child->name, json_string((const char *)content));
            }
            xmlFree(content); // Libération de la mémoire allouée pour le contenu.
        } else if (child->type == XML_ELEMENT_NODE) { // Vérification si le nœud enfant est un nœud élément.
            json_t *childJson = xmlNodeToJson(child); // Conversion récursive du nœud enfant en JSON.
            // Ajout du nœud enfant converti en JSON à l'objet JSON.
            json_object_set_new(jsonNode, (const char *)child->name, childJson);
        }
    }

    return jsonNode; // Renvoie l'objet JSON résultant.
}


typedef struct {
    char *jsonStr;
    double conversionTime;
} XmlToJsonResult;

/**
 * Cette fonction permet de convertir une chaîne de caractères XML en une chaîne de caractères JSON.
 * @param xml
 * @return
 */
XmlToJsonResult xmlToJson(const char *xml) { // Définition de la fonction xmlToJson qui prend une chaîne de caractères XML en entrée et renvoie un objet XmlToJsonResult.

    clock_t begin = clock(), end; // Enregistrement du temps de début.
    double time_spent; // Variable pour stocker le temps écoulé.
    xmlDoc *doc = xmlReadMemory(xml, strlen(xml), "noname.xml", NULL, 0); // Lecture du XML à partir de la mémoire.
    XmlToJsonResult result; // Création d'un objet result de type XmlToJsonResult.

    if (doc == NULL) { // Vérification si le document XML a été correctement lu.
        printf("Failed to parse XML\n"); // Si la lecture a échoué, un message d'erreur est affiché.
        result.jsonStr = NULL; // Le champ jsonStr de l'objet result est mis à NULL.
        result.conversionTime = 0.0; // Le champ conversionTime de l'objet result est mis à 0.0.
        return result; // L'objet result est renvoyé tel quel.
    }

    xmlNode *root = xmlDocGetRootElement(doc); // Obtention du nœud racine du document XML.
    json_t *rootJson = xmlNodeToJson(root); // Conversion du nœud racine en JSON.
    result.jsonStr = json_dumps(rootJson, JSON_INDENT(4)); // Conversion du JSON en une chaîne de caractères avec une indentation de 4 espaces.

    xmlFreeDoc(doc); // Libération de la mémoire allouée pour le document XML.
    json_decref(rootJson); // Diminution du compteur de référence de l'objet JSON. L'objet est libéré s'il n'est plus référencé.

    end = clock(); // Enregistrement du temps de fin.
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC; // Calcul du temps écoulé.

    result.conversionTime = time_spent; // Stockage du temps écoulé dans le champ conversionTime de l'objet result.
    return result; // Renvoi de l'objet result avec la chaîne de caractères JSON et le temps de conversion.
}


//-------------------- Fonctions de création de documents --------------------

/**
 * Cette fonction permet de créer une collection dans la base de données 'actiaDataBase'
 */
void createCollection(mongoc_client_t *client,const char *db_name,const char* nomCollection){
    mongoc_database_t *database;
    mongoc_collection_t *collection;
    bson_error_t error; // Pour stocker l'information d'erreur.
    bson_t *doc;
    char **strv;
    int i;

    mongoc_init (); // Initialisation du driver MongoDB C.

    // Récupération de la base de données spécifiée
    database = mongoc_client_get_database(client, db_name);

    // Récupération de la liste des collections
    strv = mongoc_database_get_collection_names(database, &error);

    // Parcourir la liste pour vérifier si la collection existe
    for (i = 0; strv[i]; i++) {
        if (strcmp(strv[i], nomCollection) == 0) {
            printf("La collection %s existe deja !\n", nomCollection);
            return; // Si la collection existe, on arrête la fonction
        }
    }

    // Récupération de la collection spécifiée dans la base de données spécifiée.
    collection = mongoc_client_get_collection (client, db_name, nomCollection);

    doc = bson_new (); // Création d'un nouveau document BSON.
    BSON_APPEND_UTF8 (doc, "value", "value"); // Ajout de la valeur donnée dans le document BSON sous la clé "value".

    if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) { // Insertion du document dans la collection.
        fprintf (stderr, "%s\n", error.message); // Affichage de l'erreur.
    }

    bson_destroy (doc); // Libération de la mémoire allouée pour le document BSON.
    mongoc_collection_destroy (collection); // Libération de la mémoire allouée pour la collection MongoDB.
    mongoc_database_destroy(database); // Libération de la mémoire allouée pour la base de données MongoDB.
    mongoc_cleanup (); // Nettoyage du driver MongoDB C.
    printf("Collection %s creee avec succes !\n",nomCollection);
}


/**
 * Cette fonction permet d'insérer un document dans une collection
 */
void insert_document_into_collection (mongoc_client_t *client,const char *db_name, const char *collection_name,const char* value) {
    // Initialisation des pointeurs vers le client MongoDB, la collection MongoDB, le document BSON.
    mongoc_collection_t *collection;
    bson_error_t error; // Pour stocker l'information d'erreur.
    bson_t *doc;

    mongoc_init (); // Initialisation du driver MongoDB C.

    // Récupération de la collection spécifiée dans la base de données spécifiée.
    collection = mongoc_client_get_collection (client, db_name, collection_name);

    doc = bson_new (); // Création d'un nouveau document BSON.
    BSON_APPEND_UTF8 (doc, "value", value); // Ajout de la valeur donnée dans le document BSON sous la clé "value".

    // Insertion du document dans la collection. Si l'insertion échoue, le message d'erreur est affiché.
    if (!mongoc_collection_insert_one (collection, doc, NULL, NULL, &error)) {
        printf ("%s\n", error.message);
    }
    bson_destroy (doc); // Destruction du document BSON.
    mongoc_collection_destroy (collection); // Destruction de la collection.
    mongoc_cleanup (); // Nettoyage du driver MongoDB C.
    printf("Insertion reussie\n");
}


/**
 * Cette fonction permet d'insérer un JSON à partir d'un fichier XML dans une collection
 */
void insert_xml_into_collection(mongoc_client_t *client,const char *db_name, const char *collection_name,const char* pathXML) {
    bson_error_t error; // Pour stocker l'information d'erreur.
    bson_t *document; // Pour stocker le document BSON.

    mongoc_collection_t *collection = mongoc_client_get_collection (client, db_name, collection_name);

    // Ouverture du fichier XML spécifié.
    FILE *file = fopen(pathXML, "r");
    if (file == NULL) { // Si l'ouverture du fichier échoue, un message d'erreur est affiché.
        printf("Failed to open the file\n");
        return;
    }

    // Détermination de la taille du fichier.
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Lecture du contenu du fichier.
    char *xml = malloc(fsize + 1); // Alloue fsize + 1 octets.
    fread(xml, 1, fsize, file);
    xml[fsize] = '\0'; // Assure la terminaison par NULL.

    // Trouve la fin du document XML et termine la chaîne là.
    const char *endTag = "</tt:MetadataStream>";
    char *endOfXml = strstr(xml, endTag);
    if (endOfXml != NULL) {
        endOfXml[strlen(endTag)] = '\0'; // Termine la chaîne après la balise de fin.
    }

    // Convertit le XML en JSON.
    XmlToJsonResult result = xmlToJson(xml);
    char *json = result.jsonStr;
    double conversionTime = result.conversionTime;

    // Insère le JSON dans MongoDB.
    document = bson_new_from_json ((const uint8_t *)json, -1, &error);
    if (!mongoc_collection_insert_one (collection, document, NULL, NULL, &error)) { // Si l'insertion échoue, le message d'erreur est affiché.
        printf ("Insertion echoue: %s\n", error.message);
        return;
    }
    else{
        printf("Insertion reussie\n"); // Si l'insertion réussit, un message de succès est affiché.
    }

    bson_destroy (document); // Destruction du document BSON.
    mongoc_collection_destroy (collection); // Destruction de la collection.
    printf("Temps de conversion : %.2f seconds.\n", conversionTime); // Affichage du temps de conversion.
}


/**
 * Cette fonction permet d'insérer tous les fichiers XML d'un répertoire dans une collection MongoDB
 * Les fichiers XML sont traités un par un et convertis en JSON avant d'être insérés dans la collection "testCollection"
 * @param client
 * @param dirPath
 * Cette fonction ne charge pas tous les fichiers XML en mémoire, elle les traite un par un
 */
void process_files_one_by_one(mongoc_client_t *client,const char *db_name,const char *collection_name,const char* dirPath) {
    clock_t begin = clock(), end; // Initialisation des variables pour mesurer le temps d'exécution total.
    double time_spent;
    double tempsConversion = 0.0; // Variable pour mesurer le temps total de conversion.

    DIR *d;
    struct dirent *dir;
    struct stat st;
    char path[1024];
    bson_error_t error;


    mongoc_collection_t *collection = mongoc_client_get_collection (client, db_name, collection_name); // Récupération de la collection MongoDB.
    printf("Traitement en cours...\n");
    d = opendir(dirPath); // Ouverture du répertoire spécifié.
    if (d) {
        while ((dir = readdir(d)) != NULL) { // Parcours des fichiers dans le répertoire.
            snprintf(path, sizeof(path), "%s/%s", dirPath, dir->d_name);
            stat(path, &st);
            if (S_ISREG(st.st_mode) && strstr(dir->d_name, ".xml") != NULL) { // Si le fichier est un fichier XML ordinaire.

                // Ouverture du fichier et lecture de son contenu.
                FILE *file = fopen(path, "r");
                if (file == NULL) {
                    printf("Failed to open the file\n");
                    return;
                }
                // Obtention de la taille du fichier.
                fseek(file, 0, SEEK_END);
                long fsize = ftell(file);
                fseek(file, 0, SEEK_SET);
                // Lecture du contenu du fichier.
                char *xml = malloc(fsize + 1);
                fread(xml, 1, fsize, file);
                xml[fsize] = '\0'; // Assure une terminaison par null.
                fclose(file);

                // Recherche de la fin du document XML et termine la chaîne à cet endroit.
                const char *endTag = "</tt:MetadataStream>";
                char *endOfXml = strstr(xml, endTag);
                if (endOfXml != NULL) {
                    endOfXml[strlen(endTag)] = '\0'; // Termine la chaîne après la balise de fin.
                }

                // Conversion du XML en JSON.
                XmlToJsonResult result = xmlToJson(xml);
                char *json = result.jsonStr;
                double jsonConversion = result.conversionTime;
                tempsConversion += jsonConversion; // Ajout du temps de conversion à la variable tempsConversion.

                // Insertion du JSON dans MongoDB.
                bson_t* document = bson_new_from_json ((const uint8_t *)json, -1, &error);

                if (!mongoc_collection_insert_one (collection, document, NULL, NULL, &error)) { // Si l'insertion échoue, imprime un message d'erreur.
                    printf ("Insertion echoue: %s\n", error.message);
                    return;
                }
            }
        }
        closedir(d); // Fermeture du répertoire.
    }

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC; // Calcul du temps d'exécution total.
    printf("Temps d'execution: %f secondes\n", time_spent); // Impression du temps d'exécution.
    printf("Temps de conversion: %f secondes\n", tempsConversion); // Impression du temps total de conversion.
}


/**
 * Cette fonction permet d'insérer tous les fichiers XML d'un répertoire dans une collection MongoDB
 * Les fichiers XML sont d'abord chargés en mémoire avant d'être convertis en JSON et insérés dans la collection "testCollection"
 * @param client
 * @param dirPath
 * Cette fonction charge tous les fichiers XML en mémoire avant de les traiter
 */
typedef struct Node {
    char* xmlContent;
    struct Node* next;
} Node;

void process_all_files_at_once(mongoc_client_t *client,const char* db_name,const char* collection_name,const char* dirPath) {
    bson_error_t error;
    bson_t *document;


    mongoc_collection_t *collection = mongoc_client_get_collection (client, db_name, collection_name); // Récupération de la collection dans MongoDB
    clock_t beginInsertion = clock(), end; // Pour mesurer le temps d'insertion
    clock_t beginMemory = clock(), endmemory; // Pour mesurer le temps de chargement en mémoire
    double tempsMemory = 0.0; // Initialisation du temps de chargement en mémoire
    double tempsConversion = 0.0; // Initialisation du temps de conversion
    double tempsInsertion = 0.0; // Initialisation du temps d'insertion
    DIR *d;
    struct dirent *dir;
    struct stat st;
    char path[1024];

    // Initialise une liste pour contenir le contenu de tous les fichiers XML et un tableau pour contenir les documents BSON.
    Node* xmlFiles = NULL; // Ceci est notre tête de liste
    bson_t **documents = NULL;
    int numDocs = 0;

    d = opendir(dirPath); // Ouvre le répertoire
    if (d) {
        while ((dir = readdir(d)) != NULL) { // Lit chaque fichier du répertoire
            snprintf(path, sizeof(path), "%s/%s", dirPath, dir->d_name);
            stat(path, &st);
            if (S_ISREG(st.st_mode) && strstr(dir->d_name, ".xml") != NULL) { // Si le fichier est un fichier XML régulier

                // Ouvre le fichier et lit son contenu
                FILE *file = fopen(path, "r");
                if (file == NULL) {
                    printf("Failed to open the file\n");
                    return;
                }

                // Obtient la taille du fichier
                fseek(file, 0, SEEK_END);
                long fsize = ftell(file);
                fseek(file, 0, SEEK_SET);

                // Lit le contenu du fichier
                char *xml = malloc(fsize + 1);
                fread(xml, 1, fsize, file);
                xml[fsize] = '\0'; // Assure une terminaison par null
                fclose(file);

                // Trouve la fin du document XML et termine la chaîne là
                const char *endTag = "</tt:MetadataStream>";
                char *endOfXml = strstr(xml, endTag);
                if (endOfXml != NULL) {
                    endOfXml[strlen(endTag)] = '\0'; // Termine la chaîne après la balise de fin
                }

                // Stocke le contenu du fichier dans la liste
                Node* newNode = malloc(sizeof(Node));
                newNode->xmlContent = xml;
                newNode->next = xmlFiles;
                xmlFiles = newNode;

                // Convertit le XML en BSON et le stocke dans le tableau
                XmlToJsonResult result = xmlToJson(xml);
                char *json = result.jsonStr;
                double jsonConversion = result.conversionTime;
                tempsConversion += jsonConversion; // Ajoute le temps de conversion à la variable tempsConversion
                bson_t* document = bson_new_from_json ((const uint8_t *)json, -1, &error);
                documents = realloc(documents, (numDocs + 1) * sizeof(bson_t *));
                documents[numDocs] = document;
                numDocs++;
            }
        }
        closedir(d);
    }
    endmemory = clock();
    tempsMemory = (double)(endmemory - beginMemory) / CLOCKS_PER_SEC;

    // Maintenant, tous les fichiers XML sont convertis en BSON. Insérons-les tous en une fois.
    if (!mongoc_collection_insert_many(collection, (const bson_t * const *)documents, numDocs, NULL, NULL, &error)) {
        printf ("Insertion failed: %s\n", error.message); // Si l'insertion échoue, imprime un message d'erreur.
        return;
    }
    end = clock();
    tempsInsertion = (double)(end - beginInsertion) / CLOCKS_PER_SEC;
    printf("Temps d'insertion : %f en secondes\n", tempsInsertion); // Imprime le temps d'insertion.
    printf("Temps de conversion: %f en secondes\n", tempsConversion); // Imprime le temps de conversion.
    printf("Temps de mise en mémoire : %f en secondes\n", tempsMemory); // Imprime le temps de chargement en mémoire.

    //Libère le tableau de documents et son contenu.
    for (int i = 0; i < numDocs; i++) {
        bson_destroy(documents[i]);
    }
    free(documents);
}












