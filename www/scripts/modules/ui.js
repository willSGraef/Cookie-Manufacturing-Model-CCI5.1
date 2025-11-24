import { isPagesDirectory } from './utils.js';

/**
 * Initialize Modals
 */
export function initializeModals() {
    const clickableCards = document.querySelectorAll(".clickable-card");
    const modals = document.querySelectorAll(".modal");

    // Open modal when card is clicked
    clickableCards.forEach((card) => {
        card.addEventListener("click", function () {
            const modalId = this.getAttribute("data-modal");
            const modal = document.getElementById(modalId);
            if (modal) {
                modal.classList.add("active");
                modal.setAttribute("aria-hidden", "false");
                document.body.style.overflow = "hidden";
            }
        });

        // Allow keyboard interaction
        card.addEventListener("keypress", function (e) {
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

        const closeModal = () => {
            modal.classList.remove("active");
            modal.setAttribute("aria-hidden", "true");
            document.body.style.overflow = "";
        };

        if (closeBtn) closeBtn.addEventListener("click", closeModal);
        if (overlay) overlay.addEventListener("click", closeModal);

        // Close modal with Escape key
        document.addEventListener("keydown", function (e) {
            if (e.key === "Escape" && modal.classList.contains("active")) {
                closeModal();
            }
        });
    });
}

/**
 * Initialize Image Carousels
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
