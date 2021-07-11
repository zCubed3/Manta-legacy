#!/bin/bash

if ! type -p make; then
   print "Missing make! Please install it\n"
fi

if test -f ".MANTA_CONFIG"; then
   echo "Warning: Manta is going to move to a dedicated build utility eventually, do not rely on these bash scripts for stability... sorry about that!"

   printf ".MANTA_CONFIG found!\n"
   source .MANTA_CONFIG

   echo "Running make..."
   make -C ${CMAKE_PATH}

   echo "Copying content to engine"
   cp -r "./projects/engine_data/." "${BUILD_PATH}/data"

   MMDESC_FILES=$(find ./projects/mantamono | grep -Ew ".+MMDESC")
   # An MMDESC is basically a .sh file that has variables for compilation arguments

   declare -a CS_PROJ_FILES
   ORIGINAL_PATH=$(pwd)
   for MMDESC in "${MMDESC_FILES[@]}"; do
      source "${MMDESC}"
      CS_BASE_PATH="$(dirname $MMDESC)"
      
      cd $CS_BASE_PATH
      CS_PROJ_FILES=$(find "." | grep -Ew ".+\.cs")

      csc "/target:library" "/out:$BUILD_PATH/bin/$CS_PROJ_BIN.dll" $CS_PROJ_FILES
      
      cd $ORIGINAL_PATH
   done
else
   printf ".MANTA_CONFIG not found, did you forget to run configure.sh?\n"
fi
