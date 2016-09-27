GEN = build
STZ_FLAGS = -pkg-path ${GEN}

STANZADIR = ~/stanza
STZ_COMPILER_MAIN = ${STANZADIR}/compiler/stz-main.stanza

COMMON_FLAGS = ${GEN}/file-system.o

OS2 := $(strip $(shell uname))

ifeq ($(OS2), Darwin)
  $(info Compiling for Darwin)
  CFLAGS = -DMACOSX=1
  FLAGS = $(COMMON_FLAGS) -lglfw3 -framework Carbon -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
else
  $(info Compiling for non Darwin)
  OS := $(strip $(shell uname -o))
  ifeq ($(OS), GNU/Linux)
    $(info Compiling for GNU/Linux)
    CFLAGS = 
    FLAGS = $(COMMON_FLAGS) -lglfw -lrt -lm -lXrandr -lXinerama -lXi -lXcursor -lXrender -lGL -lGLU -lpthread -ldl -ldrm -lXdamage -lXfixes -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-randr -lxcb-xfixes -lxcb-render -lxcb-shape -lxcb-sync -lxshmfence -lXxf86vm -lXext -lX11 -lxcb -lXau -lXdmcp
  endif
endif

clean:
	rm -f ${GEN}/*; echo "build products" > ${GEN}/readme.txt

