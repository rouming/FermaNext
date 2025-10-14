"""
===========================
Laplace Solver Comparison
===========================

Purpose:
--------

This script compares two methods for finding the value of a point
inside a 2D polygon based on the values defined on its boundaries. It
serves as a visual demonstration of how a geometric approximation
compares to a physics-based simulation.

Methods Implemented:
--------------------
1. Monte Carlo (Laplace Solver):

   - Simulates thousands of random walks from a target point. The
     final value is the average of the boundary values where the walks
     terminate.

   - This method approximates the solution to the Laplace equation,
     which models physical phenomena like steady-state heat
     distribution.

2. Geometric Interpolation:

   - Calculates the value as a weighted average based on the inverse
     distance to each boundary line. This is a fast heuristic where
     closer boundaries have more influence.

Motivation:
-----------

The script visually answers the question: "How well does a simple
geometric heuristic approximate a true physical model?" It shows how
factors like shape symmetry cause the methods to either converge or
produce significantly different results.

"""

import numpy as np
import matplotlib.pyplot as plt
import random

def cross2d(x, y):
    return x[..., 0] * y[..., 1] - x[..., 1] * y[..., 0]

class BoundaryLine:
    """
    Represents a boundary line segment with dynamic values and weighting functions.
    """
    def __init__(self, p1, p2, value_func, weight_func=None):
        """
        Initializes a line from two points and associated functions.

        Args:
            p1 (tuple): The (x, y) start point of the line segment.
            p2 (tuple): The (x, y) end point of the line segment.
            value_func (callable): A function that takes a normalized position `t` (0 to 1)
                                  and returns the value at that point on the line.
            weight_func (callable, optional): A function that takes a distance `d` 
                                             and returns a raw weight for interpolation.
        """
        self.p1 = np.array(p1)
        self.p2 = np.array(p2)
        self.value_func = value_func
        self.weight_func = weight_func
        self.line_vec = self.p2 - self.p1
        self.length_sq = np.dot(self.line_vec, self.line_vec)

    def get_projection_info(self, point):
        """Calculates the projection of a point onto this line segment."""
        point_np = np.array(point, dtype=float)
        point_vec = point_np - self.p1

        t = 0.0
        if self.length_sq > 1e-9:
            t = np.dot(point_vec, self.line_vec) / self.length_sq

        t_clamped = max(0.0, min(1.0, t))
        projected_point = self.p1 + t_clamped * self.line_vec
        distance = np.linalg.norm(point_np - projected_point)

        return distance, t_clamped

    def is_point_outside(self, point):
        """
        Checks if a point is 'outside' the half-plane defined by this line.
        Assumes a counter-clockwise ordering of polygon vertices.
        The "inside" is to the left of the vector from p1 to p2.
        """
        # Using the sign of the 2D cross product
        cross_product = cross2d(self.p2 - self.p1, np.array(point, dtype=float) - self.p1)
        return cross_product < 0

def solve_laplace_monte_carlo(start_point, boundary_lines, num_walks=10000, step_size=0.1, max_steps=20000):
    """
    Approximates the solution to the Laplace equation using random walks.
    """
    # --- Start: Input Validation ---
    is_inside = all(not line.is_point_outside(start_point) for line in boundary_lines)
    if not is_inside:
        raise ValueError("The specified start_point is outside the polygon boundary.")
    # --- End: Input Validation ---

    boundary_hits = []
    sample_paths = []

    for i in range(num_walks):
        current_point = np.array(start_point, dtype=float) # Explicitly set dtype=float
        path = [current_point.copy()]

        for step_num in range(max_steps):
            # Take a random step
            angle = random.uniform(0, 2 * np.pi)
            step = np.array([np.cos(angle), np.sin(angle)]) * step_size
            current_point += step

            # Check if the walk has hit a boundary
            hit_boundary = False
            for line in boundary_lines:
                if line.is_point_outside(current_point):
                    # Walk has exited the polygon
                    _, t = line.get_projection_info(current_point)
                    boundary_value = line.value_func(t)
                    boundary_hits.append(boundary_value)
                    hit_boundary = True
                    break

            if hit_boundary:
                if i < 10: # Store first 5 paths for visualization
                    path.append(current_point)
                    sample_paths.append(np.array(path))
                break
        else:
            # This 'else' belongs to the for loop. It runs if 'break' is not hit.
            print(f"Warning: Walk {i+1} exceeded max steps ({max_steps}) and was discarded.")
        # --- End: Loop replacement ---

    # --- Start: Safe averaging ---
    if not boundary_hits:
        print("Warning: No random walks hit the boundary. Returning 0.")
        return 0.0, sample_paths
    # --- End: Safe averaging ---

    return np.mean(boundary_hits), sample_paths

def solve_geometric_interpolation(point, boundary_lines):
    """
    Calculates an interpolated value based on distance-weighted influence.
    """
    influences = []
    total_weight = 0.0

    for line in boundary_lines:
        distance, t = line.get_projection_info(point)

        if distance < 1e-9:
            return line.value_func(t)

        if line.weight_func is None:
            raise ValueError("A weight_func must be defined for each line for interpolation.")

        raw_weight = line.weight_func(distance)
        value_at_projection = line.value_func(t)

        influences.append({'weight': raw_weight, 'value': value_at_projection})
        total_weight += raw_weight

    if total_weight < 1e-9:
        return np.mean([inf['value'] for inf in influences])

    final_value = sum(inf['weight'] * inf['value'] for inf in influences) / total_weight
    return final_value

# --- Helper and Definition Functions ---
def make_linear_gradient(start, end):
    """Factory to create a linear value function."""
    return lambda t: start * (1 - t) + end * t

def inverse_distance(d):
    """Weighting function for interpolation."""
    return 1.0 / d

def visualize_results(target_point, boundary_lines, mc_result, interp_result, paths):
    """Creates a plot to show the results."""
    fig, ax = plt.subplots(figsize=(8, 8))

    # Plot boundary lines
    for line in boundary_lines:
        ax.plot([line.p1[0], line.p2[0]], [line.p1[1], line.p2[1]], 'k-', lw=2)

    # Plot sample random walk paths
    for path in paths:
        ax.plot(path[:, 0], path[:, 1], '-', lw=0.5, alpha=0.7)

    # Plot the target point
    ax.plot(target_point[0], target_point[1], 'ro', markersize=10, label='Target Point')

    ax.set_title("Laplace Solver Comparison")
    ax.set_aspect('equal', 'box')
    ax.grid(True, linestyle='--', alpha=0.6)

    # Add text results
    text_content = (
        f"Target Point: ({target_point[0]}, {target_point[1]})\n\n"
        f"Monte Carlo (Laplace): {mc_result:.2f}\n"
        f"Geometric Interpolation: {interp_result:.2f}"
    )
    ax.text(0.05, 0.95, text_content, transform=ax.transAxes, fontsize=12,
            verticalalignment='top', bbox=dict(boxstyle='round,pad=0.5', fc='wheat', alpha=0.5))

    plt.legend()
    plt.show()

# --- Main Execution ---
if __name__ == "__main__":
    seed = 42
    random.seed(seed)
    np.random.seed(seed)

    # 1. Define the shape and boundary conditions
    p1, p2, p3, p4 = (0, 0), (10, 0), (10, 10), (0, 10)

    # Each line has a value function (gradient) and a weight function
    # (for interpolation)
    shape_lines = [
        BoundaryLine(p1, p2, value_func=make_linear_gradient(0, 0),
                     weight_func=inverse_distance),       # Bottom: Value = 0
        BoundaryLine(p2, p3, value_func=make_linear_gradient(50, 50),
                     weight_func=inverse_distance),   # Right:  Value = 50
        BoundaryLine(p3, p4, value_func=make_linear_gradient(100, 100),
                     weight_func=inverse_distance), # Top:    Value = 100
        BoundaryLine(p4, p1, value_func=make_linear_gradient(50, 50),
                     weight_func=inverse_distance),   # Left:   Value = 50
    ]

    errs = 0

    # 2. Define points to test
    for _ in range(10):
        x = np.random.randint(11)
        y = np.random.randint(11)

        target = (x, y)

        # 3. Run both solvers
        print(f"Calculate for point {target}:")
        mc_value, sample_paths = solve_laplace_monte_carlo(target, shape_lines, num_walks=100,
                                                           step_size=0.1)
        print(f"  Monte Carlo:   {mc_value:.4f}")

        interp_value = solve_geometric_interpolation(target, shape_lines)
        print(f"  Interpolation: {interp_value:.4f}")

        err = (mc_value - interp_value) / np.mean([mc_value, interp_value])
        errs += np.abs(err*100)
        print(f"  Error: {err * 100:.0f}%")

    errs /= 10
    print(f"Average error {errs:.0f}%")

    # 4. Visualize the comparison
    #visualize_results(target, shape_lines, mc_value, interp_value, sample_paths)
