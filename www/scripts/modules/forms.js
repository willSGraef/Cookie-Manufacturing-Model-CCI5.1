/**
 * Handle form submissions and validations
 */
export function initializeFormHandlers() {
    const forms = document.querySelectorAll('form');

    forms.forEach(form => {
        form.addEventListener('submit', function (e) {
            if (!validateForm(this)) {
                e.preventDefault();
                return false;
            }
        });

        // Real-time validation for email fields
        const emailInputs = form.querySelectorAll('input[type="email"]');
        emailInputs.forEach(input => {
            input.addEventListener('blur', function () {
                validateEmail(this);
            });
        });

        // Real-time validation for required fields
        const requiredFields = form.querySelectorAll('[required]');
        requiredFields.forEach(field => {
            field.addEventListener('blur', function () {
                validateRequired(this);
            });
        });
    });
}

/**
 * Validate form data
 * @param {HTMLFormElement} form
 * @returns {boolean}
 */
export function validateForm(form) {
    const requiredFields = form.querySelectorAll('[required]');
    let isValid = true;

    requiredFields.forEach(field => {
        if (!field.value.trim()) {
            field.style.borderColor = 'var(--color-accent-red)';
            field.setAttribute('aria-invalid', 'true');
            isValid = false;
        } else {
            field.style.borderColor = '';
            field.removeAttribute('aria-invalid');
        }
    });

    return isValid;
}

/**
 * Validate email format
 * @param {HTMLInputElement} emailInput
 * @returns {boolean}
 */
export function validateEmail(emailInput) {
    const email = emailInput.value.trim();
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;

    if (email && !emailRegex.test(email)) {
        emailInput.style.borderColor = 'var(--color-accent-red)';
        emailInput.setAttribute('aria-invalid', 'true');
        return false;
    } else {
        emailInput.style.borderColor = '';
        emailInput.removeAttribute('aria-invalid');
        return true;
    }
}

/**
 * Validate required field
 * @param {HTMLInputElement} field
 * @returns {boolean}
 */
export function validateRequired(field) {
    if (!field.value.trim()) {
        field.style.borderColor = 'var(--color-accent-red)';
        field.setAttribute('aria-invalid', 'true');
        return false;
    } else {
        field.style.borderColor = '';
        field.removeAttribute('aria-invalid');
        return true;
    }
}
