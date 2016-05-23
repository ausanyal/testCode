
#define NUM_LID_INFO_ENTRIES (128*1024)

enum {
    DETAIL_LOG,
    EVENT_LOG,
    TRACE_LOG,
    ERR_LOG,
    MAX_LOG
};

#define DEBUG_LEVEL     EVENT_LOG
//#define DEBUG_LEVEL     DETAIL_LOG

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

typedef struct lid_info_ {
	int lid;
} lid_info_t;

typedef struct pending_db_list_ {
	int rid;
	int lid;
}pending_db_list_t;

typedef enum db_type_ {
	DBTYPE_EFFECTIVE,
	DBTYPE_PENDING,
	DBTYPE_MAX
} db_type_t;

typedef std::list<pending_db_list_t> pendingDbList;
typedef std::list<pending_db_list_t>::iterator Iter;