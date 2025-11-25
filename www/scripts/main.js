/**
 * Main entry point for the application
 */
import { loadComponents } from './modules/components.js';
import { initializeNavigation, initializeBreadcrumb } from './modules/navigation.js';
import { initializeAccessibility, initializeModals, initializeCarousels } from './modules/ui.js';
import { initializeFormHandlers, validateForm, validateEmail } from './modules/forms.js';
import { scrollToSection } from './modules/utils.js';
import { initializeAssessment } from './modules/assessment.js';
import { initializeScenario } from './modules/scenario.js';

/**
 * Initialize the platform when DOM is ready
 */
document.addEventListener('DOMContentLoaded', async function () {
    await loadComponents();
    initializeNavigation();
    initializeBreadcrumb();
    initializeAccessibility();
    initializeFormHandlers();
    initializeModals();
    initializeCarousels();

    // Initialize page-specific functionality (now async)
    await initializeAssessment();
    await initializeScenario();
});
