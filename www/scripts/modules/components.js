import { adjustPaths, isPagesDirectory } from "./utils.js";

/**
 * Show loading state for a placeholder element
 * Displays a loading message while component content is being fetched
 * @param {HTMLElement} placeholder - The placeholder element to show loading state in
 * @returns {void}
 */
function showLoadingState(placeholder) {
  if (!placeholder) return;
  placeholder.innerHTML = `
    <div class="component-loading">
      <p>Loading...</p>
    </div>
  `;
}

/**
 * Show error message for failed component load
 * Displays an error message with a retry button when component loading fails
 * @param {HTMLElement} placeholder - The placeholder element to show error state in
 * @param {string} componentName - Name of the component that failed to load (e.g., "header", "footer")
 * @param {Function} retryFn - Callback function to retry loading the component
 * @returns {void}
 */
function showErrorState(placeholder, componentName, retryFn) {
  if (!placeholder) return;

  const errorDiv = document.createElement('div');
  errorDiv.className = 'component-error alert alert-danger';
  errorDiv.setAttribute('role', 'alert');
  

  errorDiv.innerHTML = `
    <p><strong>Error:</strong> Failed to load ${componentName}.</p>
    <button class="btn btn-small retry-btn mt-sm">
      Retry
    </button>
  `;

  placeholder.innerHTML = '';
  placeholder.appendChild(errorDiv);

  // Add event listener to retry button
  const retryBtn = errorDiv.querySelector('.retry-btn');
  if (retryBtn) {
    retryBtn.addEventListener('click', () => {
      retryFn();
    });
  }
}

/**
 * Fetch with retry logic and exponential backoff
 * Attempts to fetch a resource multiple times with increasing delays between retries
 * @param {string} url - The URL to fetch
 * @param {number} [maxRetries=2] - Maximum number of retry attempts after initial failure
 * @returns {Promise<Response>} Promise that resolves to the fetch Response object
 * @throws {Error} Throws the last error if all retry attempts fail
 */
async function fetchWithRetry(url, maxRetries = 2) {
  let lastError;

  for (let i = 0; i <= maxRetries; i++) {
    try {
      const response = await fetch(url);
      if (response.ok) {
        return response;
      }
      lastError = new Error(`HTTP ${response.status}: ${response.statusText}`);
    } catch (error) {
      lastError = error;
      // Wait before retrying (exponential backoff)
      if (i < maxRetries) {
        await new Promise(resolve => setTimeout(resolve, Math.pow(2, i) * 500));
      }
    }
  }

  throw lastError;
}

/**
 * Load shared components (Header, Footer)
 * Dynamically fetches and injects header and footer HTML into their respective placeholders.
 * Adjusts paths based on current directory location and handles errors with retry logic.
 * @returns {Promise<void>} Promise that resolves when both components are loaded
 */
export async function loadComponents() {
  const headerPlaceholder = document.getElementById("header-placeholder");
  const footerPlaceholder = document.getElementById("footer-placeholder");

  // Determine path to components based on current location
  const isPagesDir = isPagesDirectory();
  const basePath = isPagesDir ? "../" : "";

  const loadHeader = async () => {
    if (headerPlaceholder) {
      showLoadingState(headerPlaceholder);
      try {
        const response = await fetchWithRetry(`${basePath}components/header.html`);
        const html = await response.text();
        // Adjust paths in header HTML if we are in pages dir
        const adjustedHtml = adjustPaths(html, isPagesDir);
        headerPlaceholder.innerHTML = adjustedHtml;
        return true;
      } catch (error) {
        console.error("Error loading header:", error);
        showErrorState(headerPlaceholder, "header", loadHeader);
        return false;
      }
    }
    return false;
  };

  const loadFooter = async () => {
    if (footerPlaceholder) {
      showLoadingState(footerPlaceholder);
      try {
        const response = await fetchWithRetry(`${basePath}components/footer.html`);
        const html = await response.text();
        const adjustedHtml = adjustPaths(html, isPagesDir);
        footerPlaceholder.innerHTML = adjustedHtml;
        return true;
      } catch (error) {
        console.error("Error loading footer:", error);
        showErrorState(footerPlaceholder, "footer", loadFooter);
        return false;
      }
    }
    return false;
  };

  await Promise.all([loadHeader(), loadFooter()]);
}
