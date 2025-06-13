
# David's Dynamic Graph - Advanced Interactive Graph Optimization Challenge

## Problem Overview

This is an advanced interactive graph optimization challenge that combines multiple algorithmic concepts:
- Multi-phase graph evolution (Discovery → Adversarial → Optimization)
- Multi-objective optimization (k-coloring, weighted domination, resource allocation)
- Strategic query management with varying costs
- Game-theoretic adversarial elements
- Dynamic graph structures

## Problem Structure

### Core Challenge
You are given a dynamic weighted graph with n vertices that evolves through 3 phases:
1. **Discovery Phase**: Learn the initial graph structure
2. **Adversarial Phase**: An adversary modifies the graph based on your queries
3. **Optimization Phase**: Solve the final multi-objective problem

### Multi-Objective Goals
Simultaneously optimize:
- **k-Coloring**: Find proper k-coloring with minimum k (k ≥ 3)
- **Weighted Domination**: Find minimum-weight dominating set
- **Resource Allocation**: Distribute limited resources optimally

### Interactive Queries (with costs)
- `STRUCTURE x` (Cost: 1) - Get neighbors and edge weights for vertex x
- `COLOR_CHECK k S` (Cost: 2) - Check if set S can be k-colored properly
- `DOMINATION_WEIGHT S` (Cost: 3) - Get total weight of dominating set S
- `DISTANCE_MATRIX S` (Cost: 5) - Get all-pairs shortest paths within set S
- `ADVERSARY_PREDICT` (Cost: 10) - Get hint about adversary's next move

## Project Structure

```
DavidsDynamicGraph/
├── README.md                    # This file
├── prompt.md                    # Original problem statement
├── requirements.json            # Problem constraints and parameters
├── solution_correct.cpp         # Reference solution (multi-objective approach)
├── solution_greedy.cpp          # Alternative greedy solution
├── solution_dp.cpp              # Dynamic programming approach
├── generator.cpp                # Test case generator
├── validator.cpp                # Interactive judge system
├── test_generator.sh            # Script to generate all test cases
├── test_runner.sh               # Script to run all tests
├── validate_binary.sh           # Binary validation script
├── clean.sh                     # Cleanup script
├── tests/                       # Generated test cases (45+ cases)
│   ├── test_01.in/.out
│   ├── test_gen_*.in/.out
│   └── ...
├── qwen/                        # Alternative solution attempts
│   ├── conversations.md
│   ├── solution_01.cpp
│   └── ...
├── output/                      # Compiled binaries
└── performance/                 # Performance analysis
    ├── metrics.csv
    └── analysis.md
```

## Constraints

- **Graph Size**: 5 ≤ n ≤ 25
- **Query Budget**: 4*n + 20 queries total
- **Resource Budget**: R = 2*n
- **Time Phases**: T₁ = n, T₂ = 2n, T₃ = 3n
- **Vertex Weights**: w[i] ∈ [1, 100]
- **Edge Weights**: e[i,j] ∈ [1, 50]
- **Capacity Constraints**: c[i] ∈ [1, 20]

## Scoring Function

```
Score = 1000 - 50*(k-χ) - 10*W_dom - 5*R_waste - 100*P_violations
```

Where:
- k = colors used, χ = chromatic number
- W_dom = weight of dominating set
- R_waste = unused resources
- P_violations = constraint violations

## Usage

### Generate Test Cases
```bash
./test_generator.sh
```

### Run All Tests
```bash
./test_runner.sh
```

### Validate Solutions
```bash
./validate_binary.sh
```

### Clean Build Files
```bash
./clean.sh
```

## Algorithm Approaches

### Reference Solution (solution_correct.cpp)
- Multi-phase strategy with adaptive query selection
- Game-theoretic adversarial prediction
- Multi-objective optimization using weighted scoring
- Dynamic programming for resource allocation

### Alternative Solutions
- **Greedy Approach**: Fast heuristic for smaller instances
- **Dynamic Programming**: Optimal for specific subproblems
- **Approximation Algorithms**: Handle NP-hard components

## Test Case Coverage

The test suite includes 45+ comprehensive test cases:
- **Small graphs** (n=5-8): All phase transitions, various structures
- **Medium graphs** (n=9-18): Complex adversarial scenarios
- **Large graphs** (n=19-25): Resource optimization challenges
- **Graph types**: Complete, tree, cycle, sparse, bipartite, random
- **Adversarial patterns**: Predictable, random, adaptive opponents

## Performance Analysis

Performance metrics are tracked for:
- Query efficiency across phases
- Multi-objective optimization quality
- Adversarial prediction accuracy
- Resource allocation optimality
- Overall scoring performance

## Educational Value

This problem develops skills in:
- Strategic algorithmic planning
- Game theory and adversarial thinking
- Multi-objective optimization
- Resource management
- Dynamic algorithm adaptation
- Interactive problem solving

---

**Problem Status**: Complete and Verified  
**Difficulty**: Advanced (2-4 hours for optimal solution)  
**Innovation Level**: High (multiple algorithmic advances)
