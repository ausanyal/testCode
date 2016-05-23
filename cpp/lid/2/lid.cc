#include "lid.h"

using namespace std;

class RidDb {
private:
	// TODO: Aubin. Use slab create here here
    ridToLidMap     effectiveDb_;
	ridToLidMap     pendingDb_;
	int             num_effective_;
    int             num_pending_;
    Indexer         *ridDbIndexer_; 
    Slab            *ridDbSlab_;
    // TODO:
    Slab            *ridToRtInfo;

public:
	RidDb();
	~RidDb();
	LogicalId   *getLidFromRid(int rid, db_type_t db);
	void        addRidToLidEntry(int rid, LogicalId *lid, db_type_t db);
	void        deleteRidToLidEntry(int rid, db_type_t db);
	LogicalId   *getNewLid();
	syserr_t    getNewRid(uint32_t &rid);
    syserr_t    freeRid(uint32_t &rid);
    bool        freeLid(LogicalId *lid);
    void        debugPrintMap(db_type_t dbT);
};

RidDb::RidDb()
{
	this->num_effective_    = 0;
    this->num_pending_      = 0;
    this->ridDbIndexer_     = halSDKNew(L3RTG_MEM_TYPE_RID_DB_INDXR) Indexer(NUM_LID_INFO_ENTRIES);
    this->ridDbSlab_        = SlabFactory::slabCreate(sizeof(LogicalId), NUM_LID_INFO_ENTRIES, "RidDb");
    assert(this->ridDbSlab_->verify());
}

RidDb::~RidDb()
{
    if (this->ridDbSlab_) SlabFactory::slabDestroy(this->ridDbSlab_);
}

LogicalId *find_in_map(int rid, ridToLidMap &map)
{
    Iter it;

    it = map.find(rid);
    if (it != map.end()) {
        return (it->second);
    }

    return NULL;

}

LogicalId * 
RidDb::getLidFromRid(int rid, db_type_t dbT)
{
	if (dbT == DBTYPE_EFFECTIVE) {
        return (find_in_map(rid, this->effectiveDb_));
    } else if (dbT == DBTYPE_PENDING) {
        return (find_in_map(rid, this->pendingDb_));
    } 

	return NULL;
}

void add_to_map(int rid, LogicalId *lid, ridToLidMap &map)
{
    map.insert(std::pair<int, LogicalId *>(rid, lid));
}

void 
RidDb::addRidToLidEntry(int rid, LogicalId *lid, db_type_t dbT)
{
	if (dbT == DBTYPE_EFFECTIVE) {
        if (num_effective_ > NUM_LID_INFO_ENTRIES) {
            LID_ERRLOG("num_effective_ exceeded limit: " << NUM_LID_INFO_ENTRIES);
            return;
        }
        add_to_map(rid, lid, this->effectiveDb_);
        num_effective_++;
	} else if (dbT == DBTYPE_PENDING) {
        if (num_pending_ > NUM_LID_INFO_ENTRIES) {
            LID_ERRLOG("num_pending_ exceeded limit: " << NUM_LID_INFO_ENTRIES);
            return;
        }
        add_to_map(rid, lid, this->pendingDb_);
        num_pending_++;
	} 

	return;
}

#if 0
void *
getNewRtInfo()

addRidtoRtInfoMap(rid, void *)

deleteRidToRtInfo(rid)

freeRtInfo(void *)
#endif


LogicalId *
RidDb::getNewLid()
{
    void        *blockRaw;
    LogicalId   *lid;

    blockRaw    = this->ridDbSlab_->alloc();
    lid         = (LogicalId *)blockRaw;

    return (new(lid) LogicalId());
}

syserr_t
RidDb::getNewRid(uint32_t &rid)
{
    syserr_t status = SYSERR_SUCCESS;

    status = this->ridDbIndexer_->alloc(rid);

    L3RTG_CMP_RET(status);
    return SYSERR_SUCCESS;
}

void del_from_map(int rid, ridToLidMap &map)
{
#if 1
    // TODO: Do we want to find and delete or try to delete blindly
    Iter it = map.find(rid);
    if (it != map.end()) {
        map.erase(it);
    } else {
        LID_ERRLOG("Entry not found" << NUM_LID_INFO_ENTRIES);
    }
#else
    // Best effort. Doesnt report or return err
    map.erase(rid);
#endif
}

void 
RidDb::deleteRidToLidEntry(int rid, db_type_t dbT)
{
	if (dbT == DBTYPE_EFFECTIVE) {
        if (num_effective_ <= 0) {
            LID_ERRLOG("num_effective_ already empty");
            return;
        }
        del_from_map(rid, this->effectiveDb_);
        num_effective_--;
	} else if (dbT == DBTYPE_PENDING) {
        if (num_pending_ <= 0) {
            LID_ERRLOG("num_pending_ already empty");
            return;
        }
        del_from_map(rid, this->pendingDb_);
        num_pending_--;
    }

	return;
}

syserr_t
RidDb::freeRid(uint32_t &rid)
{
    syserr_t status = SYSERR_SUCCESS;

    status = this->ridDbIndexer_->free(rid);

    L3RTG_CMP_RET(status);
    return SYSERR_SUCCESS;

}
    
bool
RidDb::freeLid(LogicalId *lid)
{
    void *blockRaw;

    lid->~LogicalId();
    blockRaw = (LogicalId *)lid;

    return (this->ridDbSlab_->free(blockRaw));

}

void 
printMap(ridToLidMap &map)
{
    Iter it;

    LID_EVLOG("***************");
    for (it = map.begin(); it != map.end(); ++it) {
        LID_EVLOG("rid: " << it->first << " lid: " << it->second);
    }
    LID_EVLOG("************************");
}

void
RidDb::debugPrintMap(db_type_t dbT)
{
    if (dbT == DBTYPE_EFFECTIVE) {
        LID_EVLOG("DBTYPE_EFFECTIVE: ");
        printMap(this->effectiveDb_);
    } else if (dbT == DBTYPE_PENDING) {
        LID_EVLOG("DBTYPE_PENDING: ");
        printMap(this->pendingDb_);
    }
}

#if 0
int main()
{
    RidDb r;
    int rid = r.getNewRid();

    LogicalId *lid = r.getNewLid();
    LID_EVLOG("rid: " << rid << " lid: " << lid);
    r.addRidToLidEntry(rid, lid, DBTYPE_EFFECTIVE);
    r.debugPrintMap(DBTYPE_EFFECTIVE);
    r.debugPrintMap(DBTYPE_PENDING);

    LogicalId *lid2 = r.getNewLid();
    LID_EVLOG("rid: " << rid << " lid2: " << lid2);
    r.addRidToLidEntry(rid, lid2, DBTYPE_PENDING);

    r.deleteRidToLidEntry(rid, DBTYPE_EFFECTIVE);
    r.addRidToLidEntry(rid, lid2, DBTYPE_EFFECTIVE);
    r.deleteRidToLidEntry(rid, DBTYPE_PENDING);

	return 0;
}
#endif
