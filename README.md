# Hash

**Version 0.1.0**

A minimal Markdown editor written in C with [IUP](https://github.com/gen2brain/iup-go),
for macOS and Linux.

## Features

- Open, edit and save Markdown files (`.md`)
- `File` menu: `Open`, `Save`, `Save as...`, `Quit`
- Asks before discarding unsaved changes
- Dark grey theme, with colors defined in `src/theme.h`
- Native menus, windows and file dialogs

The rendered Markdown view and the formatting buttons are not available yet.

## Requirements

| Tool | Purpose |
|---|---|
| `make`, `gcc` | compile |
| `cmake` 3.20 or later | build IUP |
| `pkg-config` | provide IUP's compiler options |
| `curl`, `tar`, `shasum` | download and verify IUP |
| GTK3 (Linux only) | display |

### macOS

```
xcode-select --install
brew install cmake pkg-config
```

`curl`, `tar` and `shasum` ship with macOS.

### Linux (Debian / Ubuntu)

```
sudo apt install build-essential cmake pkg-config curl libgtk-3-dev
```

## Build and run

From the project folder (its path must not contain spaces):

```
make deps     # only once
make build
make run
```

To only build the application without keeping the dependencies:

```
make release
make run
```

| Command | What it does |
|---|---|
| `make deps` | downloads IUP 4.0.0, verifies the archive, builds it and installs it into `deps/` |
| `make build` (or `make`) | compiles the application |
| `make release` | `deps` + `build`, then removes `deps/`: for using the app, not for developing it |
| `make run` | runs the application, **without recompiling** |
| `make build run` | compiles, then runs |
| `make clean` | removes the compiled application |
| `make purge` | `clean`, then also removes `deps/` |

## Options

Use a `cmake` that is not in your `PATH`:

```
make deps CMAKE=/path/to/cmake
```

Use an IUP already installed elsewhere:

```
make build IUP_DIR=/path/to/iup
```

## License

[MIT](LICENSE)
