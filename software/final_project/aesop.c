/**
 * AES 128-bit encryption algorithm
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "aes.h"
#include "aesop.h"

static const uint8_t CNST_AES_NB = 4;
static const uint8_t CNST_AES_NR = 10;
static const uint8_t CNST_AES_NK = 4;

typedef union AES_STATE {
	uint32_t state32[4];
	uint8_t state8[4][4];
} aes_state;

/** STORAGE STRUCTURE.
[0 0]  [1 0] [2 0] ...
[0 1]  [1 1] ...
[0 2]  ...
[0 3] ...
*/

void aes_encrypt(uint8_t* data_in, uint32_t* data_out, uint32_t* keyarr) {
	aes_state state; // = malloc(sizeof(uint32_t) * 4); //Flattened.
	aes_state state_in; // = malloc(sizeof(uint32_t) * 4); //Temporary.
	// Generate round keys.
	
	uint32_t key_out[(CNST_AES_NR+1)][CNST_AES_NB];
	key_expansion(&keyarr[0], &key_out[0][0], CNST_AES_NK, CNST_AES_NB, CNST_AES_NR);

	memcpy(state.state8, data_in, sizeof(state));
	
	add_round_key(state.state32, key_out[0], state_in.state32);
	memcpy(&state, &state_in, sizeof(state_in));
	
	aes_state state_in0, state_in1, state_in2, state_in3;
	
	for(uint8_t round = 1; round < CNST_AES_NR; round += 1) {
		// Sub words.
		for (uint8_t k = 0; k < 4; k++) sub_word(&state.state8[k], &state_in0.state8[k]);
		//memcpy(&state, &state_in, sizeof(state_in));
		
		// Shift Rows.
		shift_rows(state_in0.state8, state_in1.state8);
		// memcpy(&state, &state_in, sizeof(state_in));
		
		// Mix Columns.
		mix_columns(state_in1.state8, state_in2.state8);
		//memcpy(&state, &state_in, sizeof(state_in));
		
		// Add Round Key
		add_round_key(state_in2.state32, key_out[round], state_in3.state32);
		memcpy(&state, &state_in3, sizeof(state_in3));
	}
	
	// Sub words.
	for (uint8_t k = 0; k < 4; k++) sub_word(&state.state8[k], &state_in0.state8[k]);
	//memcpy(&state, &state_in, sizeof(state_in));
	
	// Shift Rows.
	shift_rows(&state_in0.state8, &state_in1.state8);
	//memcpy(&state, &state_in, sizeof(state_in));

	// Add Round Key
	add_round_key(&state_in1.state32, &key_out[CNST_AES_NR], &state_in2.state32);
	//memcpy(&state, &state_in, sizeof(state_in));

	// Now everything is ready.
	memcpy(data_out, state_in2.state32, sizeof(state_in2.state32));
}

// Verified working.
void key_expansion(uint8_t* key_in, uint32_t* key_out, uint8_t Nk, uint8_t Nb, uint8_t Nr) {
	uint32_t temp;
	for(uint8_t i=0; i<Nk; i++) {
		key_out[i] = key_in[4*i+3] | (key_in[4*i+2] << 8) | (key_in[4*i+1] << 16) | (key_in[4*i] << 24);
	}
	uint8_t i = Nk;
	while(i<Nb*(Nr+1)) {
		temp = key_out[i-1];
		if(i % Nk == 0) {
			temp = sub_word32(rot_word(temp)) ^ Rcon[i/Nk];
		}
		key_out[i] = key_out[i-Nk] ^ temp;
		++i;
	}
}

// Verified working.
void shift_rows(uint8_t* in, uint8_t* result) {
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			result[j*4+i] = in[((-i+3+j)%4)*4+i];
		}
	}
}

uint8_t sub_byte(uint8_t byte_in) {
	return aes_sbox[(((byte_in & 0xF0) >> 4) * 16) + (byte_in & 0x0F)];
}

uint32_t sub_word32(uint32_t word_in) {
	uint8_t bo_3 = sub_byte((word_in & 0xFF000000) >> 24);
	uint8_t bo_2 = sub_byte((word_in & 0x00FF0000) >> 16);
	uint8_t bo_1 = sub_byte((word_in & 0x0000FF00) >> 8);
	uint8_t bo_0 = sub_byte((word_in & 0x000000FF));

	return (bo_3 << 24) | (bo_2 << 16) | (bo_1 << 8) | bo_0;
}

void sub_word(uint8_t* word_in, uint8_t* word_out) {
	word_out[3] = sub_byte(word_in[3]);
	word_out[2] = sub_byte(word_in[2]);
	word_out[1] = sub_byte(word_in[1]);
	word_out[0] = sub_byte(word_in[0]);
}

uint32_t rot_word(uint32_t word_in) {
	return (((word_in & 0x00FFFFFF) << 8) & 0xFFFFFF00) | ((word_in & 0xFF000000) >> 24);
}

void mix_columns(uint8_t* mat_in, uint8_t* result) {
	uint8_t state_deasm[4][4];	// State_disassembled.
	uint8_t result_deasm[4][4];	// Result disassembled.
	
	memcpy(&state_deasm, mat_in, sizeof(state_deasm));
		
	// Calculate.
	for(uint8_t i=0; i<4; i++) {
		result_deasm[i][3] = 
			gf_mul[state_deasm[i][3]][0] ^ gf_mul[state_deasm[i][2]][1] ^ state_deasm[i][1] ^ state_deasm[i][0];
		result_deasm[i][2] = 
			state_deasm[i][3] ^ gf_mul[state_deasm[i][2]][0] ^ gf_mul[state_deasm[i][1]][1] ^ state_deasm[i][0];
		result_deasm[i][1] = 
			state_deasm[i][3] ^ state_deasm[i][2] ^ gf_mul[state_deasm[i][1]][0] ^ gf_mul[state_deasm[i][0]][1];
		result_deasm[i][0] = 
			gf_mul[state_deasm[i][3]][1] ^ state_deasm[i][2] ^ state_deasm[i][1] ^ gf_mul[state_deasm[i][0]][0];
	}
	
	memcpy(result, &result_deasm, sizeof(state_deasm));
}

// Each have a shape of uint32_t[4]
void add_round_key(uint32_t* state, uint32_t* round_key, uint32_t* result) {
	result[0] = state[0] ^ round_key[0];
	result[1] = state[1] ^ round_key[1];
	result[2] = state[2] ^ round_key[2];
	result[3] = state[3] ^ round_key[3];
}

void transpose(uint8_t* arr_in, uint8_t* arr_out) {
	for(int i=0; i<4; i++) {
		for(int j=0; j<4; j++) {
			arr_out[j*4+i] = arr_in[i*4+j];
		}
	}
}
