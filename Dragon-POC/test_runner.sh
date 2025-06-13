
#!/bin/bash

# Test Runner Script for David's Dynamic Graph
# Runs all solutions against all test cases and generates expected outputs

echo "=== David's Dynamic Graph Test Runner ==="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Ensure all binaries are compiled
echo "Checking compiled solutions..."
SOLUTIONS="solution_correct solution_greedy solution_dp"
for sol in $SOLUTIONS; do
    if [ ! -f "output/$sol" ]; then
        echo -e "${YELLOW}Compiling $sol...${NC}"
        g++ -o "output/$sol" "$sol.cpp" -std=c++17 -O2
        if [ $? -ne 0 ]; then
            echo -e "${RED}ERROR: Failed to compile $sol${NC}"
            exit 1
        fi
    fi
done

# Compile validator
if [ ! -f "output/validator" ]; then
    echo -e "${YELLOW}Compiling validator...${NC}"
    g++ -o output/validator validator.cpp -std=c++17 -O2
    if [ $? -ne 0 ]; then
        echo -e "${RED}ERROR: Failed to compile validator${NC}"
        exit 1
    fi
fi

# Create performance tracking
mkdir -p performance
echo "test_case,solution,score,queries_used,time_ms,memory_kb" > performance/metrics.csv

# Function to run a single test
run_test() {
    local test_file=$1
    local solution=$2
    local test_name=$(basename "$test_file" .in)
    
    echo -n "Running $test_name with $solution... "
    
    # Run with timeout and capture metrics
    timeout 30s /usr/bin/time -f "%e,%M" -o temp_time.txt \
        ./output/validator "$test_file" < <(timeout 25s ./output/$solution < "$test_file" 2>/dev/null) \
        > "tests/${test_name}_${solution}.out" 2>/dev/null
    
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        # Extract score from output
        local score=$(grep -o "Score: [0-9]*" "tests/${test_name}_${solution}.out" | grep -o "[0-9]*" || echo "0")
        
        # Extract timing info
        if [ -f temp_time.txt ]; then
            local time_mem=$(cat temp_time.txt)
            local time_ms=$(echo "$time_mem" | cut -d',' -f1 | awk '{print $1*1000}')
            local memory_kb=$(echo "$time_mem" | cut -d',' -f2)
            rm -f temp_time.txt
        else
            local time_ms="0"
            local memory_kb="0"
        fi
        
        # Estimate queries used (simplified)
        local queries_used=$(grep -c ">" "tests/${test_name}_${solution}.out" || echo "0")
        
        echo "$test_name,$solution,$score,$queries_used,$time_ms,$memory_kb" >> performance/metrics.csv
        echo -e "${GREEN}OK${NC} (Score: $score)"
    else
        echo -e "${RED}FAILED${NC}"
        echo "$test_name,$solution,0,0,0,0" >> performance/metrics.csv
    fi
}

# Run tests for each solution
echo -e "\n${BLUE}Running all test cases...${NC}"

TEST_FILES=$(ls tests/*.in 2>/dev/null | wc -l)
SOLUTION_COUNT=$(echo $SOLUTIONS | wc -w)
TOTAL_TESTS=$((TEST_FILES * SOLUTION_COUNT))
CURRENT_TEST=0

for test_file in tests/*.in; do
    if [ -f "$test_file" ]; then
        for solution in $SOLUTIONS; do
            CURRENT_TEST=$((CURRENT_TEST + 1))
            echo -e "${YELLOW}[$CURRENT_TEST/$TOTAL_TESTS]${NC}"
            run_test "$test_file" "$solution"
        done
    fi
done

# Generate expected outputs using reference solution
echo -e "\n${BLUE}Generating expected outputs...${NC}"
for test_file in tests/*.in; do
    if [ -f "$test_file" ]; then
        test_name=$(basename "$test_file" .in)
        if [ ! -f "tests/${test_name}.out" ]; then
            echo "Generating expected output for $test_name..."
            timeout 30s ./output/validator "$test_file" < <(timeout 25s ./output/solution_correct < "$test_file" 2>/dev/null) \
                > "tests/${test_name}.out" 2>/dev/null
        fi
    fi
done

# Generate summary report
echo -e "\n${BLUE}Generating performance summary...${NC}"
python3 -c "
import pandas as pd
import sys

try:
    df = pd.read_csv('performance/metrics.csv')
    
    print('=== PERFORMANCE SUMMARY ===')
    print(f'Total test runs: {len(df)}')
    print(f'Successful runs: {len(df[df.score > 0])}')
    print()
    
    # Summary by solution
    print('Performance by solution:')
    summary = df.groupby('solution').agg({
        'score': ['mean', 'min', 'max'],
        'queries_used': 'mean',
        'time_ms': 'mean'
    }).round(2)
    print(summary)
    print()
    
    # Best scores
    print('Top 5 scores:')
    top_scores = df.nlargest(5, 'score')[['test_case', 'solution', 'score']]
    print(top_scores.to_string(index=False))
    
except ImportError:
    print('pandas not available, generating basic summary...')
    with open('performance/metrics.csv', 'r') as f:
        lines = f.readlines()[1:]  # Skip header
        total_runs = len(lines)
        successful_runs = len([l for l in lines if int(l.split(',')[2]) > 0])
        print(f'Total test runs: {total_runs}')
        print(f'Successful runs: {successful_runs}')
" 2>/dev/null || echo "Summary generation completed (pandas not available)"

echo -e "\n${GREEN}Test execution completed!${NC}"
echo "Results saved in tests/ directory"
echo "Performance metrics saved in performance/metrics.csv"
echo ""
echo "Use './validate_binary.sh' to validate specific solutions"
