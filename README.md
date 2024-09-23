# Hardware Fingerprint (HWFP)

It started with building a no-fuss licensing system for our Computer
Vision models, software and pipelines that are to be installed on
captive hardware of various customers; and as the fate would have it
(for an extra vocal proponent of anti-licensing), I had to build one.

The first step is to fingerprint the hardware, so that licenses we
generate server-side are unique to that hardware (our demands and
restrictions are very relaxed, as reflected by what we do in this
lib).

I could not find an open source fingerprinting library that can be
used (with bindings) across different programming languages, so I
built a **very basic** (as of now) that can be easily interfaced
with `Python`, `Java`, `Rust` etc. Current implementation is _very_
**very** limited; it finds out (**only on a linux platform**, as of
now):

1. The CPU information -- as much as the modern Linux allows
2. The board information -- as much as I had the patience of gathering
3. (In future) The disk information on which this library is to be
   installed (bundled with your or our software) -- as much as modern
   Linux allows.
4. (In future) As much information as possible -- if you tell me your
   requirements and support me with the hardware
5. (In future) On AS MANY OS PLATFORMS as possible (Windows 1x is
   ready but not committed here, because I could decide upon the proper
   license -- I do not want to support Windows unless I am paid
   handsomely!)

Currently,

1. Simple `Python >= 3.7` bindings (and packaging) is available in
   this repo (with `swig`). I do not intend to provide `bdist wheels`
   because it is extremely easy to write the code in this repo, and if
   you want, you can do it yourself, in pure `Python` and `Java` and
   `xyz`.

# HOWTO

1. Clone, and

```bash

make
pip install .
```

**Ensure** that you do it all in a virtual environment, if you are
averse of adventure.

Try it:

```bash
python -c 'import hwfp; print(hwfp.get_cpu_identifier().to_dict()); print(hwfp.get_board_info().to_dict())'
```

You should try being `root` and run the same to get some more
unique info.

That's it! There are only 2 functions now.
