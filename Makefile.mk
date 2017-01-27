stanza_utils_all: ${GEN}/eval.pkg ${GEN}/file-system.pkg ${GEN}/socket.pkg  ${GEN}/process.pkg ${GEN}/utils.pkg 

LIBS += ${GEN}/socket.o ${GEN}/file-system.o ${GEN}/process.o

ALL_PKG_DEPS += stanza_utils_all

${GEN}/eval.pkg: stanza-utils/eval.stanza ${GEN}/utils.pkg ${GEN}/file-system.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/file-system.o: stanza-utils/file-system.c
	cc $(CFLAGS) -c stanza-utils/file-system.c -o $@

${GEN}/file-system.pkg: stanza-utils/file-system.stanza ${GEN}/utils.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/socket.o: stanza-utils/socket.c
	cc $(CFLAGS) -c stanza-utils/socket.c -o $@

${GEN}/socket.pkg: stanza-utils/socket.stanza ${GEN}/socket.o
	stanza $< $(STZ_FLAGS)

${GEN}/process.o: stanza-utils/process.c
	cc $(CFLAGS) -c stanza-utils/process.c -o $@

${GEN}/process.pkg: stanza-utils/process.stanza ${GEN}/process.o ${GEN}/utils.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/utils.pkg: stanza-utils/utils.stanza ${GEN}/file-system.o ${GEN}/socket.o ${GEN}/process.o 
	stanza $< $(STZ_FLAGS)

