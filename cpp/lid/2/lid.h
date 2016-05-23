
#include <iostream>
#include <map>
#include "l3rtg_include.h"
#include "slab/Slab.h"
#include "indexer.h"

#define NUM_LID_INFO_ENTRIES (64000)

enum {
    DETAIL_LOG,
    EVENT_LOG,
    TRACE_LOG,
    ERR_LOG,
    MAX_LOG
};

#define DEBUG_LEVEL     EVENT_LOG

#define LID_LOG(lvl, msg) {\
    if (lvl >= DEBUG_LEVEL) {\
        std::cout << __func__ << "(" << __LINE__ << "): " << msg << std::endl;\
    }\
}

#define LID_DLOG(msg) \
    LID_LOG(DETAIL_LOG, msg)

#define LID_EVLOG(msg) \
     LID_LOG(EVENT_LOG, msg)

#define LID_TRLOG(msg) \
    LID_LOG(TRACE_LOG, msg)

#define LID_ERRLOG(msg) \
    LID_LOG(ERR_LOG, msg)

typedef enum db_type_ {
	DBTYPE_EFFECTIVE,
	DBTYPE_PENDING,
	DBTYPE_MAX
} db_type_t;

typedef std::map<int, class LogicalId*> ridToLidMap;
typedef std::map<int, class LogicalId*>::iterator Iter;

