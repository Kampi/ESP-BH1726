set(COMPONENT_SRCS
    "src/bh1726.cpp"
    )

set(COMPONENT_ADD_INCLUDEDIRS
	"include"
	)

set(COMPONENT_PRIV_REQUIRES driver)

register_component()