import { isPagesDirectory } from './utils.js';

// Track last active trigger to restore focus and manage aria-expanded
let lastActiveTrigger = null;
const modalKeydownHandlers = new Map();

/**
 * Open a modal by ID
 * Adds active class, sets aria-hidden to false, prevents body scroll, and focuses modal
 * @param {string} modalId - The ID of the modal element to open
 * @returns {void}
 */
export function openModal(modalId) {
    const modal = document.getElementById(modalId);
    if (!modal) {
        console.warn(`Modal with ID "${modalId}" not found`);
        return;
    }

    // Remember the element that triggered open for focus restore
    lastActiveTrigger = document.activeElement instanceof HTMLElement ? document.activeElement : null;
    if (lastActiveTrigger && lastActiveTrigger.getAttribute('aria-controls') === modalId) {
        lastActiveTrigger.setAttribute('aria-expanded', 'true');
    }

    modal.classList.add("active");
    modal.setAttribute("aria-hidden", "false");
    document.body.style.overflow = "hidden";

    // Focus management: prefer modal-content, else modal itself
    const modalContent = modal.querySelector('.modal-content');
    const focusTarget = (modalContent instanceof HTMLElement ? modalContent : modal);
    if (focusTarget && focusTarget instanceof HTMLElement) {
        if (!focusTarget.hasAttribute('tabindex')) {
            focusTarget.setAttribute('tabindex', '-1');
        }
        focusTarget.focus();
    }

    // Focus trap within modal
    const getFocusable = () => Array.from(modal.querySelectorAll(
        'a[href], area[href], button:not([disabled]), input:not([disabled]), select:not([disabled]), textarea:not([disabled]), [tabindex]:not([tabindex="-1"])'
    )).filter(el => el instanceof HTMLElement && el.offsetParent !== null);

    const trapHandler = (e) => {
        if (e.key !== 'Tab') return;
        const focusables = getFocusable();
        if (focusables.length === 0) return;
        const first = focusables[0];
        const last = focusables[focusables.length - 1];
        if (e.shiftKey) {
            if (document.activeElement === first) {
                e.preventDefault();
                last.focus();
            }
        } else {
            if (document.activeElement === last) {
                e.preventDefault();
                first.focus();
            }
        }
    };
    modal.addEventListener('keydown', trapHandler);
    modalKeydownHandlers.set(modalId, trapHandler);
}

/**
 * Close a modal by ID
 * Removes active class, sets aria-hidden to true, restores body scroll, and optionally returns focus
 * @param {string} modalId - The ID of the modal element to close
 * @param {HTMLElement} [returnFocus=null] - Optional element to return keyboard focus to after closing
 * @returns {void}
 */
export function closeModal(modalId, returnFocus = null) {
    const modal = document.getElementById(modalId);
    if (!modal) {
        console.warn(`Modal with ID "${modalId}" not found`);
        return;
    }

    modal.classList.remove("active");
    modal.setAttribute("aria-hidden", "true");
    document.body.style.overflow = "";

    // Remove focus trap
    const handler = modalKeydownHandlers.get(modalId);
    if (handler) {
        modal.removeEventListener('keydown', handler);
        modalKeydownHandlers.delete(modalId);
    }

    // Restore aria-expanded on trigger and return focus
    let targetToFocus = returnFocus || lastActiveTrigger;
    if (lastActiveTrigger && lastActiveTrigger.getAttribute('aria-controls') === modalId) {
        lastActiveTrigger.setAttribute('aria-expanded', 'false');
    }
    if (targetToFocus && targetToFocus instanceof HTMLElement) {
        targetToFocus.focus();
    }
}

/**
 * Initialize modal event handlers
 * Sets up click and keyboard handlers for opening/closing modals.
 * Handles clickable cards, close buttons, overlay clicks, and Escape key.
 * @returns {void}
 */
export function initializeModals() {
    // Select both clickable cards and any element with data-modal attribute
    const modalTriggers = document.querySelectorAll("[data-modal], .clickable-card");
    const modals = document.querySelectorAll(".modal");

    // Open modal when trigger is clicked
    modalTriggers.forEach((trigger) => {
        // Annotate triggers for a11y
        const targetId = trigger.getAttribute('data-modal');
        if (targetId) {
            if (!trigger.hasAttribute('aria-controls')) {
                trigger.setAttribute('aria-controls', targetId);
            }
            if (!trigger.hasAttribute('aria-expanded')) {
                trigger.setAttribute('aria-expanded', 'false');
            }
        }

        trigger.addEventListener("click", function () {
            const modalId = this.getAttribute("data-modal");
            if (modalId) {
                openModal(modalId);
            }
        });

        // Allow keyboard interaction
        trigger.addEventListener("keypress", function (e) {
            if (e.key === "Enter" || e.key === " ") {
                e.preventDefault();
                this.click();
            }
        });
    });

    // Close modal handlers
    modals.forEach((modal) => {
        const closeBtn = modal.querySelector(".modal-close");
        const overlay = modal.querySelector(".modal-overlay");

        const closeModalFn = () => closeModal(modal.id);

        if (closeBtn) closeBtn.addEventListener("click", closeModalFn);
        if (overlay) overlay.addEventListener("click", closeModalFn);

        // Close modal with Escape key
        document.addEventListener("keydown", function (e) {
            if (e.key === "Escape" && modal.classList.contains("active")) {
                closeModalFn();
            }
        });
    });
}

/**
 * Initialize image carousel functionality
 * Sets up image rotation with click, keyboard (arrows/Enter/Space), and navigation buttons.
 * Automatically adjusts paths based on page directory location.
 * @returns {void}
 */
export function initializeCarousels() {
    const carousels = document.querySelectorAll(".image-carousel");

    carousels.forEach((carousel) => {
        const dataImages = carousel.getAttribute("data-images");
        if (!dataImages) return;

        const images = dataImages.split(",");
        let currentIndex = 0;
        const img = carousel.querySelector(".carousel-image");
        const imageCounter = carousel.querySelector("p");

        // Ensure the counter is announced to assistive tech
        if (imageCounter) {
            imageCounter.setAttribute('role', 'status');
            imageCounter.setAttribute('aria-live', 'polite');
            imageCounter.setAttribute('aria-atomic', 'true');
        }

        // Helper to update image
        const updateImage = () => {
            // Determine path prefix based on location
            const isPagesDir = isPagesDirectory();
            const prefix = isPagesDir ? '../assets/images/' : 'assets/images/';

            const imagePath = prefix + images[currentIndex];
            img.src = imagePath;
            img.alt = "Carousel image - view " + (currentIndex + 1);
            if (imageCounter) {
                imageCounter.textContent =
                    "Click image to rotate views (" +
                    (currentIndex + 1) +
                    "/" +
                    images.length +
                    ")";
            }
        };

        // Add navigation arrows if there are multiple images
        if (images.length > 1 && !carousel.querySelector(".image-carousel-arrow")) {
            // Create left arrow
            const leftArrow = document.createElement("button");
            leftArrow.className = "image-carousel-arrow image-carousel-arrow-left";
            leftArrow.type = 'button';
            leftArrow.setAttribute("aria-label", "Previous image");
            leftArrow.textContent = "❮";
            leftArrow.addEventListener("click", (e) => {
                e.preventDefault();
                currentIndex = (currentIndex - 1 + images.length) % images.length;
                updateImage();
            });

            // Create right arrow
            const rightArrow = document.createElement("button");
            rightArrow.className = "image-carousel-arrow image-carousel-arrow-right";
            rightArrow.type = 'button';
            rightArrow.setAttribute("aria-label", "Next image");
            rightArrow.textContent = "❯";
            rightArrow.addEventListener("click", (e) => {
                e.preventDefault();
                currentIndex = (currentIndex + 1) % images.length;
                updateImage();
            });

            carousel.appendChild(leftArrow);
            carousel.appendChild(rightArrow);
        }

        if (img) {
            img.addEventListener("click", function () {
                currentIndex = (currentIndex + 1) % images.length;
                updateImage();
            });

            // Allow keyboard interaction (Enter to rotate)
            img.setAttribute("tabindex", "0");
            img.addEventListener("keypress", function (e) {
                if (e.key === "Enter" || e.key === " ") {
                    e.preventDefault();
                    this.click();
                }
            });

            // Allow keyboard arrow keys to navigate
            img.addEventListener("keydown", function (e) {
                if (e.key === "ArrowLeft") {
                    e.preventDefault();
                    currentIndex = (currentIndex - 1 + images.length) % images.length;
                    updateImage();
                } else if (e.key === "ArrowRight") {
                    e.preventDefault();
                    currentIndex = (currentIndex + 1) % images.length;
                    updateImage();
                }
            });
        }
    });
}

/**
 * Enhance accessibility features
 * Adds keyboard support (Enter/Space) for details/summary disclosure elements
 * @returns {void}
 */
export function initializeAccessibility() {
    // Add keyboard support for details/summary elements
    const details = document.querySelectorAll('details');
    details.forEach(detail => {
        const summary = detail.querySelector('summary');
        if (summary) {
            summary.addEventListener('keydown', function (e) {
                if (e.key === 'Enter' || e.key === ' ') {
                    e.preventDefault();
                    detail.open = !detail.open;
                }
            });
        }
    });
}
