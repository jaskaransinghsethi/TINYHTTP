#include "stdafx.h"
#include "MongoDB.h"


MongoDB::MongoDB()
{

	std::cout << "\n Initialising MongoDB Environment:";
	mongoc_init();
	std::cout << "\n Successfully initialised MongoDB\n";
}


MongoDB::~MongoDB()
{
	std::cout << "\n Cleaning up MongoDB Environment";
	mongoc_collection_destroy(collection);
	mongoc_database_destroy(database);
	mongoc_uri_destroy(uri);
	mongoc_client_destroy(client);
	mongoc_cursor_destroy(cursor);
	mongoc_cleanup();
	std::cout << "\n Successfully destroyed MongoDB handlers\n";
}

int MongoDB::createConnection(const char* uri_string)
{
	uri = mongoc_uri_new_with_error(uri_string, &error);
	if (!uri) {
		fprintf(stderr,
			"failed to parse URI: %s\n"
			"error message:       %s\n",
			uri_string,
			error.message);
		return EXIT_FAILURE;
	}

	client = mongoc_client_new_from_uri(uri);
	if (!client) {
		return EXIT_FAILURE;
	}

	return 0;
}

int MongoDB::createDB(const char* dbName, const char* dbCollection)
{
	database = mongoc_client_get_database(client, dbName);
	collection = mongoc_client_get_collection(client, dbName, dbCollection);

	command = BCON_NEW("ping", BCON_INT32(1));

	retval = mongoc_client_command_simple(
		client, "admin", command, NULL, &reply, &error);

	if (!retval) {
		fprintf(stderr, "%s\n", error.message);
		return EXIT_FAILURE;
	}

	return 0;
}

bool MongoDB::insertDoc(const char* colmn, const char* value)
{
	insert = bson_new();
	bson_oid_init(&oid, NULL);
	BSON_APPEND_OID(insert, "_id", &oid);
	BSON_APPEND_UTF8(insert, colmn, value);

	if (!mongoc_collection_insert_one(collection, insert, NULL, NULL, &error)) {
		fprintf(stderr, "%s\n", error.message);
		bson_destroy(insert);
		return false;
	}

	str = bson_as_json(&reply, NULL);
	std::cout << "\n" << str;
	bson_destroy(insert);
	bson_free(str);
	return true;
}

std::vector<std::string> MongoDB::findAllDoc()
{
	query = bson_new();
	mongoc_cursor_t* cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
	const bson_t* doc;
	std::vector<std::string> result;
	while (mongoc_cursor_next(cursor, &doc)) {
		str = bson_as_canonical_extended_json(doc, NULL);
		result.push_back(str);
		printf("%s\n", str);
		bson_free(str);
	}

	bson_destroy(query);
	return result;
}

std::vector<std::string> MongoDB::findDoc(const char* colmn, const char* value)
{
	query = bson_new();
	BSON_APPEND_UTF8(query, colmn, value);
	const bson_t* doc;
	cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
	std::vector<std::string> result;
	while (mongoc_cursor_next(cursor, &doc)) {
		str = bson_as_canonical_extended_json(doc, NULL);
		result.push_back(str);
		printf("%s\n", str);
		bson_free(str);
	}

	bson_destroy(query);

	return result;
}

bool MongoDB::deleteDoc(const char* colmn, const char* value)
{
	bson_t* doc = bson_new();
	BSON_APPEND_UTF8(doc, colmn, value);

	if (!mongoc_collection_delete_many(
		collection, doc, NULL, NULL, &error)) {
		fprintf(stderr, "Delete failed: %s\n", error.message);
		return false;
	}

	bson_destroy(doc);

	return true;
}

#ifdef TEST_MONGODB
int main() {

	const char* uri_string = "mongodb://localhost:27017";

	MongoDB instance;

	std::cout << "\n Connecting to mongoDB at:" << uri_string;
	if (!instance.createConnection(uri_string))
		std::cout << "\n Successfully connected to mongoDB";

	std::cout << "\n Creating database:";
	if (!instance.createDB("mongoDBTest", "mongoCollectionTest"))
		std::cout << "\n Successfully created database";

	std::cout << "\n Testing insertion:";
	if (instance.insertDoc("colmn1", "colmnvalue1"))
		std::cout << "\n Successfully inserted into the database";

	instance.insertDoc("colmn2", "colmnvalue2");
	instance.insertDoc("colmn3", "colmnvalue3");
	instance.insertDoc("colmn4", "colmnvalue4");



	std::cout << "\n Testing findDoc:";
	instance.findDoc("colmn1", "colmnvalue1");

	std::cout << "\n Testing findAllDoc";
	instance.findAllDoc();

	std::cout << "\n Testing deleteDoc";
	instance.deleteDoc("colmn1", "colmnvalue1");
	instance.findAllDoc();

	return 0;
}
#endif