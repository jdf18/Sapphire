#include "database.h"


void parse_db_tables(Database* database, Page* root_page) {
    uint32_t num_tables = database->num_tables;
    Table** tables = database->tables;

    // Set the pointer to the current table information being parsed
    void* table_pointer = ((void*)root_page + DB_TABLES_OFFSET);

    // Create some pointers which can be used to retrieve information from the page.
    uint32_t* length;
    uint32_t* table_root_node;
    uint32_t* num_columns;
    uint8_t* type;
    uint8_t* constraints;
    Table* temp_table;
    TableSchema* temp_table_schema;

    // Iterate through each of the tables being defined
    for (uint32_t i = 0; i < num_tables; i++) {
        // Read the length of the table definition and number of columns in it
        length = (void*)(table_pointer + DB_TABLES_LENGTH_OFFSET);
        table_root_node = (void*)(table_pointer + DB_TABLES_ROOT_PAGE_OFFSET);
        num_columns = (void*)(table_pointer + DB_TABLES_NUM_COLUMNS_OFFSET);

        // Allocate memory for the table and its schema
        temp_table = malloc(sizeof(Table));
        if (temp_table == NULL) {
            printf("temp table null\n");
            exit(EXIT_FAILURE);
        }
        temp_table_schema = (TableSchema*)malloc(sizeof(TableSchema) + ((long long)(*num_columns) * sizeof(TableSchemaColumn)));
        if (temp_table_schema == NULL) {
            printf("temp table schema null\n");
            exit(EXIT_FAILURE);
        }

        // Copy the table identifier into the new table struct
        memcpy(&(temp_table->identifier), table_pointer + DB_TABLES_IDENTIFIER_OFFSET, 8);

        // Iterate through the definitions for the table columns
        for (uint32_t j = 0; j < *num_columns; j++) {
            // Get the values required from each of the columns and store them in the schema
            type = (void*)(table_pointer + DB_COLUMNS_OFFSET + DB_COLUMN_TYPE_OFFSET + (j * DB_COLUMN_SIZE));
            constraints = (void*)(table_pointer + DB_COLUMNS_OFFSET + DB_COLUMN_CONSTRAINT_OFFSET + (j * DB_COLUMN_SIZE));

            temp_table_schema->columns[j].type = (int)(*type);
            temp_table_schema->columns[j].constraints = *constraints;

            // Copy the column identifier into the column object
            memcpy(&(temp_table_schema->columns[j].identifier), (void*)(table_pointer + DB_COLUMNS_OFFSET + DB_COLUMN_IDENTIFIER_OFFSET + (j * DB_COLUMN_SIZE)), 8);
        }

        // Store the table and its schema into the database object
        temp_table_schema->num_columns = *num_columns;
        temp_table->schema = temp_table_schema;

        update_row_sizes_from_schema(temp_table);

        temp_table->root_page_num = *table_root_node;
        database->tables[i] = temp_table;



        // Advance the table pointer to the next table using the length found earlier
        table_pointer = (void*)((void*)table_pointer + (*length));
    }

    // All tables have been found, no information left in this page.
};

void parse_db_file(Database* database, Page* root_page) {
    // The file should start with the 4 character sequence "DBJF", defined in MAGIC_STRING
    char* magic = malloc(5);
    memcpy(magic, root_page + DB_HEADER_MAGIC_OFFSET, 4);
    magic[4] = '\0';
    int res = strcmp(magic, DB_HEADER_MAGIC_STRING);
    if (res != 0) {
        printf("Magic string at start of db file is incorrect. Can not parse.\n");
        exit(EXIT_FAILURE);
    }

    // Retrieve version, page size, and number of tables from the header
    uint32_t* version = (void*)((void*)root_page + DB_HEADER_VERSION_OFFSET);
    database->version = *version;

    uint32_t* page_size = (void*)((void*)root_page + DB_HEADER_PAGE_SIZE_OFFSET);
    database->page_size = *page_size;

    uint32_t* num_tables = (void*)((void*)root_page + DB_HEADER_NUM_TABLES_OFFSET);
    database->num_tables = *num_tables;
    // TODO: Should test if the version is different to that of the software and either suggest
    // TODO:  a software upgrade or update the file to the newer format

    // Allocate memory for the pointers to each of the tables
    Table** tables = malloc((*num_tables) * sizeof(Table*));
    database->tables = tables;

    parse_db_tables(database, root_page);
}


Database* open_db(const char* filepath) {
    Database* database = malloc(sizeof(Database));

    // Create the pager to start reading the .db file header
    Pager* pager = pager_open(filepath);
    Page* root_page = get_page(pager,0);
    database->pager = pager;

    parse_db_file(database, root_page);

    printf("Database loaded successfully.\n");

    return database;
}


void close_db(Database* database) {
    // Free all the database tables
    Table** tables = database->tables;
    uint32_t num_tables = database->num_tables;

    for (uint32_t i = 0; i < num_tables; i++) {
        free_table(tables[i]);
        tables[i] = NULL;
    }

    // Free and flush all the pages stored by the pager
    Pager* pager = database->pager;

    for (uint32_t i = 0; i < pager->num_pages; i++) {
        if (pager->pages[i] == NULL) {
            continue;
        }
        pager_flush(pager, i);
        free(pager->pages[i]);
        pager->pages[i] = NULL;
    }

    close_file(pager->file);

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        Page* page = pager->pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }

    free(pager);

    // Free the database object from memory
    free(database);
}