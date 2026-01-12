#!/bin/bash
# Chaos Crew - WSL Launch Script
# Ensures proper locale configuration for i18n support

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}=====================================${NC}"
echo -e "${GREEN}   Chaos Crew - WSL Launch Script   ${NC}"
echo -e "${GREEN}=====================================${NC}"
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo -e "${RED}Error: build directory not found!${NC}"
    echo "Please compile the project first:"
    echo "  mkdir build && cd build"
    echo "  cmake .. -DCMAKE_BUILD_TYPE=Release"
    echo "  cmake --build . -j\$(nproc)"
    exit 1
fi

# Check if executable exists
if [ ! -f "build/bin/chaos-crew" ]; then
    echo -e "${RED}Error: chaos-crew executable not found in build/bin/${NC}"
    echo "Please compile the project first."
    exit 1
fi

# Check current locale
echo -e "${YELLOW}Checking locale configuration...${NC}"
if [ -z "$LANG" ] || [ "$LANG" = "C.UTF-8" ] || [ "$LANG" = "C" ]; then
    echo -e "${YELLOW}Warning: LANG not configured or set to generic locale${NC}"
    echo "Setting locale to es_ES.UTF-8 for this session..."
    export LANG=es_ES.UTF-8
    export LC_ALL=es_ES.UTF-8

    echo ""
    echo -e "${YELLOW}Tip: To make this permanent, add to ~/.bashrc:${NC}"
    echo "  echo 'export LANG=es_ES.UTF-8' >> ~/.bashrc"
    echo "  echo 'export LC_ALL=es_ES.UTF-8' >> ~/.bashrc"
else
    echo -e "${GREEN}Locale configured: $LANG${NC}"
fi

echo ""
echo -e "${GREEN}Launching Chaos Crew...${NC}"
echo -e "${GREEN}Press L during gameplay to toggle language (ES/EN)${NC}"
echo ""

# Change to build directory and run
cd build
./bin/chaos-crew

# Return to original directory
cd ..
