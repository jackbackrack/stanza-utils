# boot/gendoc: stanza-utils/registry.stanza stanza-utils/gendoc.stanza ${DEPENDENCIES_SYNTAX_FILES} 
# 	stanza ${DEPENDENCIES_SYNTAX_FILES} stanza-utils/registry.stanza stanza-utils/gendoc.stanza -o boot/gendoc ${DEPENDENCIES_SYNTAX_CC_FILES} # -optimize 

boot/dependencies: stanza-utils/registry.stanza stanza-utils/gen-makefile.stanza stanza-utils/read-dependencies.stanza 
	stanza stanza-utils/registry.stanza stanza-utils/gen-makefile.stanza stanza-utils/read-dependencies.stanza -o boot/dependencies

boot/dependencies.mk: boot/dependencies
#	boot/dependencies . ${DEPS_STANZA}
	boot/dependencies . ${DEPS_STANZA} > boot/dependencies.mk
