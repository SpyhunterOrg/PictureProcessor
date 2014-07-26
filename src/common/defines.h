#ifndef DEFINES_H
#define DEFINES_H

#define SAFE_DELETE( p ) { if (p) { delete (p); ( p ) = NULL;  } }
//#define SAFE_RSA_FREE( p ) { if (p) { RSA_free((p)); ( p ) = NULL;  } }
//#define SAFE_EVP_PKEY_FREE( p ) { if (p) { EVP_PKEY_free((p)); ( p ) = NULL;  } }
//#define SAFE_X509_FREE( p ) { if (p) { X509_free((p)); ( p ) = NULL;  } }
//#define SAFE_BIO_FREE( p ) { if (p) { BIO_free((p)); ( p ) = NULL;  } }

#define IN
#define OUT

typedef long long secs;

#define JPG_FILETYPE "*.jpg"
#define JPEG_FILETYPE "*.jpeg"


#endif
