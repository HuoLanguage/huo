#ifndef VARINT_H
#define VARINT_H 1

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

struct varint_t {
    size_t sign:1; // 1 = negative
    size_t size: sizeof(size_t) * CHAR_BIT - 1;
    unsigned char *data; // NOTE: low chars first.
};

struct varint_t varint_zero();

struct varint_t varint_u_new(const uintmax_t val);
struct varint_t varint_new(intmax_t val);

void varint_free(struct varint_t *const v);


struct varint_t varint_copy(const struct varint_t *const v);
void varint_copy_to(struct varint_t *const to, const struct varint_t *const from);

bool varint_can_be_uintmax(const struct varint_t *const v);
uintmax_t varint_as_uintmax(const struct varint_t *const v);

unsigned char varint_char_at_pos(const struct varint_t *const v, const size_t pos);

void varint_print_dec(const struct varint_t *const v);
struct String varint_to_dec_string(const struct varint_t *const v);

struct String varint_to_hex_string(const struct varint_t *const v);
void varint_print_hex(const struct varint_t *const v);

struct String varint_to_base_string(const struct varint_t *const v, const uintmax_t base, const size_t digits_in_group, const char *const digit_sep, char * (* const digit_func)(const uintmax_t));

struct varint_t varint_negate(const struct varint_t *const v);
void varint_negate_to(struct varint_t *const v);

struct varint_t varint_add(const struct varint_t *const a, const struct varint_t *const b);
void varint_add_to(struct varint_t *const a, const struct varint_t *const b);

struct varint_t varint_sub(const struct varint_t *const a, const struct varint_t *const b) ;
void varint_sub_to(struct varint_t *const a, const struct varint_t *const b);

struct varint_t varint_mul(const struct varint_t *const a, const struct varint_t *const b);
void varint_mul_to(struct varint_t *const a, const struct varint_t *const b);

struct varint_t varint_div(const struct varint_t *const v, const struct varint_t *const div_by);
void varint_div_to(struct varint_t *const v, const struct varint_t *const div_by);

struct varint_t varint_mod(const struct varint_t *const v, const struct varint_t *const mod_by);
void varint_mod_to(struct varint_t *const v, const struct varint_t *const mod_by);

void varint_div_mod(struct varint_t *const div, struct varint_t *const mod, const struct varint_t *const v_in, const struct varint_t *const div_by_in);

struct varint_t varint_shift_right_word(const struct varint_t *const v, const size_t num);
void varint_shift_right_word_to(struct varint_t *const v, const size_t num);

void varint_shift_right_one_bit(struct varint_t *const v);

struct varint_t varint_shift_left_word(const struct varint_t *const v, const size_t num);
void varint_shift_left_word_to(struct varint_t *const v, const size_t num);

void varint_mul_small_to(struct varint_t *const a, const unsigned char c);
struct varint_t varint_mul_small(const struct varint_t *const a, const unsigned char c);

bool varint_equals(const struct varint_t *const a, const struct varint_t *const b);
bool varint_equals_abs(const struct varint_t *const a, const struct varint_t *const b);

bool varint_equals_zero(const struct varint_t *const v);
bool varint_equals_pm_one(const struct varint_t *const v);

bool varint_greater_than(const struct varint_t *const a, const struct varint_t *const b);
bool varint_greater_than_abs(const struct varint_t *const a, const struct varint_t *const b);

int_fast8_t varint_signum(const struct varint_t *const v);

#endif
