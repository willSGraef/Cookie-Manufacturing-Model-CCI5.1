import { getCurrentPage, isPagesDirectory } from './utils.js';

/**
 * Initialize navigation active state
 */
export function initializeNavigation() {
    const currentPage = getCurrentPage();
    const navLinks = document.querySelectorAll('.dock-item, .nav-link');

    navLinks.forEach(link => {
        const href = link.getAttribute('href').split('/').pop() || 'index.html';
        if (href === currentPage || (currentPage === '' && href === 'index.html')) {
            link.classList.add('active');
            link.setAttribute('aria-current', 'page');
        } else {
            link.classList.remove('active');
            link.removeAttribute('aria-current');
        }
    });
}

/**
 * Initialize dynamic breadcrumb
 */
export function initializeBreadcrumb() {
    const breadcrumbContainer = document.getElementById('dynamic-breadcrumb');
    if (!breadcrumbContainer) return;

    const currentPage = getCurrentPage();
    const isPagesDir = isPagesDirectory();
    const homePath = isPagesDir ? '../index.html' : 'index.html';

    let breadcrumbHtml = `<span class="breadcrumb-item"><a href="${homePath}">Home</a></span>`;

    if (currentPage === 'scenario.html') {
        breadcrumbHtml += `<span class="breadcrumb-item active">Scenario</span>`;
    } else if (currentPage === 'assessment.html') {
        breadcrumbHtml += `<span class="breadcrumb-item active">Assessment</span>`;
    } else if (currentPage === 'resources.html') {
        breadcrumbHtml += `<span class="breadcrumb-item active">Resources</span>`;
    } else if (currentPage === 'support.html') {
        breadcrumbHtml += `<span class="breadcrumb-item active">Support</span>`;
    }

    // Only show breadcrumb if we are not on home page
    if (currentPage !== 'index.html' && currentPage !== '') {
        breadcrumbContainer.innerHTML = breadcrumbHtml;
    } else {
        breadcrumbContainer.style.display = 'none';
    }
}
