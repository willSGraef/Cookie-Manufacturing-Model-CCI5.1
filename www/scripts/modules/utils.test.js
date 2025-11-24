/**
 * Unit tests for utility functions module
 * Tests path adjustment, page detection, and navigation utilities
 */

import { describe, it, expect, beforeEach, afterEach, vi } from 'vitest';
import { scrollToSection, adjustPaths, getCurrentPage, isPagesDirectory } from './utils.js';

describe('Utility Functions Module', () => {
    describe('scrollToSection', () => {
        let mockSection;

        beforeEach(() => {
            // Create mock section
            mockSection = document.createElement('section');
            mockSection.id = 'test-section';
            document.body.appendChild(mockSection);

            // Mock scrollIntoView
            mockSection.scrollIntoView = vi.fn();
        });

        afterEach(() => {
            document.body.innerHTML = '';
        });

        it('should scroll to section when it exists', () => {
            scrollToSection('test-section');
            expect(mockSection.scrollIntoView).toHaveBeenCalledWith({ behavior: 'smooth' });
        });

        it('should not throw error when section does not exist', () => {
            expect(() => scrollToSection('non-existent-section')).not.toThrow();
        });

        it('should not call scrollIntoView when section does not exist', () => {
            scrollToSection('non-existent-section');
            expect(mockSection.scrollIntoView).not.toHaveBeenCalled();
        });
    });

    describe('adjustPaths', () => {
        it('should not modify HTML when not in pages directory', () => {
            const html = '<a href="index.html">Home</a><img src="assets/image.png">';
            const result = adjustPaths(html, false);
            expect(result).toBe(html);
        });

        it('should adjust index.html path when in pages directory', () => {
            const html = '<a href="index.html">Home</a>';
            const result = adjustPaths(html, true);
            expect(result).toContain('href="../index.html"');
        });

        it('should adjust assets paths when in pages directory', () => {
            const html = '<img src="assets/images/test.png">';
            const result = adjustPaths(html, true);
            expect(result).toContain('src="../assets/images/test.png"');
        });

        it('should adjust styles paths when in pages directory', () => {
            const html = '<link href="styles/main.css" rel="stylesheet">';
            const result = adjustPaths(html, true);
            expect(result).toContain('href="../styles/main.css"');
        });

        it('should remove pages/ from hrefs when in pages directory', () => {
            const html = '<a href="pages/scenario.html">Scenario</a>';
            const result = adjustPaths(html, true);
            expect(result).toContain('href="scenario.html"');
            expect(result).not.toContain('pages/');
        });

        it('should handle multiple path types in same HTML', () => {
            const html = `
        <a href="index.html">Home</a>
        <a href="pages/scenario.html">Scenario</a>
        <img src="assets/images/logo.png">
        <link href="styles/main.css" rel="stylesheet">
      `;
            const result = adjustPaths(html, true);

            expect(result).toContain('href="../index.html"');
            expect(result).toContain('href="scenario.html"');
            expect(result).toContain('src="../assets/images/logo.png"');
            expect(result).toContain('href="../styles/main.css"');
        });

        it('should preserve HTML structure and attributes', () => {
            const html = '<a href="index.html" class="btn" data-id="1">Home</a>';
            const result = adjustPaths(html, true);
            expect(result).toContain('class="btn"');
            expect(result).toContain('data-id="1"');
        });

        it('should handle empty HTML string', () => {
            const result = adjustPaths('', true);
            expect(result).toBe('');
        });
    });

    describe('getCurrentPage', () => {
        let originalLocation;

        beforeEach(() => {
            originalLocation = window.location;
            delete window.location;
        });

        afterEach(() => {
            window.location = originalLocation;
        });

        it('should return filename from pathname', () => {
            window.location = { pathname: '/test/scenario.html' };
            expect(getCurrentPage()).toBe('scenario.html');
        });

        it('should return filename from root path', () => {
            window.location = { pathname: '/index.html' };
            expect(getCurrentPage()).toBe('index.html');
        });

        it('should return filename from nested path', () => {
            window.location = { pathname: '/pages/assessment.html' };
            expect(getCurrentPage()).toBe('assessment.html');
        });

        it('should return index.html for root path with trailing slash', () => {
            window.location = { pathname: '/' };
            expect(getCurrentPage()).toBe('index.html');
        });

        it('should return index.html for empty pathname', () => {
            window.location = { pathname: '' };
            expect(getCurrentPage()).toBe('index.html');
        });

        it('should handle paths without file extension', () => {
            window.location = { pathname: '/test/page' };
            expect(getCurrentPage()).toBe('page');
        });
    });

    describe('isPagesDirectory', () => {
        let originalLocation;

        beforeEach(() => {
            originalLocation = window.location;
            delete window.location;
        });

        afterEach(() => {
            window.location = originalLocation;
        });

        it('should return true when in pages directory', () => {
            window.location = { pathname: '/pages/scenario.html' };
            expect(isPagesDirectory()).toBe(true);
        });

        it('should return true when in pages subdirectory', () => {
            window.location = { pathname: '/pages/subdir/file.html' };
            expect(isPagesDirectory()).toBe(true);
        });

        it('should return false when not in pages directory', () => {
            window.location = { pathname: '/index.html' };
            expect(isPagesDirectory()).toBe(false);
        });

        it('should return false for root path', () => {
            window.location = { pathname: '/' };
            expect(isPagesDirectory()).toBe(false);
        });

        it('should return false for other directories', () => {
            window.location = { pathname: '/assets/images/test.png' };
            expect(isPagesDirectory()).toBe(false);
        });

        it('should be case-sensitive', () => {
            window.location = { pathname: '/Pages/scenario.html' };
            expect(isPagesDirectory()).toBe(false);
        });
    });

    describe('Integration: adjustPaths with isPagesDirectory', () => {
        let originalLocation;

        beforeEach(() => {
            originalLocation = window.location;
            delete window.location;
        });

        afterEach(() => {
            window.location = originalLocation;
        });

        it('should correctly adjust paths based on current directory', () => {
            // When in pages directory
            window.location = { pathname: '/pages/scenario.html' };
            const html = '<a href="index.html">Home</a>';
            const result = adjustPaths(html, isPagesDirectory());
            expect(result).toContain('../index.html');
        });

        it('should not adjust paths when at root', () => {
            // When at root
            window.location = { pathname: '/index.html' };
            const html = '<a href="index.html">Home</a>';
            const result = adjustPaths(html, isPagesDirectory());
            expect(result).toBe(html);
        });
    });
});
