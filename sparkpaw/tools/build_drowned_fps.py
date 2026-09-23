#!/usr/bin/env python3
"""Compile the prepared full-level assets with the real Makefile's 020 flags.

Run make drowned-full first when assets need regeneration. Does not stage or
launch anything. Produces both plain controls and the matching TOD A/B pair.
"""
from pathlib import Path
import hashlib
import json
import os
import shlex
import subprocess
import argparse

R = Path(__file__).resolve().parents[1]
OUT = R / 'build/drowned-fps-round'


def main():
    global OUT
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--reset-copy', action='store_true',
                        help='compare runtime full-ring CPU vs Blitter rebuild; retain rear pointer change in both')
    parser.add_argument('--direct-water', action='store_true',
                        help='compare staged rear water with canonical-as-stage transfer')
    parser.add_argument("--enemy-bounds", action="store_true", help="compare full enemy cells against exact mask row bounds")
    parser.add_argument('--two-copy-ring', action='store_true',
                        help='compare retained baseline with rebased two-copy ring')
    options = parser.parse_args()
    assert sum((options.reset_copy, options.direct_water, options.enemy_bounds, options.two_copy_ring)) <= 1
    if options.enemy_bounds:
        OUT = R / "build/drowned-enemy-bounds"
    if options.direct_water:
        OUT = R / 'build/drowned-rear-direct'
    if options.reset_copy:
        OUT = R / 'build/drowned-fps-reset'
    if options.two_copy_ring:
        OUT = R / 'build/drowned-two-copy'
    OUT.mkdir(parents=True, exist_ok=True)
    plan = subprocess.check_output(
        ['make', '-Bn', 'drowned-full', 'PYTHON=../.venv/bin/python3'],
        cwd=R, text=True).replace('\\\n', ' ')
    line = next(line for line in plan.splitlines()
                if '+aos68k' in line and '-o build/sparkpaw-drowned-full' in line)
    args = shlex.split(line)
    pos = args.index('-o')
    del args[pos:pos + 2]
    assert '-DSPARKPAW_RENDER_DIAGNOSTIC' not in args
    # Preserve historical A/B controls after promotion into the full target.
    args = [a for a in args if a != '-DSPARKPAW_DROWNED_TWO_COPY_RING']
    if not options.two_copy_ring:
        args = [a for a in args if a != '-DSPARKPAW_DROWNED_ENEMY_BOUNDS']
    if not (options.enemy_bounds or options.two_copy_ring):
        args = [a for a in args if a != '-DSPARKPAW_DROWNED_REAR_DIRECT_WATER']
    env = dict(os.environ, VBCC=str(R / '.toolchain/sdk'))
    env['PATH'] = str(R / '.toolchain/sdk/bin') + ':' + env['PATH']
    # Bind the log to this prepared source/asset generation, not just A or B.
    inputs = set(R.glob('src/*.[ch]')) | set((R/'build/drowned-full').glob('*.h'))
    inputs.update(R/arg for arg in args if arg.endswith(('.c', '.o')))
    inputs.update((R/'build/drowned-full/assets').glob('*'))
    generation = hashlib.sha256()
    generation.update(json.dumps(args).encode())
    for path in sorted(inputs):
        if path.is_file():
            generation.update(str(path.relative_to(R)).encode())
            generation.update(path.read_bytes())
    build_id = 'fps_' + generation.hexdigest()[:20]
    proof = {}
    for variant in ('A', 'B'):
        if options.two_copy_ring:
            flags = ['-DSPARKPAW_DROWNED_TWO_COPY_TEST']
            if variant == 'B':
                flags += ['-DSPARKPAW_DROWNED_TWO_COPY_RING']
        elif options.enemy_bounds:
            flags = ['-DSPARKPAW_DROWNED_ENEMY_BOUNDS_TEST']
            if variant == 'B':
                flags += ['-DSPARKPAW_DROWNED_ENEMY_BOUNDS']
        elif options.direct_water:
            flags = ['-DSPARKPAW_DROWNED_REAR_DIRECT_TEST']
            if variant == 'B':
                flags += ['-DSPARKPAW_DROWNED_REAR_DIRECT_WATER']
        elif options.reset_copy:
            flags = ['-DSPARKPAW_DROWNED_RESET_TEST']
            if variant == 'B':
                flags += ['-DSPARKPAW_DROWNED_RESET_BLIT']
        else:
            flags = ['-DSPARKPAW_DROWNED_REAR_STAGE_REFERENCE'] if variant == 'A' else []
        for measured in (False, True):
            name = f'{variant}-' + ('cadence' if measured else 'plain')
            executable = OUT / name
            command = args + flags
            if measured:
                command += ['-DSPARKPAW_DROWNED_FPS',
                            f'-DSPARKPAW_DROWNED_FPS_BUILD_ID={build_id}_{variant}',
                            'src/drowned_fps.c']
            command += ['-o', str(executable)]
            with (OUT / f'{name}-compile.log').open('w') as log:
                subprocess.run(command, cwd=R, env=env, stdout=log,
                               stderr=subprocess.STDOUT, check=True)
            proof[name] = {'command': command, 'bytes': executable.stat().st_size,
                          'sha256': hashlib.sha256(executable.read_bytes()).hexdigest()}
            if measured:
                proof[name]['build_id'] = build_id + '_' + variant
            if measured:
                observer_args = [arg for arg in args if not arg.endswith(('.c', '.o'))]
                for unit in ('main', 'platform_amiga', 'renderer', 'drowned_fps'):
                    subprocess.run(observer_args + flags + ['-DSPARKPAW_DROWNED_FPS',
                        f'-DSPARKPAW_DROWNED_FPS_BUILD_ID={build_id}_{variant}',
                        '-S', '-o', str(OUT / f'{name}-{unit}.s'), f'src/{unit}.c'],
                        cwd=R, env=env, check=True, stdout=subprocess.DEVNULL)
        # Full translation units: inspect actual generated code, not a toy loop.
        compile_args = [arg for arg in args if not arg.endswith(('.c', '.o'))]
        for unit in ('renderer', 'main', 'platform_amiga', 'game', 'enemies',
                     'collision', 'spillwing', 'audio_mix', 'level1_audio'):
            output = OUT / f'{variant}-{unit}.s'
            subprocess.run(compile_args + flags + ['-S', '-o', str(output),
                           f'src/{unit}.c'], cwd=R, env=env, check=True,
                           stdout=subprocess.DEVNULL)
    baseline = (R/'build/drowned-fps-round/B-plain' if options.reset_copy or options.direct_water
                else OUT / 'baseline/Drowned-Test')
    if options.enemy_bounds:
        baseline = R/'build/drowned-rear-direct/B-plain'
    if options.two_copy_ring:
        baseline = R/'build/drowned-enemy-bounds/B-plain'
    if baseline.exists():
        assert proof['A-plain']['sha256'] == hashlib.sha256(baseline.read_bytes()).hexdigest(), \
            'Reference plain build is not byte-identical to the preserved played binary'
    (OUT / 'builds.json').write_text(json.dumps(proof, indent=2) + '\n')
    print(json.dumps({k: {key: v[key] for key in ('bytes', 'sha256')}
                      for k, v in proof.items()}, indent=2))


if __name__ == '__main__':
    main()
