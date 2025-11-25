/**
 * Assessment page functionality
 * Manages task modals, deliverables modal, and task completion tracking
 */

import { isPagesDirectory } from './utils.js';
import { openModal, closeModal } from './ui.js';

// Task data - loaded from external JSON
let taskData = {};

// Current active task
let currentTaskId = null;

/**
 * Load task data from external JSON file
 * Fetches task descriptions, instructions, and metadata from data/tasks.json.
 * Shows loading indicator and handles errors gracefully.
 * @returns {Promise<Object>} Promise resolving to task data object with task IDs as keys
 * @throws {Error} Logs error and displays user-friendly message if loading fails
 */
async function loadTaskData() {
    try {
        const basePath = isPagesDirectory() ? '../' : '';

        // Show loading indicator
        const mainContent = document.querySelector('main');
        if (mainContent && !taskData || Object.keys(taskData).length === 0) {
            const loadingDiv = document.createElement('div');
            loadingDiv.className = 'data-loading';
            loadingDiv.style.cssText = 'text-align: center; padding: var(--spacing-xl); color: var(--color-text-muted);';
            loadingDiv.innerHTML = '<p>Loading task data...</p>';
            mainContent.insertBefore(loadingDiv, mainContent.firstChild);
        }

        const response = await fetch(`${basePath}data/tasks.json`);
        if (!response.ok) {
            throw new Error(`Failed to load tasks: ${response.status}`);
        }
        taskData = await response.json();

        // Remove loading indicator
        const loadingDiv = document.querySelector('.data-loading');
        if (loadingDiv) {
            loadingDiv.remove();
        }

        return taskData;
    } catch (error) {
        console.error('Error loading task data:', error);

        // Remove loading indicator
        const loadingDiv = document.querySelector('.data-loading');
        if (loadingDiv) {
            loadingDiv.remove();
        }

        // Fallback: show error to user
        const errorMsg = document.createElement('div');
        errorMsg.className = 'alert alert-danger';
        errorMsg.setAttribute('role', 'alert');
        errorMsg.innerHTML = '<p><strong>Error:</strong> Unable to load task data. Please refresh the page.</p>';
        document.querySelector('main')?.prepend(errorMsg);
        return {};
    }
}

/**
 * Initialize assessment page functionality
 * Sets up task modal interactions, deliverables modal, and task completion tracking.
 * Loads task data from external JSON and attaches event listeners to all interactive elements.
 * @returns {Promise<void>} Promise that resolves when initialization is complete
 */
export async function initializeAssessment() {
    // Load task data first
    await loadTaskData();

    // Task Modal setup
    const taskModal = document.getElementById("task-modal");
    const taskModalClose = document.querySelector("#task-modal .modal-close");
    const viewDetailsButtons = document.querySelectorAll(".view-details-btn");
    const taskCompleteBtn = document.getElementById("task-complete-btn");
    const taskCloseBtn = document.getElementById("task-close-btn");

    // Deliverables Modal setup
    const deliverablesModal = document.getElementById("deliverables-modal");
    const viewDeliverablesBtn = document.getElementById("view-deliverables-btn");
    const deliverablesCloseBtn = document.getElementById("deliverables-close-btn");

    if (!taskModal || !deliverablesModal) {
        // Silently return - assessment modals only exist on assessment page
        return;
    }

    // Open Task Details modal when View Details is clicked
    viewDetailsButtons.forEach((btn) => {
        btn.addEventListener("click", function (e) {
            e.preventDefault();
            const taskId = this.getAttribute("data-task");
            currentTaskId = taskId;

            // Update modal title and description
            const task = taskData[taskId];
            document.getElementById("modal-task-title").textContent = task.title;
            document.getElementById("modal-task-description").textContent =
                task.description;
            document.getElementById("modal-task-instructions").innerHTML =
                task.instructions;

            // Change status to "Started"
            const statusBadge = document.querySelector(
                `[data-task="${taskId}"] .task-status`
            );
            if (statusBadge) {
                statusBadge.textContent = "Started";
                statusBadge.classList.remove("badge-warning");
                statusBadge.classList.add("badge-info");
            }

            // Open modal using unified function
            openModal('task-modal');
        });
    });

    // Open Deliverables modal
    if (viewDeliverablesBtn) {
        viewDeliverablesBtn.addEventListener("click", function (e) {
            e.preventDefault();
            openModal('deliverables-modal');
        });
    }

    // Close Task modal
    function closeTaskModal() {
        closeModal('task-modal');
        currentTaskId = null;
    }

    // Close Deliverables modal
    function closeDeliverablesModal() {
        closeModal('deliverables-modal');
    }

    // Task modal close handlers
    if (taskModalClose) taskModalClose.addEventListener("click", closeTaskModal);
    if (taskCloseBtn) taskCloseBtn.addEventListener("click", closeTaskModal);

    // Deliverables modal close handlers
    if (deliverablesCloseBtn) {
        deliverablesCloseBtn.addEventListener("click", closeDeliverablesModal);
    }

    // Close modals on overlay click
    const overlays = document.querySelectorAll(".modal-overlay");
    overlays.forEach((overlay) => {
        overlay.addEventListener("click", function () {
            if (taskModal.classList.contains("active")) {
                closeTaskModal();
            }
            if (deliverablesModal.classList.contains("active")) {
                closeDeliverablesModal();
            }
        });
    });

    // Close modals on Escape key
    document.addEventListener("keydown", function (e) {
        if (e.key === "Escape") {
            if (taskModal.classList.contains("active")) {
                closeTaskModal();
            }
            if (deliverablesModal.classList.contains("active")) {
                closeDeliverablesModal();
            }
        }
    });

    // Complete task
    if (taskCompleteBtn) {
        taskCompleteBtn.addEventListener("click", function () {
            // Change status to "Finished"
            const statusBadge = document.querySelector(
                `[data-task="${currentTaskId}"] .task-status`
            );
            if (statusBadge) {
                statusBadge.textContent = "Finished";
                statusBadge.classList.remove("badge-warning", "badge-info");
                statusBadge.classList.add("badge-success");
            }

            // Show confirmation message
            alert("Task marked as complete!");

            // Close modal
            closeTaskModal();
        });
    }
}
