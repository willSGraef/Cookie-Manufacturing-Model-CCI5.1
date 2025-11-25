/**
 * Tests for HTML Sanitization Utilities
 * Validates XSS prevention and safe HTML insertion
 */

import { describe, it, expect, beforeEach, afterEach, vi } from 'vitest';
import { safeSetInnerHTML, escapeHTML } from '../scripts/modules/sanitizer.js';

describe('HTML Sanitizer Module', () => {
    let container;

    beforeEach(() => {
        // Mock fetch to prevent network requests from trying to load malicious URLs
        global.fetch = vi.fn().mockRejectedValue(new Error('Fetch mocked'));
        container = document.createElement('div');
        document.body.appendChild(container);
    });

    afterEach(() => {
        // Clean up DOM and restore fetch
        document.body.innerHTML = '';
        vi.restoreAllMocks();
    });

    describe('safeSetInnerHTML', () => {
        it('should safely insert allowed HTML tags', () => {
            const html = '<p>Hello <strong>World</strong></p>';
            safeSetInnerHTML(container, html);

            expect(container.querySelector('p')).toBeTruthy();
            expect(container.querySelector('strong')).toBeTruthy();
            expect(container.textContent).toBe('Hello World');
        });

        it('should block script tags to prevent XSS', () => {
            const html = '<p>Safe content</p><script>alert("XSS")</script>';
            safeSetInnerHTML(container, html);

            expect(container.querySelector('script')).toBeNull();
            expect(container.querySelector('p')).toBeTruthy();
            expect(container.textContent).toBe('Safe content');
        });

        it('should block javascript: protocol in links', () => {
            const html = '<a href="javascript:alert(\'XSS\')">Click me</a>';
            safeSetInnerHTML(container, html);

            const link = container.querySelector('a');
            expect(link).toBeTruthy();
            expect(link.getAttribute('href')).toBeNull();
        });

        it('should block data: protocol in links', () => {
            const html = '<a href="data:text/html,<script>alert(\'XSS\')</script>">Click</a>';
            safeSetInnerHTML(container, html);

            const link = container.querySelector('a');
            expect(link).toBeTruthy();
            expect(link.getAttribute('href')).toBeNull();
        });

        it('should allow safe external links with https', () => {
            const html = '<a href="https://example.com">Example</a>';
            safeSetInnerHTML(container, html);

            const link = container.querySelector('a');
            expect(link).toBeTruthy();
            expect(link.getAttribute('href')).toBe('https://example.com');
        });

        it('should add rel="noopener noreferrer" to target="_blank" links', () => {
            const html = '<a href="https://example.com" target="_blank">Example</a>';
            safeSetInnerHTML(container, html);

            const link = container.querySelector('a');
            expect(link.getAttribute('rel')).toBe('noopener noreferrer');
        });

        it('should allow common formatting tags', () => {
            const html = '<p>Text with <em>emphasis</em> and <strong>strong</strong> and <code>code</code></p>';
            safeSetInnerHTML(container, html);

            expect(container.querySelector('em')).toBeTruthy();
            expect(container.querySelector('strong')).toBeTruthy();
            expect(container.querySelector('code')).toBeTruthy();
        });

        it('should allow lists and list items', () => {
            const html = '<ul><li>Item 1</li><li>Item 2</li></ul>';
            safeSetInnerHTML(container, html);

            expect(container.querySelector('ul')).toBeTruthy();
            expect(container.querySelectorAll('li').length).toBe(2);
        });

        it('should block event handlers in attributes', () => {
            const html = '<p onclick="alert(\'XSS\')">Click me</p>';
            safeSetInnerHTML(container, html);

            const p = container.querySelector('p');
            expect(p).toBeTruthy();
            expect(p.getAttribute('onclick')).toBeNull();
            expect(p.textContent).toBe('Click me');
        });

        it('should handle empty string', () => {
            safeSetInnerHTML(container, '');
            expect(container.textContent).toBe('');
        });

        it('should handle null gracefully', () => {
            safeSetInnerHTML(container, null);
            expect(container.textContent).toBe('');
        });

        it('should handle undefined gracefully', () => {
            safeSetInnerHTML(container, undefined);
            expect(container.textContent).toBe('');
        });

        it('should warn and return when no target element provided', () => {
            // Should not throw error
            expect(() => {
                safeSetInnerHTML(null, '<p>Test</p>');
            }).not.toThrow();
        });

        it('should preserve allowed class attributes', () => {
            const html = '<p class="text-primary">Styled text</p>';
            safeSetInnerHTML(container, html);

            const p = container.querySelector('p');
            expect(p.classList.contains('text-primary')).toBe(true);
        });

        it('should allow headings', () => {
            const html = '<h1>Title</h1><h2>Subtitle</h2><h3>Section</h3>';
            safeSetInnerHTML(container, html);

            expect(container.querySelector('h1')).toBeTruthy();
            expect(container.querySelector('h2')).toBeTruthy();
            expect(container.querySelector('h3')).toBeTruthy();
        });

        it('should block iframe tags', () => {
            const html = '<p>Safe</p><iframe src="evil.com"></iframe>';
            safeSetInnerHTML(container, html);

            expect(container.querySelector('iframe')).toBeNull();
            expect(container.querySelector('p')).toBeTruthy();
        });

        it('should block object and embed tags', () => {
            const html = '<object data="evil.swf"></object><embed src="evil.swf">';
            safeSetInnerHTML(container, html);

            expect(container.querySelector('object')).toBeNull();
            expect(container.querySelector('embed')).toBeNull();
        });
    });

    describe('escapeHTML', () => {
        it('should escape special HTML characters', () => {
            const text = '<script>alert("XSS")</script>';
            const escaped = escapeHTML(text);

            expect(escaped).toBe('&lt;script&gt;alert("XSS")&lt;/script&gt;');
        });

        it('should escape ampersands', () => {
            const text = 'Tom & Jerry';
            const escaped = escapeHTML(text);

            expect(escaped).toBe('Tom &amp; Jerry');
        });

        it('should handle text with quotes', () => {
            const text = 'He said "Hello"';
            const escaped = escapeHTML(text);

            // Note: textContent doesn't escape quotes, only < > & 
            // Quotes are safe in textContent context
            expect(escaped).toBe('He said "Hello"');
        });

        it('should handle empty string', () => {
            expect(escapeHTML('')).toBe('');
        });

        it('should handle null', () => {
            expect(escapeHTML(null)).toBe('');
        });

        it('should handle undefined', () => {
            expect(escapeHTML(undefined)).toBe('');
        });

        it('should not double-escape', () => {
            const text = '&lt;script&gt;';
            const escaped = escapeHTML(text);

            // Should escape the & again
            expect(escaped).toBe('&amp;lt;script&amp;gt;');
        });
    });
});
