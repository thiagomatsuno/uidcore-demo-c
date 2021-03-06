/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __BIP32_H__
#define __BIP32_H__

#include <stdint.h>
#include <stdlib.h>
#include "ecdsa.h"
#include "options.h"

#define TESTNET
#ifdef TESTNET
	#define NETWORK_BYTE 0x6f
	#define VERSION_KPUB    0x043587CF
	#define VERSION_KPRIV   0x04358394
#else
	#define NETWORK_BYTE 0x00
	#define VERSION_KPUB    0x0488B21E
	#define VERSION_KPRIV   0x0488ADE4
#endif

typedef struct {
	const char *bip32_name;    // string for generating BIP32 xprv from seed
	const ecdsa_curve *params; // ecdsa curve parameters, null for ed25519
} curve_info;

typedef struct {
	uint32_t depth;
	uint32_t child_num;
	uint8_t chain_code[32];
	uint8_t private_key[32];
	uint8_t public_key[33];
	const curve_info *curve;
} HDNode;

int hdnode_from_xpub(uint32_t depth, uint32_t child_num, const uint8_t *chain_code, const uint8_t *public_key, const char *curve, HDNode *out);

int hdnode_from_xprv(uint32_t depth, uint32_t child_num, const uint8_t *chain_code, const uint8_t *private_key, const char *curve, HDNode *out);

int hdnode_from_seed(const uint8_t *seed, int seed_len, const char *curve, HDNode *out);

#define hdnode_private_ckd_prime(X, I) hdnode_private_ckd((X), ((I) | 0x80000000))

int hdnode_private_ckd(HDNode *inout, uint32_t i);

int hdnode_public_ckd(HDNode *inout, uint32_t i);

int hdnode_public_ckd_address_optimized(const curve_point *pub, const uint8_t *public_key, const uint8_t *chain_code, uint32_t i, uint8_t version, char *addr, int addrsize);

#if USE_BIP32_CACHE

int hdnode_private_ckd_cached(HDNode *inout, const uint32_t *i, size_t i_count);

#endif

uint32_t hdnode_fingerprint(HDNode *node);

void hdnode_fill_public_key(HDNode *node);

int hdnode_sign(HDNode *node, const uint8_t *msg, uint32_t msg_len, uint8_t *sig, uint8_t *pby);
int hdnode_sign_digest(HDNode *node, const uint8_t *digest, uint8_t *sig, uint8_t *pby);

void hdnode_serialize_public(const HDNode *node, uint32_t fingerprint, char *str, int strsize);

void hdnode_serialize_private(const HDNode *node, uint32_t fingerprint, char *str, int strsize);

int hdnode_deserialize(const char *str, HDNode *node);

// Private
void hdnode_serialize(const HDNode *node, uint32_t fingerprint, uint32_t version, char use_public, char *str, int strsize);

void hdnode_get_address_raw(HDNode *node, uint8_t version, uint8_t *addr_raw);

const curve_info *get_curve_by_name(const char *curve_name);

#endif
