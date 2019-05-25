# boot/gendoc: stanza-utils/registry.stanza stanza-utils/gendoc.stanza ${DEPENDENCIES_SYNTAX_FILES} 
# 	stanza ${DEPENDENCIES_SYNTAX_FILES} stanza-utils/registry.stanza stanza-utils/gendoc.stanza -o boot/gendoc ${DEPENDENCIES_SYNTAX_CC_FILES} # -optimize 

boot/dependencies: stanza-utils/registry.stanza stanza-utils/gen-makefile.stanza stanza-utils/read-dependencies.stanza ${SYNTAX_STANZA}
	stanza stanza-utils/registry.stanza stanza-utils/gen-makefile.stanza stanza-utils/read-dependencies.stanza -o boot/dependencies

boot/dependencies.mk: boot/dependencies
#	boot/dependencies . ${SYNTAX_STANZA}
	boot/dependencies . ${SYNTAX_STANZA} > boot/dependencies.mk
