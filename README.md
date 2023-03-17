# Travelling Salesman Problem / Simulated Annealing in C
The following program uses the Simulated Annealing Heuristic
to find solutions to the Travelling Salesman Problem.
## Installation
Run:

```
meson setup -Dbuildtype=release build
```

and:

```
meson compile -C build/
```

in the root directory.

## Execution

Run:

```
./build/TSP_SA
```

in the root directory.

## Dependencies

```
[meson](https://www.sqlite.org/download.html)
Version: 1.0.0
```
```
[sqlite3](https://mesonbuild.com/Getting-meson.html)
Version: 3.40.1 2022-12-28
```
```
[glib](https://docs.gtk.org/glib/index.html)
```
