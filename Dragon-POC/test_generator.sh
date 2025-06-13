
#!/bin/bash

# Test Case Generator Script for David's Dynamic Graph
# Generates comprehensive test cases covering all scenarios

echo "=== David's Dynamic Graph Test Generator ==="
echo "Generating test cases..."

# Ensure output directory exists
mkdir -p tests output

# Compile generator if not already compiled
if [ ! -f "output/generator" ]; then
    echo "Compiling generator..."
    g++ -o output/generator generator.cpp -std=c++17 -O2
    if [ $? -ne 0 ]; then
        echo "ERROR: Failed to compile generator"
        exit 1
    fi
fi

# Generate all test cases
echo "Running generator..."
./output/generator

# Generate additional edge cases
echo "Generating edge cases..."

# Minimum size graph
./output/generator 5 complete predictable
if [ -f "test_custom.in" ]; then
    mv test_custom.in tests/test_edge_min.in
fi

# Maximum size graph
./output/generator 25 sparse adaptive
if [ -f "test_custom.in" ]; then
    mv test_custom.in tests/test_edge_max.in
fi

# Special structure graphs
./output/generator 10 tree random
if [ -f "test_custom.in" ]; then
    mv test_custom.in tests/test_tree_10.in
fi

./output/generator 15 bipartite adaptive
if [ -f "test_custom.in" ]; then
    mv test_custom.in tests/test_bipartite_15.in
fi

# Count generated test cases
TEST_COUNT=$(ls tests/*.in 2>/dev/null | wc -l)
echo "Generated $TEST_COUNT test cases successfully!"

# List test cases by category
echo ""
echo "Test case breakdown:"
echo "- Small graphs (n=5-8): $(ls tests/test_0*.in tests/test_1*.in 2>/dev/null | wc -l) cases"
echo "- Medium graphs (n=9-18): $(ls tests/test_2*.in tests/test_3*.in 2>/dev/null | wc -l) cases"
echo "- Large graphs (n=19-25): $(ls tests/test_4*.in tests/test_5*.in 2>/dev/null | wc -l) cases"
echo "- Edge cases: $(ls tests/test_edge*.in tests/test_*_*.in 2>/dev/null | wc -l) cases"

echo ""
echo "Test generation completed!"
echo "Use './test_runner.sh' to run all tests"
