/**
 * Scenario page functionality
 * Manages control logic diagrams carousel, topology modal, and functional area modals
 */

import { isPagesDirectory } from './utils.js';
import { openModal, closeModal } from './ui.js';

// Control Logic Diagrams - loaded from external JSON
let controlLogicDiagrams = [];

let currentControlLogicIndex = 0;

/**
 * Load diagram data from external JSON file
 * Fetches control logic diagram metadata from data/diagrams.json.
 * Shows loading indicator and handles errors gracefully.
 * @returns {Promise<Array>} Promise resolving to array of diagram objects with src, alt, title, and description
 * @throws {Error} Logs error and displays user-friendly message if loading fails
 */
async function loadDiagramData() {
    try {
        const basePath = isPagesDirectory() ? '../' : '';

        // Show loading indicator on carousel
        const carouselSection = document.querySelector('.carousel-section');
        if (carouselSection && controlLogicDiagrams.length === 0) {
            const loadingDiv = document.createElement('div');
            loadingDiv.className = 'diagram-loading';
            loadingDiv.style.cssText = 'text-align: center; padding: var(--spacing-xl); color: var(--color-text-muted);';
            loadingDiv.innerHTML = '<p>Loading diagrams...</p>';
            carouselSection.appendChild(loadingDiv);
        }

        const response = await fetch(`${basePath}data/diagrams.json`);
        if (!response.ok) {
            throw new Error(`Failed to load diagrams: ${response.status}`);
        }
        controlLogicDiagrams = await response.json();

        // Remove loading indicator
        const loadingDiv = document.querySelector('.diagram-loading');
        if (loadingDiv) {
            loadingDiv.remove();
        }

        return controlLogicDiagrams;
    } catch (error) {
        console.error('Error loading diagram data:', error);

        // Remove loading indicator
        const loadingDiv = document.querySelector('.diagram-loading');
        if (loadingDiv) {
            loadingDiv.remove();
        }

        // Fallback: show error to user
        const errorMsg = document.createElement('div');
        errorMsg.className = 'alert alert-danger';
        errorMsg.setAttribute('role', 'alert');
        errorMsg.innerHTML = '<p><strong>Error:</strong> Unable to load diagram data. Please refresh the page.</p>';
        document.querySelector('main')?.prepend(errorMsg);
        return [];
    }
}

/**
 * Update the control logic carousel display
 * Updates both the main page carousel and modal carousel with the current diagram.
 * Synchronizes image, title, counter, and description across both views.
 * @returns {void}
 */
function updateControlLogicDisplay() {
    const diagram = controlLogicDiagrams[currentControlLogicIndex];

    // Update main page carousel
    const controlLogicImageMain = document.getElementById("control-logic-image-main");
    const controlLogicTitleMain = document.getElementById("control-logic-title-main");
    const controlLogicCounterMain = document.getElementById("control-logic-counter-main");
    const descriptionAreaMain = document.getElementById("control-logic-description-main");

    if (controlLogicImageMain) {
        controlLogicImageMain.src = diagram.src;
        controlLogicImageMain.alt = diagram.alt;
    }
    if (controlLogicTitleMain) {
        controlLogicTitleMain.textContent = diagram.title;
    }
    if (controlLogicCounterMain) {
        controlLogicCounterMain.textContent = `(${currentControlLogicIndex + 1} of ${controlLogicDiagrams.length})`;
    }
    if (descriptionAreaMain) {
        descriptionAreaMain.innerHTML = diagram.description;
    }

    // Update modal carousel if it exists
    const controlLogicImage = document.getElementById("control-logic-image");
    const controlLogicTitle = document.getElementById("control-logic-title");
    const controlLogicCounter = document.getElementById("control-logic-counter");
    const descriptionArea = document.getElementById("control-logic-description");

    if (controlLogicImage) {
        controlLogicImage.src = diagram.src;
        controlLogicImage.alt = diagram.alt;
    }
    if (controlLogicTitle) {
        controlLogicTitle.textContent = diagram.title;
    }
    if (controlLogicCounter) {
        controlLogicCounter.textContent = `(${currentControlLogicIndex + 1} of ${controlLogicDiagrams.length})`;
    }
    if (descriptionArea) {
        descriptionArea.innerHTML = diagram.description;
    }
}

/**
 * Show carousel arrows on hover
 * Reduces image opacity to make navigation arrows more visible
 * @returns {void}
 */
function showCarouselArrows() {
    const img = document.getElementById("control-logic-image-main");
    if (img) img.style.opacity = "0.9";
}

/**
 * Hide carousel arrows
 * Restores full image opacity when not hovering
 * @returns {void}
 */
function hideCarouselArrows() {
    const img = document.getElementById("control-logic-image-main");
    if (img) img.style.opacity = "1";
}

/**
 * Open control logic modal
 * Resets carousel to first diagram and opens the modal view
 * @returns {void}
 */
function openControlLogicModal() {
    currentControlLogicIndex = 0;
    updateControlLogicDisplay();
    openModal('control-logic-modal');
}

/**
 * Close control logic modal
 * Closes the modal and restores page scroll
 * @returns {void}
 */
function closeControlLogicModal() {
    closeModal('control-logic-modal');
}

/**
 * Navigate to next diagram
 * Advances carousel to next diagram with wrapping (loops back to first)
 * @returns {void}
 */
function nextControlLogicDiagram() {
    currentControlLogicIndex = (currentControlLogicIndex + 1) % controlLogicDiagrams.length;
    updateControlLogicDisplay();
}

/**
 * Navigate to previous diagram
 * Moves carousel to previous diagram with wrapping (loops to last)
 * @returns {void}
 */
function prevControlLogicDiagram() {
    currentControlLogicIndex =
        (currentControlLogicIndex - 1 + controlLogicDiagrams.length) % controlLogicDiagrams.length;
    updateControlLogicDisplay();
}

/**
 * Open control logic image in fullscreen modal
 * Displays the current diagram image in a larger fullscreen view
 * @returns {void}
 */
function openControlLogicImageModal() {
    let controlLogicImage = document.getElementById("control-logic-image-main");
    if (!controlLogicImage) {
        controlLogicImage = document.getElementById("control-logic-image");
    }

    const fullscreenImage = document.getElementById("carousel-image-fullscreen");
    const modal = document.getElementById("carousel-image-modal");

    if (!fullscreenImage || !modal) return;

    fullscreenImage.src = controlLogicImage.src;
    fullscreenImage.alt = controlLogicImage.alt;
    modal.setAttribute("aria-label", "Full screen " + controlLogicDiagrams[currentControlLogicIndex].title);
    openModal('carousel-image-modal');
}

/**
 * Close carousel image modal
 * Closes the fullscreen image modal and restores page scroll
 * @returns {void}
 */
function closeCarouselImageModal() {
    closeModal('carousel-image-modal');
}

/**
 * Open topology modal
 * Opens the network topology diagram modal for detailed viewing
 * @returns {void}
 */
function openTopologyModal() {
    openModal('topology-modal');
}

/**
 * Close topology modal
 * Closes the network topology modal and returns focus to topology image
 * @returns {void}
 */
function closeTopologyModal() {
    const topologyImage = document.getElementById("topology-image");
    closeModal('topology-modal', topologyImage);
}

/**
 * Open organization modal
 * Opens the organization structure and personnel modal
 * @returns {void}
 */
function openOrganizationModal() {
    openModal('organization-modal');
}

/**
 * Close organization modal
 * Closes the organization modal
 * @returns {void}
 */
function closeOrganizationModal() {
    closeModal('organization-modal');
}

/**
 * Open vendor information modal
 * Opens the vendor and external partners modal
 * @returns {void}
 */
function openVendorModal() {
    openModal('vendor-modal');
}

/**
 * Close vendor modal
 * Closes the vendor modal
 * @returns {void}
 */
function closeVendorModal() {
    closeModal('vendor-modal');
}

/**
 * Open network information modal
 * Opens the network architecture and topology modal
 * @returns {void}
 */
function openNetworkModal() {
    openModal('network-modal');
}

/**
 * Close network modal
 * Closes the network modal
 * @returns {void}
 */
function closeNetworkModal() {
    closeModal('network-modal');
}

/**
 * Open OT network details modal
 * Opens the operational technology network details modal
 * @returns {void}
 */
function openOTNetworkModal() {
    openModal('ot-network-modal');
}

/**
 * Close OT network modal
 * Closes the OT network modal
 * @returns {void}
 */
function closeOTNetworkModal() {
    closeModal('ot-network-modal');
}

/**
 * Initialize scenario page functionality
 * Sets up all modals, carousels, keyboard navigation, and interactive elements.
 * Loads diagram data and attaches event listeners for topology and control logic interactions.
 * @returns {Promise<void>} Promise that resolves when initialization is complete
 */
export async function initializeScenario() {
    // Only run on scenario page
    if (!document.getElementById("topology-image")) {
        return;
    }

    // Load diagram data first
    await loadDiagramData();

    // Initialize main page carousel display
    currentControlLogicIndex = 0;
    updateControlLogicDisplay();

    // Topology image event handlers
    const topologyImage = document.getElementById("topology-image");
    if (topologyImage) {
        topologyImage.addEventListener("click", openTopologyModal);
        topologyImage.addEventListener("keydown", function (e) {
            if (e.key === "Enter" || e.key === " ") {
                e.preventDefault();
                openTopologyModal();
            }
        });
    }

    // Topology modal close handlers
    const topologyModal = document.getElementById("topology-modal");
    if (topologyModal) {
        topologyModal.addEventListener("click", closeTopologyModal);
    }

    // Control Logic carousel navigation buttons - Main page
    const prevBtnMain = document.getElementById("control-logic-prev-btn-main");
    const nextBtnMain = document.getElementById("control-logic-next-btn-main");

    if (prevBtnMain) {
        prevBtnMain.addEventListener("click", prevControlLogicDiagram);
    }
    if (nextBtnMain) {
        nextBtnMain.addEventListener("click", nextControlLogicDiagram);
    }

    // Control Logic carousel navigation buttons - Modal
    const prevBtn = document.getElementById("control-logic-prev-btn");
    const nextBtn = document.getElementById("control-logic-next-btn");

    if (prevBtn) {
        prevBtn.addEventListener("click", prevControlLogicDiagram);
    }
    if (nextBtn) {
        nextBtn.addEventListener("click", nextControlLogicDiagram);
    }

    // Control Logic image keyboard support - Main page
    const controlLogicImageMain = document.getElementById("control-logic-image-main");
    if (controlLogicImageMain) {
        controlLogicImageMain.addEventListener("click", openControlLogicImageModal);
        controlLogicImageMain.addEventListener("keydown", function (e) {
            if (e.key === "Enter" || e.key === " ") {
                e.preventDefault();
                openControlLogicImageModal();
            }
        });
    }

    // Control Logic image keyboard support - Modal
    const controlLogicImage = document.getElementById("control-logic-image");
    if (controlLogicImage) {
        controlLogicImage.addEventListener("click", openControlLogicImageModal);
        controlLogicImage.addEventListener("keydown", function (e) {
            if (e.key === "Enter" || e.key === " ") {
                e.preventDefault();
                openControlLogicImageModal();
            }
        });
    }

    // Modal close buttons
    const controlLogicCloseBtn = document.getElementById("control-logic-close-btn");
    if (controlLogicCloseBtn) {
        controlLogicCloseBtn.addEventListener("click", closeControlLogicModal);
    }

    // Organization modal close button
    const organizationCloseBtn = document.getElementById("organization-close-btn");
    if (organizationCloseBtn) {
        organizationCloseBtn.addEventListener("click", closeOrganizationModal);
    }

    // Vendor modal close button
    const vendorCloseBtn = document.getElementById("vendor-close-btn");
    if (vendorCloseBtn) {
        vendorCloseBtn.addEventListener("click", closeVendorModal);
    }

    // Network modal close button
    const networkCloseBtn = document.getElementById("network-close-btn");
    if (networkCloseBtn) {
        networkCloseBtn.addEventListener("click", closeNetworkModal);
    }

    // OT Network modal close button
    const otNetworkCloseBtn = document.getElementById("ot-network-close-btn");
    if (otNetworkCloseBtn) {
        otNetworkCloseBtn.addEventListener("click", closeOTNetworkModal);
    }

    // Carousel image modal close
    const carouselImageModal = document.getElementById("carousel-image-modal");
    if (carouselImageModal) {
        carouselImageModal.addEventListener("click", closeCarouselImageModal);
    }

    // Close modals on Escape key
    document.addEventListener("keydown", function (e) {
        const controlLogicModal = document.getElementById("control-logic-modal");
        const imageModal = document.getElementById("carousel-image-modal");
        const topologyModal = document.getElementById("topology-modal");

        if (e.key === "Escape") {
            if (imageModal && imageModal.classList.contains("active")) {
                closeCarouselImageModal();
            } else if (controlLogicModal && controlLogicModal.classList.contains("active")) {
                closeControlLogicModal();
            } else if (topologyModal && topologyModal.classList.contains("active")) {
                closeTopologyModal();
            }
        }

        // Arrow keys and WASD for carousel navigation
        if (controlLogicModal && controlLogicModal.classList.contains("active")) {
            if (e.key === "ArrowLeft" || e.key === "a" || e.key === "A") {
                e.preventDefault();
                prevControlLogicDiagram();
            } else if (e.key === "ArrowRight" || e.key === "d" || e.key === "D") {
                e.preventDefault();
                nextControlLogicDiagram();
            }
        }
    });

    // Expose functions globally for onclick handlers in HTML
    window.showCarouselArrows = showCarouselArrows;
    window.hideCarouselArrows = hideCarouselArrows;
    window.openTopologyModal = openTopologyModal;
    window.closeTopologyModal = closeTopologyModal;
    window.openControlLogicImageModal = openControlLogicImageModal;
    window.closeCarouselImageModal = closeCarouselImageModal;
    window.openOrganizationModal = openOrganizationModal;
    window.closeOrganizationModal = closeOrganizationModal;
    window.openVendorModal = openVendorModal;
    window.closeVendorModal = closeVendorModal;
    window.openNetworkModal = openNetworkModal;
    window.closeNetworkModal = closeNetworkModal;
    window.openOTNetworkModal = openOTNetworkModal;
    window.closeOTNetworkModal = closeOTNetworkModal;
}
