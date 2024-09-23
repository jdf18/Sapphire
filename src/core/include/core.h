#ifndef SAPPHIRE_CORE_H
#define SAPPHIRE_CORE_H

typedef struct {

} Database;

Database* create_db(const char* filepath);
Database* open_db(const char* filepath);
void close_db(Database* database);

#endif //SAPPHIRE_CORE_H
