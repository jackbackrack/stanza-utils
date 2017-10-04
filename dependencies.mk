build/dependencies: stanza-utils/registry.stanza stanza-utils/dependencies.stanza ${DEPENDENCIES_SYNTAX_FILES}
	stanza stanza-utils/registry.stanza stanza-utils/dependencies.stanza ${DEPENDENCIES_SYNTAX_FILES} -o build/dependencies ${DEPENDENCIES_SYNTAX_CC_FILES} # -optimize 

build/dependencies.mk: build/dependencies
	build/dependencies . > build/dependencies.mk
