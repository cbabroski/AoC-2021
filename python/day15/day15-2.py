import heapq
from dataclasses import dataclass, field
from typing import Any
import numpy as np

"""
Solution based on: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
"""

# No diagonals
NEIGHBOR_MAP = [(-1, 0), (0, -1), (0, 1), (1, 0)]


@dataclass(order=True)
class Node:
    risk: int
    pos: Any = field(compare=False)


input_risks = []
with open('input.txt') as f:
    lines = f.read().splitlines()
    for row, line in enumerate(lines):
        input_risks.append([int(c) for c in line])

# For part 2 copy the first tile to create the larger cavern with
# stale risk values. Then later we can figure out the correct risks.
# TODO probably don't need to create the whole thing, just wastes time
actual_risks = np.tile(input_risks, (5, 5))

num_rows = len(actual_risks)
num_cols = len(actual_risks[0])


def get_actual_risk(row, col):
    # Take into account added risks for each tile
    tile_row = row // len(input_risks)
    tile_col = col // len(input_risks[0])
    risk = actual_risks[row][col]
    risk = risk + tile_row + tile_col
    if (risk > 9):
        risk = (risk % 10) + 1
    return risk


# Map of (row, col) -> risk level
risks = {}
risks[(0, 0)] = 0

# Maintain priority queue sorted by node risk and set of visited nodes
pq = []
visited = set()

# Mark all nodes except for the first with an infinite tentative risk level
for row, verts in enumerate(actual_risks):
    for col, v in enumerate(verts):
        if (row, col) != (0, 0):
            risks[(row, col)] = float('inf')
        heapq.heappush(pq, Node(risk=risks[(row, col)], pos=(row, col)))

# Take the lowest risk node, update its neighbors' risks and continue
# until reached the destination
while True:
    cur = heapq.heappop(pq)
    cr, cc = cur.pos
    if (cr, cc) == (num_rows - 1, num_cols - 1):
        print("found destination!")
        break

    for (dr, dc) in NEIGHBOR_MAP:
        (nr, nc) = (cr + dr, cc + dc)
        if nr < 0 or nr >= num_rows or nc < 0 or nc >= num_cols:
            continue

        if (nr, nc) in visited:
            continue

        alt = risks[(cr, cc)] + get_actual_risk(nr, nc)
        if alt < risks[(nr, nc)]:
            risks[(nr, nc)] = alt

            # Python's heapq doesn't allow for decreasing a priority, so just add
            # new risk on to the priority queue and keep track of visited nodes
            heapq.heappush(pq, Node(risk=alt, pos=(nr, nc)))

    visited.add(cur.pos)

print(risks[(num_rows - 1, num_cols - 1)])
