// Copyright (c) 2013-2014 The Slimcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DCRYPT_H
#define DCRYPT_H

#include "dcrypt_sha256.h"

#define DCRYPT_DIGEST_LENGTH SHA256_DIGEST_LENGTH 

int scanhash_dcrypt_opt(int thr_id, uint32_t *pdata,
                    unsigned char *digest, const uint32_t *ptarget,
                    uint32_t max_nonce, unsigned long *hashes_done, unsigned long *hashes_skipped);

int scanhash_dcrypt_gpu(int thr_id, uint32_t *pdata,
                    unsigned char *digest, const uint32_t *ptarget,
                    uint32_t max_nonce, unsigned long *hashes_done, unsigned long *hashes_skipped, GPU *gpu);

u8int *dcrypt_buffer_alloc();

#endif
