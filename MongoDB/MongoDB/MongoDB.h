#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <mongoc/mongoc.h>

class MongoDB
{
public:
	MongoDB();
	~MongoDB();
	int createConnection(const char* uri_string = "mongodb://localhost:27017");
	int createDB(const char* dbName, const char* dbCollection);
	bool insertDoc(const char* colmn, const char* value);
	std::vector<std::string> findAllDoc();
	std::vector<std::string> findDoc(const char* colmn, const char* value);
	bool deleteDoc(const char* colmn, const char* value);

private:
	mongoc_uri_t* uri;							//Address and port number at which mongoDB is running
	mongoc_client_t* client;					//Stores information about the connection with the mongoDB
	mongoc_database_t* database;				//Handle to database
	mongoc_collection_t* collection;			//Handle to collection
	mongoc_cursor_t* cursor;
	bson_t* command, reply, * insert, * query;	//Command - pinging db, reply - recieving response from mongoDB, 
												//insert - inserting document in mongoDB
	bson_error_t error;							//For display error message
	bson_oid_t oid;
	char* str;
	bool retval;
};

