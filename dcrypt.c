// Copyright (c) 2013-2014 The Slimcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <string.h>
#include <stdlib.h>
#include <math.h>          //pow

#include "miner.h"
#include "dcrypt.h"

//the base size for malloc/realloc will be 1KB
#define REALLOC_BASE_SZ   (1024*4)

typedef struct
{
  uint8_t *array;
  unsigned long long actual_array_sz;
  uint32_t times_realloced;

} Extend_Array;

inline void Extend_Array_init(Extend_Array *ExtArray)
{
  //initial values
  ExtArray->array = 0;
  ExtArray->actual_array_sz = 0;
  ExtArray->times_realloced = 0;
  return;
}

uint32_t hex_char_to_int(uint8_t c)
{
  if(c >= '0' && c <= '9')
    return c - '0';

  if(c >= 'a' && c <= 'f')
    return 10 + c - 'a';

  if(c >= 'A' && c <= 'F')
    return 10 + c - 'A';

  return -1;
}

inline void join_to_array(uint8_t *array, uint8_t join)
{
  *(array + SHA256_LEN) = join;
  return;
}

void extend_array(Extend_Array *extend_array, unsigned long long used_array_sz,
                  uint8_t *extend, uint32_t extend_sz, uint8_t hashed_end)
{
  if(!extend_array)
    return;

  //make a few references
  unsigned long long *actual_array_sz = &extend_array->actual_array_sz;
  uint32_t *times_realloced = &extend_array->times_realloced;

  //if there is not enough room
  if((*actual_array_sz - used_array_sz) < (extend_sz + hashed_end))
  {
    //if extend_array->array has already been malloc'd
    if(*times_realloced)
    {
      //reallocate on an exponential curve, modern computers have plenty ram
      *actual_array_sz += pow(2, (*times_realloced)++) * REALLOC_BASE_SZ;
      extend_array->array = (uint8_t*)realloc(extend_array->array, *actual_array_sz);
    }else{
      //allocate the base size
      *actual_array_sz += REALLOC_BASE_SZ;
      (*times_realloced)++;

      extend_array->array = (uint8_t*)malloc(*actual_array_sz); //if we have not allocated anything, malloc
    }
  }

  //copy the data to be extended
  memcpy(extend_array->array + used_array_sz, extend, extend_sz);

  if(hashed_end)
    *(extend_array->array + used_array_sz + extend_sz) = 0; //add the final \000 of the whole string array

  return;
}

uint64 mix_hashed_nums(uint8_t *hashed_nums, const uint8_t *unhashedData, size_t unhashed_sz,
                       uint8_t **mixed_hash, uint8_t *hash_digest)
{
  uint32_t i, index = 0;
  const uint32_t hashed_nums_len = SHA256_LEN;

  uint64 count;
  uint8_t tmp_val, tmp_array[SHA256_LEN + 2];

  //initialize the class for the extend hash
  Extend_Array new_hash;
  Extend_Array_init(&new_hash);

  //set the first hash length in the temp array to all 0xff
  memset(tmp_array, 0xff, SHA256_LEN);
  //set the last two bytes to \000
  *(tmp_array + SHA256_LEN) = *(tmp_array + SHA256_LEN + 1) = 0;

  for(count = 0;; count++)
  {
    //+1 to keeps a 0 value of *(hashed_nums + index) moving on
    i = hex_char_to_int(*(hashed_nums + index)) + 1;
    index += i;

    //if we hit the end of the hash, rehash it
    if(index >= hashed_nums_len)
    {
      index = index % hashed_nums_len;
      sha256_to_str(hashed_nums, hashed_nums_len, hashed_nums, hash_digest); //rescramble
    }

    tmp_val = *(hashed_nums + index);

    join_to_array(tmp_array, tmp_val); //plop tmp_val at the end of tmp_array
    sha256_to_str(tmp_array, SHA256_LEN + 1, tmp_array, hash_digest);

    //extend the expanded hash to the array
    extend_array(&new_hash, count * SHA256_LEN, tmp_array, SHA256_LEN, false);

    //check if the last value of hashed_nums is the same as the last value in tmp_array
    if(index == hashed_nums_len - 1)
      if(tmp_val == *(tmp_array + SHA256_LEN - 1))
      {
        //add to count since we extended the array, but break will exit the for loop and count
        // will not get incremenented by the for loop
        count++;
        break;
      }

  }

  //extend the unhashed data to the end and add the \000 to the end
  extend_array(&new_hash, count * SHA256_LEN, (u8int*)unhashedData, unhashed_sz, true);

  //assign the address of new_hash's array to mixed_hash
  *mixed_hash = new_hash.array;

  return count * SHA256_LEN + unhashed_sz;
}

u8int *dcrypt_buffer_alloc()
{
  return malloc(DCRYPT_DIGEST_LENGTH);
}

void dcrypt(const uint8_t *data, size_t data_sz, uint8_t *hash_digest, u32int *hashRet)
{
  uint8_t hashed_nums[SHA256_LEN + 1], *mix_hash;

  bool allocDigest = false;
  if(!hash_digest)
  {
    hash_digest = (uint8_t*)malloc(DCRYPT_DIGEST_LENGTH);
    allocDigest = true;
  }

  sha256_to_str(data, data_sz, hashed_nums, hash_digest);

  //mix the hashes up, magority of the time takes here
  uint64 mix_hash_len = mix_hashed_nums(hashed_nums, data, data_sz, &mix_hash, hash_digest);

  //apply the final hash to the output
  sha256((const uint8_t*)mix_hash, mix_hash_len, hashRet);

  free(mix_hash);

  if(allocDigest)
    free(hash_digest);

  //sucess
  return;
}


// CPU Optimized
void extend_array_opt(uint8_t *array, unsigned long long used_array_sz,
                  uint8_t *extend, uint32_t extend_sz, uint8_t hashed_end)
{
  //if there is not enough room
  if(REALLOC_BASE_SZ < (used_array_sz + extend_sz + hashed_end))
  {
  	  applog(LOG_ERR, "Extend array: %u, %u", used_array_sz, REALLOC_BASE_SZ);
  	  exit(-1);
  }

  //copy the data to be extended
  memcpy(array + used_array_sz, extend, extend_sz);

  if(hashed_end)   
    *(array + used_array_sz + extend_sz) = 0; //add the final \000 of the whole string array

  return;
}

int mix_hashed_nums_opt(uint8_t *hashed_nums, const uint8_t *unhashedData, size_t unhashed_sz,
                       uint8_t *mixed_hash, uint8_t *hash_digest)
{
  uint32_t i, index = 0;
  const uint32_t hashed_nums_len = SHA256_LEN;

  uint64 count;
  uint8_t tmp_val, tmp_array[SHA256_LEN + 2];

  bool found = false;
  for(count = 0; count < opt_depth; count++)
  {
	    //+1 to keeps a 0 value of *(hashed_nums + index) moving on
	    i = hex_char_to_int(*(hashed_nums + index)) + 1;
	    index += i;

	    //if we hit the end of the hash, rehash it
	    if(index >= hashed_nums_len)
	    	break;

	    if(index == hashed_nums_len - 1)
	    	found = true;
  }
  if (!found)
	  return -1;

  index = 0;
  //initialize the class for the extend hash

  //set the first hash length in the temp array to all 0xff
  memset(tmp_array, 0xff, SHA256_LEN);
  //set the last two bytes to \000
  *(tmp_array + SHA256_LEN) = *(tmp_array + SHA256_LEN + 1) = 0;

  for(count = 0;; count++)
  {
    //+1 to keeps a 0 value of *(hashed_nums + index) moving on
    i = hex_char_to_int(*(hashed_nums + index)) + 1;
    index += i;
    
    //if we hit the end of the hash, rehash it
    if(index >= hashed_nums_len)
    {
    	  applog(LOG_ERR, "Out of array: %u", index);
    	  return -2;
//      index = index % hashed_nums_len;
//      sha256_to_str(hashed_nums, hashed_nums_len, hashed_nums, hash_digest); //rescramble
    }
    
    tmp_val = *(hashed_nums + index);

    join_to_array(tmp_array, tmp_val); //plop tmp_val at the end of tmp_array
    sha256_to_str(tmp_array, SHA256_LEN + 1, tmp_array, hash_digest);

    //extend the expanded hash to the array
    extend_array_opt(mixed_hash, count * SHA256_LEN, tmp_array, SHA256_LEN, false);
    if (count * SHA256_LEN + SHA256_LEN >= REALLOC_BASE_SZ)
    	applog(LOG_INFO, "Extend array on step %u ", count);

    //check if the last value of hashed_nums is the same as the last value in tmp_array
    if(index == hashed_nums_len - 1) {
      if(tmp_val == *(tmp_array + SHA256_LEN - 1))
      {
//          applog(LOG_INFO, "Last char check passed %c - %c", tmp_val, *(tmp_array + SHA256_LEN - 1));
//       	  applog(LOG_INFO, "Found path in %u steps, %c - %c", count, tmp_val, *(tmp_array + SHA256_LEN - 1));
        //add to count since we extended the array, but break will exit the for loop and count
        // will not get incremenented by the for loop
        count++;
        break;
      }
//      applog(LOG_INFO, "Last char check failed %c - %c", tmp_val, *(tmp_array + SHA256_LEN - 1));
    	return 0;
    }
  }

  //extend the unhashed data to the end and add the \000 to the end
  extend_array_opt(mixed_hash, count * SHA256_LEN, (u8int*)unhashedData, unhashed_sz, true);
  if (count * SHA256_LEN + unhashed_sz >= REALLOC_BASE_SZ)
  	applog(LOG_INFO, "Extend array with data on step %u ", count);

  return count * SHA256_LEN + unhashed_sz;
}

int dcrypt_opt(const uint8_t *data, size_t data_sz, uint8_t *hash_digest, u32int *hashRet, uint8_t *mix_hash)
{
  uint8_t hashed_nums[SHA256_LEN + 1];

  sha256_to_str(data, data_sz, hashed_nums, hash_digest);

  //mix the hashes up, magority of the time takes here
  int mix_hash_len = mix_hashed_nums_opt(hashed_nums, data, data_sz, mix_hash, hash_digest);

  //apply the final hash to the output
  if (mix_hash_len > 0)
	  sha256((const uint8_t*)mix_hash, mix_hash_len, hashRet);

  //sucess
  return mix_hash_len;
}

//#define OUTPUT_SIZE 0xffffff
//#define OUTPUT_COUNTER ((OUTPUT_SIZE >> 2) - 1)


// OpenCL version
int scanhash_dcrypt_gpu(int thr_id, uint32_t *pdata, unsigned char *digest,
		const uint32_t *ptarget, uint32_t max_nonce, unsigned long *hashes_done,
		unsigned long *hashes_skipped, GPU *gpu) {
	uint32_t block[20], hash[8];
	uint32_t nNonce = pdata[19] - 1;
	const uint32_t first_nonce = pdata[19];
	const uint32_t Htarg = ptarget[7]; //the last element in the target is the first 32 bits of the target
	uint8_t *new_hash = (uint8_t*) malloc(REALLOC_BASE_SZ);

	//copy the block (first 80 bytes of pdata) into block
	memcpy(block, pdata, 80);

	nNonce = max_nonce;
	scanhash_dcrypt_opencl(thr_id, block, ptarget, first_nonce, max_nonce, gpu);
	applog(LOG_DEBUG, "GPU: %02d, hashes checked: %u", thr_id, gpu->output[(gpu->output_size >> 2) - 1]);
	int j;
	for (j = 0; j < 19; j++)
		block[j] = swab32(block[j]);
/*
	 int i;
	 for (i = 0; i < gpu->output[OUTPUT_COUNTER]; i++) {
	 int nonce = gpu->output[i*9];
	 uint8_t hash_str[SHA256_LEN + 1];
	 digest_to_string(&gpu->output[i*9+1], hash_str);

	 block[19] = nonce;
	 uint8_t hashed_nums[SHA256_LEN + 1];
	 sha256_to_str(block, 80, hashed_nums, digest);
	 applog(LOG_INFO, "GPU: %02d, %s", thr_id, hashed_nums);
	 applog(LOG_INFO, "GPU: %02d, found: %u, %s", thr_id, nonce, hash_str);
	 }
*/
	int i = 0;
	for (; i < gpu->output[(gpu->output_size >> 2) - 1]; i++) {
		uint32_t nonce = gpu->output[i];
//		uint32_t last = gpu->output[i*2+1];
		 block[19] = nonce;

		 uint8_t hashed_nums[SHA256_LEN + 1];
		 sha256_to_str(block, 80, hashed_nums, digest);
//		 applog(LOG_INFO, "GPU: %02d, %s", thr_id, hashed_nums);

//		int status = dcrypt_opt((u8int*) block, 80, digest, hash, new_hash);
		int status = 1;
		dcrypt((u8int*) block, 80, digest, hash);
		if (status > 0) {
			*hashes_skipped += 1;
			//hash[7] <= Htarg just compares the first 32 bits of the hash with the target
			// full_test fully compares the entire hash with the entire target
		    if(hash[7] <= target) {
				applog(LOG_INFO,
						"GPU: %02d, share found: %u, %08x%08x%08x%08x%08x%08x%08x%08x",
						thr_id, nonce, hash[7], hash[6], hash[5], hash[4], hash[3],
						hash[2], hash[1], hash[0]);
				shares_found++;
		    }
		    if(hash[7] <= Htarg && fulltest(hash, ptarget)) {
				applog(LOG_INFO,
						"GPU: %02d, block found: %u, %08x%08x%08x%08x%08x%08x%08x%08x",
						thr_id, nonce, hash[7], hash[6], hash[5], hash[4], hash[3],
						hash[2], hash[1], hash[0]);

				*hashes_done = nNonce - first_nonce + 1;
				pdata[19] = nonce;
				return true;
			}
		} else {
//			if (status < 0)
				applog(LOG_INFO, "GPU: %02d, hash skipped: %u, status=%d", thr_id, nonce, status);
		}
	}

	*hashes_done = nNonce - pdata[19] + 1;
	pdata[19] = nNonce;
	//No luck yet
	free(new_hash);
	return 0;
}

// CPU optimized version
int scanhash_dcrypt_opt(int thr_id, uint32_t *pdata, unsigned char *digest,
		const uint32_t *ptarget, uint32_t max_nonce, unsigned long *hashes_done,
		unsigned long *hashes_skipped) {
	uint32_t block[20], hash[8], hash2[8];
	uint32_t nNonce = pdata[19] - 1;
	const uint32_t first_nonce = pdata[19];
	const uint32_t Htarg = ptarget[7]; //the last element in the target is the first 32 bits of the target
	uint8_t *new_hash = (uint8_t*) malloc(REALLOC_BASE_SZ);
	uint8_t digest2[32];

	//copy the block (first 80 bytes of pdata) into block
	memcpy(block, pdata, 80);
	int j;
	for (j = 0; j < 19; j++)
		block[j] = swab32(block[j]);
	do {
		//increment nNonce
		block[19] = ++nNonce;

		if (dcrypt_opt((u8int*) block, 80, digest, hash, new_hash) > 0) {
		    if(hash[7] <= target) {
				applog(LOG_INFO,
						"GPU: %02d, share found: %u, %08x%08x%08x%08x%08x%08x%08x%08x",
						thr_id, nNonce, hash[7], hash[6], hash[5], hash[4], hash[3],
						hash[2], hash[1], hash[0]);
				shares_found++;
				dcrypt((u8int*) block, 80, digest2, hash2);
				if (hash2[7] > target)
					applog(LOG_INFO,
							"GPU: %02d, share check failed: %u, %08x%08x%08x%08x%08x%08x%08x%08x",
							thr_id, nNonce, hash2[7], hash2[6], hash2[5], hash2[4], hash2[3],
							hash2[2], hash2[1], hash2[0]);
		    }
			if (hash[7] <= Htarg && fulltest(hash, ptarget)) {
				*hashes_done = nNonce - pdata[19] + 1;
				pdata[19] = block[19];

				//found a hash!
				return 1;
			}
		} else
			*hashes_skipped += 1;
	} while (nNonce < max_nonce && !work_restart[thr_id].restart);
	*hashes_done = nNonce - pdata[19] + 1;
	pdata[19] = nNonce;
	//No luck yet
	free(new_hash);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////
//////////////////// Various tests
////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/* Tests the comparison to two hashes

  //Hash the word "Dog" with Dcrypt and strinify the hash
  u32int ret[8];
  char string[65];
  dcrypt("Dog", 3, 0, ret);
  digest_to_string((u8int*)ret, string);
  printf("String is %s\n", string);

  //hash the word "Doge" with Dcrypt and stringify the hash
  u32int ret2[8];
  char string2[65];
  dcrypt("Doge", 4, 0, ret2);
  digest_to_string((u8int*)ret2, string2);
  printf("String2 is %s\n", string2);

  //compare the last elements, which correspond the the uint256's first 32 bytes
  if(ret[7] < ret2[7])
    printf("String1 is smaller %08x < %08x\n", ret[7], ret2[7]);
  else
    printf("String1 is greater %08x >= %08x\n", ret[7], ret2[7]);

  //Apply the full test to make sure
  printf("Full test returns %d\n", fulltest(ret2, ret));

 */

/* Tests the scan feature of dcrypt
  u8int digest[DCRYPT_DIGEST_LENGTH], string[65], strTarget[65];
  unsigned long hDone;
  u32int pdata[20], retHash[8], target[8];

  //fill pdata with something
  memset(pdata, 0xff, 20 * sizeof(u32int));
  pdata[19] = 0; //element 19 is the beginning of where nNonce begins

  //fill the target with 1's
  memset(target, 0xff, 8 * sizeof(u32int));
  //the last element is the uint256's first 32 bits, set the target to 0x00000ffffffffff....
  target[7] = 0x000ffff;

  //scan for them hashes
  scanhash_dcrypt(0, pdata, digest, target, -1, &hDone);

  //Get the hash of pdata
  dcrypt((u8int*)pdata, 80, digest, retHash);

  //stringify the returned hash and the target
  digest_to_string((u8int*)retHash, string);
  digest_to_string((u8int*)target, strTarget);

  printf("  Hash is %s %08x\n", string, retHash[7]);
  printf("Target is %s %08x\n", strTarget, target[7]);
  printf("Nonce %d Hashes Done %ld\n", pdata[19], hDone);
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////
//////////////////// Various tests
////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
