cmake_minimum_required(VERSION 3.2)

FIND_PACKAGE( Boost COMPONENTS system timer chrono log thread program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
set (CMAKE_CXX_STANDARD 11)
add_definitions(-DBOOST_LOG_DYN_LINK)
ADD_EXECUTABLE( tesim ControlLoops.cpp PI.cpp TEChannel.cpp
TEController.cpp TEErrorFreeChannel.cpp TEGEErrorChannel.cpp
TEIIDErrorChannel.cpp TENames.cpp TEPlant.cpp teprob.cpp TETimeSync.cpp
TETypes.cpp tesim_main.cpp)

TARGET_LINK_LIBRARIES( tesim LINK_PUBLIC ${Boost_LIBRARIES} rt pthread)
