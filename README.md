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
./build/TSP_SA [tsp-parameters] [cities-ids] [options]
```
### Parameters:

```
-t
Sets the desired temperature for the execution.
```

```
-m
Sets the desired maximum number of iterations to find a proper batch.
```

```
-l
Sets the desired batch size.
```

```
-e
Sets the desired epsilon value.
```

```
-p
Sets the desired phi value.
```

```
-a
Sets the desired average of accepted cities.
```

```
-k
Sets the desired temperature batch size.
```
### Options:

```
-s
Sets the initial seed the program will use.
```

```
-n
Sets the number of seeds the program will use.
```

```
-v
Prints the evaluation for each path the program finds.
```

```
-c
Sets the ids of the desired instance. It can be a file or a list of ids.
```

```
-f
Parses a file which contains the parameters.

```

in the root directory.
### 

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
