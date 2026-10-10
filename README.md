# OLC Game Jame "Change"

This project is my entry to the olc GameJam with the theme "Change"

This project is built with [OlcPixelGameEngine V3](https://github.com/OneLoneCoder/olcPixelGameEngine3).

## Idea

The developed idea is a idle-platformer where you need to switch/change yourself between different characters
to defeat hordes of enemies.

See [GDD](./GDD.md)

## How to build

Configure and generate Makefile

`cmake -S . -B build`

Compile

`cmake --build build`

Run

`./build/main`
or
`cmake --build build && ./build/main`

### Build for Web

Configure and generate Makefile

`emcmake cmake -S . -B build-web`
`cmake --build build-web`
`npx http-server`

At `localhost:8080/build/Main.html` the game should appear.

#### Package as tce element

The root `Makefile` wraps the web build above into a repeatable, uploadable package.

Build the web target (configures `build-web/` on first run, then always rebuilds):

`make web`

Package the build into a `tce` (Tobidot Custom Element) zip:

`make tce`

This stages `index.html`, `index.js`, `index.wasm` and `index.data` from `build-web/` into `build-tce/dist/`, then zips
them into `releases/<name>-v<version>.zip`. The archive's only top-level entry is the `dist/` folder, matching the
convention used by the other `tce-*` web elements.

The package name, kind, and version default to:

- `NAME` = `tce-change`
- `KIND` = `element`
- version = the content of the `VERSION` file (plain `major.minor.patch`, e.g. `1.0.0`)

`NAME` must stay a valid, hyphenated custom-element tag name (e.g. `tce-change`), since the Laravel app's viewer
creates a `<NAME>` HTML element directly from it - a plain word like `change` without a hyphen can never become a
real custom element in the browser.

Bump `VERSION` by hand before each release, and override `NAME`/`KIND` on the command line if needed, e.g.:

`make tce NAME=tce-change KIND=element`

To remove the staging folder and the zip for the current `NAME`/`VERSION` without touching `build-web`/`build`/`build-debug`:

`make clean-tce`

#### **Uploading the package**

The package can be uploaded either manually or automatically.

Manually, upload the generated `releases/<name>-v<version>.zip` to the `game-object-de` Laravel app via:

- The Nova admin UI, using the `UploadTobidotElement` action, or
- `POST /api/v1/tobidot-elements` with a bearer token, sending `name`, `version`, `kind` and the `zip` file.

Automatically, `make upload` (depends on `tce`) does the same `POST` for you via `curl`. It needs a Sanctum bearer
token with access to that endpoint. Store it locally (never commit it) in a gitignored `upload.env` file next to the
`Makefile`:

```
UPLOAD_TOKEN=<your-sanctum-token>
```

Then run:

`make upload`

This targets `UPLOAD_URL` (default `http://localhost/api/v1/tobidot-elements`) and can be overridden on the command
line, e.g. for the live site:

`make upload UPLOAD_URL=https://www.game-object.de/api/v1/tobidot-elements UPLOAD_TOKEN=...`

The command prints the JSON response and fails (non-zero exit code) if the HTTP status is not in the 2xx range.

## Problem Solving

Make debug build

`cmake --build build-debug && gdb ./build-debug/Main`

```text
start
next                                    // line
step                                    // into
catch throw
break -source <file.cpp> -line <XX>     // eg "break -source src/entities/PlayerBase.cpp -line 60"
continue
info stack
info locals
print <expression>
quit
```
