import { adjustPaths, isPagesDirectory } from "./utils.js";

/**
 * Load shared components (Header, Footer)
 */
export async function loadComponents() {
  const headerPlaceholder = document.getElementById("header-placeholder");
  const footerPlaceholder = document.getElementById("footer-placeholder");

  // Determine path to components based on current location
  const isPagesDir = isPagesDirectory();
  const basePath = isPagesDir ? "../" : "";

  const loadHeader = async () => {
    if (headerPlaceholder) {
      try {
        const response = await fetch(`${basePath}components/header.html`);
        if (response.ok) {
          const html = await response.text();
          // Adjust paths in header HTML if we are in pages dir
          const adjustedHtml = adjustPaths(html, isPagesDir);
          headerPlaceholder.innerHTML = adjustedHtml;
          return true;
        }
      } catch (error) {
        console.error("Error loading header:", error);
      }
    }
    return false;
  };

  const loadFooter = async () => {
    if (footerPlaceholder) {
      try {
        const response = await fetch(`${basePath}components/footer.html`);
        if (response.ok) {
          const html = await response.text();
          const adjustedHtml = adjustPaths(html, isPagesDir);
          footerPlaceholder.innerHTML = adjustedHtml;
          return true;
        }
      } catch (error) {
        console.error("Error loading footer:", error);
      }
    }
    return false;
  };

  await Promise.all([loadHeader(), loadFooter()]);
}
