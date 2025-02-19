#include <stddef.h>

// wtp - wire type for protobufs
// wtf - wire type for flatbufs
// mt - memory type

// tag
// type
// size_offset
// data_offset
// data_size
// array_size


#if 0
LTYPE identifier	Value	Storage format
PB_LTYPE_BOOL	0x00	Boolean.
PB_LTYPE_VARINT	0x01	Integer.
PB_LTYPE_UVARINT	0x02	Unsigned integer.
PB_LTYPE_SVARINT	0x03	Integer, zigzag encoded.
PB_LTYPE_FIXED32	0x04	32-bit integer or floating point.
PB_LTYPE_FIXED64	0x05	64-bit integer or floating point.
PB_LTYPE_BYTES	0x06	Structure with size_t field and byte array.
PB_LTYPE_STRING	0x07	Null-terminated string.
PB_LTYPE_SUBMESSAGE	0x08	Submessage structure.
PB_LTYPE_SUBMSG_W_CB	0x09	Submessage with pre-decoding callback.
PB_LTYPE_EXTENSION	0x0A	Pointer to pb_extension_t.
PB_LTYPE_FIXED_LENGTH_BYTES	0x0B	Inline pb_byte_t array of fixed size.

HTYPE identifier	Value	Field handling
PB_HTYPE_REQUIRED	0x00	Verify that field exists in decoded message.
PB_HTYPE_OPTIONAL	0x10	Use separate has_<field> boolean to specify whether the field is present.
PB_HTYPE_SINGULAR	0x10	Proto3 field, which is present when its value is non-zero.
PB_HTYPE_REPEATED	0x20	A repeated field with preallocated array. Separate <field>_count for number of items.
PB_HTYPE_FIXARRAY	0x20	A repeated field that has constant length.
PB_HTYPE_ONEOF	0x30	Oneof-field, only one of each group can be present.

ATYPE identifier	Value	Allocation method
PB_ATYPE_STATIC	0x00	Statically allocated storage in the structure.
PB_ATYPE_POINTER	0x80	Dynamically allocated storage. Struct field contains a pointer to the storage.
PB_ATYPE_CALLBACK	0x40	A field with dynamic storage size. Struct field contains a pointer to a callback function.


HTYPE
OPTIONAL	// has has_
FIXARRAY
ARRAY		// has <field>_count

ATYPE
	STATIC
	POINTER
	CALLBACK

LTYPE
	LABEL_REPEATED
	LABEL_REQUIRED
	LABEL_OPTIONAL
MTYPE
size
signed


signed(1)
size(2)		8 16 32 64
ltype(2)  repeated,required,optional,repeated-fixed
		required: member
		optional: has_member; member
		repeated-fixed: member[]
		repeated: membert_count; member[]
type int,float,double,

#endif

enum ndb_mt {
	mt_none = 0,
	mt_bool = 1,
	mt_u8 = 2,
	mt_u16 = 3,
	mt_u32 = 4,
	mt_u64 = 5,
	mt_s8 = 6,
	mt_s16 = 7,
	mt_s32 = 8,
	mt_s64 = 9,
	mt_float = 10,
	mt_double = 11,
	mt_fstring = 12,
	mt_pstring = 13,
	mt_object = 14,
	mt_oneof = 15,
};

enum ndb_wtp {
	wtp_varint = 0,
	wtp_64bit = 1,
	wtp_string = 2,
	wtp_32bit = 5
};

enum ndb_type {
	ndbt_none = 0,
	ndbt_int = 0
};

struct message_fields {
	char *mf_name;
	enum ndb_type mf_type;
	size_t mf_offset;
};

struct message {
	struct message_fields *ms_fields;
	int ms_fieldcnt;
};

// structname, atype, htype, ltype, fieldname, tag

#define str(s) #s
#define X(a,b,c,d)  {str(b), offsetof(a,b),d, & _ ## c ## _message},
struct ndb_message;
struct ndb_members {
	char *name;
	size_t off;
	size_t tag;
	struct ndb_message *type;
};

struct ndb_message {
	struct ndb_members *nm_members;
	size_t nm_members_cnt;
	size_t nm_message_size;
};

extern struct ndb_message _string_message;
