/**
 * Initialize form validation handlers for all forms on the page
 * Sets up real-time validation, error messages, and accessibility attributes.
 * Adds blur and input listeners to email and required fields.
 * @returns {void}
 */
export function initializeFormHandlers() {
    const forms = document.querySelectorAll('form');

    forms.forEach(form => {
        // Add validation summary container if it doesn't exist
        if (!form.querySelector('.validation-summary')) {
            const summary = document.createElement('div');
            summary.className = 'validation-summary alert alert-danger';
            summary.style.display = 'none';
            summary.setAttribute('role', 'alert');
            summary.setAttribute('aria-live', 'polite');
            form.insertBefore(summary, form.firstChild);
        }

        form.addEventListener('submit', function (e) {
            if (!validateForm(this)) {
                e.preventDefault();
                return false;
            }
        });

        // Real-time validation for email fields
        const emailInputs = form.querySelectorAll('input[type="email"]');
        emailInputs.forEach(input => {
            // Add error message container if it doesn't exist
            if (!input.nextElementSibling || !input.nextElementSibling.classList.contains('error-message')) {
                const errorMsg = document.createElement('span');
                errorMsg.className = 'error-message';
                errorMsg.style.display = 'none';
                errorMsg.setAttribute('role', 'alert');
                input.parentNode.insertBefore(errorMsg, input.nextSibling);

                // Link error message to input
                const errorId = `${input.id || input.name}-error`;
                errorMsg.id = errorId;
                input.setAttribute('aria-describedby', errorId);
            }

            input.addEventListener('blur', function () {
                validateEmail(this);
            });

            input.addEventListener('input', function () {
                // Clear error on input
                if (this.value.trim()) {
                    clearFieldError(this);
                }
            });
        });

        // Real-time validation for required fields
        const requiredFields = form.querySelectorAll('[required]');
        requiredFields.forEach(field => {
            // Add error message container if it doesn't exist
            if (!field.nextElementSibling || !field.nextElementSibling.classList.contains('error-message')) {
                const errorMsg = document.createElement('span');
                errorMsg.className = 'error-message';
                errorMsg.style.display = 'none';
                errorMsg.setAttribute('role', 'alert');
                field.parentNode.insertBefore(errorMsg, field.nextSibling);

                // Link error message to input
                const errorId = `${field.id || field.name}-error`;
                errorMsg.id = errorId;
                const existingDescribedBy = field.getAttribute('aria-describedby');
                field.setAttribute('aria-describedby', existingDescribedBy ? `${existingDescribedBy} ${errorId}` : errorId);
            }

            field.addEventListener('blur', function () {
                validateRequired(this);
            });

            field.addEventListener('input', function () {
                // Clear error on input
                if (this.value.trim()) {
                    clearFieldError(this);
                }
            });
        });
    });
}

/**
 * Clear error styling and message for a form field
 * Removes red border, aria-invalid attribute, and hides error message
 * @param {HTMLElement} field - The form input element to clear errors from
 * @returns {void}
 */
function clearFieldError(field) {
    field.style.borderColor = '';
    field.removeAttribute('aria-invalid');

    const errorMsg = field.nextElementSibling;
    if (errorMsg && errorMsg.classList.contains('error-message')) {
        errorMsg.style.display = 'none';
        errorMsg.textContent = '';
    }
}

/**
 * Show error styling and message for a form field
 * Adds red border, sets aria-invalid, and displays error message below field
 * @param {HTMLElement} field - The form input element to show error on
 * @param {string} message - The error message text to display
 * @returns {void}
 */
function showFieldError(field, message) {
    field.style.borderColor = 'var(--color-accent-red)';
    field.setAttribute('aria-invalid', 'true');

    const errorMsg = field.nextElementSibling;
    if (errorMsg && errorMsg.classList.contains('error-message')) {
        errorMsg.textContent = message;
        errorMsg.style.display = 'block';
    }
}

/**
 * Update validation summary at the top of the form
 * Displays a list of all current validation errors or hides summary if no errors
 * @param {HTMLFormElement} form - The form element containing the validation summary
 * @param {Array<string>} errors - Array of error message strings to display
 * @returns {void}
 */
function updateValidationSummary(form, errors) {
    const summary = form.querySelector('.validation-summary');
    if (!summary) return;

    if (errors.length === 0) {
        summary.style.display = 'none';
        summary.innerHTML = '';
    } else {
        summary.style.display = 'block';
        summary.innerHTML = `
            <p><strong>Please fix the following errors:</strong></p>
            <ul>
                ${errors.map(error => `<li>${error}</li>`).join('')}
            </ul>
        `;
        summary.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
    }
}

/**
 * Validate all fields in a form
 * Checks all required fields and email fields, displays errors, and updates validation summary
 * @param {HTMLFormElement} form - The form element to validate
 * @returns {boolean} True if all validations pass, false if any field is invalid
 */
export function validateForm(form) {
    const requiredFields = form.querySelectorAll('[required]');
    const emailFields = form.querySelectorAll('input[type="email"]');
    let isValid = true;
    const errors = [];

    // Validate required fields
    requiredFields.forEach(field => {
        if (!field.value.trim()) {
            const fieldName = field.getAttribute('aria-label') || field.name || 'This field';
            showFieldError(field, `${fieldName} is required.`);
            errors.push(`${fieldName} is required`);
            isValid = false;
        } else {
            clearFieldError(field);
        }
    });

    // Validate email fields
    emailFields.forEach(field => {
        if (field.value.trim() && !validateEmail(field)) {
            const fieldName = field.getAttribute('aria-label') || field.name || 'Email';
            errors.push(`${fieldName} must be a valid email address`);
            isValid = false;
        }
    });

    // Update validation summary
    updateValidationSummary(form, errors);

    return isValid;
}

/**
 * Validate email format using regex pattern
 * Checks if email matches standard email format (user@domain.extension)
 * @param {HTMLInputElement} emailInput - The email input element to validate
 * @returns {boolean} True if email is valid or empty, false if invalid format
 */
export function validateEmail(emailInput) {
    const email = emailInput.value.trim();
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

    if (email && !emailRegex.test(email)) {
        const fieldName = emailInput.getAttribute('aria-label') || emailInput.name || 'Email';
        showFieldError(emailInput, `${fieldName} must be a valid email address (e.g., user@example.com).`);
        return false;
    } else {
        clearFieldError(emailInput);
        return true;
    }
}

/**
 * Validate required field has a value
 * Checks if field has non-empty trimmed value
 * @param {HTMLInputElement} field - The input element to validate
 * @returns {boolean} True if field has value, false if empty
 */
export function validateRequired(field) {
    if (!field.value.trim()) {
        const fieldName = field.getAttribute('aria-label') || field.name || 'This field';
        showFieldError(field, `${fieldName} is required.`);
        return false;
    } else {
        clearFieldError(field);
        return true;
    }
}
