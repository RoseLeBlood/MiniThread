idf_component_register(
	   INCLUDE_DIRS .  ./inc 
)

add_prebuilt_library(libmini_Thread lib/libmini_Thread.a)

target_link_libraries(${COMPONENT_LIB} INTERFACE libmini_Thread)
