#!/usr/bin/python3
import google.protobuf.descriptor_pb2 as descriptor
import ndb_pb2
import sys
from pathlib import Path

FieldD = descriptor.FieldDescriptorProto

typemap = {
    FieldD.TYPE_DOUBLE : {"c_encode" : "double" },
    FieldD.TYPE_FLOAT : { "c_encode" : "float"},
    FieldD.TYPE_INT64 : { "c_encode" : "int64_t"},
    FieldD.TYPE_UINT64 : { "c_encode" : "uint64_t"},
    FieldD.TYPE_INT32 : { "c_encode" : "int32_t"},
    FieldD.TYPE_FIXED64 : { "c_encode" : "uint64_t"},
    FieldD.TYPE_FIXED32 : { "c_encode" : "uint32_t"},
    FieldD.TYPE_BOOL : { "c_encode" : "bool"},
    FieldD.TYPE_STRING : { "c_encode" : "char *"},

    FieldD.TYPE_BYTES : { "c_encode" : "bytes"},
    FieldD.TYPE_UINT32 : { "c_encode" : "uint32_t"},
    FieldD.TYPE_ENUM : { "c_encode" : "uint32_t"},
    FieldD.TYPE_SFIXED32 : { "c_encode" : "int32_t"},
    FieldD.TYPE_SFIXED64 : { "c_encode" : "int64_t"},
    FieldD.TYPE_SINT32 : { "c_encode" : "int32_t"},
    FieldD.TYPE_SINT64 : { "c_encode" : "int64_t"},
    }

labelmap = {
        FieldD.LABEL_OPTIONAL : 1,
        FieldD.LABEL_REQUIRED : 2,
        FieldD.LABEL_REPEATED : 3
        }


def tmap(i):
    if i.type == FieldD.TYPE_MESSAGE:
        return i.type_name[1:]
    if i.type in typemap:
        return typemap[i.type]['c_encode']
    print("unknown type", i)
    return i

def print_decode(i, f, indent = 1):
# #define PB_GEN_FIELD_INFO_1(structname, atype, htype, ltype, fieldname, tag) \
    #print(i)
    print("struct ndb_members _" + i.name+ "_members[] = {", file = f)
    for j in i.field:
        if j.type_name:
            print( "X("+ i.name+',', j.name,",", j.type_name[1:], ',', str(j.number),")", file = f)
        else:
            print( "X("+ i.name+',', j.name,",", "string", ',', str(j.number),")", file = f)
    print("};", file = f)

def print_struct(i, f):
    print("struct ndb_message _" + i.name+ "_message = {", file = f)
    print("   _" + i.name+ "_members,", file = f)
    print("   sizeof( _" + i.name+ "_members)/sizeof( _" + i.name + "_members[0]),", file = f)
    print("   sizeof(" + i.name, ")", file = f)
    print("};", file = f)

def print_extern_struct(i, f):
    print("extern struct ndb_message _" + i.name + "_message;", file = f)

def print_message(i, f, indent = 1):
    #print(i)
#    tab0 = "    " * (indent - 1)
#    if i['type'] == "ONEOF":
#        print(tab0, "typedef struct", '_u_' + i['name'], "{", file = f)
#    else:
#        print(tab0, "typedef struct", '_' + i['name'], "{", file = f)
#    tab = "    " * indent
#    if i['type'] == "ONEOF":
#        print(tab, "uint32_t which_" + i['name'], ";", file = f)
#        print(tab, "union {", file = f)
#        for k in i['members']:
#            if k:
#                print(tab, "    ", tmap(k['type']), k['name'] + ";", file = f)
#        print(tab, "}", i['name'], ";", file = f)
#        print(tab0, "}",i['name'], ";", file = f)
#    else:
        tab0 = "    " * (indent - 1)
        tab = "    " * (indent)
        print(tab0, "typedef struct", '_' + i.name, "{", file = f)
        for j in i.field:
            if j.type == "ONEOF":
                print(tab, "struct _u_" + j['name'], j['name'] + ";", file = f)
            #elif 'members' in j:
                #print_message(j, indent + 1, file = f)
            #    continue
            else:
                k = {}
                max_size = None
                if j.options.HasExtension(ndb_pb2.nanodb):
                    field_options = j.options.Extensions[ndb_pb2.nanodb]
                    print("Options", field_options)
                    if field_options.HasField("max_size"):
                        max_size = field_options.max_size

                #max_count = k['max_count'] if 'max_count' in k else None
                #if 'option' in j:
                #    k = j['option']
                #print(j.options)
                #if 'repeated' in j:
                if j.type == 11:
                    print(tab, j.type_name[1:], str(j.name) + ";", file = f)
                elif False:
                    print(tab, 'int16_t', str(j['name']) + "_count;", file = f)
                    if 'max_count' in k:
                        print(tab, tmap(j), str(j.name) + "[", k['max_count'],"];", file = f)
                    else:
                        print(tab, tmap(j), "*" + str(j['name']) + ";", file = f)
                else:
                    if max_size:
                        print("I GOT MAX_SIZE")
                    #print("HELP", j)
                    if j.type == FieldD.TYPE_STRING  and max_size:
                        print(tab, 'char', str(j.name) + '[' + str(max_size) + "];", file = f)
                    else:
                        print(tab, tmap(j), str(j.name) + ";", file = f)
        print(tab0, "}",i.name, ";", file = f)

def print_enum(i, f):
    print("enum", i.name, "{", file = f)
    for j in i.value:
        print("    ", j.name, "=", str(j.number) + ",", file = f)
    print("};", file = f)

def print_enum_string(i, f):
    print("const char *pb_enum" + i.name, "( enum", i.name, "value )\n{", file = f)
    print("  switch (value) {", file = f)
    for j in i.value:
        print("    case ", j.number, ": return \"" + j.name + "\";", file = f)
    print("    default: return NULL;", file = f)
    print("  }", file = f)
    print("}", file = f)

def main():
    filepath = sys.argv[1]

    filename = Path(filepath)
    file_wo_ext = filename.with_suffix('')

    with open(filepath, 'rb') as f:
        data = f.read()
    #print(filepath, len(data))
    m=descriptor.FileDescriptorSet()
    m.ParseFromString(data)

    gmessages = []
    #print(m)
    l = len(m.file)
    for i in m.file[l-1].message_type:
        gmessages.append(i)

    genums = []
    for i in m.file[l-1].enum_type:
        genums.append(i)

    gservice = []
    for i in m.file[l-1].service:
        gservice.append(i)

    #print(gservice)
    for i in gservice:
        print(i.name)
        for m in i.method:
            print(m)

    with open(filename.with_suffix('.h'), 'w') as f:
        print('#include "stdint.h"', file = f)
        print('#include "nanodb.h"', file = f)
        print("// genums:", file = f)
        for i in genums:
            print_enum(i, f)

        print("// gmessages:", file = f)
        for i in gmessages:
            print_message(i, f)

        for i in gmessages:
            print_extern_struct(i, f)

    with open(filename.with_suffix('.c'), 'w') as f:
        print('#include "'+str(filename.with_suffix('.h'))+'"', file = f)

        for i in genums:
            print_enum_string(i, f)

        print("// gmembers:", file = f)
        for i in gmessages:
            print_decode(i, f)

        print("// messages:", file = f)
        for i in gmessages:
            print_struct(i, f)

if __name__ == '__main__':
    main()
