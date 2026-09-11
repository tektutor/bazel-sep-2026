"""Unit tests for md2pdf.py"""

import os
import sys
import tempfile
import unittest
from pathlib import Path

try:
    from src import md2pdf
except ImportError:
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
    from src import md2pdf


# ── is_github_url ─────────────────────────────────────────────────────────────

class TestIsGithubUrl(unittest.TestCase):

    def test_https_url(self):
        self.assertTrue(md2pdf.is_github_url("https://github.com/user/repo"))

    def test_http_url(self):
        self.assertTrue(md2pdf.is_github_url("http://github.com/user/repo"))

    def test_ssh_url(self):
        self.assertTrue(md2pdf.is_github_url("git@github.com:user/repo.git"))

    def test_local_path_is_not_github(self):
        self.assertFalse(md2pdf.is_github_url("/home/user/myrepo"))

    def test_gitlab_is_not_github(self):
        self.assertFalse(md2pdf.is_github_url("https://gitlab.com/user/repo"))

    def test_empty_string(self):
        self.assertFalse(md2pdf.is_github_url(""))


# ── find_readme ───────────────────────────────────────────────────────────────

class TestFindReadme(unittest.TestCase):

    def setUp(self):
        self.tmpdir = Path(tempfile.mkdtemp())

    def tearDown(self):
        import shutil; shutil.rmtree(self.tmpdir)

    def test_finds_standard_readme(self):
        (self.tmpdir / "README.md").write_text("# Hello")
        self.assertIsNotNone(md2pdf.find_readme(self.tmpdir))

    def test_finds_lowercase_readme(self):
        (self.tmpdir / "readme.md").write_text("# Hello")
        self.assertIsNotNone(md2pdf.find_readme(self.tmpdir))

    def test_returns_none_when_missing(self):
        self.assertIsNone(md2pdf.find_readme(self.tmpdir))

    def test_returns_none_for_txt_extension(self):
        (self.tmpdir / "README.txt").write_text("# Hello")
        self.assertIsNone(md2pdf.find_readme(self.tmpdir))

    def test_returns_path_object(self):
        (self.tmpdir / "README.md").write_text("# Hello")
        self.assertIsInstance(md2pdf.find_readme(self.tmpdir), Path)

    def test_returns_correct_path(self):
        readme = self.tmpdir / "README.md"
        readme.write_text("# Hello")
        self.assertEqual(md2pdf.find_readme(self.tmpdir), readme)


# ── fix_img_dimensions ────────────────────────────────────────────────────────

class TestFixImgDimensions(unittest.TestCase):

    def test_removes_1920x1200(self):
        html = '<img width="1920" height="1200" alt="img" src="https://example.com/a.png" />'
        result = md2pdf.fix_img_dimensions(html)
        self.assertNotIn('width="1920"', result)
        self.assertNotIn('height="1200"', result)

    def test_preserves_src(self):
        html = '<img width="1920" height="1200" src="https://example.com/a.png" />'
        result = md2pdf.fix_img_dimensions(html)
        self.assertIn('src="https://example.com/a.png"', result)

    def test_preserves_alt(self):
        html = '<img width="1920" height="1200" alt="screenshot" src="a.png" />'
        result = md2pdf.fix_img_dimensions(html)
        self.assertIn('alt="screenshot"', result)

    def test_preserves_percentage_width(self):
        html = '<img width="50%" height="50%" src="diagram.svg">'
        result = md2pdf.fix_img_dimensions(html)
        self.assertIn('width="50%"', result)

    def test_handles_alt_before_width(self):
        html = '<img alt="image" width="1920" height="1200" src="a.png" />'
        result = md2pdf.fix_img_dimensions(html)
        self.assertNotIn('width="1920"', result)
        self.assertNotIn('height="1200"', result)

    def test_img_tag_preserved(self):
        html = '<img width="800" height="600" src="a.png" />'
        result = md2pdf.fix_img_dimensions(html)
        self.assertIn('<img', result)
        self.assertIn('src="a.png"', result)

    def test_no_img_tag_unchanged(self):
        html = '<p>No images here</p>'
        self.assertEqual(md2pdf.fix_img_dimensions(html), html)


# ── fix_local_image_paths ─────────────────────────────────────────────────────

class TestFixLocalImagePaths(unittest.TestCase):

    def test_relative_path_becomes_absolute(self):
        html = '<img src="diagram.svg">'
        result = md2pdf.fix_local_image_paths(html, "/tmp/day1")
        self.assertIn("file://", result)
        self.assertIn("diagram.svg", result)

    def test_https_url_unchanged(self):
        url = "https://github.com/user-attachments/assets/abc123"
        html = '<img src="{}">'.format(url)
        result = md2pdf.fix_local_image_paths(html, "/tmp/day1")
        self.assertIn(url, result)
        self.assertNotIn("file://", result)

    def test_http_url_unchanged(self):
        html = '<img src="http://example.com/img.png">'
        result = md2pdf.fix_local_image_paths(html, "/tmp/day1")
        self.assertIn("http://example.com/img.png", result)

    def test_data_uri_unchanged(self):
        html = '<img src="data:image/png;base64,abc123">'
        result = md2pdf.fix_local_image_paths(html, "/tmp/day1")
        self.assertIn("data:image/png;base64,abc123", result)

    def test_already_file_uri_unchanged(self):
        html = '<img src="file:///tmp/day1/img.png">'
        result = md2pdf.fix_local_image_paths(html, "/tmp/day1")
        self.assertIn("file:///tmp/day1/img.png", result)


# ── md_to_html_fragment ───────────────────────────────────────────────────────

class TestMdToHtmlFragment(unittest.TestCase):

    def test_converts_heading(self):
        html = md2pdf.md_to_html_fragment("# Title", "/tmp")
        self.assertIn("<h1", html)
        self.assertIn("Title", html)

    def test_first_h1_gets_class(self):
        html = md2pdf.md_to_html_fragment("# Title\n## Sub", "/tmp")
        self.assertIn('class="first-heading"', html)

    def test_only_first_h1_gets_class(self):
        html = md2pdf.md_to_html_fragment("# One\n# Two\n# Three", "/tmp")
        self.assertEqual(html.count('class="first-heading"'), 1)

    def test_fenced_code_block(self):
        html = md2pdf.md_to_html_fragment("```python\nprint('hi')\n```", "/tmp")
        self.assertIn("<code", html)

    def test_table(self):
        md = "| A | B |\n|---|---|\n| 1 | 2 |"
        html = md2pdf.md_to_html_fragment(md, "/tmp")
        self.assertIn("<table", html)

    def test_inline_img_dimension_fixed(self):
        md = '<img width="1920" height="1200" src="https://example.com/a.png" />'
        html = md2pdf.md_to_html_fragment(md, "/tmp")
        self.assertNotIn('width="1920"', html)

    def test_local_img_path_fixed(self):
        md = '<img src="diagram.svg">'
        html = md2pdf.md_to_html_fragment(md, "/tmp/day1")
        self.assertIn("file://", html)

    def test_empty_markdown(self):
        self.assertEqual(md2pdf.md_to_html_fragment("", "/tmp"), "")


# ── build_pdf ─────────────────────────────────────────────────────────────────

class TestBuildPdf(unittest.TestCase):

    def _make_repo(self, days=("day1", "day2", "day3"), content=None):
        tmpdir = Path(tempfile.mkdtemp())
        for day in days:
            d = tmpdir / day
            d.mkdir()
            text = content or (
                "# {day}\n\n## Section\n\nContent for {day}.\n\n"
                "```python\nprint('{day}')\n```\n".format(day=day)
            )
            (d / "README.md").write_text(text)
        return tmpdir

    def test_creates_pdf(self):
        repo = self._make_repo()
        out = str(repo / "out.pdf")
        try:
            md2pdf.build_pdf(str(repo), ["day1", "day2", "day3"], out, "Test")
            self.assertTrue(os.path.exists(out))
        finally:
            import shutil; shutil.rmtree(repo)

    def test_pdf_magic_bytes(self):
        repo = self._make_repo()
        out = str(repo / "out.pdf")
        try:
            md2pdf.build_pdf(str(repo), ["day1", "day2", "day3"], out, "Test")
            with open(out, "rb") as f:
                self.assertEqual(f.read(5), b"%PDF-")
        finally:
            import shutil; shutil.rmtree(repo)

    def test_missing_folder_skipped(self):
        repo = self._make_repo(days=("day1", "day2"))
        out = str(repo / "out.pdf")
        try:
            md2pdf.build_pdf(str(repo), ["day1", "day2", "day3"], out, "Test")
            self.assertTrue(os.path.exists(out))
        finally:
            import shutil; shutil.rmtree(repo)

    def test_oversized_images_dont_crash(self):
        """PDF generation should succeed even when README contains 1920x1200 images."""
        repo = self._make_repo(days=("day1",), content=(
            "# Day 1\n\n"
            '<img width="1920" height="1200" alt="screenshot" '
            'src="https://github.com/user-attachments/assets/abc123" />\n\n'
            "Some text after the image.\n"
        ))
        out = str(repo / "out.pdf")
        try:
            md2pdf.build_pdf(str(repo), ["day1"], out, "Test")
            self.assertTrue(os.path.exists(out))
            self.assertGreater(os.path.getsize(out), 1000)
        finally:
            import shutil; shutil.rmtree(repo)

    def test_pdf_non_empty(self):
        repo = self._make_repo()
        out = str(repo / "out.pdf")
        try:
            md2pdf.build_pdf(str(repo), ["day1", "day2", "day3"], out, "Test")
            self.assertGreater(os.path.getsize(out), 1000)
        finally:
            import shutil; shutil.rmtree(repo)




# ── parse_cover_metadata ──────────────────────────────────────────────────────

class TestParseCoverMetadata(unittest.TestCase):

    SAMPLE = """# Bazel May 2026 (25-27 May 2026)

## Course designed and delivered by Jeganathan Swaminathan
#### jegan@tektutor.org
#### https://www.tektutor.org

#### GitHub - https://github.com/tektutor
#### LinkedIn - www.linkedin.com/in/jeganathan-swaminathan
"""

    def test_title_extracted(self):
        meta = md2pdf.parse_cover_metadata(self.SAMPLE)
        self.assertEqual(meta['title'], 'Bazel May 2026')

    def test_dates_extracted(self):
        meta = md2pdf.parse_cover_metadata(self.SAMPLE)
        self.assertEqual(meta['dates'], '25-27 May 2026')

    def test_subtitle_extracted(self):
        meta = md2pdf.parse_cover_metadata(self.SAMPLE)
        self.assertIn('Jeganathan Swaminathan', meta['subtitle'])

    def test_email_extracted(self):
        meta = md2pdf.parse_cover_metadata(self.SAMPLE)
        self.assertEqual(meta['email'], 'jegan@tektutor.org')

    def test_website_extracted(self):
        meta = md2pdf.parse_cover_metadata(self.SAMPLE)
        self.assertIn('tektutor.org', meta['website'])

    def test_github_extracted(self):
        meta = md2pdf.parse_cover_metadata(self.SAMPLE)
        self.assertIn('github.com/tektutor', meta['github'])

    def test_linkedin_extracted(self):
        meta = md2pdf.parse_cover_metadata(self.SAMPLE)
        self.assertIn('linkedin', meta['linkedin'])

    def test_title_without_dates(self):
        meta = md2pdf.parse_cover_metadata("# Just A Title\n")
        self.assertEqual(meta['title'], 'Just A Title')
        self.assertEqual(meta['dates'], '')

    def test_empty_readme_returns_defaults(self):
        meta = md2pdf.parse_cover_metadata("")
        self.assertEqual(meta['title'], '')
        self.assertEqual(meta['email'], '')


# ── build_cover_html ──────────────────────────────────────────────────────────

class TestBuildCoverHtml(unittest.TestCase):

    def _meta(self):
        return {
            'title':    'Bazel May 2026',
            'dates':    '25-27 May 2026',
            'subtitle': 'Course designed and delivered by Jeganathan Swaminathan',
            'email':    'jegan@tektutor.org',
            'website':  'https://www.tektutor.org',
            'github':   'https://github.com/tektutor',
            'linkedin': 'www.linkedin.com/in/jeganathan-swaminathan',
        }

    def test_title_in_cover(self):
        html = md2pdf.build_cover_html(self._meta())
        self.assertIn('Bazel May 2026', html)

    def test_dates_in_cover(self):
        html = md2pdf.build_cover_html(self._meta())
        self.assertIn('25-27 May 2026', html)

    def test_author_name_in_cover(self):
        html = md2pdf.build_cover_html(self._meta())
        self.assertIn('Jeganathan Swaminathan', html)

    def test_email_in_cover(self):
        html = md2pdf.build_cover_html(self._meta())
        self.assertIn('jegan@tektutor.org', html)

    def test_website_in_cover(self):
        html = md2pdf.build_cover_html(self._meta())
        self.assertIn('tektutor.org', html)

    def test_github_in_cover(self):
        html = md2pdf.build_cover_html(self._meta())
        self.assertIn('github.com/tektutor', html)

    def test_returns_string(self):
        self.assertIsInstance(md2pdf.build_cover_html(self._meta()), str)

if __name__ == "__main__":
    unittest.main(verbosity=2)
