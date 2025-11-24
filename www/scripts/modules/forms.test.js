/**
 * Unit tests for form validation module
 * Tests email validation, required field validation, and form validation
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import { validateEmail, validateRequired, validateForm, initializeFormHandlers } from './forms.js';

describe('Form Validation Module', () => {
    let mockForm;
    let mockEmailInput;
    let mockRequiredInput;

    beforeEach(() => {
        // Reset DOM before each test
        document.body.innerHTML = '';

        // Create mock form
        mockForm = document.createElement('form');
        mockForm.id = 'test-form';

        // Create mock email input
        mockEmailInput = document.createElement('input');
        mockEmailInput.type = 'email';
        mockEmailInput.id = 'email';
        mockEmailInput.name = 'email';
        mockEmailInput.setAttribute('aria-label', 'Email');

        // Create mock required input
        mockRequiredInput = document.createElement('input');
        mockRequiredInput.type = 'text';
        mockRequiredInput.id = 'name';
        mockRequiredInput.name = 'name';
        mockRequiredInput.required = true;
        mockRequiredInput.setAttribute('aria-label', 'Name');

        // Add error message containers
        const emailError = document.createElement('span');
        emailError.className = 'error-message';
        emailError.id = 'email-error';
        emailError.style.display = 'none';

        const nameError = document.createElement('span');
        nameError.className = 'error-message';
        nameError.id = 'name-error';
        nameError.style.display = 'none';

        mockForm.appendChild(mockEmailInput);
        mockForm.appendChild(emailError);
        mockForm.appendChild(mockRequiredInput);
        mockForm.appendChild(nameError);

        document.body.appendChild(mockForm);
    });

    describe('validateEmail', () => {
        it('should return true for valid email addresses', () => {
            const validEmails = [
                'user@example.com',
                'test.user@domain.co.uk',
                'name+tag@company.org',
                'user123@test-domain.com'
            ];

            validEmails.forEach(email => {
                mockEmailInput.value = email;
                expect(validateEmail(mockEmailInput)).toBe(true);
                expect(mockEmailInput.style.borderColor).toBe('');
                expect(mockEmailInput.getAttribute('aria-invalid')).toBeNull();
            });
        });

        it('should return false for invalid email addresses', () => {
            const invalidEmails = [
                'notanemail',
                'missing@domain',
                '@nodomain.com',
                'spaces in@email.com',
                'double@@domain.com'
            ];

            invalidEmails.forEach(email => {
                mockEmailInput.value = email;
                expect(validateEmail(mockEmailInput)).toBe(false);
                expect(mockEmailInput.style.borderColor).toBe('var(--color-accent-red)');
                expect(mockEmailInput.getAttribute('aria-invalid')).toBe('true');
            });
        });

        it('should return true for empty email field', () => {
            mockEmailInput.value = '';
            expect(validateEmail(mockEmailInput)).toBe(true);
            expect(mockEmailInput.style.borderColor).toBe('');
        });

        it('should display error message for invalid email', () => {
            mockEmailInput.value = 'invalid-email';
            validateEmail(mockEmailInput);

            const errorMsg = mockEmailInput.nextElementSibling;
            expect(errorMsg.textContent).toContain('valid email address');
            expect(errorMsg.style.display).toBe('block');
        });

        it('should clear error message for valid email', () => {
            // First set invalid
            mockEmailInput.value = 'invalid';
            validateEmail(mockEmailInput);

            // Then set valid
            mockEmailInput.value = 'valid@email.com';
            validateEmail(mockEmailInput);

            const errorMsg = mockEmailInput.nextElementSibling;
            expect(errorMsg.textContent).toBe('');
            expect(errorMsg.style.display).toBe('none');
        });
    });

    describe('validateRequired', () => {
        it('should return true for non-empty field', () => {
            mockRequiredInput.value = 'Some value';
            expect(validateRequired(mockRequiredInput)).toBe(true);
            expect(mockRequiredInput.style.borderColor).toBe('');
            expect(mockRequiredInput.getAttribute('aria-invalid')).toBeNull();
        });

        it('should return false for empty field', () => {
            mockRequiredInput.value = '';
            expect(validateRequired(mockRequiredInput)).toBe(false);
            expect(mockRequiredInput.style.borderColor).toBe('var(--color-accent-red)');
            expect(mockRequiredInput.getAttribute('aria-invalid')).toBe('true');
        });

        it('should return false for whitespace-only field', () => {
            mockRequiredInput.value = '   ';
            expect(validateRequired(mockRequiredInput)).toBe(false);
            expect(mockRequiredInput.style.borderColor).toBe('var(--color-accent-red)');
        });

        it('should display error message for empty required field', () => {
            mockRequiredInput.value = '';
            validateRequired(mockRequiredInput);

            const errorMsg = mockRequiredInput.nextElementSibling;
            expect(errorMsg.textContent).toContain('is required');
            expect(errorMsg.style.display).toBe('block');
        });

        it('should use aria-label for error message', () => {
            mockRequiredInput.value = '';
            validateRequired(mockRequiredInput);

            const errorMsg = mockRequiredInput.nextElementSibling;
            expect(errorMsg.textContent).toContain('Name');
        });
    });

    describe('validateForm', () => {
        it('should return true when all fields are valid', () => {
            mockEmailInput.value = 'valid@email.com';
            mockRequiredInput.value = 'Valid Name';

            const result = validateForm(mockForm);
            expect(result).toBe(true);
        });

        it('should return false when required field is empty', () => {
            mockEmailInput.value = 'valid@email.com';
            mockRequiredInput.value = '';

            const result = validateForm(mockForm);
            expect(result).toBe(false);
        });

        it('should return false when email is invalid', () => {
            mockEmailInput.value = 'invalid-email';
            mockRequiredInput.value = 'Valid Name';

            const result = validateForm(mockForm);
            expect(result).toBe(false);
        });

        it('should validate all fields in form', () => {
            mockEmailInput.value = 'invalid';
            mockRequiredInput.value = '';

            const result = validateForm(mockForm);
            expect(result).toBe(false);

            // Both fields should show errors
            expect(mockEmailInput.getAttribute('aria-invalid')).toBe('true');
            expect(mockRequiredInput.getAttribute('aria-invalid')).toBe('true');
        });

        it('should clear errors when fields become valid', () => {
            // First make invalid
            mockEmailInput.value = 'invalid';
            mockRequiredInput.value = '';
            validateForm(mockForm);

            // Then make valid
            mockEmailInput.value = 'valid@email.com';
            mockRequiredInput.value = 'Valid Name';
            validateForm(mockForm);

            expect(mockEmailInput.getAttribute('aria-invalid')).toBeNull();
            expect(mockRequiredInput.getAttribute('aria-invalid')).toBeNull();
        });

        it('should update validation summary when it exists', () => {
            // Create validation summary first (normally done by initializeFormHandlers)
            const summary = document.createElement('div');
            summary.className = 'validation-summary alert alert-danger';
            summary.style.display = 'none';
            mockForm.insertBefore(summary, mockForm.firstChild);

            mockEmailInput.value = 'invalid';
            mockRequiredInput.value = '';

            validateForm(mockForm);

            expect(summary).not.toBeNull();
            expect(summary.style.display).toBe('block');
            expect(summary.innerHTML).toContain('Please fix the following errors');
        });

        it('should hide validation summary when all fields are valid', () => {
            // Create validation summary first
            const summary = document.createElement('div');
            summary.className = 'validation-summary alert alert-danger';
            summary.style.display = 'none';
            mockForm.insertBefore(summary, mockForm.firstChild);

            // First make invalid
            mockEmailInput.value = 'invalid';
            mockRequiredInput.value = '';
            validateForm(mockForm);

            // Then make valid
            mockEmailInput.value = 'valid@email.com';
            mockRequiredInput.value = 'Valid Name';
            validateForm(mockForm);

            expect(summary.style.display).toBe('none');
        });
    });

    describe('initializeFormHandlers', () => {
        it('should add validation summary to forms without one', () => {
            initializeFormHandlers();

            const summary = mockForm.querySelector('.validation-summary');
            expect(summary).not.toBeNull();
            expect(summary.className).toBe('validation-summary alert alert-danger');
            expect(summary.getAttribute('role')).toBe('alert');
        });

        it('should add submit event listener to form', () => {
            initializeFormHandlers();

            const submitHandler = vi.fn((e) => e.preventDefault());
            mockForm.addEventListener('submit', submitHandler);

            // Trigger invalid form submission
            mockEmailInput.value = 'invalid';
            mockRequiredInput.value = '';

            const event = new Event('submit', { bubbles: true, cancelable: true });
            mockForm.dispatchEvent(event);

            expect(event.defaultPrevented).toBe(true);
        });

        it('should add error message containers to inputs', () => {
            // Remove existing error containers
            const errors = mockForm.querySelectorAll('.error-message');
            errors.forEach(e => e.remove());

            initializeFormHandlers();

            const emailError = mockEmailInput.nextElementSibling;
            const nameError = mockRequiredInput.nextElementSibling;

            expect(emailError).not.toBeNull();
            expect(emailError.className).toBe('error-message');
            expect(nameError).not.toBeNull();
            expect(nameError.className).toBe('error-message');
        });

        it('should add blur event listeners to email inputs', () => {
            initializeFormHandlers();

            mockEmailInput.value = 'invalid';
            const event = new Event('blur');
            mockEmailInput.dispatchEvent(event);

            expect(mockEmailInput.getAttribute('aria-invalid')).toBe('true');
        });

        it('should add blur event listeners to required fields', () => {
            initializeFormHandlers();

            mockRequiredInput.value = '';
            const event = new Event('blur');
            mockRequiredInput.dispatchEvent(event);

            expect(mockRequiredInput.getAttribute('aria-invalid')).toBe('true');
        });

        it('should clear errors on input event', () => {
            initializeFormHandlers();

            // First trigger validation error
            mockRequiredInput.value = '';
            const blurEvent = new Event('blur');
            mockRequiredInput.dispatchEvent(blurEvent);

            // Then type something
            mockRequiredInput.value = 'Some text';
            const inputEvent = new Event('input');
            mockRequiredInput.dispatchEvent(inputEvent);

            expect(mockRequiredInput.style.borderColor).toBe('');
        });
    });
});
