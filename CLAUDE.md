# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

ChordLive is a Windows desktop viewer for chord/lyric sheets: it loads `.cls` XML song
files, renders chord lines above lyric lines, and scrolls with the keyboard. A File >
Import dialog live-parses pasted plain-text chord/lyric blocks with a preview pane.

Dormant personal project — 8 commits, last touched 2009. Pre-C++11 C++ against
wxWidgets 2.8. There is no README content beyond the title.

## Build

- **No CMake, Makefile, or package manifest exists.** The only build is
  `build\msvc\ChordLive_vc9.sln` — Visual Studio 2008, legacy `.vcproj` format, which
  modern `msbuild` cannot consume without VS's one-way upgrade.
- Requires two environment variables: `WXWIN` (wxWidgets 2.8, static Unicode `vc_lib`
  build) and `BOOST` (headers only). `SDL` is referenced by the x64 configs only and is
  vestigial.
- **Build the `Win32` configs only.** `Debug|x64` and `Release|x64` cannot build the
  current sources: they omit `$(BOOST)` from the include path and
  `wxbase28u*_xml.lib` / `wxexpat*.lib` from `AdditionalDependencies`, all of which
  `src/clsong.cpp` needs.
- Output lands in `build\msvc\binvc\<Platform><Config>\`, intermediates in
  `build\msvc\objvc\`. There is no `.gitignore`, so both appear as untracked — don't
  commit them.

## Tests, lint, CI

None of the three exist — no test framework or test target, no `.clang-format` /
`.clang-tidy` / `.editorconfig`, no CI config. There is no test command to find.
Verification is: build a Win32 config and run the GUI. The Import dialog's preview
pane doubles as the interactive harness for `CLSong::Parse`.

## Architecture

Single-window wxWidgets app. All five type clusters live flat in `src/` as `.h`/`.cpp`
pairs named `cl<role>`. Everything runs on the wx GUI thread inside wx's own event
loop — no threads, timers, or idle handler.

- `CLApp` (`clapp.cpp`) — `IMPLEMENT_APP` entry point (no hand-written `main`); creates
  `CLMain`, sizes it 640x480, shows it. That is all of startup.
- `CLMain` (`clmain.cpp`) — `wxFrame`; builds the menu bar imperatively, owns one
  `CLPanel`, opens `CLImport` modally.
- `CLPanel` (`clpanel.cpp`) — `wxPanel` owning the `CLSong`. Holds the scroll offset
  `startpos_`, turns arrow/PageUp/PageDown/Home into offset deltas (needs
  `wxWANTS_CHARS` to see arrow keys), repaints through `wxBufferedPaintDC` with an empty
  `EVT_ERASE_BACKGROUND` handler as the anti-flicker idiom.
- `CLSong` / `CLSongLine` (`clsong.cpp`) — the model, with no widget dependencies. A
  song is a `deque` of `boost::shared_ptr<CLSongLine>`, each either `LK_LINE` (a chords
  string plus the lyric string beneath it) or `LK_SPACE`. There is no chord type and no
  positional model — horizontal chord placement is literal space padding inside the
  string. Two independent input paths: `LoadFromStream` (wx XML DOM walk over `.cls`)
  and `Parse` (plain text, buffering each chord line onto the lyric line that follows).
- `CLSongDraw` (`clsong.cpp`) — the renderer and layout engine, a `friend` of `CLSong`
  reaching straight into `title_`/`artist_`/`lines_`. Constructed fresh on every paint;
  its `rect` argument is unused, so every line of every song is drawn every frame.
  Alignment depends on `wxFONTFAMILY_MODERN` being monospace — a proportional chord font
  breaks it.

No observer, binding, or dirty flag. The entire update protocol is **mutate the model
through `CLPanel::GetSong()`, then call `Refresh()` on the hosting panel** — both
`CLMain` and `CLImport` do exactly that. Flow: import text box → `CLSong::Parse` →
`CLPanel::Refresh` → `CLSongDraw::Draw`.

`CLPanel` is used twice — as the main view and as the import dialog's live preview —
so keep it context-free.

Ownership mixes three schemes on purpose: `CLSong*` is raw-owned by `CLPanel`,
`CLSongLine`s are `shared_ptr`-owned by `CLSong`, and `CLSongDraw`'s `CLSong*` is
non-owning and stack-scoped.

`.cls` format: root `<chordlivesong>` with `title`, `artist`, `keywords`, `lines`; each
`<line kind="line|space">` holds optional `<chords>` and `<lyrics>` CDATA.
`songs\Beatles - I Want to Hold Your Hand.cls` is the reference sample.

## Non-obvious traps

- **There is no SDL and no audio in this codebase**, despite `SDL.lib` in the x64
  configs and the commit "Fixed SDL hanging" (`5376242`). The project was forked from
  John Ratliff's wx-SDL tutorial; commit `87c94a9` removed SDL entirely and replaced
  the SDL blit panel with plain `wxDC` drawing. The stale x64 configs and the About box
  are the only traces left.
- **Import is unfinished.** `CLImport`'s OK button is a bare `wxID_OK` with no handler,
  so a parsed song never reaches the main window. `CLSong::SaveToStream` is an empty
  stub, which makes `SaveToFile` a silent truncate of whatever path it is given.
  Persistence is unimplemented, not broken.
- `src\clmain.cpp:41` hardcodes an absolute path to the sample song. There is no
  File > Open — that line *is* the file-open feature, so the app shows nothing from any
  other directory, and it runs unguarded in the `CLMain` constructor where a load
  failure escapes.
- `clsong.cpp` throws by pointer (`throw new CLSongException(...)`), so
  `catch (CLSongException&)` never matches. Nothing catches it today; fix the throw
  before writing a handler.
- `CLSongLine::IsChord` only checks that a token's first character is one of
  `C D E F G A B`, so lyric lines starting with those letters are misread as chord
  lines, and `IsChordLine` returns `true` for a blank line.
- `climport.cpp` declares `IMPLEMENT_CLASS(CLImport, wxFrame)` while `CLImport` derives
  from `wxDialog`, and passes `wxDEFAULT_FRAME_STYLE` to `Create`. Its ID enum also
  starts at 0 and collides with wx's reserved IDs — `CLMain`'s deliberately bases at
  10000, so follow that when adding IDs.
- `CLPanel::startpos_` is unclamped in both directions; there is no scrollbar and no
  mouse-wheel handling.
- `<keywords>` in `.cls` is parsed then discarded (`// TODO` in `clsong.cpp`).

Any port to wxWidgets 3.x has to replace `wxXmlNode::GetPropVal` (removed; now
`GetAttribute`) in `clsong.cpp`. `boost::shared_ptr` is the only Boost use, so
`std::shared_ptr` would drop the dependency entirely.

## Style to match

Tabs for indentation (the older `clapp.cpp` / `clmain.cpp` use spaces),
`H__CLSONG__H`-style include guards, trailing-underscore private members, banner
comments (`/**** ... ***/`) separating classes in `.cpp` files. Window classes pair
`DECLARE_CLASS` + `DECLARE_EVENT_TABLE` in the header with `IMPLEMENT_CLASS` +
`BEGIN_EVENT_TABLE` in the `.cpp`; `CLMain` and `CLImport` use wx's
default-ctor-then-`Create()` two-phase construction.
