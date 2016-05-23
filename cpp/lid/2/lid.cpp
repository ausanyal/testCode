#include <iostream>
#include <list>
#include "lid.h"

using namespace std;

class RidDb {
private:
	// TODO: Aubin. Use slab here
	lid_info_t *eff_db[NUM_LID_INFO_ENTRIES];
	pendingDbList pending_db;
	int num_effective;

public:
	RidDb();
	~RidDb();
	int getLidFromRid(int rid, db_type_t db);
	void addRidToLidEntry(int rid, int lid, db_type_t db);
	void *getNewLid();
	void *getNewRid();
	void deleteLid();
	void deleteRid();
};

RidDb::RidDb()
{
	num_effective = 0;
	for (int i = 0; i < NUM_LID_INFO_ENTRIES; ++i) {
		eff_db[i] = NULL;
	}
}

RidDb::~RidDb()
{
	for (int i = 0; i < NUM_LID_INFO_ENTRIES; ++i) {
		if (eff_db[i] != NULL) {
			free(eff_db[i]);
			eff_db[i] = NULL;
		}
	}
}

int 
RidDb::getLidFromRid(int rid, db_type_t dbT)
{
	if (dbT == DBTYPE_EFFECTIVE) {
		if (rid >= NUM_LID_INFO_ENTRIES) {
			return -1;
		}

		return eff_db[rid]->lid;

	} else if (dbT == DBTYPE_PENDING) {
		// Lookup list for rid
		for (Iter it = pending_db.begin(); it != pending_db.end(); ++it) {
			if (it->rid == rid) {
				return it->lid;
			}
		}
	} else {
		return -1;
	}

	return -1;
}

void 
RidDb::addRidToLidEntry(int rid, int lid, db_type_t dbt)
{
	return;
}

void *
RidDb::getNewLid()
{
	if (num_effective > NUM_LID_INFO_ENTRIES) {
		LID_ERRLOG("Exceeded max number of entries" << num_effective);
		return NULL;
	}
	return NULL;
}

void *
RidDb::getNewRid()
{
	return NULL;
}

void 
RidDb::deleteLid()
{
	return;
}

void 
RidDb::deleteRid()
{
	return;
}

int main()
{

	return 0;
}