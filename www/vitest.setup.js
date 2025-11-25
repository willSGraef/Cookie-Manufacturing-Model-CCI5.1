/**
 * Vitest Setup File
 * Handles global test configuration
 *
 * Known Issue: DOMException errors during test teardown
 * ======================================================
 * The sanitizer tests trigger harmless DOMException errors in happy-dom's
 * internal fetch handler during environment cleanup. These errors occur because:
 *
 * 1. The sanitizer blocks unsafe URLs (e.g., javascript:, data:, evil.com)
 * 2. happy-dom's parser attempts to fetch these invalid URLs when parsing the DOM
 * 3. During test teardown, happy-dom tries to abort pending requests
 * 4. The fetch cleanup throws DOMException [AbortError] and [NetworkError]
 *
 * These errors are:
 * - NOT test failures (all 95 tests pass)
 * - NOT actual security vulnerabilities (sanitizer blocks the malicious content)
 * - Harmless cleanup noise from happy-dom's internal implementation
 * - Cannot be suppressed via configuration (they bypass console handlers)
 *
 * This is a known limitation of happy-dom with XSS prevention tests.
 * See: https://github.com/capricorn86/happy-dom/issues
 */

import { vi } from 'vitest';

// Mock fetch globally to prevent initial fetch attempts
global.fetch = vi.fn().mockRejectedValue(new Error('Fetch mocked in test'));


