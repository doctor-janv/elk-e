#!/bin/sh

echo ""
echo "***** Making HTML Documentation *****"

echo "# Requirements Traceability Matrix" > doc/RTM.md
echo "\page RTM Requirements Traceability Matrix" >> doc/RTM.md
cat TraceAbilityMatrix.md >> doc/RTM.md

doxygen doc/doxygen_settings/doxyconfig_elke