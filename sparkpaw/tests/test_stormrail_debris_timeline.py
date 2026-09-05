#!/usr/bin/env python3
"""Reject locally repetitive or invalid Stormrail debris choreography."""
from pathlib import Path
import re

SOURCE = (Path(__file__).parents[1] / "src" / "game.c").read_text()


def values(name: str, ctype: str) -> list[int]:
    match = re.search(
        rf"static const {ctype} {name}\[STORMRAIL_DEBRIS_EVENT_COUNT\]=\{{(.*?)\}};",
        SOURCE,
        re.S,
    )
    assert match, name
    return [int(value) for value in re.findall(r"-?\d+", match.group(1))]


types = values("type", "UBYTE")
frames = values("frame", "UBYTE")
distances = values("distance", "ULONG")
xs = values("x", "WORD")
ys = values("y", "WORD")
dxs = values("dx", "WORD")
dys = values("dy", "WORD")
drops = values("dropId", "UBYTE")
assert all(
    len(items) == 48
    for items in (distances, types, frames, xs, ys, dxs, dys, drops)
)

widths = (48, 16, 32)
heights = (40, 16, 40)
max_base_frame = (2, 4, 0)
motion_signatures = []
for index, (kind, frame, x, y, dx, dy) in enumerate(
    zip(types, frames, xs, ys, dxs, dys)
):
    assert 0 <= kind <= 2
    assert frame % 2 == 0 and frame <= max_base_frame[kind], (index, kind, frame)
    assert 0 <= y <= 208 - heights[kind], (index, kind, y)
    assert dx != 0 or dy != 0
    assert 328 <= x <= 360 and dx < 0, (index, kind, x, dx)
    if kind == 0:
        assert dx <= -3, (index, kind, dx)
    assert -1 <= dy <= 1, (index, kind, dy)
    motion_signatures.append((y // 32, dx, -1 if dy < 0 else (1 if dy > 0 else 0)))

assert types.count(0) == 8
assert types.count(1) == 24
assert types.count(2) == 16
assert len(set(xs)) >= 4
small_medium_horizontal = [
    index for index, (kind, dy) in enumerate(zip(types, dys))
    if kind in (1, 2) and dy == 0
]
assert len(small_medium_horizontal) <= 3
assert not any(48 <= ys[index] <= 144 for index in small_medium_horizontal)
assert all(70 <= later - earlier <= 170 for earlier, later in zip(distances, distances[1:]))
big_events = [index for index, kind in enumerate(types) if kind == 0]
for earlier, later in zip(big_events, big_events[1:]):
    crossing_ticks = (xs[earlier] + widths[0]) // -dxs[earlier] + 1
    assert distances[later] - distances[earlier] >= crossing_ticks * 4
assert drops.count(32) == 1 and drops.count(33) == 1
loot_events = [index for index, drop in enumerate(drops) if drop < 64]
assert any(types[index] == 0 and dxs[index] == -3 for index in loot_events)
for start in range(43):
    assert len(set(types[start : start + 6])) >= 2, start
    assert len({signature[0] for signature in motion_signatures[start : start + 6]}) >= 3, start
    assert len(set(motion_signatures[start : start + 6])) >= 3, start
    assert not (
        types[start : start + 6] == types[start + 6 : start + 12]
        if start + 12 <= 48
        else False
    ), start

# Simulate the distance-timed admission and straight-line lifetime contract.
# This catches a globally diverse table that collapses into a homogeneous
# visible tail because some types live longer or wait for pool vacancies.
slots = []
handled = set()
visible = []
occupancy = []
visible_snapshots = []
for distance in range(distances[0], distances[-1] + 1201, 4):
    slots = [
        item
        for item in slots
        if item["age"] < 180
        and item["x"] >= -widths[item["kind"]]
        and 0 <= item["y"] <= 208 - heights[item["kind"]]
    ]
    big_active = sum(item["kind"] == 0 for item in slots)
    for event, event_distance in enumerate(distances):
        if event in handled or distance < event_distance:
            continue
        loot_carrier = drops[event] < 64
        if len(slots) < 6 and (types[event] != 0 or big_active < 1):
            slots.append(
                {
                    "kind": types[event],
                    "x": xs[event],
                    "y": ys[event],
                    "dx": dxs[event],
                    "dy": dys[event],
                    "age": 0,
                }
            )
            visible.append(event)
            handled.add(event)
            big_active += types[event] == 0
        elif not loot_carrier:
            handled.add(event)
    for item in slots:
        item["x"] += item["dx"]
        item["y"] += item["dy"]
        item["age"] += 1
    occupancy.append(len(slots))
    visible_snapshots.append(tuple(item["kind"] for item in slots))

visible_types = [types[event] for event in visible]
assert visible_types.count(0) == 8
assert visible_types.count(1) >= 21
assert visible_types.count(2) >= 14
assert all(event in visible for event in (11, 31))
assert max(occupancy) == 6
field_snapshots = visible_snapshots[: (distances[-1] - distances[0]) // 4 + 1]
busy_snapshots = [snapshot for snapshot in field_snapshots if len(snapshot) >= 3]
assert busy_snapshots
assert sum(0 in snapshot for snapshot in busy_snapshots) >= len(busy_snapshots) // 3
assert sum(2 in snapshot for snapshot in busy_snapshots) >= len(busy_snapshots) * 2 // 3
assert max(
    sum(kind == 1 for kind in snapshot) for snapshot in busy_snapshots
) <= 4
assert not visible_snapshots[-1]

print("PASS: 48-event debris timeline keeps a varied visible mix and ends empty")
