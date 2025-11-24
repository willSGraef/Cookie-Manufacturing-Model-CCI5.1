# Security Headers Configuration Guide

## Overview

This document provides deployment instructions for configuring security headers to protect the CIE Assessment Platform from common web vulnerabilities. These headers must be configured at the web server level.

## Required Security Headers

### 1. Content-Security-Policy (CSP)

Prevents Cross-Site Scripting (XSS) attacks by controlling which resources can be loaded.

**Recommended Policy**:

```
Content-Security-Policy: default-src 'self'; script-src 'self'; style-src 'self' 'unsafe-inline'; img-src 'self' data:; font-src 'self'; connect-src 'self'; frame-ancestors 'none'; base-uri 'self'; form-action 'self'
```

**Explanation**:

- `default-src 'self'` - Only load resources from same origin
- `script-src 'self'` - Only execute scripts from same origin
- `style-src 'self' 'unsafe-inline'` - Allow inline styles (needed for dynamic styling)
- `img-src 'self' data:` - Allow images from same origin and data URIs
- `font-src 'self'` - Only load fonts from same origin
- `connect-src 'self'` - Only allow AJAX/fetch to same origin
- `frame-ancestors 'none'` - Prevent embedding in frames (clickjacking protection)
- `base-uri 'self'` - Restrict base tag to same origin
- `form-action 'self'` - Forms can only submit to same origin

### 2. X-Content-Type-Options

Prevents MIME type sniffing attacks.

**Header**:

```
X-Content-Type-Options: nosniff
```

**Purpose**: Forces browser to respect declared Content-Type, preventing interpretation of files as different types (e.g., image as JavaScript).

### 3. X-Frame-Options

Prevents clickjacking attacks by controlling page embedding.

**Header**:

```
X-Frame-Options: DENY
```

**Purpose**: Prevents the page from being embedded in `<iframe>`, `<frame>`, `<embed>`, or `<object>` tags.

**Alternative**: Use `SAMEORIGIN` if you need to embed pages within your own domain.

### 4. Referrer-Policy

Controls how much referrer information is sent with requests.

**Header**:

```
Referrer-Policy: strict-origin-when-cross-origin
```

**Purpose**:

- Sends full URL for same-origin requests
- Sends only origin for cross-origin HTTPS requests
- Sends nothing for HTTPS→HTTP requests
- Balances privacy with functionality

### 5. Permissions-Policy (Optional but Recommended)

Controls which browser features can be used.

**Header**:

```
Permissions-Policy: geolocation=(), microphone=(), camera=(), payment=(), usb=(), magnetometer=(), gyroscope=()
```

**Purpose**: Disables unnecessary browser APIs that the platform doesn't use, reducing attack surface.

### 6. Strict-Transport-Security (HSTS) - For HTTPS Only

Forces HTTPS connections (only use if site is fully HTTPS).

**Header**:

```
Strict-Transport-Security: max-age=31536000; includeSubDomains; preload
```

**Purpose**:

- Forces all connections over HTTPS for 1 year
- Applies to all subdomains
- Eligible for browser preload list

**Warning**: Only implement if HTTPS is fully configured and tested.

## Server Configuration Examples

### Apache (.htaccess or httpd.conf)

```apache
# Security Headers
<IfModule mod_headers.c>
    # Content Security Policy
    Header always set Content-Security-Policy "default-src 'self'; script-src 'self'; style-src 'self' 'unsafe-inline'; img-src 'self' data:; font-src 'self'; connect-src 'self'; frame-ancestors 'none'; base-uri 'self'; form-action 'self'"
    
    # Prevent MIME type sniffing
    Header always set X-Content-Type-Options "nosniff"
    
    # Prevent clickjacking
    Header always set X-Frame-Options "DENY"
    
    # Referrer Policy
    Header always set Referrer-Policy "strict-origin-when-cross-origin"
    
    # Permissions Policy
    Header always set Permissions-Policy "geolocation=(), microphone=(), camera=(), payment=(), usb=(), magnetometer=(), gyroscope=()"
    
    # HSTS (only if using HTTPS)
    # Header always set Strict-Transport-Security "max-age=31536000; includeSubDomains; preload"
</IfModule>
```

### Nginx (nginx.conf or site configuration)

```nginx
# Security Headers
add_header Content-Security-Policy "default-src 'self'; script-src 'self'; style-src 'self' 'unsafe-inline'; img-src 'self' data:; font-src 'self'; connect-src 'self'; frame-ancestors 'none'; base-uri 'self'; form-action 'self'" always;
add_header X-Content-Type-Options "nosniff" always;
add_header X-Frame-Options "DENY" always;
add_header Referrer-Policy "strict-origin-when-cross-origin" always;
add_header Permissions-Policy "geolocation=(), microphone=(), camera=(), payment=(), usb=(), magnetometer=(), gyroscope=()" always;

# HSTS (only if using HTTPS)
# add_header Strict-Transport-Security "max-age=31536000; includeSubDomains; preload" always;
```

### Node.js/Express

```javascript
const helmet = require('helmet');

app.use(helmet({
  contentSecurityPolicy: {
    directives: {
      defaultSrc: ["'self'"],
      scriptSrc: ["'self'"],
      styleSrc: ["'self'", "'unsafe-inline'"],
      imgSrc: ["'self'", "data:"],
      fontSrc: ["'self'"],
      connectSrc: ["'self'"],
      frameAncestors: ["'none'"],
      baseUri: ["'self'"],
      formAction: ["'self'"]
    }
  },
  xContentTypeOptions: 'nosniff',
  xFrameOptions: 'DENY',
  referrerPolicy: { policy: 'strict-origin-when-cross-origin' },
  // HSTS (only if using HTTPS)
  // hsts: {
  //   maxAge: 31536000,
  //   includeSubDomains: true,
  //   preload: true
  // }
}));
```

## Testing Security Headers

### 1. Browser Developer Tools

Open DevTools → Network tab → Select any request → View Response Headers

### 2. Online Testing Tools

- **Security Headers**: <https://securityheaders.com/>
- **Mozilla Observatory**: <https://observatory.mozilla.org/>
- **SSL Labs**: <https://www.ssllabs.com/ssltest/> (for HTTPS/TLS)

### 3. Command Line (curl)

```bash
curl -I https://your-domain.com
```

Look for the security headers in the response.

### 4. Browser Extensions

- **HTTP Header Live** (Firefox)
- **ModHeader** (Chrome/Firefox)

## Troubleshooting

### Issue: CSS not loading after CSP

**Solution**: Ensure `style-src 'self' 'unsafe-inline'` is in CSP. Inline styles are needed for dynamic styling in JavaScript.

### Issue: External resources blocked

**Solution**: Add allowed domains to CSP. Example for Google Fonts:

```
font-src 'self' https://fonts.gstatic.com; style-src 'self' https://fonts.googleapis.com
```

### Issue: Development vs Production

**Solution**: Use different CSP policies:

- **Development**: More permissive (e.g., allow `'unsafe-eval'` for dev tools)
- **Production**: Strict policy as documented above

### Issue: Legacy Browser Support

**Solution**: Some older browsers don't support all headers. Monitor browser compatibility:

- CSP: IE 10+, Edge 12+, Safari 10+, Chrome 25+, Firefox 23+
- All other headers: Widely supported

## Deployment Checklist

- [ ] Configure all required headers on web server
- [ ] Test headers using online tools (securityheaders.com)
- [ ] Verify application still functions correctly
- [ ] Test on multiple browsers (Chrome, Firefox, Safari, Edge)
- [ ] Monitor browser console for CSP violations
- [ ] Set up CSP reporting endpoint (optional)
- [ ] Document any custom exceptions needed for your environment
- [ ] Enable HSTS only after confirming HTTPS works perfectly
- [ ] Add domain to HSTS preload list (optional, after 30 days of HSTS)

## CSP Reporting (Optional)

To monitor CSP violations, add a reporting endpoint:

```
Content-Security-Policy: default-src 'self'; ...; report-uri /csp-report
```

Then set up an endpoint to receive reports:

```javascript
app.post('/csp-report', (req, res) => {
  console.log('CSP Violation:', req.body);
  res.status(204).end();
});
```

## Regular Maintenance

- **Review headers quarterly** - Update as security best practices evolve
- **Monitor CSP violations** - Adjust policy if legitimate resources are blocked
- **Test after updates** - Verify headers remain configured after server updates
- **Stay informed** - Follow OWASP and web security best practices

## Additional Resources

- [OWASP Secure Headers Project](https://owasp.org/www-project-secure-headers/)
- [MDN Web Security](https://developer.mozilla.org/en-US/docs/Web/Security)
- [Content Security Policy Reference](https://content-security-policy.com/)
- [Security Headers Quick Reference](https://securityheaders.com/)
- [Helmet.js Documentation](https://helmetjs.github.io/) (for Node.js)

## Support

For deployment assistance or questions about security headers:

- Review server documentation (Apache, Nginx, etc.)
- Contact your hosting provider
- Consult OWASP guidelines
- Test thoroughly in staging environment before production deployment
