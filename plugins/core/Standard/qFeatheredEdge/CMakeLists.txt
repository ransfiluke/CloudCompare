option( PLUGIN_STANDARD_QFEATHEREDEDGE "Install qFeatheredEdge plugin" OFF )

if( PLUGIN_STANDARD_QFEATHEREDEDGE )
	project( QFEATHEREDEDGE_PLUGIN )

	AddPlugin( NAME ${PROJECT_NAME} )

	add_subdirectory( include )
	add_subdirectory( src )
	add_subdirectory( ui )
endif()
