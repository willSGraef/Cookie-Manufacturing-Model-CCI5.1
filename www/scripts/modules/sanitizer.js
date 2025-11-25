/**
 * HTML Sanitization Utilities
 * Provides safe HTML insertion to prevent XSS attacks
 */

/**
 * Sanitize and safely insert HTML content into a DOM element
 * Uses DOMParser to parse HTML in a safe context and only allows specific safe tags
 * @param {HTMLElement} targetElement - The element to insert content into
 * @param {string} htmlString - The HTML string to sanitize and insert
 * @returns {void}
 */
export function safeSetInnerHTML(targetElement, htmlString) {
    if (!targetElement) {
        console.warn('safeSetInnerHTML: No target element provided');
        return;
    }

    if (!htmlString || typeof htmlString !== 'string') {
        targetElement.textContent = '';
        return;
    }

    // Parse HTML in isolated context
    const parser = new DOMParser();
    const doc = parser.parseFromString(htmlString, 'text/html');

    // Check for parsing errors
    const parserError = doc.querySelector('parsererror');
    if (parserError) {
        console.error('HTML parsing error:', parserError.textContent);
        targetElement.textContent = 'Error loading content';
        return;
    }

    // Define allowed tags for educational content
    const allowedTags = [
        'P', 'SPAN', 'DIV', 'BR', 'HR',
        'H1', 'H2', 'H3', 'H4', 'H5', 'H6',
        'UL', 'OL', 'LI',
        'STRONG', 'EM', 'B', 'I', 'U',
        'A', 'CODE', 'PRE',
        'TABLE', 'THEAD', 'TBODY', 'TR', 'TH', 'TD',
        'BLOCKQUOTE'
    ];

    // Allowed attributes per tag
    const allowedAttributes = {
        'A': ['href', 'title', 'target', 'rel'],
        'IMG': ['src', 'alt', 'title', 'width', 'height'],
        'CODE': ['class'],
        'PRE': ['class'],
        '*': ['class', 'id'] // Allowed on all tags
    };

    /**
     * Recursively sanitize nodes
     * @param {Node} node - The node to sanitize
     * @returns {Node|null} Sanitized node or null if not allowed
     */
    function sanitizeNode(node) {
        // Text nodes are always safe
        if (node.nodeType === Node.TEXT_NODE) {
            return node.cloneNode(false);
        }

        // Only allow element nodes
        if (node.nodeType !== Node.ELEMENT_NODE) {
            return null;
        }

        const tagName = node.tagName.toUpperCase();

        // Reject disallowed tags
        if (!allowedTags.includes(tagName)) {
            console.warn(`Blocked potentially unsafe tag: ${tagName}`);
            return null;
        }

        // Create sanitized element
        const sanitizedElement = document.createElement(tagName);

        // Copy allowed attributes
        const tagAllowedAttrs = allowedAttributes[tagName] || [];
        const globalAllowedAttrs = allowedAttributes['*'] || [];
        const combinedAllowed = [...new Set([...tagAllowedAttrs, ...globalAllowedAttrs])];

        Array.from(node.attributes).forEach(attr => {
            const attrName = attr.name.toLowerCase();
            
            if (combinedAllowed.includes(attrName)) {
                let attrValue = attr.value;

                // Sanitize href attributes - only allow http, https, mailto
                if (attrName === 'href') {
                    try {
                        const url = new URL(attrValue, window.location.href);
                        if (!['http:', 'https:', 'mailto:'].includes(url.protocol)) {
                            console.warn(`Blocked unsafe URL protocol: ${url.protocol}`);
                            return;
                        }
                    } catch (e) {
                        // Relative URLs are okay
                        if (!attrValue.startsWith('/') && !attrValue.startsWith('#')) {
                            console.warn('Blocked invalid URL:', attrValue);
                            return;
                        }
                    }
                }

                // Sanitize target attribute - add rel="noopener noreferrer" for _blank
                if (attrName === 'target' && attrValue === '_blank') {
                    sanitizedElement.setAttribute('rel', 'noopener noreferrer');
                }

                // Block javascript: and data: protocols in any attribute
                if (attrValue.toLowerCase().includes('javascript:') || 
                    attrValue.toLowerCase().includes('data:')) {
                    console.warn(`Blocked unsafe attribute value: ${attrValue}`);
                    return;
                }

                sanitizedElement.setAttribute(attrName, attrValue);
            }
        });

        // Recursively sanitize child nodes
        Array.from(node.childNodes).forEach(child => {
            const sanitizedChild = sanitizeNode(child);
            if (sanitizedChild) {
                sanitizedElement.appendChild(sanitizedChild);
            }
        });

        return sanitizedElement;
    }

    // Clear target element
    targetElement.textContent = '';

    // Sanitize and append all body children
    Array.from(doc.body.childNodes).forEach(node => {
        const sanitizedNode = sanitizeNode(node);
        if (sanitizedNode) {
            targetElement.appendChild(sanitizedNode);
        }
    });
}

/**
 * Escape HTML special characters to prevent XSS
 * Use this when you need to display user input as plain text
 * @param {string} text - Text to escape
 * @returns {string} Escaped text safe for HTML insertion
 */
export function escapeHTML(text) {
    if (!text || typeof text !== 'string') return '';
    
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}
