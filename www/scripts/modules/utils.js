/**
 * Utility functions for the application
 */

/**
 * Smooth scroll to section
 * @param {string} sectionId - The ID of the section to scroll to
 */
export function scrollToSection(sectionId) {
    const section = document.getElementById(sectionId);
    if (section) {
        section.scrollIntoView({ behavior: 'smooth' });
    }
}

/**
 * Adjust relative paths in loaded HTML content
 * @param {string} html - The HTML content to adjust
 * @param {boolean} isPagesDir - Whether the current page is in the pages directory
 * @returns {string} - The adjusted HTML
 */
export function adjustPaths(html, isPagesDir) {
    if (!isPagesDir) return html;
    
    // Replace "assets/" with "../assets/" and "index.html" with "../index.html" etc.
    return html
        .replace(/href="index\.html"/g, 'href="../index.html"')
        .replace(/href="pages\//g, 'href="')
        .replace(/src="assets\//g, 'src="../assets/')
        .replace(/href="styles\//g, 'href="../styles/');
}

/**
 * Get the current page filename
 * @returns {string} - The filename of the current page
 */
export function getCurrentPage() {
    return window.location.pathname.split('/').pop() || 'index.html';
}

/**
 * Check if the current page is in the pages directory
 * @returns {boolean}
 */
export function isPagesDirectory() {
    return window.location.pathname.includes('/pages/');
}
