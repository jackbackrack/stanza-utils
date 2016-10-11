stanza_utils_all: ${GEN}/eval.pkg ${GEN}/file-system.pkg ${GEN}/socket.pkg ${GEN}/utils.pkg 

ALL_PKG_DEPS += stanza_utils_all

${GEN}/eval.pkg: stanza-utils/eval.stanza 
	stanza $< $(STZ_FLAGS) -pkg ${GEN}

${GEN}/file-system.o: stanza-utils/file-system.c
	cc $(CFLAGS) -c stanza-utils/file-system.c -o $@

${GEN}/file-system.pkg: stanza-utils/file-system.stanza  utils.pkg 
	stanza $< $(STZ_FLAGS) -pkg ${GEN}

${GEN}/socket.o: stanza-utils/socket.c
	cc $(CFLAGS) -c stanza-utils/socket.c -o $@

${GEN}/socket.pkg: stanza-utils/socket.stanza ${GEN}/socket.o
	stanza $< $(STZ_FLAGS) -pkg ${GEN}

${GEN}/utils.pkg: stanza-utils/utils.stanza ${GEN}/file-system.o 
	stanza $< $(STZ_FLAGS) -pkg ${GEN}

