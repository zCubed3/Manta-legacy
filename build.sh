#!/bin/bash

if ! type -p make; then
   print "Missing make! Please install it\n"
fi

if test -f ".MANTA_CONFIG"; then
   printf ".MANTA_CONFIG found!\n"
   source .MANTA_CONFIG

   echo "Running make..."
   make -C ${CMAKE_PATH}

   echo "Copying content to engine"
   cp -r "./projects/engine_data/." "${BUILD_PATH}/data"

   CS_SOURCE_FILES=$(find ./projects/mantamono | grep -Ew ".+\.cs")
   
   csc "/target:library" "/out:${BUILD_PATH}/MantaMono.dll" $CS_SOURCE_FILES
   echo $CS_SOURCE_FILES
else
   printf ".MANTA_CONFIG not found, did you forget to run configure.sh?\n"
fi
