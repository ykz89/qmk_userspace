#!/usr/bin/env python3
"""Draw keymap.svg for every ykz89 keymap with keymap-drawer.

Per board: `qmk info` for the physical layout, cpp + `qmk c2json` for the
keymap (the wrapper macros need preprocessing first), `keymap parse`, then
layer names, readable legends, layer-binding colours and thumb chords, then
`keymap draw`. Needs qmk, cpp and keymap on PATH, and PyYAML.

    tools/keymap-drawer/draw.py            # all boards
    tools/keymap-drawer/draw.py crkbd/rev1 # one board
"""
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parents[2]
CONFIG = Path(__file__).with_name('config.yaml')

# Key indices in LAYOUT order. thumbs: key held for a layer (list = chord);
# ptr: pointer-layer LT keys; chord_l/chord_r: thumb chords (ykz89_combos.c);
# qmk_home: QMK tree (relative to the userspace) when the board isn't in the default one;
# x_fix: {key index: x} corrections to the board's physical layout.
BOARDS = {
    'sporkus/le_chiffre_stm32': dict(
        keymap='keyboards/sporkus/le_chiffre_stm32/keymaps/ykz89', pointing=False,
        thumbs={'Fun': 31, 'Nav': 32, 'Media': [31, 32], 'Num': 33, 'Sym': 34},
        chord_l=[31, 32], chord_r=[33, 34]),
    'bastardkb/skeletyl/promicro': dict(
        keymap='keyboards/bastardkb/skeletyl/keymaps/ykz89', pointing=False,
        thumbs={'Media': 30, 'Nav': 31, 'Fun': 32, 'Sym': 33, 'Num': 34}),
    'bastardkb/charybdis/3x5/elitec': dict(
        keymap='keyboards/bastardkb/charybdis/3x5/keymaps/ykz89', pointing=True,
        thumbs={'Media': 30, 'Nav': 31, 'Fun': 32, 'Sym': 33, 'Num': 34}, ptr=[20, 29],
        chord_r=[33, 34]),
    'bastardkb/dilemma/3x5_2/promicro': dict(
        keymap='keyboards/bastardkb/dilemma/3x5_2/keymaps/ykz89', pointing=True,
        thumbs={'Nav': 30, 'Fun': 31, 'Media': [30, 31], 'Sym': 32, 'Num': 33}, ptr=[20, 29],
        chord_l=[30, 31], chord_r=[32, 33]),
    'bastardkb/dilemma/4x6_4': dict(
        keymap='keyboards/bastardkb/dilemma/4x6_4/keymaps/ykz89', pointing=True,
        thumbs={'Media': 49, 'Nav': 50, 'Fun': 51, 'Sym': 52, 'Num': 53}, ptr=[37, 46]),
    # Board exists only on the qmk_firmware `bastardkb` branch, so qmk runs in that worktree.
    'bastardkb/dilemma/3x5_3_trackball': dict(
        keymap='keyboards/bastardkb/dilemma/3x5_3_trackball/keymaps/ykz89', pointing=True,
        thumbs={'Media': 30, 'Nav': 31, 'Fun': 32, 'Sym': 33, 'Num': 34}, ptr=[20, 29],
        qmk_home='../qmk_firmware-bastardkb',
        # keyboard.json draws the right thumbs mirrored (x 11, 10, 9); its RGB
        # matrix and the vendor keymap put [7,1] (key 33, Enter) innermost.
        x_fix={33: 9, 35: 11}),
    'crkbd/rev1': dict(
        keymap='keyboards/crkbd/keymaps/ykz89', pointing=False,
        thumbs={'Media': 36, 'Nav': 37, 'Fun': 38, 'Sym': 39, 'Num': 40}),
}

# Layer enum order (ykz89.h); the pointer layer needs a pointing device.
LAYER_NAMES = {False: ['Base', 'Fun', 'Nav', 'Media', 'Num', 'Sym'],
               True: ['Base', 'Fun', 'Nav', 'Media', 'Ptr', 'Num', 'Sym']}
LAYER_SHORT = {'FUNCTION': 'Fun', 'NAVIGATION': 'Nav', 'MEDIA': 'Media', 'POINTER': 'Ptr', 'NUMERAL': 'Num', 'SYMBOLS': 'Sym'}
BASE_THUMB = {'Fun': 'Tab', 'Nav': 'Space', 'Media': 'Esc', 'Num': 'Bspc', 'Sym': 'Enter'}

LABEL = {
    'QK BOOT': 'Boot', 'EE CLR': 'EE Clr', 'CW TOGG': 'Caps Word', 'PSCR': 'PrtSc', 'SCRL': 'ScrLk',
    'PAUS': 'Pause', 'BSPC': 'Bspc', 'ENT': 'Enter', 'DEL': 'Del', 'INS': 'Ins', 'HOME': 'Home', 'END': 'End',
    'PGDN': 'PgDn', 'PGUP': 'PgUp', 'LEFT': '←', 'DOWN': '↓', 'UP': '↑', 'RGHT': '→', 'SPC': 'Space', 'TAB': 'Tab', 'ESC': 'Esc', 'SLSH': '/', 'Z': 'Z',
    'MPRV': '⏮', 'MNXT': '⏭', 'MPLY': '⏯', 'MSTP': '⏹', 'MUTE': 'Mute', 'VOLD': 'Vol-', 'VOLU': 'Vol+',
    'RM TOGG': 'RGB', 'RM NEXT': 'Mode', 'RM PREV': 'Mode-', 'RM HUEU': 'Hue+', 'RM HUED': 'Hue-',
    'RM SATU': 'Sat+', 'RM SATD': 'Sat-', 'RM VALU': 'Bri+', 'RM VALD': 'Bri-', 'RM SPDU': 'Spd+', 'RM SPDD': 'Spd-',
    'LGUI': 'Gui', 'RGUI': 'Gui', 'LALT': 'Alt', 'RALT': 'AltGr', 'LCTL': 'Ctrl', 'RCTL': 'Ctrl', 'LSFT': 'Shift', 'RSFT': 'Shift',
    'MS BTN1': 'Btn1', 'MS BTN2': 'Btn2', 'MS BTN3': 'Btn3', 'MS WHLU': 'Whl↑', 'MS WHLD': 'Whl↓',
    'DPI MOD': 'DPI', 'S D MOD': 'Snipe DPI', 'DRGSCRL': 'Drag Scrl', 'SNIPING': 'Snipe',
    'LM ANIM': {'t': 'LED Anim', 's': '⇧ back'},
    'TD(TD MUTE PLAY)': {'t': 'Slack Mute', 'h': 'Mic Mute', 's': '⏯ ×2'},
}
LT_RE = re.compile(r'LT\(LAYER (\w+),(.+)\)')


def legend(k):
    if isinstance(k, str):
        m = LT_RE.match(k)
        if m:
            return {'t': LABEL.get(m.group(2), m.group(2)), 'h': LAYER_SHORT[m.group(1)]}
        v = LABEL.get(k, k)
        return dict(v) if isinstance(v, dict) else v
    if isinstance(k, dict):
        for f in ('t', 'h', 's'):
            if isinstance(k.get(f), str):
                k[f] = LABEL.get(k[f], k[f])
    return k


def run(*cmd, **kw):
    return subprocess.run(cmd, check=True, text=True, capture_output=True, **kw).stdout


def draw(board, spec, work):
    kdir = ROOT / spec['keymap']
    info = work / 'info.json'
    home = ROOT / spec['qmk_home'] if 'qmk_home' in spec else None  # qmk uses the tree it runs in
    info.write_text(run('qmk', 'info', '-kb', board, '-f', 'json', cwd=home))
    if 'x_fix' in spec:
        ij = json.loads(info.read_text())
        keys = ij['layouts'][next(iter(ij['layouts']))]['layout']
        for i, x in spec['x_fix'].items():
            keys[i]['x'] = x
        info.write_text(json.dumps(ij))
    stub = work / 'stub'; stub.mkdir(exist_ok=True); (stub / 'quantum.h').touch()
    defs = ['-DRGB_MATRIX_ENABLE'] + (['-DPOINTING_DEVICE_ENABLE'] if spec['pointing'] else [])
    # Community modules the keymap loads, so keys they add (e.g. LM_ANIM) are drawn.
    kj = kdir / 'keymap.json'
    if kj.exists():
        defs += [f'-DCOMMUNITY_MODULE_{m.split("/")[-1].upper()}_ENABLE' for m in json.loads(kj.read_text()).get('modules', [])]
    cpp = ['cpp', '-P', f'-I{kdir}', f'-I{ROOT}/users/ykz89', f'-I{stub}', '-DQMK_KEYBOARD_H="quantum.h"', *defs]
    src = (kdir / 'keymap.c').read_text()

    # TUCK_L/TUCK_R (ykz89.h) are C expressions c2json can't read: ask cpp which
    # layer each tuck thumb holds, then redefine them to plain keycodes.
    probe = work / 'tuck_probe.c'
    probe.write_text(src + '\n@TUCK THUMB_TUCK_L THUMB_TUCK_R\n')
    tuck = re.findall(r'LT\(LAYER_(\w+),', run(*cpp, str(probe)).split('@TUCK')[-1])
    assert len(tuck) == 2, f'{board}: could not resolve THUMB_TUCK_L/R'
    resolve = '\n'.join(f'#undef TUCK_{s}\n#define TUCK_{s}(layer, kc) TUCK_{s}_##layer(kc)\n'
                        + ''.join(f'#define TUCK_{s}_LAYER_{l}(kc) {"kc" if l == t else "XXXXXXX"}\n' for l in LAYER_SHORT)
                        for s, t in zip('LR', tuck))
    src = src.replace('#include "ykz89.h"\n', '#include "ykz89.h"\n' + resolve, 1)
    pp = work / 'keymap_pp.c'
    pp.write_text(run(*cpp, '-', input=src))
    kjson = work / 'keymap.json'
    run('qmk', 'c2json', '--no-cpp', '-kb', board, '-km', 'ykz89', '-o', str(kjson), str(pp), cwd=home)
    d = yaml.safe_load(run('keymap', 'parse', '-c', '10', '-q', str(kjson)))

    names = LAYER_NAMES[spec['pointing']]
    layers = list(d['layers'].values())
    assert len(layers) == len(names), f'{board}: {len(layers)} layers, expected {len(names)}'
    flat = {n: [legend(k) for row in rows for k in row] for n, rows in zip(names, layers)}

    # Colour the activator on Base and the held key(s) on its layer.
    for layer, pos in spec['thumbs'].items():
        cls = layer.lower()
        for p in ([pos] if isinstance(pos, int) else pos):
            if isinstance(pos, int):  # chords are drawn as combos below
                flat['Base'][p]['type'] = cls
            flat[layer][p] = {'t': BASE_THUMB[layer] if isinstance(pos, int) else legend(flat['Base'][p])['t'], 'type': f'held {cls}'}
    for p in spec.get('ptr', []):
        flat['Base'][p]['type'] = 'ptr'
        flat['Ptr'][p] = {'t': flat['Base'][p]['t'], 'type': 'held ptr'}

    combos = []
    if 'chord_l' in spec:
        combos += [{'p': spec['chord_l'], 'k': {'t': 'Esc', 'h': 'Media'}, 'l': ['Base'], 'type': 'media'},
                   {'p': spec['chord_l'], 'k': '.', 'l': ['Num']},
                   {'p': spec['chord_l'], 'k': '(', 'l': ['Sym']}]
        if spec['pointing']:
            combos.append({'p': spec['chord_l'], 'k': 'Btn2', 'l': ['Ptr']})
    if 'chord_r' in spec:
        combos += [{'p': spec['chord_r'], 'k': 'Del', 'l': ['Base']},
                   {'p': spec['chord_r'], 'k': 'Del', 'l': ['Nav']},
                   {'p': spec['chord_r'], 'k': 'Mute', 'l': ['Media']}]
        if spec['pointing']:
            combos.append({'p': spec['chord_r'], 'k': 'Btn2', 'l': ['Ptr']})

    out = {'layout': {'qmk_keyboard': board, 'layout_name': d['layout']['layout_name']},
           'layers': {n: [keys[i:i + 10] for i in range(0, len(keys), 10)] for n, keys in flat.items()}}
    if combos:
        out['combos'] = combos
    header = (f'# Generated by tools/keymap-drawer/draw.py for {board}; edit that script, not this file.\n'
              f'# Draw by hand: qmk info -kb {board} -f json > info.json && '
              f'keymap -c tools/keymap-drawer/config.yaml draw -j info.json keymap-drawer.yaml > keymap.svg\n')
    (kdir / 'keymap-drawer.yaml').write_text(header + yaml.safe_dump(out, allow_unicode=True, sort_keys=False, default_flow_style=None, width=120))
    (kdir / 'keymap.svg').write_text(run('keymap', '-c', str(CONFIG), 'draw', '-j', str(info), str(kdir / 'keymap-drawer.yaml')))
    print(f'{board}: {kdir.relative_to(ROOT)}/keymap.svg')


if __name__ == '__main__':
    todo = sys.argv[1:] or list(BOARDS)
    with tempfile.TemporaryDirectory() as tmp:
        for b in todo:
            draw(b, BOARDS[b], Path(tmp))
