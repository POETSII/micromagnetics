/* Devices will continue to compute until forcefully stopped. We're interested
 * in "stopping" the simulation when simulated time reaches a certain value (or
 * specifically, when the iteration counter reaches a certain value). These
 * properties are defined by the generator.
 *
 * - nodeCount: Number of runners to expect.
 * - finishLine: Where the finish line is (how many iterations) */
uint32_t nodeCount = {{v:node_count}};
uint32_t finishLine = {{v:final_iteration}};