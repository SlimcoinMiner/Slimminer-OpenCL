#define SHA256_LEN 64
#define OUTPUT_COUNTER ((OUTPUT_SIZE >> 2) - 1)

#define ror(x,n) ((x >> n) | (x << (32-n)))
#define Ch(x,y,z) bitselect(z, y, x)
#define Maj(x,y,z) bitselect(x, y, z ^ x)

#define Sigma0(x) ((ror(x,2))  ^ (ror(x,13)) ^ (ror(x,22)))
#define Sigma1(x) ((ror(x,6))  ^ (ror(x,11)) ^ (ror(x,25)))
#define sigma0(x) ((ror(x,7))  ^ (ror(x,18)) ^(x>>3))
#define sigma1(x) ((ror(x,17)) ^ (ror(x,19)) ^(x>>10))

#define SWAP32(n)       as_uint(as_uchar4(n).s3210) 

/*
#define rotl(x,y) rotate(x,y)
#define Ch(x,y,z) bitselect(z,y,x)
#define Maj(x,y,z) Ch((x^z),y,z)

#define Sigma0(x)		(rotl(x, 30U) ^ rotl(x, 19U) ^ rotl(x, 10U))
#define Sigma1(x)		(rotl(x, 26U) ^ rotl(x, 21U) ^ rotl(x, 7U))
#define sigma0(x)		(rotl(x, 25U) ^ rotl(x, 14U) ^ (x>>3U))
#define sigma1(x)		(rotl(x, 15U) ^ rotl(x, 13U) ^ (x>>10U))

__constant uint ES[2] = { 0x00FF00FF, 0xFF00FF00 };
#define SWAP32(n) (rotl(n & ES[0], 24U)|rotl(n & ES[1], 8U))
*/


__constant uint k[] = {
   0x428a2f98U, 0x71374491U, 0xb5c0fbcfU, 0xe9b5dba5U, 0x3956c25bU, 0x59f111f1U, 0x923f82a4U, 0xab1c5ed5U,
   0xd807aa98U, 0x12835b01U, 0x243185beU, 0x550c7dc3U, 0x72be5d74U, 0x80deb1feU, 0x9bdc06a7U, 0xc19bf174U,
   0xe49b69c1U, 0xefbe4786U, 0x0fc19dc6U, 0x240ca1ccU, 0x2de92c6fU, 0x4a7484aaU, 0x5cb0a9dcU, 0x76f988daU,
   0x983e5152U, 0xa831c66dU, 0xb00327c8U, 0xbf597fc7U, 0xc6e00bf3U, 0xd5a79147U, 0x06ca6351U, 0x14292967U,
   0x27b70a85U, 0x2e1b2138U, 0x4d2c6dfcU, 0x53380d13U, 0x650a7354U, 0x766a0abbU, 0x81c2c92eU, 0x92722c85U,
   0xa2bfe8a1U, 0xa81a664bU, 0xc24b8b70U, 0xc76c51a3U, 0xd192e819U, 0xd6990624U, 0xf40e3585U, 0x106aa070U,
   0x19a4c116U, 0x1e376c08U, 0x2748774cU, 0x34b0bcb5U, 0x391c0cb3U, 0x4ed8aa4aU, 0x5b9cca4fU, 0x682e6ff3U,
   0x748f82eeU, 0x78a5636fU, 0x84c87814U, 0x8cc70208U, 0x90befffaU, 0xa4506cebU, 0xbef9a3f7U, 0xc67178f2U
};

#define hash0 (uint8)(0x6a09e667U, 0xbb67ae85U,	0x3c6ef372U, 0xa54ff53aU, 0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U)

#define a state.s0	
#define b state.s1	
#define c state.s2	
#define d state.s3	
#define e state.s4	
#define f state.s5	
#define g state.s6	
#define h state.s7

__kernel void search(__global uint* output)
{
}

__kernel void hashes(__global const uint* data, __global uint*restrict output)
{
	uint nonce = get_global_id(0);  
	uint w[63];

	uint8 hash = hash0;
	
// 1 chunk	
	uint8 state = hash;
	((uint16*)w)[0] = ((__global uint16*)data)[0];
	for (int i = 16; i < 64; i++) {
		w[i] = w[i-16] + sigma0(w[i-15]) + w[i-7] + sigma1(w[i-2]); 
	}
	for (int i = 0; i < 64; i++) {
		uint t1 = h + Sigma1(e) + Ch(e,f,g) + k[i] + w[i];
		uint t2 = Sigma0(a) + Maj(a,b,c);
		h = g; g = f; f = e; e = d+t1; d = c; c = b; b = a; a = t1+t2;
	}
	hash += state;

// 2 chunk
	state = hash;	
	((uint16*)w)[0] = (uint16)(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	w[0] = data[16];
	w[1] = data[17];
	w[2] = data[18];
	w[3] = SWAP32(nonce);
	w[4] = 0x80000000U; // message ending 1 bit
	w[15] = 640U; // message length in bits

	for (int i = 16; i < 64; i++) {
		w[i] = w[i-16] + sigma0(w[i-15]) + w[i-7] + sigma1(w[i-2]); 
	}
	for (int i = 0; i < 64; i++) {
		uint t1 = h + Sigma1(e) + Ch(e,f,g) + k[i] + w[i];
		uint t2 = Sigma0(a) + Maj(a,b,c);
		h = g; g = f; f = e; e = d+t1; d = c; c = b; b = a; a = t1+t2;
	}
	hash += state;
	hash = (uint8)(SWAP32(hash.s0), SWAP32(hash.s1), SWAP32(hash.s2), SWAP32(hash.s3),
				 SWAP32(hash.s4), SWAP32(hash.s5), SWAP32(hash.s6), SWAP32(hash.s7));

	uchar index = 0;
	uchar value; 
	for (int step = 0; step < STEPS && index < SHA256_LEN - 1; step++) {
		value = ((uchar*)&hash)[index>>1];
		index += (index % 2 == 0 ? value >> 4 : value & 0x0F) + 1;
	}

	if (index == SHA256_LEN - 1) {
/*	
		int p = output[OUTPUT_COUNTER]++;	
		output[p] = nonce;
//		*(__global uint8*)(&output[9*p+1]) = hash;
*/
	
		index = 0;
		uchar tmp_array[SHA256_LEN + 1];
		*((ulong8*)tmp_array) = 0xffffffffffffffff;
		for (int step = 0; step < STEPS && index < SHA256_LEN - 1; step++) {
			value = ((uchar*)&hash)[index>>1];
			index += (index % 2 == 0 ? value >> 4 : value & 0x0F) + 1;
			
			value = ((uchar*)&hash)[index>>1];
			value = (index % 2 == 0 ? value >> 4 : value & 0x0F);
			value += (value < 10 ? 48 : 87);
			tmp_array[SHA256_LEN] = value;
			
			uint8 hash_temp = hash0;
	// 1 chunk	
			state = hash_temp;
			w[0] = SWAP32(((uint*)tmp_array)[0]);
			w[1] = SWAP32(((uint*)tmp_array)[1]);
			w[2] = SWAP32(((uint*)tmp_array)[2]);
			w[3] = SWAP32(((uint*)tmp_array)[3]);
			w[4] = SWAP32(((uint*)tmp_array)[4]);
			w[5] = SWAP32(((uint*)tmp_array)[5]);
			w[6] = SWAP32(((uint*)tmp_array)[6]);
			w[7] = SWAP32(((uint*)tmp_array)[7]);
			w[8] = SWAP32(((uint*)tmp_array)[8]);
			w[9] = SWAP32(((uint*)tmp_array)[9]);
			w[10] = SWAP32(((uint*)tmp_array)[10]);
			w[11] = SWAP32(((uint*)tmp_array)[11]);
			w[12] = SWAP32(((uint*)tmp_array)[12]);
			w[13] = SWAP32(((uint*)tmp_array)[13]);
			w[14] = SWAP32(((uint*)tmp_array)[14]);
			w[15] = SWAP32(((uint*)tmp_array)[15]);
			for (int i = 16; i < 64; i++) {
				w[i] = w[i-16] + sigma0(w[i-15]) + w[i-7] + sigma1(w[i-2]); 
			}
			for (int i = 0; i < 64; i++) {
				uint t1 = h + Sigma1(e) + Ch(e,f,g) + k[i] + w[i];
				uint t2 = Sigma0(a) + Maj(a,b,c);
				h = g; g = f; f = e; e = d+t1; d = c; c = b; b = a; a = t1+t2;
			}
			hash_temp += state;
		
		// 2 chunk
			state = hash_temp;	
			((uint16*)w)[0] = 0;
			((uchar*)w)[3] = value;
			((uchar*)w)[2] = 0x80; // message ending 1 bit
			w[15] = 520U; // message length in bits
		
			for (int i = 16; i < 64; i++) {
				w[i] = w[i-16] + sigma0(w[i-15]) + w[i-7] + sigma1(w[i-2]); 
			}
			for (int i = 0; i < 64; i++) {
				uint t1 = h + Sigma1(e) + Ch(e,f,g) + k[i] + w[i];
				uint t2 = Sigma0(a) + Maj(a,b,c);
				h = g; g = f; f = e; e = d+t1; d = c; c = b; b = a; a = t1+t2;
			}
			hash_temp += state;
			hash_temp = (uint8)(SWAP32(hash_temp.s0), SWAP32(hash_temp.s1), SWAP32(hash_temp.s2), SWAP32(hash_temp.s3),
						 SWAP32(hash_temp.s4), SWAP32(hash_temp.s5), SWAP32(hash_temp.s6), SWAP32(hash_temp.s7));
			
	//		((uint8*)mixed_hash)[count] = hash_temp;
			for (int j = 0; j < SHA256_LEN; j++) {
				uchar v = ((uchar*)&hash_temp)[j>>1];
				v = (j % 2 == 0 ? v >> 4 : v & 0x0F);
				tmp_array[j] = v + (v < 10 ? 48 : 87);
			}
		}
//  	
		if (index == SHA256_LEN - 1 && value == tmp_array[SHA256_LEN - 1]) {
			int p = output[OUTPUT_COUNTER]++;	
			output[p] = nonce;
//			output[2*p+1] = value;
//			*(__global uint8*)(&output[9*p+1]) = hash;
		}
	}	
}
/*
__kernel void hashes_(__global const uint* data, __global uint* output)
{
	uint nonce = get_global_id(0);  
	uint w[63];

	uint8 hash = {0x6a09e667, 0xbb67ae85,	0x3c6ef372,	0xa54ff53a,	0x510e527f,	0x9b05688c,	0x1f83d9ab,	0x5be0cd19};
	
// 1 chunk	
	uint8 state = hash;
//	((uint16*)w)[0] = (uint16)(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	w[0] = 0x80000000; // message ending 1 bit
	w[15] = 0; // message length in bits
#pragma unroll
	for (int i = 16; i < 64; i++) {
		w[i] = w[i-16] + sigma0(w[i-15]) + w[i-7] + sigma1(w[i-2]); 
	}
#pragma unroll
	for (int i = 0; i < 64; i++) {
		uint t1 = h + Sigma1(e) + Ch(e,f,g) + k[i] + w[i];
		uint t2 = Sigma0(a) + Maj(a,b,c);
		h = g; g = f; f = e; e = d+t1; d = c; c = b; b = a; a = t1+t2;
	}
	hash += state;

	hash = (uint8)(SWAP32(hash.s0), SWAP32(hash.s1), SWAP32(hash.s2), SWAP32(hash.s3),
				 SWAP32(hash.s4), SWAP32(hash.s5), SWAP32(hash.s6), SWAP32(hash.s7));


	if (get_global_id(0) - get_global_offset(0) == 1) {
		int p = output[OUTPUT_COUNTER]++;	
		output[9*p] = nonce;
		*(__global uint8*)(&output[9*p+1]) = hash;
	}	
}
*/