#!/usr/bin/env python3
"""
md2pdf.py — convert README.md files from day folders into a single PDF.

The root README.md is parsed for author/course metadata and rendered
as a book cover page. Day folder README files become the content chapters.

Usage:
    bazel run //src:md2pdf -- --source /path/to/repo --output $PWD/notes.pdf
    bazel run //src:md2pdf -- --source https://github.com/user/repo --output $PWD/notes.pdf
    bazel run //src:md2pdf -- --source /path/to/repo \\
        --folders day1 day2 day3 --output $PWD/notes.pdf
"""

import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import markdown
from weasyprint import HTML

# ── Stylesheet ────────────────────────────────────────────────────────────────

STYLESHEET = """
@page {
    margin: 2cm 2.5cm;
    @bottom-right {
        content: counter(page) " / " counter(pages);
        font-size: 9pt;
        color: #888;
    }
}

/* ── Cover page ── */
.cover-page {
    page-break-after: always;
    background: #1a1a2e;
    min-height: 26cm;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    text-align: center;
    padding: 3cm 2cm;
    margin: -2cm -2.5cm;   /* bleed to page edge */
    box-sizing: border-box;
}
.cover-title {
    font-family: "DejaVu Sans", Arial, sans-serif;
    font-size: 32pt;
    font-weight: bold;
    color: #ffffff;
    margin: 0 0 0.3em 0;
    line-height: 1.2;
}
.cover-dates {
    font-family: "DejaVu Sans", Arial, sans-serif;
    font-size: 14pt;
    color: #a0c4ff;
    margin: 0 0 2em 0;
}
.cover-divider {
    width: 80px;
    height: 4px;
    background: #a0c4ff;
    margin: 0 auto 2em auto;
    border: none;
}
.cover-subtitle {
    font-family: "DejaVu Sans", Arial, sans-serif;
    font-size: 13pt;
    color: #e0e0e0;
    margin: 0 0 2.5em 0;
    font-style: italic;
}
.cover-author-block {
    background: rgba(255,255,255,0.07);
    border-radius: 8px;
    padding: 1.2em 2em;
    margin-top: 1em;
    min-width: 14cm;
}
.cover-author-name {
    font-family: "DejaVu Sans", Arial, sans-serif;
    font-size: 15pt;
    color: #ffffff;
    font-weight: bold;
    margin: 0 0 0.6em 0;
}
.cover-meta {
    font-family: "DejaVu Sans Mono", monospace;
    font-size: 10pt;
    color: #b0c8e8;
    line-height: 1.9;
}
.cover-meta a { color: #a0c4ff; text-decoration: none; }

/* ── Body ── */
body {
    font-family: "DejaVu Sans", "Liberation Sans", Arial, sans-serif;
    font-size: 11pt;
    line-height: 1.7;
    color: #1a1a1a;
}
h1 {
    font-size: 20pt;
    color: #1a1a2e;
    border-bottom: 3px solid #1a1a2e;
    padding-bottom: 8px;
    margin-top: 0;
    page-break-before: always;
}
h1.first-heading { page-break-before: avoid; }
h2 {
    font-size: 14pt;
    color: #16213e;
    margin-top: 1.6em;
    border-bottom: 1px solid #ddd;
    padding-bottom: 4px;
}
h3 { font-size: 12pt; color: #0f3460; margin-top: 1.2em; }
code {
    background: #f4f4f4;
    padding: 2px 6px;
    border-radius: 3px;
    font-family: "DejaVu Sans Mono", "Liberation Mono", monospace;
    font-size: 0.88em;
    color: #c0392b;
}
pre {
    background: #f8f8f8;
    border: 1px solid #e0e0e0;
    border-left: 4px solid #1a1a2e;
    padding: 1em 1.2em;
    border-radius: 4px;
    font-size: 0.87em;
    line-height: 1.5;
    white-space: pre-wrap;
    word-wrap: break-word;
}
pre code { background: none; padding: 0; color: inherit; border-radius: 0; }
blockquote {
    border-left: 4px solid #ccc;
    margin: 1em 0;
    padding: 0.5em 1em;
    color: #555;
    background: #fafafa;
}
table { border-collapse: collapse; width: 100%; margin: 1em 0; }
th, td { border: 1px solid #ddd; padding: 8px 12px; text-align: left; }
th { background: #1a1a2e; color: white; }
tr:nth-child(even) { background: #f9f9f9; }
hr { border: none; border-top: 1px solid #ddd; margin: 2em 0; }
a { color: #0f3460; }
ul, ol { margin: 0.5em 0; padding-left: 2em; }
li { margin: 0.3em 0; }
img {
    max-width: 100% !important;
    height: auto !important;
    display: block;
    margin: 1em auto;
}
"""

# ── Cover page parser ─────────────────────────────────────────────────────────

def parse_cover_metadata(md_text):
    """
    Parse the root README.md for course/author metadata.

    Expected format (tektutor style):
        # Course Title (dates)
        ## Course designed and delivered by Author Name
        #### email@domain.com
        #### https://www.website.com
        #### GitHub - https://github.com/user
        #### LinkedIn - linkedin.com/in/profile
    
    Returns a dict with keys: title, dates, subtitle, email,
    website, github, linkedin. Missing fields are empty strings.
    """
    meta = {
        'title':    '',
        'dates':    '',
        'subtitle': '',
        'email':    '',
        'website':  '',
        'github':   '',
        'linkedin': '',
    }

    # Title — first # heading; dates in parens extracted separately
    m = re.search(r'^#\s+(.+)$', md_text, re.MULTILINE)
    if m:
        full = m.group(1).strip()
        date_m = re.search(r'\(([^)]+)\)', full)
        if date_m:
            meta['title'] = full[:date_m.start()].strip()
            meta['dates'] = date_m.group(1).strip()
        else:
            meta['title'] = full

    # Subtitle — first ## heading
    m = re.search(r'^##\s+(.+)$', md_text, re.MULTILINE)
    if m:
        meta['subtitle'] = m.group(1).strip()

    # Email
    m = re.search(r'####\s+([\w.+-]+@[\w.-]+)', md_text)
    if m:
        meta['email'] = m.group(1).strip()

    # Website (https://www....)
    m = re.search(r'####\s+(https?://www\.[^\s\n]+)', md_text)
    if m:
        meta['website'] = m.group(1).strip()

    # GitHub
    m = re.search(r'[Gg]it[Hh]ub\s*[-–]\s*(https?://[^\s\n]+)', md_text)
    if m:
        meta['github'] = m.group(1).strip()

    # LinkedIn
    m = re.search(r'[Ll]inked[Ii]n\s*[-–]\s*([^\s\n]+)', md_text)
    if m:
        meta['linkedin'] = m.group(1).strip()

    return meta


def build_cover_html(meta):
    """Render a book cover page from parsed metadata."""

    # Author name extracted from subtitle "Course designed and delivered by <Name>"
    author_name = ''
    m = re.search(r'\bby\s+(.+)$', meta['subtitle'], re.IGNORECASE)
    if m:
        author_name = m.group(1).strip()

    meta_lines = []
    if meta['email']:
        meta_lines.append('<a href="mailto:{0}">{0}</a>'.format(meta['email']))
    if meta['website']:
        meta_lines.append('<a href="{0}">{0}</a>'.format(meta['website']))
    if meta['github']:
        meta_lines.append('GitHub: <a href="{0}">{0}</a>'.format(meta['github']))
    if meta['linkedin']:
        meta_lines.append('LinkedIn: {}'.format(meta['linkedin']))

    return """
<div class="cover-page">
  <div class="cover-title">{title}</div>
  {dates_html}
  <div class="cover-divider"></div>
  <div class="cover-subtitle">{subtitle}</div>
  <div class="cover-author-block">
    {author_html}
    <div class="cover-meta">
      {meta_lines}
    </div>
  </div>
</div>
""".format(
        title       = meta['title'] or 'Training Notes',
        dates_html  = '<div class="cover-dates">{}</div>'.format(meta['dates'])
                      if meta['dates'] else '',
        subtitle    = meta['subtitle'],
        author_html = '<div class="cover-author-name">{}</div>'.format(author_name)
                      if author_name else '',
        meta_lines  = '<br>'.join(meta_lines),
    )


# ── Image helpers ─────────────────────────────────────────────────────────────

def fix_img_dimensions(html):
    """Remove fixed pixel width/height from img tags so CSS max-width applies."""
    def replace_img(m):
        tag = m.group(1)
        tag = re.sub(r'\s*width="\d+"', '', tag)
        tag = re.sub(r'\s*height="\d+"', '', tag)
        return '<img {}>'.format(tag.strip())
    return re.sub(r'<img\s([^>]+?)(?:\s*/)?>', replace_img, html)


def fix_local_image_paths(html, folder_path):
    """Convert relative img src paths to absolute file:// URIs."""
    def replace_src(m):
        src = m.group(1)
        if (src.startswith("http://") or src.startswith("https://")
                or src.startswith("data:") or src.startswith("file://")):
            return m.group(0)
        abs_path = os.path.abspath(os.path.join(folder_path, src))
        return 'src="file://{}"'.format(abs_path)
    return re.sub(r'src="([^"]*)"', replace_src, html)


# ── Core helpers ──────────────────────────────────────────────────────────────

def is_github_url(source):
    return (source.startswith("https://github.com")
            or source.startswith("http://github.com")
            or source.startswith("git@github.com"))


def clone_repo(url, dest):
    print("Cloning {} ...".format(url))
    result = subprocess.run(
        ["git", "clone", "--depth=1", url, dest],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        print("git clone failed:\n" + result.stderr, file=sys.stderr)
        sys.exit(1)
    print("Cloned to {}".format(dest))


def find_readme(folder):
    for name in ("README.md", "readme.md", "Readme.md", "README.MD"):
        candidate = Path(folder) / name
        if candidate.exists():
            return candidate
    return None


def md_to_html_fragment(md_text, folder_path):
    """Convert markdown to HTML with image dimension and path fixes."""
    html = markdown.markdown(
        md_text,
        extensions=["fenced_code", "tables", "toc", "nl2br", "md_in_html"],
    )
    html = re.sub(r"<h1([^>]*)>", r'<h1\1 class="first-heading">', html, count=1)
    html = fix_img_dimensions(html)
    html = fix_local_image_paths(html, str(folder_path))
    return html


# ── Main pipeline ─────────────────────────────────────────────────────────────

def build_pdf(source, folders, output, title):
    tmpdir    = None
    repo_path = source

    try:
        if is_github_url(source):
            tmpdir = tempfile.mkdtemp(prefix="md2pdf_")
            clone_repo(source, tmpdir)
            repo_path = tmpdir

        repo = Path(repo_path)
        if not repo.exists():
            print("Source path does not exist: {}".format(repo_path), file=sys.stderr)
            sys.exit(1)

        # ── Cover: parse root README if present ──────────────────────────────
        root_readme = find_readme(repo)
        if root_readme:
            print("  Reading cover metadata from {} ...".format(
                root_readme.relative_to(repo)))
            meta = parse_cover_metadata(root_readme.read_text(encoding="utf-8"))
            # Use parsed title as document title if caller didn't override it
            if title == "Training Notes" and meta['title']:
                title = meta['title']
        else:
            meta = {}

        cover_html = build_cover_html(meta) if meta else """
<div class="cover-page">
  <div class="cover-title">{}</div>
</div>""".format(title)

        # ── Content: one section per day folder ──────────────────────────────
        sections = []
        missing  = []

        for folder_name in folders:
            folder = repo / folder_name
            if not folder.is_dir():
                missing.append(folder_name)
                continue
            readme = find_readme(folder)
            if readme is None:
                missing.append("{} (no README.md)".format(folder_name))
                continue
            print("  Reading {} ...".format(readme.relative_to(repo)))
            sections.append(
                (folder_name, readme.read_text(encoding="utf-8"), folder)
            )

        if not sections:
            print("No README.md files found in: {}".format(folders), file=sys.stderr)
            sys.exit(1)

        if missing:
            print("Warning: skipped: {}".format(", ".join(missing)))

        # ── Assemble HTML ─────────────────────────────────────────────────────
        parts = [cover_html]
        for folder_name, md_text, folder_path in sections:
            parts.append(md_to_html_fragment(md_text, folder_path))

        html = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>{title}</title>
  <style>{css}</style>
</head>
<body>
{body}
</body>
</html>""".format(title=title, css=STYLESHEET, body="\n".join(parts))

        # ── Render PDF ────────────────────────────────────────────────────────
        print("Generating PDF -> {} ...".format(output))
        HTML(string=html).write_pdf(output)
        size_kb = os.path.getsize(output) // 1024
        print("Done: {} ({} KB, {} sections)".format(
            output, size_kb, len(sections)))

    finally:
        if tmpdir and os.path.exists(tmpdir):
            shutil.rmtree(tmpdir)


# ── CLI ───────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="Convert README.md files from day folders into a single PDF",
    )
    parser.add_argument("--source",  required=True,
                        help="Local repo path or GitHub URL")
    parser.add_argument("--output",  required=True,
                        help="Output PDF path — use $PWD/notes.pdf for current dir")
    parser.add_argument("--folders", nargs="+", default=["day1", "day2", "day3"],
                        help="Folder names: space-separated (day1 day2 day3) or "
                             "comma-separated single arg (day1,day2,day3) for rule use")
    parser.add_argument("--title",   default="Training Notes",
                        help="PDF title (overrides title from root README)")
    args = parser.parse_args()

    print("Source:  {}".format(args.source))
    print("Folders: {}".format(args.folders))
    print("Output:  {}".format(args.output))
    # Accept both "day1 day2 day3" (CLI) and "day1,day2,day3" (rule invocation)
    folders = args.folders
    if len(folders) == 1 and "," in folders[0]:
        folders = [f.strip() for f in folders[0].split(",")]
    build_pdf(args.source, folders, args.output, args.title)


if __name__ == "__main__":
    main()
