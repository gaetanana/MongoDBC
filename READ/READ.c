#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <time.h>

/**
 * Cette fonction permet de lire tous les documents d'une collection
 * @param client
 * @param database_name
 * @param collection_name
 */
void read_all_documents(mongoc_client_t *client,const char *db_bame, const char *collection_name) {
    bson_error_t error;
    bson_t *query;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    const bson_t *doc;

    clock_t begin = clock(), end;
    double time_spent;

    // Récupérer la collection
    collection = mongoc_client_get_collection(client, db_bame,collection_name);

    // Créer une requête vide pour sélectionner tous les documents
    query = bson_new();

    // Exécuter la requête
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    // Parcourir et afficher tous les documents
    while (mongoc_cursor_next(cursor, &doc)) {
        char *str;

        str = bson_as_canonical_extended_json(doc, NULL);
        //printf("%s\n", str);
        bson_free(str);
    }

    if (mongoc_cursor_error(cursor, &error)) {
        fprintf(stderr, "Cursor Failure: %s\n", error.message);
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Temps de lecture : %.2f seconds.\n", time_spent);

    // Nettoyer
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
}

/**
 * Cette fonction permet de lire tous les documents d'une collection filtrés
 * @param client
 * Le filtre est le type "Human"
 */
void read_filtered_documentsHuman(mongoc_client_t *client,const char *db_name, const char *collection_name) {

    int compteur = 0;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query;
    char *str;

    clock_t begin = clock(), end;
    double time_spent;

    collection = mongoc_client_get_collection(client, db_name, collection_name);
    if (!collection) {
        printf("Impossible de récupérer la collection\n");
        return;
    }

    // Construire la requête. Nous filtrons sur "VideoAnalytics.Frame.Object.Appearance.Class.Type.text" == "Human".
    query = BCON_NEW("VideoAnalytics.Frame.Object.Appearance.Class.Type.text", BCON_UTF8("Human"));

    // Execute la requête.
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    // Itérer sur les résultats.
    while (mongoc_cursor_next(cursor, &doc)) {
        compteur++;
        str = bson_as_canonical_extended_json(doc, NULL);
        //printf("%s\n", str);
        bson_free(str);
    }

    // Libérer les ressources.
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Temps de lecture : %.2f seconds.\n", time_spent);
    printf("Nombre de documents : %d\n", compteur);
}

