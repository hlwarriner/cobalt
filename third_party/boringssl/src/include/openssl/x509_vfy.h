/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 *
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 *
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 *
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */

#ifndef HEADER_X509_H
#include <openssl/x509.h>
// openssl/x509.h ends up #include-ing this file at about the only
// appropriate moment.
#endif

#ifndef HEADER_X509_VFY_H
#define HEADER_X509_VFY_H

#include <openssl/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

// Legacy X.509 library.
//
// This header is part of OpenSSL's X.509 implementation. It is retained for
// compatibility but otherwise underdocumented and not actively maintained. In
// the future, a replacement library will be available. Meanwhile, minimize
// dependencies on this header where possible.


/*
SSL_CTX -> X509_STORE
                -> X509_LOOKUP
                        ->X509_LOOKUP_METHOD
                -> X509_LOOKUP
                        ->X509_LOOKUP_METHOD

SSL	-> X509_STORE_CTX
                ->X509_STORE

The X509_STORE holds the tables etc for verification stuff.
A X509_STORE_CTX is used while validating a single certificate.
The X509_STORE has X509_LOOKUPs for looking up certs.
The X509_STORE then calls a function to actually verify the
certificate chain.
*/

#define X509_LU_X509 1
#define X509_LU_CRL 2
#define X509_LU_PKEY 3

DEFINE_STACK_OF(X509_LOOKUP)
DEFINE_STACK_OF(X509_OBJECT)
DEFINE_STACK_OF(X509_VERIFY_PARAM)

typedef int (*X509_STORE_CTX_verify_cb)(int, X509_STORE_CTX *);
typedef int (*X509_STORE_CTX_verify_fn)(X509_STORE_CTX *);
typedef int (*X509_STORE_CTX_get_issuer_fn)(X509 **issuer, X509_STORE_CTX *ctx,
                                            X509 *x);
typedef int (*X509_STORE_CTX_check_issued_fn)(X509_STORE_CTX *ctx, X509 *x,
                                              X509 *issuer);
typedef int (*X509_STORE_CTX_check_revocation_fn)(X509_STORE_CTX *ctx);
typedef int (*X509_STORE_CTX_get_crl_fn)(X509_STORE_CTX *ctx, X509_CRL **crl,
                                         X509 *x);
typedef int (*X509_STORE_CTX_check_crl_fn)(X509_STORE_CTX *ctx, X509_CRL *crl);
typedef int (*X509_STORE_CTX_cert_crl_fn)(X509_STORE_CTX *ctx, X509_CRL *crl,
                                          X509 *x);
typedef int (*X509_STORE_CTX_check_policy_fn)(X509_STORE_CTX *ctx);
typedef STACK_OF(X509) *(*X509_STORE_CTX_lookup_certs_fn)(X509_STORE_CTX *ctx,
                                                          X509_NAME *nm);
typedef STACK_OF(X509_CRL) *(*X509_STORE_CTX_lookup_crls_fn)(
    X509_STORE_CTX *ctx, X509_NAME *nm);
typedef int (*X509_STORE_CTX_cleanup_fn)(X509_STORE_CTX *ctx);

OPENSSL_EXPORT int X509_STORE_set_depth(X509_STORE *store, int depth);

OPENSSL_EXPORT void X509_STORE_CTX_set_depth(X509_STORE_CTX *ctx, int depth);

#define X509_STORE_CTX_set_app_data(ctx, data) \
  X509_STORE_CTX_set_ex_data(ctx, 0, data)
#define X509_STORE_CTX_get_app_data(ctx) X509_STORE_CTX_get_ex_data(ctx, 0)

#define X509_L_FILE_LOAD 1
#define X509_L_ADD_DIR 2

#define X509_LOOKUP_load_file(x, name, type) \
  X509_LOOKUP_ctrl((x), X509_L_FILE_LOAD, (name), (long)(type), NULL)

#define X509_LOOKUP_add_dir(x, name, type) \
  X509_LOOKUP_ctrl((x), X509_L_ADD_DIR, (name), (long)(type), NULL)

#define X509_V_OK 0
#define X509_V_ERR_UNSPECIFIED 1

#define X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT 2
#define X509_V_ERR_UNABLE_TO_GET_CRL 3
#define X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE 4
#define X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE 5
#define X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY 6
#define X509_V_ERR_CERT_SIGNATURE_FAILURE 7
#define X509_V_ERR_CRL_SIGNATURE_FAILURE 8
#define X509_V_ERR_CERT_NOT_YET_VALID 9
#define X509_V_ERR_CERT_HAS_EXPIRED 10
#define X509_V_ERR_CRL_NOT_YET_VALID 11
#define X509_V_ERR_CRL_HAS_EXPIRED 12
#define X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD 13
#define X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD 14
#define X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD 15
#define X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD 16
#define X509_V_ERR_OUT_OF_MEM 17
#define X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT 18
#define X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN 19
#define X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY 20
#define X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE 21
#define X509_V_ERR_CERT_CHAIN_TOO_LONG 22
#define X509_V_ERR_CERT_REVOKED 23
#define X509_V_ERR_INVALID_CA 24
#define X509_V_ERR_PATH_LENGTH_EXCEEDED 25
#define X509_V_ERR_INVALID_PURPOSE 26
#define X509_V_ERR_CERT_UNTRUSTED 27
#define X509_V_ERR_CERT_REJECTED 28
// These are 'informational' when looking for issuer cert
#define X509_V_ERR_SUBJECT_ISSUER_MISMATCH 29
#define X509_V_ERR_AKID_SKID_MISMATCH 30
#define X509_V_ERR_AKID_ISSUER_SERIAL_MISMATCH 31
#define X509_V_ERR_KEYUSAGE_NO_CERTSIGN 32

#define X509_V_ERR_UNABLE_TO_GET_CRL_ISSUER 33
#define X509_V_ERR_UNHANDLED_CRITICAL_EXTENSION 34
#define X509_V_ERR_KEYUSAGE_NO_CRL_SIGN 35
#define X509_V_ERR_UNHANDLED_CRITICAL_CRL_EXTENSION 36
#define X509_V_ERR_INVALID_NON_CA 37
#define X509_V_ERR_PROXY_PATH_LENGTH_EXCEEDED 38
#define X509_V_ERR_KEYUSAGE_NO_DIGITAL_SIGNATURE 39
#define X509_V_ERR_PROXY_CERTIFICATES_NOT_ALLOWED 40

#define X509_V_ERR_INVALID_EXTENSION 41
#define X509_V_ERR_INVALID_POLICY_EXTENSION 42
#define X509_V_ERR_NO_EXPLICIT_POLICY 43
#define X509_V_ERR_DIFFERENT_CRL_SCOPE 44
#define X509_V_ERR_UNSUPPORTED_EXTENSION_FEATURE 45

#define X509_V_ERR_UNNESTED_RESOURCE 46

#define X509_V_ERR_PERMITTED_VIOLATION 47
#define X509_V_ERR_EXCLUDED_VIOLATION 48
#define X509_V_ERR_SUBTREE_MINMAX 49
#define X509_V_ERR_APPLICATION_VERIFICATION 50
#define X509_V_ERR_UNSUPPORTED_CONSTRAINT_TYPE 51
#define X509_V_ERR_UNSUPPORTED_CONSTRAINT_SYNTAX 52
#define X509_V_ERR_UNSUPPORTED_NAME_SYNTAX 53
#define X509_V_ERR_CRL_PATH_VALIDATION_ERROR 54

// Suite B mode algorithm violation
#define X509_V_ERR_SUITE_B_INVALID_VERSION 56
#define X509_V_ERR_SUITE_B_INVALID_ALGORITHM 57
#define X509_V_ERR_SUITE_B_INVALID_CURVE 58
#define X509_V_ERR_SUITE_B_INVALID_SIGNATURE_ALGORITHM 59
#define X509_V_ERR_SUITE_B_LOS_NOT_ALLOWED 60
#define X509_V_ERR_SUITE_B_CANNOT_SIGN_P_384_WITH_P_256 61

// Host, email and IP check errors
#define X509_V_ERR_HOSTNAME_MISMATCH 62
#define X509_V_ERR_EMAIL_MISMATCH 63
#define X509_V_ERR_IP_ADDRESS_MISMATCH 64

// Caller error
#define X509_V_ERR_INVALID_CALL 65
// Issuer lookup error
#define X509_V_ERR_STORE_LOOKUP 66

#define X509_V_ERR_NAME_CONSTRAINTS_WITHOUT_SANS 67

// Certificate verify flags

// Send issuer+subject checks to verify_cb
#define X509_V_FLAG_CB_ISSUER_CHECK 0x1
// Use check time instead of current time
#define X509_V_FLAG_USE_CHECK_TIME 0x2
// Lookup CRLs
#define X509_V_FLAG_CRL_CHECK 0x4
// Lookup CRLs for whole chain
#define X509_V_FLAG_CRL_CHECK_ALL 0x8
// Ignore unhandled critical extensions
#define X509_V_FLAG_IGNORE_CRITICAL 0x10
// Does nothing as its functionality has been enabled by default.
#define X509_V_FLAG_X509_STRICT 0x00
// Enable proxy certificate validation
#define X509_V_FLAG_ALLOW_PROXY_CERTS 0x40
// Enable policy checking
#define X509_V_FLAG_POLICY_CHECK 0x80
// Policy variable require-explicit-policy
#define X509_V_FLAG_EXPLICIT_POLICY 0x100
// Policy variable inhibit-any-policy
#define X509_V_FLAG_INHIBIT_ANY 0x200
// Policy variable inhibit-policy-mapping
#define X509_V_FLAG_INHIBIT_MAP 0x400
// Notify callback that policy is OK
#define X509_V_FLAG_NOTIFY_POLICY 0x800
// Extended CRL features such as indirect CRLs, alternate CRL signing keys
#define X509_V_FLAG_EXTENDED_CRL_SUPPORT 0x1000
// Delta CRL support
#define X509_V_FLAG_USE_DELTAS 0x2000
// Check selfsigned CA signature
#define X509_V_FLAG_CHECK_SS_SIGNATURE 0x4000
// Use trusted store first
#define X509_V_FLAG_TRUSTED_FIRST 0x8000
// Suite B 128 bit only mode: not normally used
#define X509_V_FLAG_SUITEB_128_LOS_ONLY 0x10000
// Suite B 192 bit only mode
#define X509_V_FLAG_SUITEB_192_LOS 0x20000
// Suite B 128 bit mode allowing 192 bit algorithms
#define X509_V_FLAG_SUITEB_128_LOS 0x30000

// Allow partial chains if at least one certificate is in trusted store
#define X509_V_FLAG_PARTIAL_CHAIN 0x80000

// If the initial chain is not trusted, do not attempt to build an alternative
// chain. Alternate chain checking was introduced in 1.0.2b. Setting this flag
// will force the behaviour to match that of previous versions.
#define X509_V_FLAG_NO_ALT_CHAINS 0x100000

#define X509_VP_FLAG_DEFAULT 0x1
#define X509_VP_FLAG_OVERWRITE 0x2
#define X509_VP_FLAG_RESET_FLAGS 0x4
#define X509_VP_FLAG_LOCKED 0x8
#define X509_VP_FLAG_ONCE 0x10

// Internal use: mask of policy related options
#define X509_V_FLAG_POLICY_MASK                             \
  (X509_V_FLAG_POLICY_CHECK | X509_V_FLAG_EXPLICIT_POLICY | \
   X509_V_FLAG_INHIBIT_ANY | X509_V_FLAG_INHIBIT_MAP)

OPENSSL_EXPORT int X509_OBJECT_idx_by_subject(STACK_OF(X509_OBJECT) *h,
                                              int type, X509_NAME *name);
OPENSSL_EXPORT X509_OBJECT *X509_OBJECT_retrieve_by_subject(
    STACK_OF(X509_OBJECT) *h, int type, X509_NAME *name);
OPENSSL_EXPORT X509_OBJECT *X509_OBJECT_retrieve_match(STACK_OF(X509_OBJECT) *h,
                                                       X509_OBJECT *x);
OPENSSL_EXPORT int X509_OBJECT_up_ref_count(X509_OBJECT *a);
OPENSSL_EXPORT void X509_OBJECT_free_contents(X509_OBJECT *a);
OPENSSL_EXPORT int X509_OBJECT_get_type(const X509_OBJECT *a);
OPENSSL_EXPORT X509 *X509_OBJECT_get0_X509(const X509_OBJECT *a);
OPENSSL_EXPORT X509_STORE *X509_STORE_new(void);
OPENSSL_EXPORT int X509_STORE_up_ref(X509_STORE *store);
OPENSSL_EXPORT void X509_STORE_free(X509_STORE *v);

OPENSSL_EXPORT STACK_OF(X509_OBJECT) *X509_STORE_get0_objects(X509_STORE *st);
OPENSSL_EXPORT STACK_OF(X509) *X509_STORE_get1_certs(X509_STORE_CTX *st,
                                                     X509_NAME *nm);
OPENSSL_EXPORT STACK_OF(X509_CRL) *X509_STORE_get1_crls(X509_STORE_CTX *st,
                                                        X509_NAME *nm);
OPENSSL_EXPORT int X509_STORE_set_flags(X509_STORE *ctx, unsigned long flags);
OPENSSL_EXPORT int X509_STORE_set_purpose(X509_STORE *ctx, int purpose);
OPENSSL_EXPORT int X509_STORE_set_trust(X509_STORE *ctx, int trust);
OPENSSL_EXPORT int X509_STORE_set1_param(X509_STORE *ctx,
                                         X509_VERIFY_PARAM *pm);
OPENSSL_EXPORT X509_VERIFY_PARAM *X509_STORE_get0_param(X509_STORE *ctx);
// X509_STORE_set0_additional_untrusted sets a stack of additional, untrusted
// certificates that are available for chain building. This function does not
// take ownership of the stack.
OPENSSL_EXPORT void X509_STORE_set0_additional_untrusted(
    X509_STORE *ctx, STACK_OF(X509) *untrusted);

OPENSSL_EXPORT void X509_STORE_set_verify(X509_STORE *ctx,
                                          X509_STORE_CTX_verify_fn verify);
#define X509_STORE_set_verify_func(ctx, func) \
  X509_STORE_set_verify((ctx), (func))
OPENSSL_EXPORT void X509_STORE_CTX_set_verify(X509_STORE_CTX *ctx,
                                              X509_STORE_CTX_verify_fn verify);
OPENSSL_EXPORT X509_STORE_CTX_verify_fn X509_STORE_get_verify(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_verify_cb(
    X509_STORE *ctx, X509_STORE_CTX_verify_cb verify_cb);
#define X509_STORE_set_verify_cb_func(ctx, func) \
  X509_STORE_set_verify_cb((ctx), (func))
OPENSSL_EXPORT X509_STORE_CTX_verify_cb
X509_STORE_get_verify_cb(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_get_issuer(
    X509_STORE *ctx, X509_STORE_CTX_get_issuer_fn get_issuer);
OPENSSL_EXPORT X509_STORE_CTX_get_issuer_fn
X509_STORE_get_get_issuer(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_check_issued(
    X509_STORE *ctx, X509_STORE_CTX_check_issued_fn check_issued);
OPENSSL_EXPORT X509_STORE_CTX_check_issued_fn
X509_STORE_get_check_issued(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_check_revocation(
    X509_STORE *ctx, X509_STORE_CTX_check_revocation_fn check_revocation);
OPENSSL_EXPORT X509_STORE_CTX_check_revocation_fn
X509_STORE_get_check_revocation(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_get_crl(X509_STORE *ctx,
                                           X509_STORE_CTX_get_crl_fn get_crl);
OPENSSL_EXPORT X509_STORE_CTX_get_crl_fn
X509_STORE_get_get_crl(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_check_crl(
    X509_STORE *ctx, X509_STORE_CTX_check_crl_fn check_crl);
OPENSSL_EXPORT X509_STORE_CTX_check_crl_fn
X509_STORE_get_check_crl(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_cert_crl(
    X509_STORE *ctx, X509_STORE_CTX_cert_crl_fn cert_crl);
OPENSSL_EXPORT X509_STORE_CTX_cert_crl_fn
X509_STORE_get_cert_crl(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_lookup_certs(
    X509_STORE *ctx, X509_STORE_CTX_lookup_certs_fn lookup_certs);
OPENSSL_EXPORT X509_STORE_CTX_lookup_certs_fn
X509_STORE_get_lookup_certs(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_lookup_crls(
    X509_STORE *ctx, X509_STORE_CTX_lookup_crls_fn lookup_crls);
#define X509_STORE_set_lookup_crls_cb(ctx, func) \
  X509_STORE_set_lookup_crls((ctx), (func))
OPENSSL_EXPORT X509_STORE_CTX_lookup_crls_fn
X509_STORE_get_lookup_crls(X509_STORE *ctx);
OPENSSL_EXPORT void X509_STORE_set_cleanup(X509_STORE *ctx,
                                           X509_STORE_CTX_cleanup_fn cleanup);
OPENSSL_EXPORT X509_STORE_CTX_cleanup_fn
X509_STORE_get_cleanup(X509_STORE *ctx);


OPENSSL_EXPORT X509_STORE_CTX *X509_STORE_CTX_new(void);

OPENSSL_EXPORT int X509_STORE_CTX_get1_issuer(X509 **issuer,
                                              X509_STORE_CTX *ctx, X509 *x);

OPENSSL_EXPORT void X509_STORE_CTX_zero(X509_STORE_CTX *ctx);
OPENSSL_EXPORT void X509_STORE_CTX_free(X509_STORE_CTX *ctx);
OPENSSL_EXPORT int X509_STORE_CTX_init(X509_STORE_CTX *ctx, X509_STORE *store,
                                       X509 *x509, STACK_OF(X509) *chain);
OPENSSL_EXPORT void X509_STORE_CTX_trusted_stack(X509_STORE_CTX *ctx,
                                                 STACK_OF(X509) *sk);
OPENSSL_EXPORT void X509_STORE_CTX_cleanup(X509_STORE_CTX *ctx);

OPENSSL_EXPORT X509_STORE *X509_STORE_CTX_get0_store(X509_STORE_CTX *ctx);
OPENSSL_EXPORT X509 *X509_STORE_CTX_get0_cert(X509_STORE_CTX *ctx);

OPENSSL_EXPORT X509_LOOKUP *X509_STORE_add_lookup(X509_STORE *v,
                                                  X509_LOOKUP_METHOD *m);

OPENSSL_EXPORT X509_LOOKUP_METHOD *X509_LOOKUP_hash_dir(void);
OPENSSL_EXPORT X509_LOOKUP_METHOD *X509_LOOKUP_file(void);

OPENSSL_EXPORT int X509_STORE_add_cert(X509_STORE *ctx, X509 *x);
OPENSSL_EXPORT int X509_STORE_add_crl(X509_STORE *ctx, X509_CRL *x);

OPENSSL_EXPORT int X509_STORE_get_by_subject(X509_STORE_CTX *vs, int type,
                                             X509_NAME *name, X509_OBJECT *ret);

OPENSSL_EXPORT int X509_LOOKUP_ctrl(X509_LOOKUP *ctx, int cmd, const char *argc,
                                    long argl, char **ret);

#ifndef OPENSSL_NO_STDIO
OPENSSL_EXPORT int X509_load_cert_file(X509_LOOKUP *ctx, const char *file,
                                       int type);
OPENSSL_EXPORT int X509_load_crl_file(X509_LOOKUP *ctx, const char *file,
                                      int type);
OPENSSL_EXPORT int X509_load_cert_crl_file(X509_LOOKUP *ctx, const char *file,
                                           int type);
#endif


OPENSSL_EXPORT X509_LOOKUP *X509_LOOKUP_new(X509_LOOKUP_METHOD *method);
OPENSSL_EXPORT void X509_LOOKUP_free(X509_LOOKUP *ctx);
OPENSSL_EXPORT int X509_LOOKUP_init(X509_LOOKUP *ctx);
OPENSSL_EXPORT int X509_LOOKUP_by_subject(X509_LOOKUP *ctx, int type,
                                          X509_NAME *name, X509_OBJECT *ret);
OPENSSL_EXPORT int X509_LOOKUP_by_issuer_serial(X509_LOOKUP *ctx, int type,
                                                X509_NAME *name,
                                                ASN1_INTEGER *serial,
                                                X509_OBJECT *ret);
OPENSSL_EXPORT int X509_LOOKUP_by_fingerprint(X509_LOOKUP *ctx, int type,
                                              unsigned char *bytes, int len,
                                              X509_OBJECT *ret);
OPENSSL_EXPORT int X509_LOOKUP_by_alias(X509_LOOKUP *ctx, int type, char *str,
                                        int len, X509_OBJECT *ret);
OPENSSL_EXPORT int X509_LOOKUP_shutdown(X509_LOOKUP *ctx);

#ifndef OPENSSL_NO_STDIO
OPENSSL_EXPORT int X509_STORE_load_locations(X509_STORE *ctx, const char *file,
                                             const char *dir);
OPENSSL_EXPORT int X509_STORE_set_default_paths(X509_STORE *ctx);
#endif

OPENSSL_EXPORT int X509_STORE_CTX_get_ex_new_index(long argl, void *argp,
                                                   CRYPTO_EX_unused *unused,
                                                   CRYPTO_EX_dup *dup_unused,
                                                   CRYPTO_EX_free *free_func);
OPENSSL_EXPORT int X509_STORE_CTX_set_ex_data(X509_STORE_CTX *ctx, int idx,
                                              void *data);
OPENSSL_EXPORT void *X509_STORE_CTX_get_ex_data(X509_STORE_CTX *ctx, int idx);
OPENSSL_EXPORT int X509_STORE_CTX_get_error(X509_STORE_CTX *ctx);
OPENSSL_EXPORT void X509_STORE_CTX_set_error(X509_STORE_CTX *ctx, int s);
OPENSSL_EXPORT int X509_STORE_CTX_get_error_depth(X509_STORE_CTX *ctx);
OPENSSL_EXPORT X509 *X509_STORE_CTX_get_current_cert(X509_STORE_CTX *ctx);
OPENSSL_EXPORT X509 *X509_STORE_CTX_get0_current_issuer(X509_STORE_CTX *ctx);
OPENSSL_EXPORT X509_CRL *X509_STORE_CTX_get0_current_crl(X509_STORE_CTX *ctx);
OPENSSL_EXPORT X509_STORE_CTX *X509_STORE_CTX_get0_parent_ctx(
    X509_STORE_CTX *ctx);
OPENSSL_EXPORT STACK_OF(X509) *X509_STORE_CTX_get_chain(X509_STORE_CTX *ctx);
OPENSSL_EXPORT STACK_OF(X509) *X509_STORE_CTX_get0_chain(X509_STORE_CTX *ctx);
OPENSSL_EXPORT STACK_OF(X509) *X509_STORE_CTX_get1_chain(X509_STORE_CTX *ctx);
OPENSSL_EXPORT void X509_STORE_CTX_set_cert(X509_STORE_CTX *c, X509 *x);
OPENSSL_EXPORT void X509_STORE_CTX_set_chain(X509_STORE_CTX *c,
                                             STACK_OF(X509) *sk);
OPENSSL_EXPORT STACK_OF(X509) *X509_STORE_CTX_get0_untrusted(
    X509_STORE_CTX *ctx);
OPENSSL_EXPORT void X509_STORE_CTX_set0_crls(X509_STORE_CTX *c,
                                             STACK_OF(X509_CRL) *sk);
OPENSSL_EXPORT int X509_STORE_CTX_set_purpose(X509_STORE_CTX *ctx, int purpose);
OPENSSL_EXPORT int X509_STORE_CTX_set_trust(X509_STORE_CTX *ctx, int trust);
OPENSSL_EXPORT int X509_STORE_CTX_purpose_inherit(X509_STORE_CTX *ctx,
                                                  int def_purpose, int purpose,
                                                  int trust);
OPENSSL_EXPORT void X509_STORE_CTX_set_flags(X509_STORE_CTX *ctx,
                                             unsigned long flags);
OPENSSL_EXPORT void X509_STORE_CTX_set_time(X509_STORE_CTX *ctx, unsigned long flags,
                                            OPENSSL_port_time_t t);
OPENSSL_EXPORT void X509_STORE_CTX_set_verify_cb(
    X509_STORE_CTX *ctx, int (*verify_cb)(int, X509_STORE_CTX *));

OPENSSL_EXPORT X509_POLICY_TREE *X509_STORE_CTX_get0_policy_tree(
    X509_STORE_CTX *ctx);
OPENSSL_EXPORT int X509_STORE_CTX_get_explicit_policy(X509_STORE_CTX *ctx);

OPENSSL_EXPORT X509_VERIFY_PARAM *X509_STORE_CTX_get0_param(
    X509_STORE_CTX *ctx);
OPENSSL_EXPORT void X509_STORE_CTX_set0_param(X509_STORE_CTX *ctx,
                                              X509_VERIFY_PARAM *param);
OPENSSL_EXPORT int X509_STORE_CTX_set_default(X509_STORE_CTX *ctx,
                                              const char *name);

// X509_VERIFY_PARAM functions

OPENSSL_EXPORT X509_VERIFY_PARAM *X509_VERIFY_PARAM_new(void);
OPENSSL_EXPORT void X509_VERIFY_PARAM_free(X509_VERIFY_PARAM *param);
OPENSSL_EXPORT int X509_VERIFY_PARAM_inherit(X509_VERIFY_PARAM *to,
                                             const X509_VERIFY_PARAM *from);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set1(X509_VERIFY_PARAM *to,
                                          const X509_VERIFY_PARAM *from);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set1_name(X509_VERIFY_PARAM *param,
                                               const char *name);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set_flags(X509_VERIFY_PARAM *param,
                                               unsigned long flags);
OPENSSL_EXPORT int X509_VERIFY_PARAM_clear_flags(X509_VERIFY_PARAM *param,
                                                 unsigned long flags);
OPENSSL_EXPORT unsigned long X509_VERIFY_PARAM_get_flags(
    X509_VERIFY_PARAM *param);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set_purpose(X509_VERIFY_PARAM *param,
                                                 int purpose);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set_trust(X509_VERIFY_PARAM *param,
                                               int trust);
OPENSSL_EXPORT void X509_VERIFY_PARAM_set_depth(X509_VERIFY_PARAM *param,
                                                int depth);
OPENSSL_EXPORT void X509_VERIFY_PARAM_set_time(X509_VERIFY_PARAM *param,
                                               OPENSSL_port_time_t t);
OPENSSL_EXPORT int X509_VERIFY_PARAM_add0_policy(X509_VERIFY_PARAM *param,
                                                 ASN1_OBJECT *policy);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set1_policies(
    X509_VERIFY_PARAM *param, STACK_OF(ASN1_OBJECT) *policies);

OPENSSL_EXPORT int X509_VERIFY_PARAM_set1_host(X509_VERIFY_PARAM *param,
                                               const char *name,
                                               size_t namelen);
OPENSSL_EXPORT int X509_VERIFY_PARAM_add1_host(X509_VERIFY_PARAM *param,
                                               const char *name,
                                               size_t namelen);
OPENSSL_EXPORT void X509_VERIFY_PARAM_set_hostflags(X509_VERIFY_PARAM *param,
                                                    unsigned int flags);
OPENSSL_EXPORT char *X509_VERIFY_PARAM_get0_peername(X509_VERIFY_PARAM *);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set1_email(X509_VERIFY_PARAM *param,
                                                const char *email,
                                                size_t emaillen);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set1_ip(X509_VERIFY_PARAM *param,
                                             const unsigned char *ip,
                                             size_t iplen);
OPENSSL_EXPORT int X509_VERIFY_PARAM_set1_ip_asc(X509_VERIFY_PARAM *param,
                                                 const char *ipasc);

OPENSSL_EXPORT int X509_VERIFY_PARAM_get_depth(const X509_VERIFY_PARAM *param);
OPENSSL_EXPORT const char *X509_VERIFY_PARAM_get0_name(
    const X509_VERIFY_PARAM *param);

OPENSSL_EXPORT int X509_VERIFY_PARAM_add0_table(X509_VERIFY_PARAM *param);
OPENSSL_EXPORT int X509_VERIFY_PARAM_get_count(void);
OPENSSL_EXPORT const X509_VERIFY_PARAM *X509_VERIFY_PARAM_get0(int id);
OPENSSL_EXPORT const X509_VERIFY_PARAM *X509_VERIFY_PARAM_lookup(
    const char *name);
OPENSSL_EXPORT void X509_VERIFY_PARAM_table_cleanup(void);

OPENSSL_EXPORT int X509_policy_check(X509_POLICY_TREE **ptree,
                                     int *pexplicit_policy,
                                     STACK_OF(X509) *certs,
                                     STACK_OF(ASN1_OBJECT) *policy_oids,
                                     unsigned int flags);

OPENSSL_EXPORT void X509_policy_tree_free(X509_POLICY_TREE *tree);

OPENSSL_EXPORT int X509_policy_tree_level_count(const X509_POLICY_TREE *tree);
OPENSSL_EXPORT X509_POLICY_LEVEL *X509_policy_tree_get0_level(
    const X509_POLICY_TREE *tree, int i);

OPENSSL_EXPORT STACK_OF(X509_POLICY_NODE) *X509_policy_tree_get0_policies(
    const X509_POLICY_TREE *tree);

OPENSSL_EXPORT STACK_OF(X509_POLICY_NODE) *X509_policy_tree_get0_user_policies(
    const X509_POLICY_TREE *tree);

OPENSSL_EXPORT int X509_policy_level_node_count(X509_POLICY_LEVEL *level);

OPENSSL_EXPORT X509_POLICY_NODE *X509_policy_level_get0_node(
    X509_POLICY_LEVEL *level, int i);

OPENSSL_EXPORT const ASN1_OBJECT *X509_policy_node_get0_policy(
    const X509_POLICY_NODE *node);

OPENSSL_EXPORT STACK_OF(POLICYQUALINFO) *X509_policy_node_get0_qualifiers(
    const X509_POLICY_NODE *node);
OPENSSL_EXPORT const X509_POLICY_NODE *X509_policy_node_get0_parent(
    const X509_POLICY_NODE *node);

#ifdef __cplusplus
}
#endif
#endif
