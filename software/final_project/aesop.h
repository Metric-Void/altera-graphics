#ifndef AESOP_H_
#define AESOP_H_

void aes_encrypt(uint8_t* data_in, uint32_t* data_out, uint32_t* keyarr);
void key_expansion(uint8_t* key_in, uint32_t* key_out, uint8_t Nk, uint8_t Nb, uint8_t Nr);
void shift_rows(uint8_t* in, uint8_t* result);
uint8_t sub_byte(uint8_t byte_in);
uint32_t sub_word32(uint32_t word_in);
void sub_word(uint8_t* word_in, uint8_t* word_out);
uint32_t rot_word(uint32_t word_in);
void mix_columns(uint8_t* mat_in, uint8_t* result);
void add_round_key(uint32_t* state, uint32_t* round_key, uint32_t* result);
void transpose(uint8_t* arr_in, uint8_t* arr_out);

#endif
