##########Check if boost exists
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread date_time filesystem iostreams serialization)
if ( Boost_FOUND )
   message( "-- Boost found. include=${Boost_INCLUDE_DIR} libs=${Boost_LIBRARIES}" )
   include_directories( ${Boost_INCLUDE_DIR} )
endif ( Boost_FOUND )

FIND_PACKAGE(Magick++)
if ( Magick++_FOUND )
   message( "-- Magick++ found. include=${Magick++_INCLUDE_DIR} libs=${Magick++_LIBRARIES}" )
   include_directories( ${Magick++_INCLUDE_DIR} )
endif ( Magick++_FOUND )



SET(APOPH_SOURCE 
  Main.cc
  Work.cc
)
SET(APOPH_TARGET apoph)

INCLUDE_DIRECTORIES("../include")

ADD_EXECUTABLE(${APOPH_TARGET} ${APOPH_SOURCE})
TARGET_LINK_LIBRARIES(${APOPH_TARGET} ${Boost_LIBRARIES} ${Magick++_LIBRARY} pthread)
