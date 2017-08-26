build/dependencies: stanza-utils/registry.stanza stanza-utils/dependencies.stanza ${DEPENDENCIES_SYNTAX_FILES}
	stanza stanza-utils/registry.stanza stanza-utils/dependencies.stanza ${DEPENDENCIES_SYNTAX_FILES} -o build/dependencies

build/dependencies.mk: build/dependencies
	build/dependencies . > build/dependencies.mk
