stanza_utils_all: ${GEN}/eval.pkg ${GEN}/file-system.pkg ${GEN}/socket.pkg  ${GEN}/process.pkg ${GEN}/utils.pkg 

LIBS += ${GEN}/socket.o ${GEN}/file-system.o ${GEN}/process.o

ALL_PKG_DEPS += stanza_utils_all
BASE_EVAL_PKGS = ${GEN}/eval-core.pkg ${GEN}/eval-collections.pkg ${GEN}/eval-math.pkg ${GEN}/eval.pkg

${GEN}/eval.pkg: stanza-utils/eval.stanza ${GEN}/utils.pkg ${GEN}/file-system.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/file-system.o: stanza-utils/file-system.c
	cc $(CFLAGS) -c stanza-utils/file-system.c -o $@

${GEN}/file-system.pkg: stanza-utils/file-system.stanza ${GEN}/utils.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/eval-file-system.pkg: ${GEN}/eval-file-system.stanza ${GEN}/utils.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/socket.o: stanza-utils/socket.c
	cc $(CFLAGS) -c stanza-utils/socket.c -o $@

${GEN}/socket.pkg: stanza-utils/socket.stanza ${GEN}/socket.o
	stanza $< $(STZ_FLAGS)

${GEN}/process.o: stanza-utils/process.c
	cc $(CFLAGS) -c stanza-utils/process.c -o $@

${GEN}/process.pkg: stanza-utils/process.stanza ${GEN}/process.o ${GEN}/utils.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/gen-repl: stanza-utils/gen-repl.stanza
	stanza $< $(STZ_FLAGS) -o $@

${GEN}/eval-core.stanza: ${GEN}/gen-repl
	${GEN}/gen-repl core

${GEN}/eval-core.pkg: ${GEN}/eval-core.stanza ${GEN}/eval.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/eval-math.stanza: ${GEN}/gen-repl
	${GEN}/gen-repl math

${GEN}/eval-math.pkg: ${GEN}/eval-math.stanza ${GEN}/eval-core.pkg ${GEN}/eval.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/eval-collections.stanza: ${GEN}/gen-repl
	${GEN}/gen-repl collections

${GEN}/eval-collections.pkg: ${GEN}/eval-collections.stanza ${GEN}/eval-core.pkg ${GEN}/eval.pkg 
	stanza $< $(STZ_FLAGS)

${GEN}/utils.pkg: stanza-utils/utils.stanza ${GEN}/file-system.o ${GEN}/socket.o ${GEN}/process.o 
	stanza $< $(STZ_FLAGS)

${GEN}/eval-utils.stanza: ${GEN}/gen-repl ${GEN}/utils.pkg
	${GEN}/gen-repl utils

${GEN}/eval-utils.pkg: ${GEN}/eval-utils.stanza ${BASE_EVAL_PKGS} ${GEN}/utils.pkg
	stanza $< $(STZ_FLAGS)
