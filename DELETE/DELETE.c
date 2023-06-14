#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>
#include <time.h>

void delete_all_documents_from_collection(mongoc_client_t *client,const char *db_name, const char *collection_name) {
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_t *command, reply;
    bson_t *query;
    clock_t begin = 0, end;
    double time_spent;
    mongoc_init();
    if (!client) {
        fprintf(stderr, "Failed to parse URI.\n");
        return;
    }

    collection = mongoc_client_get_collection(client, db_name, collection_name);

    query = bson_new();

    //Supprimer tous les documents
    if (!mongoc_collection_delete_many(collection, query, NULL, &reply, &error)) {
        fprintf(stderr, "Delete failed: %s\n", error.message);
        return;
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Temps de suppression : %.2f seconds.\n", time_spent);
    char *str = bson_as_canonical_extended_json(&reply, NULL);
    printf("%s\n", str);
    bson_free(str);
    bson_destroy(query);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);
    mongoc_cleanup();
}