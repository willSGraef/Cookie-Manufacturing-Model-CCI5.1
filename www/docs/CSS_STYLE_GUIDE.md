# CSS Style Guide

## Overview

The CIE Assessment Platform uses a hybrid CSS naming convention that combines **BEM methodology for components** with **utility classes for spacing and layout**. This approach balances component encapsulation with development speed.

## Table of Contents

1. [Naming Conventions](#naming-conventions)
2. [CSS Architecture](#css-architecture)
3. [Design Tokens (CSS Variables)](#design-tokens-css-variables)
4. [Component Classes](#component-classes)
5. [Utility Classes](#utility-classes)
6. [Best Practices](#best-practices)

---

## Naming Conventions

### Component Classes (BEM-inspired)

Components use descriptive, semantic names with modifiers for variations:

**Pattern**: `.component-name`, `.component-name--modifier`, `.component-name__element`

**Examples**:

```css
.btn                  /* Base button */
.btn-primary          /* Button variant (modifier) */
.btn-small            /* Button size modifier */
.btn-large            /* Button size modifier */

.modal                /* Base modal */
.modal-overlay        /* Modal element */
.modal-close          /* Modal element */

.card                 /* Base card */
.clickable-card       /* Card variant */

.alert                /* Base alert */
.alert-info           /* Alert variant */
.alert-warning        /* Alert variant */
.alert-danger         /* Alert variant */
```

**Note**: We use single hyphens (`-`) instead of double hyphens (`--`) for simplicity, but maintain BEM principles.

### Utility Classes

Utilities follow a descriptive pattern indicating their function:

**Pattern**: `.property-value` or `.abbreviated-property-value`

**Examples**:

```css
/* Spacing */
.mb-sm                /* margin-bottom: small */
.p-lg                 /* padding: large */
.gap-md               /* gap: medium */

/* Layout */
.flex                 /* display: flex */
.grid                 /* display: grid */
.grid-2               /* 2-column grid */

/* Text */
.text-center          /* text-align: center */
.text-primary         /* color: primary */
.text-2xl             /* font-size: 2xl */

/* Width */
.max-w-1200           /* max-width: 1200px */
.w-full               /* width: 100% */
```

---

## CSS Architecture

The stylesheet is organized in a cascading import structure:

```
main.css (entry point)
├── variables.css      # CSS custom properties (design tokens)
├── reset.css          # Normalize browser styles
├── layout.css         # Page structure, header, footer, navigation
├── components.css     # Reusable UI components
└── pages.css          # Page-specific styles
```

### File Responsibilities

| File | Purpose | Examples |
|------|---------|----------|
| `variables.css` | Design tokens and CSS variables | Colors, spacing, typography |
| `reset.css` | Browser normalization | Box-sizing, margins, focus styles |
| `layout.css` | Page structure and layout | Header, footer, dock, grid systems |
| `components.css` | Reusable UI components | Buttons, cards, alerts, modals |
| `pages.css` | Page-specific styles | Hero sections, specific layouts |

---

## Design Tokens (CSS Variables)

All design values use CSS custom properties defined in `variables.css`.

### Color Tokens

```css
/* Primary Colors */
--color-primary: rgb(29, 29, 31);        /* Dark gray */
--color-secondary: #6b6b71;              /* Medium gray */
--color-accent-blue: rgb(0, 102, 204);   /* Blue */
--color-accent-red: #d70015;             /* Red */
--color-accent-green: #00b34b;           /* Green */

/* Semantic Colors */
--color-success: var(--color-accent-green);
--color-warning: #cc7500;
--color-danger: var(--color-accent-red);
--color-info: var(--color-accent-blue);

/* UI Colors */
--color-bg-primary: #ffffff;
--color-text-primary: rgb(29, 29, 31);
--color-border: #e5e5e7;
```

### Spacing Scale

Consistent 8-point grid system:

```css
--spacing-xs: 0.25rem;    /* 4px */
--spacing-sm: 0.5rem;     /* 8px */
--spacing-md: 1rem;       /* 16px */
--spacing-lg: 1.5rem;     /* 24px */
--spacing-xl: 2rem;       /* 32px */
--spacing-2xl: 2.5rem;    /* 40px */
--spacing-3xl: 3rem;      /* 48px */
--spacing-4xl: 4rem;      /* 64px */
--spacing-5xl: 6rem;      /* 96px */
```

### Typography Scale

Apple-inspired font system:

```css
/* Font Family */
--font-family-sans: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, ...;

/* Font Sizes */
--font-size-xs: 0.75rem;     /* 12px */
--font-size-sm: 0.875rem;    /* 14px */
--font-size-base: 1rem;      /* 16px */
--font-size-lg: 1.125rem;    /* 18px */
--font-size-xl: 1.25rem;     /* 20px */
--font-size-2xl: 1.5rem;     /* 24px */
--font-size-3xl: 1.875rem;   /* 30px */
--font-size-4xl: 2.25rem;    /* 36px */
--font-size-5xl: 3rem;       /* 48px */

/* Font Weights */
--font-weight-light: 300;
--font-weight-normal: 400;
--font-weight-medium: 500;
--font-weight-semibold: 600;
--font-weight-bold: 700;
```

---

## Component Classes

### Buttons

**Base class**: `.btn`

**Variants**:

- `.btn-primary` - Blue filled button
- `.btn-outline` - Outlined button
- `.btn-small` - Smaller padding/font
- `.btn-large` - Larger padding/font

**Usage**:

```html
<button class="btn btn-primary">Submit</button>
<button class="btn btn-outline btn-small">Cancel</button>
<a href="#" class="btn btn-primary btn-large">Get Started</a>
```

### Cards

**Base class**: `.card`

**Variants**:

- `.clickable-card` - Interactive card with hover effects

**Usage**:

```html
<div class="card">
  <h3>Card Title</h3>
  <p>Card content...</p>
</div>
```

### Alerts

**Base class**: `.alert`

**Variants**:

- `.alert-info` - Blue informational alert
- `.alert-warning` - Orange warning alert
- `.alert-danger` - Red error alert
- `.alert-success` - Green success alert

**Usage**:

```html
<div class="alert alert-info">
  <p>Informational message</p>
</div>
```

### Badges

**Base class**: `.badge`

**Variants**:

- `.badge-info` - Blue badge
- `.badge-warning` - Orange badge
- `.badge-success` - Green badge
- `.badge-danger` - Red badge

**Usage**:

```html
<span class="badge badge-warning">Pending</span>
<span class="badge badge-success">Finished</span>
```

### Modals

**Structure**:

```html
<div class="modal" id="modal-id">
  <div class="modal-overlay"></div>
  <div class="modal-content">
    <button class="modal-close">×</button>
    <!-- Content -->
  </div>
</div>
```

**Classes**:

- `.modal` - Container
- `.modal-overlay` - Dark background
- `.modal-content` - Content box
- `.modal-close` - Close button

### Forms

**Form inputs** use native element selectors, not classes:

```css
input[type="text"],
input[type="email"],
textarea,
select {
  /* Styles applied automatically */
}
```

**Error states** are controlled via inline styles:

- `border-color: var(--color-accent-red)` for invalid
- `aria-invalid="true"` for accessibility

---

## Utility Classes

### Spacing Utilities

**Margin**:

```css
.m-0, .m-xs, .m-sm, .m-md, .m-lg, .m-xl, .m-2xl, .m-3xl
.mt-*, .mr-*, .mb-*, .ml-*    /* Directional margins */
.mx-*, .my-*                  /* Horizontal/vertical margins */
.mx-auto                      /* Center with auto margins */
```

**Padding**:

```css
.p-0, .p-xs, .p-sm, .p-md, .p-lg, .p-xl, .p-2xl, .p-3xl
.pt-*, .pr-*, .pb-*, .pl-*    /* Directional padding */
.px-*, .py-*                  /* Horizontal/vertical padding */
```

**Gap** (for flex/grid):

```css
.gap-xs, .gap-sm, .gap-md, .gap-lg, .gap-xl, .gap-2xl
```

### Layout Utilities

**Display**:

```css
.flex           /* display: flex */
.grid           /* display: grid */
.inline-flex    /* display: inline-flex */
.hidden         /* display: none */
```

**Flexbox**:

```css
.flex-row       /* flex-direction: row */
.flex-col       /* flex-direction: column */
.items-center   /* align-items: center */
.items-start    /* align-items: flex-start */
.justify-center /* justify-content: center */
.justify-between /* justify-content: space-between */
.flex-wrap      /* flex-wrap: wrap */
```

**Grid**:

```css
.grid-2         /* 2-column grid */
.grid-3         /* 3-column grid */
.grid-col-span-full /* span all columns */
```

### Width/Height Utilities

```css
.w-full         /* width: 100% */
.h-full         /* height: 100% */
.max-w-1200     /* max-width: 1200px */
```

### Text Utilities

**Alignment**:

```css
.text-left      /* text-align: left */
.text-center    /* text-align: center */
.text-right     /* text-align: right */
```

**Color**:

```css
.text-primary   /* color: var(--color-text-primary) */
.text-secondary /* color: var(--color-text-secondary) */
```

**Size**:

```css
.text-xs, .text-sm, .text-base, .text-lg, .text-xl
.text-2xl, .text-3xl, .text-4xl, .text-5xl
```

**Weight**:

```css
.font-light, .font-normal, .font-medium, .font-semibold, .font-bold
```

### Line Height

```css
.lh-tight       /* line-height: 1.25 */
.lh-normal      /* line-height: 1.5 */
.lh-relaxed     /* line-height: 1.75 */
.lh-loose       /* line-height: 2 */
```

---

## Best Practices

### 1. Use Design Tokens

✅ **Good**:

```css
.my-component {
  padding: var(--spacing-lg);
  color: var(--color-text-primary);
  font-size: var(--font-size-base);
}
```

❌ **Bad**:

```css
.my-component {
  padding: 24px;
  color: #1d1d1f;
  font-size: 16px;
}
```

### 2. Component Classes for Structure, Utilities for Spacing

✅ **Good**:

```html
<div class="card mb-xl">
  <h3 class="text-2xl mb-md">Title</h3>
  <p class="text-secondary">Content</p>
</div>
```

❌ **Bad** (all inline or all utility classes):

```html
<div style="margin-bottom: 2rem; padding: 1.5rem; background: white;">
  ...
</div>
```

### 3. Keep Specificity Low

✅ **Good**:

```css
.btn { /* base styles */ }
.btn-primary { /* variant styles */ }
```

❌ **Bad**:

```css
div.container button.btn.btn-primary { /* too specific */ }
```

### 4. Use Semantic Class Names

✅ **Good**:

```css
.task-card
.submit-button
.error-message
```

❌ **Bad**:

```css
.blue-box
.big-text
.mt20
```

### 5. Mobile-First Responsive Design

✅ **Good**:

```css
.element {
  /* Mobile styles (base) */
  padding: var(--spacing-sm);
}

@media (min-width: var(--breakpoint-md)) {
  .element {
    /* Desktop styles (enhancement) */
    padding: var(--spacing-lg);
  }
}
```

### 6. Consistent Naming Patterns

For component variants, use consistent patterns:

```css
/* Size modifiers */
.btn-small, .btn-large

/* State modifiers */
.alert-info, .alert-warning, .alert-danger

/* Color modifiers */
.badge-success, .badge-warning
```

### 7. Avoid Deep Nesting

✅ **Good** (flat structure):

```css
.card { }
.card-header { }
.card-body { }
.card-footer { }
```

❌ **Bad** (deep nesting):

```css
.card .header .title .text { }
```

### 8. Leverage CSS Variables for Theming

```css
/* Light mode (default) */
:root {
  --color-bg-primary: #ffffff;
  --color-text-primary: #1d1d1f;
}

/* Components automatically adapt */
.card {
  background: var(--color-bg-primary);
  color: var(--color-text-primary);
}
```

---

## Creating New Components

### Checklist

1. **Choose a semantic name** (e.g., `.notification`, `.user-card`)
2. **Define base styles** in `components.css`
3. **Create variants** with consistent naming (e.g., `.notification-error`)
4. **Use design tokens** for colors, spacing, typography
5. **Make responsive** (mobile-first media queries)
6. **Add hover/focus states** for interactive elements
7. **Ensure accessibility** (focus outlines, ARIA attributes)
8. **Document usage** in this style guide

### Example Component Template

```css
/* Base Component */
.my-component {
  /* Layout */
  display: flex;
  padding: var(--spacing-md);
  
  /* Colors */
  background-color: var(--color-bg-primary);
  color: var(--color-text-primary);
  border: 1px solid var(--color-border);
  
  /* Typography */
  font-size: var(--font-size-base);
  
  /* Effects */
  border-radius: var(--radius-md);
  box-shadow: var(--shadow-sm);
  transition: all var(--transition-base);
}

/* Hover State */
.my-component:hover {
  box-shadow: var(--shadow-md);
  transform: translateY(-2px);
}

/* Focus State */
.my-component:focus-visible {
  outline: 2px solid var(--color-accent-blue);
  outline-offset: 2px;
}

/* Variant */
.my-component--large {
  padding: var(--spacing-lg);
  font-size: var(--font-size-lg);
}

/* Responsive */
@media (max-width: var(--breakpoint-md)) {
  .my-component {
    padding: var(--spacing-sm);
  }
}
```

---

## Resources

- **Design Inspiration**: Apple.com, iOS 26 Design System
- **BEM Methodology**: [bem.info](http://getbem.com/)
- **CSS Variables**: [MDN CSS Custom Properties](https://developer.mozilla.org/en-US/docs/Web/CSS/--*)
- **Accessibility**: [WCAG 2.1 Guidelines](https://www.w3.org/WAI/WCAG21/quickref/)

---

## Maintenance

- **Review quarterly** - Update design tokens as design evolves
- **Document new patterns** - Add new components to this guide
- **Refactor duplicates** - Consolidate similar styles into utilities
- **Test across browsers** - Verify compatibility (Chrome, Firefox, Safari, Edge)
- **Monitor bundle size** - Keep CSS lean and efficient
