all:	spwr.c spwr.h ndb.c ndb.h
	gcc -g main.c spwr.c ndb.c -lcjson
	./a.out

#ndb_pb2.py:	ndb.proto
#	protoc --python_out=. ndb.proto

%_pb2.py:	%.proto
	protoc --python_out=. $<

.SECONDARY:	spwr.pb

%.pb:	%.proto
	protoc --include_imports  -o $@ $<

%.h %.c: %.pb ndb_pb2.py
	./ndb_generator.py $<

clean:
	rm -rf ./a.out ndb.h ndb.c ndb.pb spwr.h spwr.c spwr.pb __pycache__ ndb_pb2.py
