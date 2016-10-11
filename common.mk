GEN = build
STZ_FLAGS = -pkg-path ${GEN}
ALL_DEPS = 

STANZADIR = `python stanza-utils/stanza-install-dir.py`
STZ_COMPILER_MAIN = ${STANZADIR}/compiler/stz-main.stanza

print_stz_compiler_main:
	echo ${STZ_COMPILER_MAIN}

COMMON_FLAGS = ${GEN}/file-system.o

OS2 := $(strip $(shell uname))

ifeq ($(OS2), Darwin)
  $(info Compiling for Darwin)
  CFLAGS = -DMACOSX=1
  LIBS = $(COMMON_LIBS)
else
  $(info Compiling for non Darwin)
  OS := $(strip $(shell uname -o))
  ifeq ($(OS), GNU/Linux)
    $(info Compiling for GNU/Linux)
    CFLAGS = 
    LIBS = $(COMMON_LIBS) -lm
  endif
endif

clean:
	rm -f ${GEN}/*; echo "build products" > ${GEN}/readme.txt

