# Layout Part Summary 

Four main steps:

- Technology Mapping
- Placement
- Routing
- Timing Analysis



## Technology Mapping

Keywords: Standard Library, Standard Cells, Transfrom to NAND and NOT, Tree Covering, Dynamic programming, Recursive

- Tree ifying
- Recursive Matching
- Minimum Cost Covering (**Dynamic programming**)



## Placement

Keywords: Wirelength Estimation, Quadratic Wirelength, Iterative Improvement, Simulated Annealing, Linear Algebra, Recursion

- Simple **Iterative Improvement** Placement
- Iterative Improvement with Hill Climbing
- **Simulated Annealing** Placement
- Quadratic Placement (**Linear Algebra**)
- **Recursive** Partitioning



## Routing

Keywords: **Maze Routing**, Dijkstra Shortest Path, Graph

- 2-Point Nets in 1 Layer
- Multi-Point Nets
- Multi-Layer Routing
- Non-Uniform Grid Costs
- How to implement the algo (important!!!)
  - Depth First
  - Global routing




## Timing Analysis

Keywords: Delay Graph, Maze Routing, Slack, Topology sorting, Recursion

- Logic-Level Timing
  1. Build **Delay Graph**
  2. **Topology sorting**
  3. Calculate **ATs, RATs and Slacks**
  4. Get Worst Paths (**Maze Routing**)
- Interconnect Timing
  - Kind of Model
  - RC Tree / Elmore Delay
  - Walk tree to get delay (Recusive)