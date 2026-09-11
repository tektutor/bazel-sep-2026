# md2pdf — Docs-as-Code PDF Generator

Converts `README.md` files from day folders into a single styled PDF.
The PDF is a proper Bazel build output — cached and only regenerated
when a source file changes.

Tested with **Bazel 9.1.0** and **Aspect CLI 2026.x**.

---

## System requirements (one-time install)

```bash
sudo apt-get install -y libpango-1.0-0 libpangoft2-1.0-0 \
    libgdk-pixbuf2.0-0 fonts-liberation shared-mime-info
```

---

## Commands

```bash
# Build everything (compiles the py_binary, runs tests)
aspect build //...

# Run all 53 tests
aspect test //...

# ── Option A: cached PDF from local README files (recommended) ──
# Only regenerates when a README.md changes — fully cached by Bazel
bazel build //docs:training_notes
ls bazel-bin/docs/training_notes.pdf

# ── Option B: PDF from a GitHub URL (always re-runs) ──
bazel run //src:md2pdf -- \
    --source https://github.com/tektutor/bazel-may-2026.git \
    --folders day1 day2 day3 \
    --output $PWD/notes.pdf
```

---

## Project layout

```
md2pdf/
├── MODULE.bazel              # rules_python 1.7.0, pip deps
├── WORKSPACE                 # stub
├── .bazelrc                  # enable_bzlmod
├── requirements.in           # direct deps: markdown, weasyprint
├── requirements.txt          # locked deps with hashes
├── BUILD                     # root (config_setting targets)
├── tools/
│   ├── BUILD
│   └── md2pdf_rule.bzl       # CUSTOM RULE — cached PDF generation
├── src/
│   ├── md2pdf.py             # converter: markdown → HTML → PDF
│   └── BUILD                 # py_library + py_binary
├── tests/
│   ├── test_md2pdf.py        # 53 unit tests
│   └── BUILD                 # py_test
├── docs/
│   └── BUILD                 # uses md2pdf rule → training_notes.pdf
├── day1/
│   ├── README.md             # Day 1 content (edit this)
│   └── BUILD                 # exports_files(["README.md"])
├── day2/
│   ├── README.md             # Day 2 content (edit this)
│   └── BUILD
└── day3/
    ├── README.md             # Day 3 content (edit this)
    └── BUILD
```

---

## Two modes explained

### Mode A — Local README files (cached)

```python
# docs/BUILD
load("//tools:md2pdf_rule.bzl", "md2pdf")

md2pdf(
    name    = "training_notes",
    title   = "Bazel Training Notes",
    folders = "day1,day2,day3",
    srcs    = [
        "//day1:README.md",
        "//day2:README.md",
        "//day3:README.md",
    ],
)
```

Bazel tracks `day1/README.md`, `day2/README.md`, `day3/README.md` as
inputs. If none of them changed since the last build:

```
$ bazel build //docs:training_notes
INFO: Build completed successfully, 0 total actions   ← cached, no work done
```

Edit `day1/README.md` and rebuild:

```
$ bazel build //docs:training_notes
INFO: 1 process: 1 action   ← only re-ran because an input changed
```

The PDF lands at `bazel-bin/docs/training_notes.pdf`.

### Mode B — GitHub URL (always re-runs)

For a remote repo, Bazel can't know whether the remote content changed,
so `bazel run` is correct. It clones the repo fresh each time.

```bash
bazel run //src:md2pdf -- \
    --source https://github.com/tektutor/bazel-may-2026.git \
    --folders day1 day2 day3 \
    --output $PWD/notes.pdf
```

---

## Replacing the sample README files with your own content

Replace `day1/README.md`, `day2/README.md`, `day3/README.md` with your
actual training content, then run:

```bash
bazel build //docs:training_notes
open bazel-bin/docs/training_notes.pdf
```

To add a book cover, add a root `README.md` in this format:

```markdown
# Course Title (dates)

## Course designed and delivered by Your Name
#### your@email.com
#### https://www.yourwebsite.com
#### GitHub - https://github.com/yourhandle
#### LinkedIn - linkedin.com/in/yourprofile
```

The cover is parsed and rendered automatically.

---

## Regenerating requirements.txt

```bash
pip install pip-tools
pip-compile requirements.in --generate-hashes --output-file requirements.txt
aspect build //...
```
