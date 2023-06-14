#include <windows.h>
#include <stdio.h>
#include <bson.h>
#include <mongoc.h>
#include <bson/bson.h>
#include <mongoc/mongoc.h>
/**
* Cette fonction permet modifier un document dans une collection grâce à son id
*/
void update_document_from_collection (const char *db_name, const char *collection_name, const char* id, const char* value) {
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *query;
    bson_t *update;

    mongoc_init ();

    client = mongoc_client_new ("mongodb://root:examplepassword@localhost:27017");
    collection = mongoc_client_get_collection (client, db_name, collection_name);

    query = bson_new ();
    bson_oid_init_from_string(&oid, id);
    BSON_APPEND_OID (query, "_id", &oid);

    update = BCON_NEW ("$set", "{", "value", BCON_UTF8 (value), "}");

    if (!mongoc_collection_update_one (collection, query, update, NULL, NULL, &error)) {
        printf ("Update failed: %s\n", error.message);
    }

    bson_destroy (update);
    bson_destroy (query);
    mongoc_collection_destroy (collection);
    mongoc_client_destroy (client);
    mongoc_cleanup ();
}

/**
* Cette fonction permet de modifier le type de tous les documents d'une collection
 * @param client
 * @param newValueType
*/
void update_all_document_type(mongoc_client_t *client, const char* newValueType) {
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_t *update = NULL;
    bson_t *update_reply;
    bson_t *query = NULL;

    clock_t begin = 0, end;
    double time_spent;
    collection = mongoc_client_get_collection(client, "actiaDataBase", "testCollection");
    if (!collection) {
        printf("Could not get collection\n");
        return;
    }

    // Construire la requête.
    query = bson_new(); // Un document vide pour sélectionner tous les documents

    // Construire la mise à jour. Nous définissons "VideoAnalytics.Frame.Object.Appearance.Class.Type.text" à `newValueType`.
    update = BCON_NEW("$set", "{", "VideoAnalytics.Frame.Object.Appearance.Class.Type.text", BCON_UTF8(newValueType), "}");

    // Exécuter la mise à jour sur tous les documents de la collection (grâce à l'option MONGOC_UPDATE_MULTI).
    bool retval = mongoc_collection_update_many(
            collection,
            query,
            update,
            NULL,  // options
            NULL,  // reply
            &error
    );
    if (!retval) {
        printf("Update failed: %s\n", error.message);
        return;
    }

    // Libérer les ressources.
    bson_destroy(update);
    bson_destroy(query);
    mongoc_collection_destroy(collection);

    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Temps de mise à jour : %.2f seconds.\n", time_spent);

}
