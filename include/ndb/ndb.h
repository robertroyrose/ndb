
#include <stdint.h>

enum ndb_key_type {
	NDB_KEY_INDEX,
	NDB_KEY_HASH,
	NDB_KEY_END = 0x7fffffff
};

struct ndb_db;
struct ndb_path;

struct ndb_keys {
	enum ndb_key_type key_type;
	union {
		intptr_t key_idx;
		const char *key_name;
	};
};

struct ndb_hstring {
	uint32_t hst_hash;
	uint16_t hst_hstr_idx;
	uint16_t hst_next;
};

struct ndb_member {
        uint16_t mbr_offset;
        uint16_t mbr_type;
        uint16_t mbr_array_len;
        uint16_t mbr_path_name;
        uint16_t mbr_pb_index;
        uint16_t mbr_pb_type;
        uint16_t mbr_json_name;
};

struct ndb_type {
        uint16_t typ_size;
        uint16_t typ_mbr_index;
        uint16_t typ_mbr_count;
	uint16_t typ_hstr_name;
};

struct ndb_db {
	void *db_base;
	uint32_t db_type_cnt;
	struct ndb_type *db_type_ptr;
	uint32_t db_member_cnt;
	struct ndb_member *db_member_ptr;
	uint32_t db_hstr_cnt;
	uint32_t db_hstr_modulo;
	uint32_t db_hstr_algo;
	struct ndb_hstring *db_hstr_ptr;
	uint32_t db_string_cnt;
	char *db_string_ptr;
	uint32_t db_key_size;
	void *db_key_ptr;
	uint32_t db_key_type;
};

struct ndb_localdb {
	uint32_t ldb_verion;
	uint32_t ldb_size;
	uint32_t ldb_type_size;
	uint32_t ldb_type_offset;
	uint32_t ldb_member_size;
	uint32_t ldb_member_offset;
	uint32_t ldb_hstr_size;
	uint32_t ldb_hstr_modulo;
	uint32_t ldb_hstr_algo;
	uint32_t ldb_hstr_offset;
	uint32_t ldb_string_size;
	uint32_t ldb_string_offset;
	uint32_t ldb_key_size;
	uint32_t ldb_key_offset;
	uint32_t ldb_key_type;
};

int ndb_open_buffer(struct ndb_db **, const char *buffer);
int ndb_close(struct ndb_db *);

int ndb_hstr_find(struct ndb_db *, const char *name);
int ndb_path_create(struct ndb_db *, struct ndb_path *, const char *name);

int ndb_get_obj(struct ndb_db *, struct ndb_path *, struct ndb_keys *, void **obj);
int ndb_set_obj(struct ndb_db *, const struct ndb_path *, struct ndb_keys *, const void *obj);
int ndb_print_obj(struct ndb_db *, const struct ndb_path *, struct ndb_keys *,const void *obj);

int ndb_get_json(struct ndb_db *, const struct ndb_path *, struct ndb_keys *, char **obj);
int ndb_set_json(struct ndb_db *, const struct ndb_path *, struct ndb_keys *, const char *obj);

int ndb_get_pb(struct ndb_db *, const struct ndb_path *, struct ndb_keys *, void **obj, size_t bufsize);
int ndb_set_pb(struct ndb_db *, const struct ndb_path *, struct ndb_keys *, const void *pb, size_t pblen);




