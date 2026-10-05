#!/usr/bin/env python3
"""Simulate the Dilemma's left-half LED matrix and render it as animated GIFs.

Builds sim.c against the real modules/bastardkb/bk_led_matrix drawing code and the
trackball keymap's layer callbacks, then renders each scenario. Needs gcc and Pillow.

    tools/led-matrix-sim/run.py                      # every scenario
    tools/led-matrix-sim/run.py roll_right circle    # just these
    tools/led-matrix-sim/run.py -D LED_MATRIX_MODULE_OCEAN_LENGTH=6   # try a setting
    tools/led-matrix-sim/run.py --style ocean roll_right   # a trackball animation by name
    tools/led-matrix-sim/run.py --compare     # every animation on the trackball scenarios
    tools/led-matrix-sim/run.py --docs        # the module README's GIFs

Output: tools/led-matrix-sim/out/<scenario>[-<style>].gif and index.html.
-D overrides any LED_MATRIX_MODULE_* setting in post_config.h, to tune before reflashing.
"""
import argparse
import re
import struct
import subprocess
import sys
from pathlib import Path

from PIL import Image, ImageDraw

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
MODULE = ROOT / 'modules/bastardkb/bk_led_matrix'
KEYMAP = ROOT / 'keyboards/bastardkb/dilemma/3x5_3_trackball/keymaps/ykz89/keymap.c'
QMK = ROOT.parent / 'qmk_firmware-bastardkb'  # the `bastardkb` worktree, as in draw.py
REAL_MODES = ROOT / 'modules/bastardkb/bk_pointing_device/bk_pointing_modes.h'
OUT = HERE / 'out'

CELL, GAP, PAD = 22, 4, 12        # LED size, gap between LEDs, border, in pixels
BG, OFF = (12, 12, 14), (30, 30, 34)  # panel background, an unlit LED


def modes(path):
    return dict(re.findall(r'(MODE_\w+)\s*=\s*(\d+)', path.read_text()))


def build(defines):
    """Compile the simulator; returns the path of the binary."""
    if modes(HERE / 'stub/bk_pointing_modes.h') != modes(REAL_MODES):
        sys.exit(f'stub/bk_pointing_modes.h no longer matches {REAL_MODES.relative_to(ROOT)}; update the stub')

    # Copy the keymap's layer-name, layer-animation and (if any) custom animation
    # callbacks into the build.
    src = KEYMAP.read_text()
    funcs = []
    for sig, required in ((r'const char \*bklm_layer_name_user\(', True), (r'uint8_t bklm_layer_anim_user\(', True),
                          (r'bool bklm_motion_user\(', False)):
        f = re.search(r'^(' + sig + r'.*?^\})', src, re.S | re.M)
        if f:
            funcs.append(f.group(1))
        elif required:
            sys.exit(f'no {sig[:-2].split()[-1]} in {KEYMAP.relative_to(ROOT)}')
    gen = HERE / 'out/build'
    gen.mkdir(parents=True, exist_ok=True)
    (gen / 'layer_names.c').write_text(f'// Generated from {KEYMAP.relative_to(ROOT)} by run.py.\n'
                                       '#include "ykz89.h"\n#include "led_matrix_layer_anims.h"\n#include "led_matrix_motion.h"\n' + '\n'.join(funcs) + '\n')

    sources = [HERE / 'sim.c', gen / 'layer_names.c', QMK / 'quantum/color.c']
    sources += [p for p in sorted(MODULE.glob('*.c')) if p.name != 'led_matrix_display.c']
    binary = gen / 'sim'
    cmd = ['gcc', '-std=gnu11', '-O1', '-Wall', '-Wno-unused-parameter', '-o', str(binary),
           f'-I{HERE}/stub', f'-I{MODULE}', f'-I{ROOT}/users/ykz89', f'-I{QMK}/quantum', f'-I{QMK}/platforms',
           '-DQMK_KEYBOARD_H="quantum.h"', '-DPOINTING_DEVICE_ENABLE',
           '-DCOMMUNITY_MODULE_BK_POINTING_DEVICE_ENABLE', '-DCOMMUNITY_MODULE_ARGOS_ENABLE',
           '-DCOMMUNITY_MODULE_BK_LED_MATRIX_ENABLE', '-DLED_MATRIX_MODULE_PIN=12',
           *[f'-D{d}' for d in defines], '-include', str(MODULE / 'post_config.h'), *map(str, sources)]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stderr)
    if r.stderr:
        print(r.stderr, file=sys.stderr)
    return binary


def read_frames(path):
    data = path.read_bytes()
    assert data[:4] == b'BKLM', path
    cols, rows = struct.unpack_from('<HH', data, 4)
    size, frames, off = cols * rows * 3, [], 8
    while off < len(data):
        (t,) = struct.unpack_from('<I', data, off)
        frames.append((t, data[off + 4:off + 4 + size]))
        off += 4 + size
    return cols, rows, frames


# --docs: the module README's GIFs as (file, scenario, style, from ms, to ms).
DOCS = [
    ('sparks', 'circle', 'sparks', 300, 3800), ('fireworks', 'roll_then_rest', 'fireworks', 300, 3300),
    ('ocean', 'circle', 'ocean', 300, 3800), ('asteroids', 'circle', 'asteroids', 300, 4300),
    ('matrix', 'idle_long', 'matrix', 0, 4000), ('tetris', 'idle_30s', 'tetris', 0, 13700),
    *[(f'layer_{l}', f'layer_{l}', None, 300, 2800) for l in ('fun', 'nav', 'media', 'ptr', 'num', 'sym')],
]
DOCS_DIR = MODULE / 'docs'


def render(cols, rows, pixels, cell=CELL, gap=GAP, pad=PAD):
    w, h = 2 * pad + cols * cell + (cols - 1) * gap, 2 * pad + rows * cell + (rows - 1) * gap
    img = Image.new('RGB', (w, h), BG)
    d = ImageDraw.Draw(img)
    for y in range(rows):
        for x in range(cols):
            i = (y * cols + x) * 3
            rgb = tuple(pixels[i:i + 3])
            x0, y0 = pad + x * (cell + gap), pad + y * (cell + gap)
            d.rounded_rectangle([x0, y0, x0 + cell - 1, y0 + cell - 1], radius=cell // 4, fill=rgb if any(rgb) else OFF)
    return img


def gif(name, cols, rows, frames, out=OUT, **size):
    """One GIF frame per painted frame, each shown until the next one was painted."""
    images, durations = [], []
    for k, (t, px) in enumerate(frames):
        nxt = frames[k + 1][0] if k + 1 < len(frames) else t + 500
        images.append(render(cols, rows, px, **size))
        durations.append(max(20, nxt - t))  # browsers clamp GIF frames below 20 ms
    path = out / f'{name}.gif'
    images[0].save(path, save_all=True, append_images=images[1:], duration=durations, loop=0, disposal=1)
    return path


def index(rows, defines):
    """rows: [(heading, [(gif name, caption)])], one row of tiles each."""
    tiles = '\n'.join(
        (f'<h2>{heading}</h2>' if heading else '') + '<main>' +
        ''.join(f'<figure><img src="{g}.gif" alt="{c}"><figcaption>{c}</figcaption></figure>' for g, c in items) + '</main>'
        for heading, items in rows)
    settings = ', '.join(defines) or 'module defaults'
    (OUT / 'index.html').write_text(f'''<!doctype html>
<meta charset="utf-8"><title>LED matrix simulator</title>
<style>
  body {{ background:#18181b; color:#d4d4d8; font:14px system-ui, sans-serif; margin:24px; }}
  main {{ display:flex; flex-wrap:wrap; gap:24px; margin-bottom:28px; }}
  h2 {{ font-size:16px; margin:8px 0 12px; text-transform:capitalize; }}
  figure {{ margin:0; text-align:center; }}
  img {{ height:420px; image-rendering:pixelated; border-radius:10px; }}
  p {{ color:#a1a1aa; }}
</style>
<h1>Dilemma LED matrix</h1>
<p>Real drawing code from modules/bastardkb/bk_led_matrix, simulated. Settings: {settings}.
Layer colours are stand-ins (the keyboard reads them from Argos).</p>
{tiles}
''')


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('scenarios', nargs='*', help='scenario names (default: all)')
    ap.add_argument('-D', dest='defines', action='append', default=[], metavar='NAME=VALUE',
                    help='override a LED_MATRIX_MODULE_* setting')
    ap.add_argument('--style', action='append', default=[], help='trackball animation(s) to use (default: the module\'s)')
    ap.add_argument('--compare', action='store_true', help='every animation, on the trackball scenarios')
    ap.add_argument('--docs', action='store_true', help=f'write the README GIFs to {DOCS_DIR.relative_to(ROOT)}')
    args = ap.parse_args()

    binary = build(args.defines)
    if args.docs:
        DOCS_DIR.mkdir(exist_ok=True)
        for name, scenario, style, t0, t1 in DOCS:
            raw = OUT / 'build' / f'docs-{name}.bin'
            subprocess.run([str(binary), scenario, str(raw)] + ([style] if style else []), check=True, capture_output=True)
            cols, nrows, frames = read_frames(raw)
            frames = [f for f in frames if t0 <= f[0] - 1000 < t1]
            path = gif(name, cols, nrows, frames, out=DOCS_DIR, cell=10, gap=2, pad=6)
            print(f'{path.relative_to(ROOT)}: {len(frames)} frames, {path.stat().st_size // 1024} KB')
        return
    ask = lambda flag: subprocess.run([str(binary), flag], capture_output=True, text=True, check=True).stdout.split()
    known, styles = ask('--list'), ask('--styles')
    todo = args.scenarios or ([s for s in known if s.startswith(('roll', 'slow', 'circle'))] if args.compare else known)
    chosen = styles if args.compare else (args.style or [None])
    bad = [s for s in todo if s not in known] + [s for s in chosen if s and s not in styles]
    if bad:
        sys.exit(f'unknown: {", ".join(bad)}; scenarios: {", ".join(known)}; styles: {", ".join(styles)}')

    rows = []
    for style in chosen:
        items = []
        for name in todo:
            tag = f'{name}-{style}' if style else name
            raw = OUT / 'build' / f'{tag}.bin'
            subprocess.run([str(binary), name, str(raw)] + ([style] if style else []), check=True, capture_output=True)
            cols, nrows, frames = read_frames(raw)
            path = gif(tag, cols, nrows, frames)
            items.append((tag, name.replace('_', ' ')))
            print(f'{tag}: {len(frames)} frames -> {path.relative_to(ROOT)}')
        rows.append((style, items))
    index(rows, args.defines)
    print(f'open {(OUT / "index.html").relative_to(ROOT)}')


if __name__ == '__main__':
    main()
