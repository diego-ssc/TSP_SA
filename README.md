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

### [Meson](https://www.sqlite.org/download.html)
```
meson
Version: 1.0.0
```
### [SQLITE](https://mesonbuild.com/Getting-meson.html)
```
sqlite3
Version: 3.40.1 2022-12-28
```
### [GLib](https://docs.gtk.org/glib/index.html)
```
glib
```
