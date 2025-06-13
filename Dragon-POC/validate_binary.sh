
#!/bin/bash

# Binary Validation Script for David's Dynamic Graph
# Validates solutions against test cases with detailed output

echo "=== David's Dynamic Graph Binary Validator ==="

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Default values
SOLUTION="solution_correct"
TEST_PATTERN="tests/test_*.in"
VERBOSE=false
TIMEOUT=30

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -s|--solution)
            SOLUTION="$2"
            shift 2
            ;;
        -t|--test)
            TEST_PATTERN="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        --timeout)
            TIMEOUT="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -s, --solution SOLUTION    Solution to test (default: solution_correct)"
            echo "  -t, --test PATTERN         Test pattern (default: tests/test_*.in)"
            echo "  -v, --verbose              Verbose output"
            echo "  --timeout SECONDS          Timeout in seconds (default: 30)"
            echo "  -h, --help                 Show this help"
            echo ""
            echo "Examples:"
            echo "  $0                         # Test default solution against all tests"
            echo "  $0 -s solution_greedy      # Test greedy solution"
            echo "  $0 -t tests/test_01.in     # Test specific case"
            echo "  $0 -v                      # Verbose output"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Check if solution exists
if [ ! -f "output/$SOLUTION" ]; then
    echo -e "${RED}ERROR: Solution binary 'output/$SOLUTION' not found${NC}"
    echo "Available solutions:"
    ls output/solution_* 2>/dev/null || echo "No solution binaries found"
    exit 1
fi

# Check if validator exists
if [ ! -f "output/validator" ]; then
    echo -e "${YELLOW}Compiling validator...${NC}"
    g++ -o output/validator validator.cpp -std=c++17 -O2
    if [ $? -ne 0 ]; then
        echo -e "${RED}ERROR: Failed to compile validator${NC}"
        exit 1
    fi
fi

# Validation function
validate_test() {
    local test_file=$1
    local test_name=$(basename "$test_file" .in)
    
    if [ "$VERBOSE" = true ]; then
        echo -e "${BLUE}=== Testing $test_name ===${NC}"
        echo "Test file: $test_file"
        echo "Solution: $SOLUTION"
        echo "Timeout: ${TIMEOUT}s"
        echo ""
    else
        echo -n "Testing $test_name... "
    fi
    
    # Create temporary files for communication
    local input_pipe=$(mktemp -u)
    local output_file=$(mktemp)
    local error_file=$(mktemp)
    
    mkfifo "$input_pipe"
    
    # Start validator in background
    timeout "$TIMEOUT" ./output/validator "$test_file" < "$input_pipe" > "$output_file" 2> "$error_file" &
    local validator_pid=$!
    
    # Start solution and connect to validator
    timeout "$((TIMEOUT-5))" ./output/$SOLUTION < "$test_file" > "$input_pipe" 2>/dev/null &
    local solution_pid=$!
    
    # Wait for completion
    wait $validator_pid
    local validator_exit=$?
    
    wait $solution_pid 2>/dev/null
    local solution_exit=$?
    
    # Clean up
    rm -f "$input_pipe"
    
    # Analyze results
    local score=0
    local queries_used=0
    
    if [ $validator_exit -eq 0 ] && [ -s "$output_file" ]; then
        score=$(grep -o "Score: [0-9]*" "$output_file" | grep -o "[0-9]*" | head -1 || echo "0")
        queries_used=$(grep -c ">" "$output_file" || echo "0")
        
        if [ "$VERBOSE" = true ]; then
            echo -e "${GREEN}✓ PASSED${NC}"
            echo "Score: $score/1000"
            echo "Queries used: $queries_used"
            echo ""
            echo "Validator output:"
            cat "$output_file"
            echo ""
        else
            echo -e "${GREEN}PASSED${NC} (Score: $score, Queries: $queries_used)"
        fi
    else
        if [ "$VERBOSE" = true ]; then
            echo -e "${RED}✗ FAILED${NC}"
            echo "Validator exit code: $validator_exit"
            echo "Solution exit code: $solution_exit"
            echo ""
            if [ -s "$output_file" ]; then
                echo "Validator output:"
                cat "$output_file"
                echo ""
            fi
            if [ -s "$error_file" ]; then
                echo "Validator errors:"
                cat "$error_file"
                echo ""
            fi
        else
            echo -e "${RED}FAILED${NC}"
        fi
    fi
    
    # Clean up temporary files
    rm -f "$output_file" "$error_file"
    
    return $validator_exit
}

# Main validation loop
echo -e "${BLUE}Validating solution: $SOLUTION${NC}"
echo -e "${BLUE}Test pattern: $TEST_PATTERN${NC}"
echo ""

TOTAL_TESTS=0
PASSED_TESTS=0
TOTAL_SCORE=0

for test_file in $TEST_PATTERN; do
    if [ -f "$test_file" ]; then
        TOTAL_TESTS=$((TOTAL_TESTS + 1))
        
        if validate_test "$test_file"; then
            PASSED_TESTS=$((PASSED_TESTS + 1))
        fi
        
        if [ "$VERBOSE" = true ]; then
            echo "----------------------------------------"
        fi
    fi
done

# Summary
echo ""
echo -e "${BLUE}=== VALIDATION SUMMARY ===${NC}"
echo "Solution: $SOLUTION"
echo "Total tests: $TOTAL_TESTS"
echo "Passed tests: $PASSED_TESTS"
echo "Failed tests: $((TOTAL_TESTS - PASSED_TESTS))"

if [ $TOTAL_TESTS -gt 0 ]; then
    success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    echo "Success rate: ${success_rate}%"
    
    if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
        echo -e "${GREEN}All tests passed!${NC}"
    elif [ $PASSED_TESTS -gt $((TOTAL_TESTS / 2)) ]; then
        echo -e "${YELLOW}Most tests passed${NC}"
    else
        echo -e "${RED}Many tests failed${NC}"
    fi
else
    echo -e "${RED}No test files found matching pattern: $TEST_PATTERN${NC}"
fi

echo ""
echo "Use -v flag for verbose output"
echo "Use -h flag for help"
