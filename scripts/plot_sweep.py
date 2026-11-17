#!/usr/bin/env python3

import csv
import getopt
import math
import os
import sys

try:
    import numpy as np
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.lines
    import matplotlib.pyplot as plt
except ImportError as err:
    print('plot_sweep.py needs numpy and matplotlib (' + str(err) + ')')
    sys.exit(2)


# categorical slots 1 to 3 (blue, orange, aqua), assigned to series in first-seen order
series_colors = ['#2a78d6', '#eb6834', '#1baf7a']
color_of = {}
ink = {'primary': '#0b0b0b', 'secondary': '#52514e', 'muted': '#898781', 'grid': '#e1e0d9',
       'axis': '#c3c2b7', 'surface': '#fcfcfb'}


def usage():
    print(sys.argv[0] + ' [OPTION]... CSV...')
    print('plots csv files written by bench_lookup.py --sweep and test_buildtimes.py --sweep against')
    print('variants per type, fits a function to each series and prints the fitted coefficients')
    print('    lookup ns:              a + b * log2(n)')
    print('    build cpu per type:     a + b * n + c * n^2, and a * n^k (k descriptive only; its R2 is')
    print('                            computed on the logs)')
    print('b is the cost of one binary search step only where the set of variants looked up stays the')
    print('same and hot (one variant repeated, 20 variants). Where the set grows with n (one type) or the')
    print('number of types shrinks with n (200 variants and up, all variants, misses), b and a also carry')
    print('cache and per-type costs, so compare them between runs, not as pure search and fixed costs.')
    print('Each csv side is one series; when the same side comes from several files (a before and an after')
    print('run), the file name is added to it. At most three series per chart.')
    print('    -h, --help            print this message')
    print('    -o  --output          png file (default sweep.png next to the first csv)')
    print('                            * with both lookup and build rows, two files: <name>_lookup.png and')
    print('                              <name>_build.png')
    print('\na fit needs at least two more points than it has coefficients; series with fewer are')
    print('plotted without one')


def read_rows(paths):
    rows = []
    for path in paths:
        with open(path, newline='') as f:
            for r in csv.DictReader(f):
                r['file'] = os.path.splitext(os.path.basename(path))[0]
                rows.append(r)
    sources = {}
    for r in rows:
        sources.setdefault(r['side'], set()).add(r['file'])
    for r in rows:
        r['series'] = r['side'] if len(sources[r['side']]) == 1 else r['side'] + ' (' + r['file'] + ')'
    return rows


def series(rows, kind):
    # {case: {series: (n array, value array)}}, cases and series in first-seen order
    out = {}
    for r in rows:
        if r['kind'] != kind:
            continue
        point = (int(r['variants']), float(r['value']))
        out.setdefault(r['case'], {}).setdefault(r['series'], []).append(point)
    for case in out:
        for side in out[case]:
            points = sorted(out[case][side])
            counts = [p[0] for p in points]
            if len(set(counts)) != len(counts):
                print('more than one ' + kind + ' value for one variant count in "' + case + '", ' + side
                      + ': use separate csv files for separate runs')
                sys.exit(1)
            out[case][side] = (np.array([p[0] for p in points], float), np.array([p[1] for p in points]))
    return out


def fit_poly(x, y, degree):
    # coefficients highest power first, their standard errors, and r squared; None with too few points
    if len(x) < degree + 3:
        return None
    coef, cov = np.polyfit(x, y, degree, cov=True)
    err = np.sqrt(np.diag(cov))
    residual = y - np.polyval(coef, x)
    total = y - y.mean()
    r2 = 1 - (residual @ residual) / (total @ total) if total @ total > 0 else math.nan
    return coef, err, r2


def fmt(value, err):
    return '{:.3g}'.format(value) + ' ± ' + '{:.2g}'.format(err)


def style(ax, title, xlabel, ylabel):
    ax.set_facecolor(ink['surface'])
    ax.set_title(title, color=ink['primary'], fontsize=10, loc='left')
    ax.set_xlabel(xlabel, color=ink['secondary'], fontsize=8)
    ax.set_ylabel(ylabel, color=ink['secondary'], fontsize=8)
    ax.grid(True, color=ink['grid'], linewidth=0.6)
    ax.set_axisbelow(True)
    ax.tick_params(colors=ink['muted'], labelsize=7)
    for spine in ('top', 'right'):
        ax.spines[spine].set_visible(False)
    for spine in ('left', 'bottom'):
        ax.spines[spine].set_color(ink['axis'])
    # headroom for the fit notes in the top left corner
    ax.set_ylim(0, ax.get_ylim()[1] * 1.45)


def plot_series(ax, x, y, side, joined):
    ax.plot(x, y, color=color_of[side], linewidth=2 if joined else 0, marker='o',
            markersize=5, label=side)


def plot_fit(ax, x, fx, side):
    ax.plot(x, fx, color=color_of[side], linewidth=1.5, linestyle='--')


def lookup_figure(data):
    cases = list(data)
    columns = min(4, len(cases))
    rows = math.ceil(len(cases) / columns)
    fig, axes = plt.subplots(rows, columns, figsize=(3.6 * columns, 3.0 * rows), squeeze=False)
    print('\nlookup ns = a + b * log2(variants per type)')
    width = max(len(c) for c in cases)
    for ax, case in zip(axes.flat, cases):
        notes = []
        for side, (x, y) in data[case].items():
            fit = fit_poly(np.log2(x), y, 1)
            plot_series(ax, x, y, side, fit is None)
            if fit is None:
                continue
            coef, err, r2 = fit
            smooth = np.geomspace(x.min(), x.max(), 100)
            plot_fit(ax, smooth, np.polyval(coef, np.log2(smooth)), side)
            notes.append(side + ': a ' + '{:.0f}'.format(coef[1]) + ', b ' + '{:.1f}'.format(coef[0])
                         + ', R² ' + '{:.2f}'.format(r2))
            print('  ' + case.rjust(width) + '  ' + side.ljust(8) + '  a ' + fmt(coef[1], err[1]).ljust(16)
                  + '  b ' + fmt(coef[0], err[0]).ljust(16) + '  R² ' + '{:.3f}'.format(r2))
        ax.set_xscale('log', base=2)
        ticks = sorted({n for x, _ in data[case].values() for n in x})
        ax.set_xticks(ticks)
        ax.set_xticklabels([str(int(n)) for n in ticks])
        ax.minorticks_off()
        style(ax, case, 'variants per type', 'ns per lookup')
        ax.text(0.02, 0.98, '\n'.join(notes), transform=ax.transAxes, va='top', fontsize=7,
                color=ink['secondary'])
    for ax in list(axes.flat)[len(cases):]:
        ax.set_visible(False)
    return fig


def build_figure(data):
    fig, axes = plt.subplots(1, len(data), figsize=(5.5 * len(data), 3.8), squeeze=False)
    print('\nbuild cpu seconds per type = a + b * n + c * n^2   and   a * n^k')
    for ax, case in zip(axes.flat, data):
        notes = []
        for side, (x, y) in data[case].items():
            fit = fit_poly(x, y, 2)
            plot_series(ax, x, y, side, fit is None)
            if fit is None:
                continue
            coef, err, r2 = fit
            smooth = np.linspace(x.min(), x.max(), 100)
            plot_fit(ax, smooth, np.polyval(coef, smooth), side)
            notes.append(side + ': ' + '{:.3g}'.format(coef[2]) + ' + ' + '{:.3g}'.format(coef[1]) + 'n + '
                         + '{:.3g}'.format(coef[0]) + 'n², R² ' + '{:.3f}'.format(r2))
            print('  ' + side.ljust(8) + '  a ' + fmt(coef[2], err[2]) + '  b ' + fmt(coef[1], err[1])
                  + '  c ' + fmt(coef[0], err[0]) + '  R² ' + '{:.3f}'.format(r2))
            if y.min() <= 0:
                print('  ' + side.ljust(8) + '  no power law fit: values must be above 0')
                continue
            power, power_err, power_r2 = fit_poly(np.log(x), np.log(y), 1)
            notes.append(side + ': ' + '{:.3g}'.format(math.exp(power[1])) + ' n^'
                         + '{:.2f}'.format(power[0]) + ', R² (log-log) ' + '{:.3f}'.format(power_r2))
            print('  ' + side.ljust(8) + '  a ' + '{:.3g}'.format(math.exp(power[1])) + '  k '
                  + fmt(power[0], power_err[0]) + '  R² (log-log) ' + '{:.3f}'.format(power_r2))
        style(ax, case, 'variants per type', 'cpu seconds per translation unit')
        ax.text(0.02, 0.98, '\n'.join(notes), transform=ax.transAxes, va='top', fontsize=7,
                color=ink['secondary'])
    return fig


def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'ho:', ['help', 'output='])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    output = None
    for o, a in opts:
        if o in ('-h', '--help'):
            usage()
            sys.exit()
        elif o in ('-o', '--output'):
            output = os.path.abspath(a)
        else:
            assert False, "unhandled option"
    if not args:
        usage()
        sys.exit(2)

    rows = read_rows(args)
    for r in rows:
        if r['series'] not in color_of:
            if len(color_of) == len(series_colors):
                print('more than ' + str(len(series_colors)) + ' series; plot fewer csv files at once')
                sys.exit(1)
            color_of[r['series']] = series_colors[len(color_of)]
    output = output or os.path.join(os.path.dirname(os.path.abspath(args[0])), 'sweep.png')
    figures = []
    lookup = series(rows, 'lookup')
    if lookup:
        figures.append(('lookup', lookup_figure(lookup)))
    build = series(rows, 'build')
    if build:
        figures.append(('build', build_figure(build)))
    if not figures:
        print('no lookup or build rows in ' + ', '.join(args))
        sys.exit(1)

    base, ext = os.path.splitext(output)
    print()
    for kind, fig in figures:
        fig.patch.set_facecolor(ink['surface'])
        handles, labels = fig.axes[0].get_legend_handles_labels()
        fit_key = matplotlib.lines.Line2D([], [], color=ink['secondary'], linewidth=1.5, linestyle='--')
        handles.append(fit_key)
        labels.append('fit')
        fig.legend(handles, labels, loc='upper right', fontsize=8, frameon=False)
        fig.tight_layout(rect=(0, 0, 0.93, 1))
        path = output if len(figures) == 1 else base + '_' + kind + ext
        fig.savefig(path, dpi=150, facecolor=fig.get_facecolor())
        print('wrote ' + path)


if __name__ == "__main__":
    main()
