# Testing Documentation

## Overview

This project uses [Vitest](https://vitest.dev/) as the testing framework with Happy DOM for browser environment simulation. The test suite focuses on utility functions with high coverage to ensure code quality and reliability.

## Running Tests

```bash
# Run tests in watch mode
npm test

# Run tests once
npm test -- --run

# Run tests with UI
npm run test:ui

# Run tests with coverage report
npm run test:coverage
```

## Test Coverage

The project targets **70% coverage for utility modules**, which has been achieved:

| Module         | Statements | Branches | Functions | Lines   |
|----------------|------------|----------|-----------|---------|
| **forms.js**       | 98%        | 68.96%   | 94.44%    | 97.97%  |
| **navigation.js**  | 100%       | 91.3%    | 100%      | 100%    |
| **utils.js**       | 100%       | 100%     | 100%      | 100%    |

### Coverage Reports

After running `npm run test:coverage`, coverage reports are generated in the `coverage/` directory:

- **HTML Report**: `coverage/index.html` - Open in browser for detailed interactive report
- **LCOV Report**: `coverage/lcov.info` - For integration with CI/CD tools
- **Text Report**: Displayed in terminal

## Test Structure

Tests are organized alongside their corresponding modules:

```sh
scripts/modules/
├── forms.js
├── forms.test.js          # Tests for form validation
├── navigation.js
├── navigation.test.js     # Tests for navigation logic
├── utils.js
└── utils.test.js          # Tests for utility functions
```

## Test Suites

### 1. Form Validation Tests (`forms.test.js`)

Tests the form validation functionality including:

- **Email Validation**: Valid/invalid email formats, error messages
- **Required Field Validation**: Empty/whitespace detection, error display
- **Form Validation**: Complete form validation, error summary management
- **Initialization**: Event listener setup, real-time validation

**Total Tests**: 23 tests

### 2. Navigation Tests (`navigation.test.js`)

Tests the navigation system including:

- **Active State Management**: Highlight current page, aria-current attributes
- **Breadcrumb Generation**: Dynamic breadcrumb creation, path adjustment
- **Integration**: Navigation and breadcrumb working together

**Total Tests**: 23 tests

### 3. Utility Tests (`utils.test.js`)

Tests utility functions including:

- **Scroll Functionality**: Smooth scrolling to sections
- **Path Adjustment**: Relative path corrections for nested directories
- **Page Detection**: Current page identification, directory detection
- **Integration**: Path adjustment with directory detection

**Total Tests**: 25 tests

## Writing New Tests

### Example Test Structure

```javascript
import { describe, it, expect, beforeEach } from 'vitest';
import { myFunction } from './myModule.js';

describe('My Module', () => {
  beforeEach(() => {
    // Setup code runs before each test
  });

  describe('myFunction', () => {
    it('should do something specific', () => {
      // Arrange
      const input = 'test';
      
      // Act
      const result = myFunction(input);
      
      // Assert
      expect(result).toBe('expected');
    });
  });
});
```

### Best Practices

1. **Descriptive Test Names**: Use clear "should" statements
2. **Arrange-Act-Assert**: Structure tests in three clear phases
3. **Isolated Tests**: Each test should be independent
4. **Mock External Dependencies**: Use Vitest mocking utilities
5. **Test Edge Cases**: Empty inputs, null values, boundary conditions
6. **Clean Up**: Use `beforeEach`/`afterEach` for setup/teardown

## DOM Testing

Tests use Happy DOM to simulate browser environment:

```javascript
beforeEach(() => {
  // Reset DOM
  document.body.innerHTML = '';
  
  // Create test elements
  const element = document.createElement('div');
  document.body.appendChild(element);
});
```

## Mock Functions

Use Vitest's `vi` utilities for mocking:

```javascript
import { vi } from 'vitest';

const mockFn = vi.fn();
mockFn.mockReturnValue('test');
expect(mockFn).toHaveBeenCalled();
```

## Continuous Integration

The test suite is designed to run in CI/CD pipelines:

```yaml
# Example GitHub Actions workflow
- name: Run Tests
  run: npm test -- --run

- name: Generate Coverage
  run: npm run test:coverage
```

## Future Testing Goals

While utility modules achieve 97-100% coverage, the following modules are not yet tested:

- `ui.js` - Modal and carousel interactions
- `components.js` - Dynamic component loading
- `assessment.js` - Assessment page functionality
- `scenario.js` - Scenario page functionality

These modules are considered lower priority as they are primarily UI integration code and are thoroughly tested manually during development.

## Troubleshooting

### Tests Fail on Import

Ensure `package.json` has `"type": "module"` set:

```json
{
  "type": "module"
}
```

### Coverage Thresholds Not Met

Check `vitest.config.js` threshold settings. Current thresholds are set based on actual coverage of tested modules.

### DOM Elements Not Found

Verify elements are created in `beforeEach()` and the test uses the correct selectors.

## Additional Resources

- [Vitest Documentation](https://vitest.dev/)
- [Happy DOM Documentation](https://github.com/capricorn86/happy-dom)
- [Testing Library Best Practices](https://kentcdodds.com/blog/common-mistakes-with-react-testing-library)
