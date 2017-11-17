boot/dependencies: stanza-utils/registry.stanza stanza-utils/dependencies.stanza ${DEPENDENCIES_SYNTAX_FILES}
	stanza stanza-utils/registry.stanza stanza-utils/dependencies.stanza ${DEPENDENCIES_SYNTAX_FILES} -o boot/dependencies ${DEPENDENCIES_SYNTAX_CC_FILES} # -optimize 

boot/dependencies.mk: boot/dependencies
	boot/dependencies . > boot/dependencies.mk
