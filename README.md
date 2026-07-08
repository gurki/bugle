# bugle 📯

A tag-based, filterable, colorful and thread-safe C++ logging library.

```
14:21:11.507 [7e72]  ┌ main …                    #envelope  [main main.cpp:56]
14:21:11.507 [7e72]  │ ├ hallo                   #info #debug  [main main.cpp:66]
14:21:11.507 [7e72]  │ ┌ sync …                  #envelope  [main::lambda main.cpp:76]
14:21:11.615 [7e72]  │ │ ├ go deeper             #info  [main::lambda main.cpp:78]
14:21:11.615 [7e72]  │ └ (108 ms) win            #envelope  [main::lambda main.cpp:76]
14:21:11.616 [ed40]  ┌ async …                   #envelope  [main::lambda main.cpp:76]
14:21:11.820 [ed40]  │ ├ go deeper               #info  [main::lambda main.cpp:78]
14:21:11.820 [ed40]  └ (204 ms) excellent        #envelope  [main::lambda main.cpp:76]
14:21:11.820 [7e72]  └ (635 ms) wow achieve      #envelope  [main main.cpp:56]
```


## Content

- [Motivation](#motivation)
- [Design Goals](#design-goals)
- [Integration](#integration)
- [Quickstart](#quickstart)
- [Concepts](#concepts)
- [Modules](#modules)
- [Filters](#filters)
- [Recipients](#recipients)
- [Banners](#banners)
- [Build Options](#build-options)
- [Discussion](#discussion)


## Motivation

Why another logging framework? Most libraries organize messages by severity level alone. In practice, messages have many orthogonal properties — which subsystem they come from, whether they carry data, whether they mark the start or end of some work. bugle treats **tags** as the primary organizing principle, structured **attributes** as first-class payload, and gets tracing and profiling almost for free through RAII scopes.

Enjoy and let me know what you think!
\- Gurki


## Design Goals

- **Easy integration.** Plain CMake, one link target, usable out of the box.
- **Tags.** Freeform tags instead of rigid severity levels; route messages to different sinks by tag expressions.
- **Structured attributes.** Attach arbitrary JSON key/values to any message — no more stringly-typed data in log lines.
- **Scopes.** RAII `Envelope`s give you indentation, tracing and duration profiling as side effects of logging.
- **Colors.** Colorized, aligned console output to spot relevant messages fast.
- **Thread-safe.** Messages are posted from any thread and dispatched in order by a background worker.
- **Zero-cost opt-out.** With `BUGLE_ENABLE` off, posts compile to early-out no-ops.


## Integration

```cmake
include( FetchContent )

FetchContent_Declare( bugle
    GIT_REPOSITORY https://github.com/gurki/bugle.git
    GIT_TAG master
)

FetchContent_MakeAvailable( bugle )

target_link_libraries( your-target PRIVATE bugle::bugle )
```

`bugle::bugle` is an umbrella that links the core and every enabled module. To pull in only what you use, link individual targets instead, e.g. `bugle::core` plus `bugle::json`.

bugle requires C++23 and depends on [nlohmann/json](https://github.com/nlohmann/json) (fetched automatically, or resolved via `find_package`). Enable it with `-DBUGLE_ENABLE=ON` — without it, all logging calls compile to no-ops.


## Quickstart

```cpp
#include <bugle/bugle.h>

int main()
{
    auto& po = bugle::PostOffice::instance();

    auto console = std::make_shared<bugle::ConsoleLogger>();
    po.addObserver( console );

    //  message, tags, attributes
    po.post( "new radio module connected", { "radio", "info" } );
    po.post( "incoming", { "measurement" }, {{ "temperature", 10 }} );

    //  RAII scope with indentation and duration
    {
        bugle::Envelope scope( po, "startup" );
        po.post( "loading config", { "info" } );
    }   //  logs "(x ms) startup" on destruction

    po.flush();
}
```

Every message automatically records its timestamp, thread id and `std::source_location` (file, function, line) — no macros needed.

Runnable demos live in [examples/](examples) (build with `-DBUGLE_BUILD_EXAMPLES=ON`):

- [console.cpp](examples/console.cpp) — tags and colors, inline attributes, nested envelopes across threads
- [filter.cpp](examples/filter.cpp) — the filter DSL incl. value comparisons, and programmatic predicates
- [json.cpp](examples/json.cpp) — structured jsonl logging
- [profiler.cpp](examples/profiler.cpp) — envelopes as profiling scopes, collapsed-stack output
- [sysinfo.cpp](examples/sysinfo.cpp) — build/session banners and a custom banner struct
- [gpu.cpp](examples/gpu.cpp) — gpu adapter banner (needs `BUGLE_MODULE_GPU=ON`)
- [main.cpp](examples/main.cpp) — kitchen-sink overview


## Concepts

bugle sticks to a postal metaphor throughout:

| Concept | Meaning |
| --- | --- |
| `PostOffice` | Central hub; receives letters and dispatches them to recipients on a worker thread. |
| `Letter` | A single message: text, tags, attributes, timestamp, thread and source location. |
| `Envelope` | RAII scope; posts an opening and closing letter with duration, and indents everything in between. |
| `Recipient` | Sink interface (`receive( const Letter& )`); registered as observer, held by `weak_ptr`. |
| `Filter` / `Address` / `Route` | Predicates deciding which letters a recipient receives. |

#### Messages

```cpp
po.post( "text with tags and attributes", { "info", "debug" }, {
    { "value", 5 },
    { "position", { 0.5f, 0.2f, 0.1f } }
});

po.memo( "just text" );          //  no tags, no attributes
po.card({ "checkpoint" });       //  just tags
```

#### Envelopes

```cpp
bugle::Envelope scope( po );              //  titled after the enclosing function
bugle::Envelope named( po, "upload" );    //  explicit title
named.close();                            //  or close early, before scope exit
```

Envelopes track per-thread nesting, so parallel scopes indent correctly per thread. With `BUGLE_PROFILE`, the `ENV_P()` macro posts `#profile`-tagged envelopes for consumption by the `Profiler` recipient.


## Modules

The core (post office, letters, envelopes, filters) is always built. Recipients and info providers are optional modules, each its own target:

| Target | Contents | CMake option | Default |
| --- | --- | --- | --- |
| `bugle::core` | PostOffice, Letter, Envelope, filters, timestamps | — | always |
| `bugle::console` | `ConsoleLogger`, formatters, themes, colors | `BUGLE_MODULE_CONSOLE` | ON |
| `bugle::json` | `JsonLogger` | `BUGLE_MODULE_JSON` | ON |
| `bugle::profiler` | `Profiler` | `BUGLE_MODULE_PROFILER` | ON |
| `bugle::sysinfo` | `BuildInfo`, `SessionInfo` | `BUGLE_MODULE_SYSINFO` | ON |
| `bugle::gpu` | `GpuInfo` via [webgpu](https://github.com/eliemichel/WebGPU-distribution) | `BUGLE_MODULE_GPU` | OFF |
| `bugle::bugle` | umbrella linking core + every enabled module | — | always |

All headers are always installed; disabling a module only removes its library, so umbrella includes stay valid in every configuration (you get link errors only if you instantiate something whose module is off).

The GPU module fetches the webgpu distribution (wgpu-native) and is therefore off by default, and currently available via FetchContent/add_subdirectory only (not `find_package`). Executables linking `bugle::gpu` must call `target_copy_webgpu_binaries( <your-target> )` to place the wgpu runtime library next to their binary.


## Filters

Recipients can subscribe to a subset of messages. Filters are boolean expressions over tags, message text, attributes, files and functions. Literals on one line are AND-ed, lines are OR-ed, `!` negates.

```cpp
bugle::Filter filter = bugle::Filter::fromString( R"(
    tag:info !tag:debug !attribute:value>100
    attribute:position
)" );

po.addObserver( sink, filter );
//  ((info AND NOT debug AND NOT value > 100) OR has-attribute 'position')
```

Available variables: `tag`, `message`, `file`, `function`, `attribute`.

Attributes support value comparisons with `>`, `>=`, `<`, `<=`, `=` and `!=`. Operands parse as JSON where possible (`value>100` compares numerically, `flag=true` as boolean) and fall back to strings (`name=core`).

Custom predicates can be composed programmatically, e.g. a `ValueFilter` with an arbitrary comparison over JSON values — see [examples/main.cpp](examples/main.cpp).


## Recipients

Built-in recipients:

- `ConsoleLogger` — colorized, aligned, tree-style console output; automatically strips colors when piped or redirected
- `JsonLogger` — newline-delimited JSON (one letter per line), or compact binary CBOR sequences via `open( path, Format::Cbor )`
- `Profiler` — turns envelope pairs into collapsed-stack profiling data

Threads can be given human-readable names, which structured logs pick up as `threadName`:

```cpp
bugle::PostOffice::setThreadName( "render" );   //  current thread
po.registerThread();                            //  or name it after the enclosing function
```

Custom sinks implement one virtual:

```cpp
struct MySink : bugle::Recipient {
    void receive( const bugle::Letter& letter ) override {
        //  letter.message, letter.tags, letter.attributes,
        //  letter.timestamp, letter.thread, letter.location
    }
};

auto sink = std::make_shared<MySink>();
po.addObserver( sink, bugle::Filter::fromString( "tag:radio !tag:debug" ) );
```

Recipients are held as `weak_ptr` — destroy your `shared_ptr` and the observer unregisters itself; no manual removal required.


## Banners

Letters whose attributes contain a `_title` key render as structured banners instead of inline key/values. The convention: `_title` and `_icon` describe the heading, object-valued attributes become sections (each with its own optional `_icon`), scalars become rows, and an optional `_order` array pins section order (unlisted keys follow alphabetically). Keys starting with `_` never render as data.

```cpp
struct ServerInfo {
    std::string _title = "Server";
    std::string _icon = "🌐";
    std::vector<std::string> _order = { "endpoint", "limits" };

    struct Endpoint {
        std::string _icon = "🔌";
        std::string host;
        int port;
    } endpoint;

    struct Limits {
        std::string _icon = "🚦";
        int maxConnections;
        float timeoutS;
    } limits;
};
//  + NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE per struct

po.post( {}, { "server", "system" }, nlohmann::json( info ) );
```

```
14:20:09.418 [c9d4]  ├ #server #system  [main main.cpp:54]
  ┌ 🌐 SERVER
  │ ┌ 🔌 Endpoint
  │ ├ host                : localhost
  │ └ port                : 8080
  │ ┌ 🚦 Limits
  │ ├ maxConnections      : 512
  └ └ timeoutS            : 2.50
```

`BuildInfo`, `SessionInfo` (module `bugle::sysinfo`) and `GpuInfo` (module `bugle::gpu`) are ready-made providers following this convention — but any struct (or raw JSON) in this shape renders the same way, with no coupling to bugle itself.


## Build Options

| Option | Default | Effect |
| --- | --- | --- |
| `BUGLE_ENABLE` | `OFF` | Enables logging; otherwise all calls are no-ops. |
| `BUGLE_PROFILE` | `OFF` | Enables the `ENV_P()` profiling macro. |
| `BUGLE_BUILD_EXAMPLES` | `OFF` | Builds [examples/](examples). |
| `BUGLE_BUILD_TESTS` | `OFF` | Builds Catch2 tests. |
| `BUGLE_MODULE_CONSOLE` | `ON` | Builds `bugle::console`. |
| `BUGLE_MODULE_JSON` | `ON` | Builds `bugle::json`. |
| `BUGLE_MODULE_PROFILER` | `ON` | Builds `bugle::profiler`. |
| `BUGLE_MODULE_SYSINFO` | `ON` | Builds `bugle::sysinfo`. |
| `BUGLE_MODULE_GPU` | `OFF` | Builds `bugle::gpu`; fetches webgpu. |

#### Migrating from 1.1

- `bugle` is now an umbrella `INTERFACE` target — prefer linking `bugle::bugle`, or individual modules.
- Injecting `APP_NAME`/`APP_VERSION`/`APP_COMMIT` compile definitions must now target the sysinfo module: `target_link_libraries( bugle-sysinfo PRIVATE your-metadata-target )` (guard with `if( TARGET bugle-sysinfo )`).
- The typed build/session/gpu console printers were replaced by the generic [banner](#banners) renderer; custom info structs need the `_title`/`_icon` shape to render as banners.
- `SessionInfo`/`BuildInfo` restructured into nested sections; the `system` section keys are now `name`/`version`/`architecture` (previously `systemName`/`systemVersion`/`systemArchitecture`).


## Discussion

Letters are queued and dispatched by a single background worker, keeping posting cheap on hot paths while preserving chronological order across threads. `flush()` blocks until the queue is fully drained; destruction drains the queue as well, so no messages are lost at shutdown.

Logging frameworks should be efficient and lightweight. bugle tries to be, but its priority is flexible, powerful and *readable* logging. Attributes allocate JSON per message — for extremely data-intensive logging (millions of messages per second), consider a specialized system instead.


## License

[MIT](LICENSE) © Tobias Gurdan
