/**
 * Unit tests for navigation module
 * Tests active state management and breadcrumb generation
 */

import { describe, it, expect, beforeEach, afterEach, vi } from 'vitest';
import { initializeNavigation, initializeBreadcrumb } from '../scripts/modules/navigation.js';

describe('Navigation Module', () => {
    let originalLocation;

    beforeEach(() => {
        // Reset DOM
        document.body.innerHTML = '';

        // Save original location
        originalLocation = window.location;
        delete window.location;
    });

    afterEach(() => {
        // Restore original location
        window.location = originalLocation;
    });

    describe('initializeNavigation', () => {
        let homeLink, scenarioLink, assessmentLink;

        beforeEach(() => {
            // Create navigation links
            homeLink = document.createElement('a');
            homeLink.href = 'index.html';
            homeLink.className = 'dock-item';

            scenarioLink = document.createElement('a');
            scenarioLink.href = 'pages/scenario.html';
            scenarioLink.className = 'dock-item';

            assessmentLink = document.createElement('a');
            assessmentLink.href = 'pages/assessment.html';
            assessmentLink.className = 'dock-item';

            document.body.appendChild(homeLink);
            document.body.appendChild(scenarioLink);
            document.body.appendChild(assessmentLink);
        });

        it('should add active class to current page link', () => {
            window.location = { pathname: '/pages/scenario.html' };
            initializeNavigation();

            expect(scenarioLink.classList.contains('active')).toBe(true);
            expect(scenarioLink.getAttribute('aria-current')).toBe('page');
        });

        it('should remove active class from other links', () => {
            window.location = { pathname: '/pages/scenario.html' };

            // Set all links active initially
            homeLink.classList.add('active');
            assessmentLink.classList.add('active');

            initializeNavigation();

            expect(homeLink.classList.contains('active')).toBe(false);
            expect(assessmentLink.classList.contains('active')).toBe(false);
            expect(scenarioLink.classList.contains('active')).toBe(true);
        });

        it('should set aria-current attribute on active link', () => {
            window.location = { pathname: '/pages/assessment.html' };
            initializeNavigation();

            expect(assessmentLink.getAttribute('aria-current')).toBe('page');
            expect(homeLink.getAttribute('aria-current')).toBeNull();
            expect(scenarioLink.getAttribute('aria-current')).toBeNull();
        });

        it('should handle index.html at root', () => {
            window.location = { pathname: '/index.html' };
            initializeNavigation();

            expect(homeLink.classList.contains('active')).toBe(true);
            expect(homeLink.getAttribute('aria-current')).toBe('page');
        });

        it('should handle empty pathname as index.html', () => {
            window.location = { pathname: '' };
            initializeNavigation();

            expect(homeLink.classList.contains('active')).toBe(true);
            expect(homeLink.getAttribute('aria-current')).toBe('page');
        });

        it('should work with nav-link class as well', () => {
            const navLink = document.createElement('a');
            navLink.href = 'pages/scenario.html';
            navLink.className = 'nav-link';
            document.body.appendChild(navLink);

            window.location = { pathname: '/pages/scenario.html' };
            initializeNavigation();

            expect(navLink.classList.contains('active')).toBe(true);
            expect(navLink.getAttribute('aria-current')).toBe('page');
        });

        it('should handle links with full paths', () => {
            window.location = { pathname: '/pages/scenario.html' };
            initializeNavigation();

            expect(scenarioLink.classList.contains('active')).toBe(true);
        });

        it('should not activate links for different pages', () => {
            window.location = { pathname: '/pages/other.html' };
            initializeNavigation();

            expect(homeLink.classList.contains('active')).toBe(false);
            expect(scenarioLink.classList.contains('active')).toBe(false);
            expect(assessmentLink.classList.contains('active')).toBe(false);
        });
    });

    describe('initializeBreadcrumb', () => {
        let breadcrumbContainer;

        beforeEach(() => {
            breadcrumbContainer = document.createElement('nav');
            breadcrumbContainer.id = 'dynamic-breadcrumb';
            document.body.appendChild(breadcrumbContainer);
        });

        it('should hide breadcrumb on home page (index.html)', () => {
            window.location = { pathname: '/index.html' };
            initializeBreadcrumb();

            expect(breadcrumbContainer.style.display).toBe('none');
        });

        it('should hide breadcrumb on root path', () => {
            window.location = { pathname: '' };
            initializeBreadcrumb();

            expect(breadcrumbContainer.style.display).toBe('none');
        });

        it('should show breadcrumb with Home > Scenario on scenario page', () => {
            window.location = { pathname: '/pages/scenario.html' };
            initializeBreadcrumb();

            const html = breadcrumbContainer.innerHTML;
            expect(html).toContain('Home');
            expect(html).toContain('Scenario');
            expect(html).toContain('../index.html');
            expect(breadcrumbContainer.style.display).not.toBe('none');
        });

        it('should show breadcrumb with Home > Assessment on assessment page', () => {
            window.location = { pathname: '/pages/assessment.html' };
            initializeBreadcrumb();

            const html = breadcrumbContainer.innerHTML;
            expect(html).toContain('Home');
            expect(html).toContain('Assessment');
            expect(html).toContain('../index.html');
        });

        it('should show breadcrumb with Home > Resources on resources page', () => {
            window.location = { pathname: '/pages/resources.html' };
            initializeBreadcrumb();

            const html = breadcrumbContainer.innerHTML;
            expect(html).toContain('Home');
            expect(html).toContain('Resources');
        });

        it('should show breadcrumb with Home > Support on support page', () => {
            window.location = { pathname: '/pages/support.html' };
            initializeBreadcrumb();

            const html = breadcrumbContainer.innerHTML;
            expect(html).toContain('Home');
            expect(html).toContain('Support');
        });

        it('should use correct path to home when in pages directory', () => {
            window.location = { pathname: '/pages/scenario.html' };
            initializeBreadcrumb();

            expect(breadcrumbContainer.innerHTML).toContain('href="../index.html"');
        });

        it('should use correct path to home when at root', () => {
            window.location = { pathname: '/other.html' };
            initializeBreadcrumb();

            expect(breadcrumbContainer.innerHTML).toContain('href="index.html"');
        });

        it('should mark current page with active class', () => {
            window.location = { pathname: '/pages/scenario.html' };
            initializeBreadcrumb();

            expect(breadcrumbContainer.innerHTML).toContain('breadcrumb-item active');
        });

        it('should create link for Home in breadcrumb', () => {
            window.location = { pathname: '/pages/scenario.html' };
            initializeBreadcrumb();

            const homeLink = breadcrumbContainer.querySelector('a[href="../index.html"]');
            expect(homeLink).not.toBeNull();
            expect(homeLink.textContent).toBe('Home');
        });

        it('should handle case when breadcrumb container does not exist', () => {
            breadcrumbContainer.remove();

            // Should not throw error
            expect(() => initializeBreadcrumb()).not.toThrow();
        });

        it('should not show breadcrumb for unknown pages', () => {
            window.location = { pathname: '/pages/unknown.html' };
            initializeBreadcrumb();

            // Should show Home link but nothing after it (just Home >)
            const html = breadcrumbContainer.innerHTML;
            expect(html).toContain('Home');
        });

        it('should generate valid HTML structure', () => {
            window.location = { pathname: '/pages/scenario.html' };
            initializeBreadcrumb();

            const breadcrumbItems = breadcrumbContainer.querySelectorAll('.breadcrumb-item');
            expect(breadcrumbItems.length).toBeGreaterThan(0);
        });
    });

    describe('Integration: Navigation and Breadcrumb', () => {
        let breadcrumbContainer, homeLink, scenarioLink;

        beforeEach(() => {
            breadcrumbContainer = document.createElement('nav');
            breadcrumbContainer.id = 'dynamic-breadcrumb';

            homeLink = document.createElement('a');
            homeLink.href = 'index.html';
            homeLink.className = 'dock-item';

            scenarioLink = document.createElement('a');
            scenarioLink.href = 'pages/scenario.html';
            scenarioLink.className = 'dock-item';

            document.body.appendChild(breadcrumbContainer);
            document.body.appendChild(homeLink);
            document.body.appendChild(scenarioLink);
        });

        it('should correctly initialize both navigation and breadcrumb', () => {
            window.location = { pathname: '/pages/scenario.html' };

            initializeNavigation();
            initializeBreadcrumb();

            // Check navigation is active
            expect(scenarioLink.classList.contains('active')).toBe(true);

            // Check breadcrumb is shown
            expect(breadcrumbContainer.innerHTML).toContain('Scenario');
            expect(breadcrumbContainer.style.display).not.toBe('none');
        });

        it('should hide breadcrumb but activate home link on index page', () => {
            window.location = { pathname: '/index.html' };

            initializeNavigation();
            initializeBreadcrumb();

            expect(homeLink.classList.contains('active')).toBe(true);
            expect(breadcrumbContainer.style.display).toBe('none');
        });
    });
});
