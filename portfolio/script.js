// Smooth scroll animations for cards
document.addEventListener('DOMContentLoaded', () => {
  const cards = document.querySelectorAll('.glass-card');
  
  const observerOptions = {
    threshold: 0.1,
    rootMargin: '0px 0px -50px 0px'
  };

  const observer = new IntersectionObserver((entries) => {
    entries.forEach((entry, index) => {
      if (entry.isIntersecting) {
        setTimeout(() => {
          entry.target.style.opacity = '1';
          entry.target.style.animation = 'fadeInUp 0.8s ease';
        }, index * 100);
      }
    });
  }, observerOptions);

  cards.forEach(card => {
    observer.observe(card);
  });
});

// Smooth scroll for anchor links
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
  anchor.addEventListener('click', function (e) {
    const href = this.getAttribute('href');
    if (href !== '#resume') {
      e.preventDefault();
      const target = document.querySelector(href);
      if (target) {
        target.scrollIntoView({ behavior: 'smooth', block: 'start' });
      }
    }
  });
});

// Add hover effect to skill tags
document.querySelectorAll('.skill-tag').forEach(tag => {
  tag.addEventListener('mouseenter', function() {
    this.style.transform = 'scale(1.1) rotate(2deg)';
  });
  tag.addEventListener('mouseleave', function() {
    this.style.transform = 'scale(1) rotate(0deg)';
  });
});

// Add active state to contact links
document.querySelectorAll('.contact-item').forEach(item => {
  item.addEventListener('click', function(e) {
    if (!this.href.includes('mailto:') && !this.href.includes('tel:')) {
      e.preventDefault();
    }
  });
});

// Parallax effect on hero section
window.addEventListener('scroll', () => {
  const heroContainer = document.querySelector('.hero-container');
  if (heroContainer) {
    const scrollPosition = window.scrollY;
    heroContainer.style.transform = `translateY(${scrollPosition * 0.5}px)`;
  }
});


.resume {
  max-width: 900px;
  margin: auto;
  display: flex;
  background: white;
  box-shadow: 0 0 20px rgba(0,0,0,0.15);
}

/* LEFT PANEL */
.left {
  width: 35%;
  background: #0f2a3d;
  color: white;
  padding: 30px;
}

.left img {
  width: 120px;
  height: 120px;
  border-radius: 50%;
  display: block;
  margin: auto;
  margin-bottom: 20px;
  border: 3px solid white;
}

.left h3 {
  margin-top: 25px;
  margin-bottom: 10px;
  border-bottom: 1px solid #ffffff55;
  padding-bottom: 5px;
}

.left p, .left li {
  font-size: 14px;
  margin-bottom: 8px;
}

.left ul {
  padding-left: 18px;
}

/* RIGHT PANEL */
.right {
  width: 65%;
  padding: 35px;
}

.right h1 {
  font-size: 28px;
  letter-spacing: 1px;
}

.right h2 {
  font-size: 16px;
  color: #555;
  margin-bottom: 25px;
}

.right section {
  margin-bottom: 25px;
}

.right h3 {
  color: #0f2a3d;
  margin-bottom: 8px;
  border-bottom: 2px solid #0f2a3d;
  display: inline-block;
  padding-bottom: 3px;
}

.right p, .right l
