
#!/bin/bash

# Cleanup Script for David's Dynamic Graph
# Removes generated files and compiled binaries

echo "=== David's Dynamic Graph Cleanup ==="

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Parse arguments
CLEAN_ALL=false
CLEAN_BINARIES=false
CLEAN_TESTS=false
CLEAN_PERFORMANCE=false

if [ $# -eq 0 ]; then
    CLEAN_ALL=true
fi

while [[ $# -gt 0 ]]; do
    case $1 in
        --all)
            CLEAN_ALL=true
            shift
            ;;
        --binaries)
            CLEAN_BINARIES=true
            shift
            ;;
        --tests)
            CLEAN_TESTS=true
            shift
            ;;
        --performance)
            CLEAN_PERFORMANCE=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  --all           Clean everything (default)"
            echo "  --binaries      Clean compiled binaries only"
            echo "  --tests         Clean test outputs only"
            echo "  --performance   Clean performance data only"
            echo "  -h, --help      Show this help"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Clean binaries
if [ "$CLEAN_ALL" = true ] || [ "$CLEAN_BINARIES" = true ]; then
    echo -e "${YELLOW}Cleaning compiled binaries...${NC}"
    if [ -d "output" ]; then
        rm -f output/solution_*
        rm -f output/generator
        rm -f output/validator
        echo "Removed compiled binaries"
    fi
fi

# Clean test outputs
if [ "$CLEAN_ALL" = true ] || [ "$CLEAN_TESTS" = true ]; then
    echo -e "${YELLOW}Cleaning test outputs...${NC}"
    if [ -d "tests" ]; then
        rm -f tests/*.out
        rm -f tests/test_*.in
        echo "Removed test files and outputs"
    fi
fi

# Clean performance data
if [ "$CLEAN_ALL" = true ] || [ "$CLEAN_PERFORMANCE" = true ]; then
    echo -e "${YELLOW}Cleaning performance data...${NC}"
    if [ -d "performance" ]; then
        rm -f performance/*.csv
        rm -f performance/*.txt
        rm -f performance/*.log
        echo "Removed performance data"
    fi
fi

# Clean temporary files
if [ "$CLEAN_ALL" = true ]; then
    echo -e "${YELLOW}Cleaning temporary files...${NC}"
    rm -f temp_*.txt
    rm -f *.tmp
    rm -f core.*
    echo "Removed temporary files"
fi

echo -e "${GREEN}Cleanup completed!${NC}"
echo ""
echo "To rebuild everything:"
echo "  ./test_generator.sh    # Generate test cases"
echo "  ./test_runner.sh       # Run all tests"
echo "  ./validate_binary.sh   # Validate solutions"
