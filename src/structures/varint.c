#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "varint.h"
#include "../base_util.h"

#define UCHAR_BASE ((uintmax_t) UCHAR_MAX + 1)

#if USHRT_MAX >= UCHAR_MAX * 3
#define LARGER_THAN_UCHAR unsigned short
#elif UINT_MAX >= UCHAR_MAX * 3
#define LARGER_THAN_UCHAR unsigned int
#elif ULONG_MAX >= UCHAR_MAX * 3
#define LARGER_THAN_UCHAR unsigned long
#elif ULONGLONG_MAX >= UCHAR_MAX * 3
#define LARGER_THAN_UCHAR unsigned long long
#elif UINTMAX_MAX >= UCHAR_MAX
#define LARGER_THAN_UCHAR uintmax_t
#else
#error "No type larger than uchar?"
#endif

#if SHRT_MAX >= CHAR_MAX * 3
#define LARGER_THAN_CHAR short
#elif INT_MAX >= CHAR_MAX * 3
#define LARGER_THAN_CHAR int
#elif LONG_MAX >= CHAR_MAX * 3
#define LARGER_THAN_CHAR long
#elif LONGLONG_MAX >= CHAR_MAX * 3
#define LARGER_THAN_CHAR long long
#elif INTMAX_MAX >= CHAR_MAX
#define LARGER_THAN_CHAR intmax_t
#else
#error "No type larger than char?"
#endif

void string_reverse(char *const str);
char *int_as_dec_digit(const uintmax_t i);
char *int_as_hex_digit(const uintmax_t i);
void varint_trim(struct varint_t *const v);
bool varint_needs_trim (const struct varint_t *const v);

struct varint_t varint_zero() {
    struct varint_t v = {
        .sign = false,
        .size = 0,
        .data = NULL
    };
    return v;
}

struct varint_t varint_u_new(const uintmax_t val) {
    uintmax_t tval = val;
    struct varint_t v = {
        .sign = false,
        .size = 0,
        .data = NULL
    };
    if (val != 0) {
        uintmax_t t_val = val;
        while (t_val > 0) {
            t_val /= UCHAR_BASE;
            v.size += 1;
        }
        RESIZE(v.data, v.size);
        t_val = val;
        v.size = 0;
        while (t_val > 0) {
            v.data[v.size] = t_val % UCHAR_BASE;
            t_val /= UCHAR_BASE;
            v.size += 1;
        }
    }
    ASSERT_NOREC(!varint_needs_trim(&v));
    ASSERT_NOREC(varint_can_be_uintmax(&v));
    ASSERT_NOREC(tval == varint_as_uintmax(&v));
    return v;
}
struct varint_t varint_new(intmax_t val) {
    // Have to be a little tricky here or the min value doesn't work properly.
    uintmax_t temp = val < 0 ? -((uintmax_t)(val)) : (uintmax_t)(val);
    struct varint_t v = varint_u_new(temp);
    if (val < 0)
        v.sign = true;
    ASSERT_NOREC(!varint_needs_trim(&v));
    return v;
}

void varint_free(struct varint_t *const v) {
    free(v->data);
    v->size = 0;
    v->data = NULL;
}


struct varint_t varint_copy(const struct varint_t *const v) {
    assert(v != NULL);
    //ASSERT_NOREC(!varint_needs_trim(v));

    struct varint_t to_ret;

    to_ret.sign = v->sign;
    to_ret.size = v->size;
    to_ret.data = ARR_MALLOC(to_ret.size, unsigned char);

    if (to_ret.data != NULL) {
        memcpy(to_ret.data, v->data, v->size);
    }

    ASSERT_NOREC(varint_equals(&to_ret, v));

    //ASSERT_NOREC(!varint_needs_trim(&to_ret));
    return to_ret;
}

void varint_copy_to(struct varint_t *const to, const struct varint_t *const from) {
    assert(from != NULL);

    assert(from != to); // Maybe throw an error instead?
    assert(from->data == NULL || from->data != to->data); // Def. an assert

    ASSERT_NOREC(!varint_needs_trim(from));

    if (to == NULL) {
        *to = varint_copy(from);
    } else {
        ASSERT_NOREC(!varint_needs_trim(to));
        to->sign = from->sign;
        to->size = from->size;
        RESIZE(to->data, to->size);
        if (to->data != NULL) {
            memcpy(to->data, from->data, from->size);
        }
    }

    ASSERT_NOREC(varint_equals(to, from));

    ASSERT_NOREC(!varint_needs_trim(to));
}

void string_reverse(char *const str) { // const pointer to char, not pointer to const char!
    const size_t len = strlen(str);
    const size_t end_pos = len - 1;
    for (size_t low = 0; low < len/2; low++) {
        size_t high = end_pos - low;

        // SWAP(str[low], str[high]);
        char temp = str[low];
        str[low] = str[high];
        str[high] = temp;
    }
}

char *int_as_dec_digit(const uintmax_t i) {
    assert (i < 10);
    char *const c = ARR_MALLOC(2, char);
    c[0] = i + '0';
    c[1] = 0;
    return c;
}

struct String varint_to_dec_string(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    /*if (varint_can_be_uintmax(v)) {
        char *str = ARR_MALLOC(1, unsigned char);
        size_t s_size = 1;
        str[0] = (v->sign ? '+' : '-');
        const uintmax_t v_i = varint_as_uintmax(v);
        const size_t chars_to_alloc = sprintf(NULL, "%" PRIuMAX, v_i);
        RESIZE(str, 1 + chars_to_alloc + 1); // Sign + number + ending null char
        sprintf(&str[1], "%" PRIuMAX, v_i);
        return str;
    } else {*/
        return varint_to_base_string(v, 10, 3, " ", int_as_dec_digit);
    //}
}

struct String varint_to_base_string(const struct varint_t *const v, const uintmax_t base, const size_t digits_in_group, const char *const digit_sep, char * (* const digit_func)(const uintmax_t)) {
    ASSERT_NOREC(!varint_needs_trim(v));

    char *str = ARR_MALLOC(2, unsigned char);
    size_t s_size = 1;
    str[0] = (v->sign ? '-' : '+');
    str[1] = 0;

    const size_t digit_sep_len = strlen(digit_sep);

    if (varint_equals_zero(v)) {
        const char *zero = digit_func(0);
        const size_t zero_len = strlen(zero);
        RESIZE(str, s_size + zero_len + 1); // null char
        strcpy(&str[s_size], zero);
        free((void *) zero);
    } else {
        struct varint_t temp = varint_copy(v);
        temp.sign = false;
        struct varint_t mod = {};
        struct varint_t base_v = varint_u_new(base);

        const char **digit_strs = NULL;
        size_t num_digits = 0;
        size_t num_digit_chars = 0;

        while (!varint_equals_zero(&temp)) {

            // Equiv to:
            // mod = temp % base_v
            // temp = temp / base_v
            varint_div_mod(&temp, &mod, &temp, &base_v);
            const uintmax_t digit = varint_as_uintmax(&mod);

            const char *digit_str = digit_func(digit);
            const size_t digit_len = strlen(digit_str);
            num_digit_chars += digit_len;

            RESIZE(digit_strs, num_digits + 1);
            digit_strs[num_digits] = digit_str;
            num_digits += 1;
        }
        varint_free(&mod);
        varint_free(&temp);
        varint_free(&base_v);
        size_t num_groups = (num_digits + digits_in_group - 1) / digits_in_group;
        size_t group_sep_chars = (num_groups - 1) * digit_sep_len;
        size_t s_full_size = s_size + num_digit_chars + group_sep_chars + 1;
        RESIZE(str, s_full_size);

        for (size_t ipo = num_digits; ipo > 0; ipo--) {
            size_t i = ipo - 1;
            const char *digit_str = digit_strs[i];
            strcpy(&str[s_size], digit_str);
            if (i != 0 && (i % digits_in_group == 0)) {
                s_size += strlen(digit_sep);
                strcpy(&str[s_size], digit_sep);
            }
            s_size += strlen(digit_str);
            free((void *) digit_str);
        }
        free(digit_strs);
    }

    struct String s = string_from_chars(str);
    free(str);

    return s;
}

void varint_print_dec(const struct varint_t *const v) {
    struct String s = varint_to_dec_string(v);
    printf("%s", string_to_chars(&s));
    string_free(&s);
}

char *int_as_hex_digit(const uintmax_t i) {
    assert (i < 16);
    char *const c = ARR_MALLOC(2, char);
    c[0] = "0123456789ABCDEF"[i];
    c[1] = 0;
    return c;
}

struct String varint_to_hex_string(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));

    if (CHAR_BIT % 4 != 0) {
        WARN_ONCE("Your system is weird. Hex strings will be slow to print");
        return varint_to_base_string(v, 16, 2, " ", &int_as_hex_digit);
    }

    const size_t char_digits = (CHAR_BIT + 3) / 4; // +3 so this rounds up.

    size_t fmt_str_len = snprintf(NULL, 0, " %%0%zdX", char_digits);
    char *fmt_str = ARR_MALLOC(fmt_str_len + 1, char); // +1 for null char

    sprintf(fmt_str, " %%0%zdX", char_digits);

    size_t s_size = (v->size)*(char_digits + 1) + 2;

    char *const str = ARR_MALLOC(s_size + 1, unsigned char); // null char at end

    str[0] = v->sign ? '-' : '+';
    str[1] = 0;

    size_t pos = 1;
    for (size_t ipo = v->size; ipo > 0; ipo--) {
        size_t i = ipo - 1;
        int rv = sprintf(&str[pos], fmt_str, v->data[i]);
        if (rv < 0)
            ERROR(strerror(errno));
        pos += rv;
    }

    free((void *) fmt_str);

    struct String s = string_from_chars(str);

    free(str);

    return s;
}

void varint_print_hex(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    struct String s = varint_to_hex_string(v);
    printf("(0x) (%c) (%zu) %s", (v->sign ? '-' : '+'), v->size, string_to_chars(&s));
    string_free(&s);
}

bool varint_needs_trim (const struct varint_t *const v) {
    return (v->size > 0) && (v->data[v->size - 1] == 0);
}

void varint_trim(struct varint_t *const v) {
    struct varint_t copy = varint_copy(v);
    for (size_t ipo = v->size; ipo > 0; ipo--) {
        size_t i = ipo - 1;
        unsigned char c = v->data[i];
        if (c != 0) {
            RESIZE(v->data, i + 1);
            v->size = i + 1;
            ASSERT_NOREC(varint_equals(&copy, v));
            return;
        }
    }
    RESIZE(v->data, 0);
    v->size = 0;
    ASSERT_NOREC(varint_equals(&copy, v));
    ASSERT_NOREC(!varint_needs_trim(v));
    varint_free(&copy);
}

void varint_negate_to(struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    v->sign = !v->sign;
    ASSERT_NOREC(!varint_needs_trim(v));
}

bool varint_assert_double_negate(const struct varint_t *const a, const struct varint_t *const b) {
    struct varint_t negated_copy = varint_negate(b);
    bool ret = varint_equals(a, &negated_copy);
    varint_free(&negated_copy);
    return ret;
}

struct varint_t varint_negate(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    struct varint_t copy = varint_copy(v);

    varint_negate_to(&copy);

    ASSERT_NOREC(varint_assert_double_negate(v, &copy));

    ASSERT_NOREC(!varint_needs_trim(&copy));
    return copy;
}

bool varint_assert_add(const struct varint_t *const a, const struct varint_t *const b, const struct varint_t *const c) {
    bool ret = true;

    struct varint_t a_p_b = varint_add(a, b);
    struct varint_t b_p_a = varint_add(b, a);
    struct varint_t c_m_b = varint_sub(c, b);
    struct varint_t c_m_a = varint_sub(c, a);

    if (!varint_equals(c, &a_p_b)) { // a + b = c
        ret = false;
    } else if (!varint_equals(c, &b_p_a)) { // b + a = c
        ret = false;
    } else if (!varint_equals(a, &c_m_b)) { // c - b = a
        ret = false;
    } else if (!varint_equals(b, &c_m_a)) {// c - a = b
        ret = false;
    }
    varint_free(&a_p_b);
    varint_free(&b_p_a);
    varint_free(&c_m_b);
    varint_free(&c_m_a);
    return ret;
}

struct varint_t varint_add(const struct varint_t *const a, const struct varint_t *const b) {
    ASSERT_NOREC(!varint_needs_trim(a));
    ASSERT_NOREC(!varint_needs_trim(b));
    struct varint_t copy = varint_copy(a);
    varint_add_to(&copy, b);

    ASSERT_NOREC(varint_assert_add(a, b, &copy));


    ASSERT_NOREC(!varint_needs_trim(&copy));
    return copy;
}
void varint_add_to(struct varint_t *const a, const struct varint_t *const b) {
    ASSERT_NOREC(!varint_needs_trim(a));
    ASSERT_NOREC(!varint_needs_trim(b));
    if (a->sign != b->sign) {
        // a + (-b) -> a - b
        struct varint_t neg_b = *b;
        varint_negate_to(&neg_b);
        varint_sub_to(a, &neg_b);
        return;
    }
    bool overflow = false;
    for (size_t i = 0; ;i++) {
        const unsigned char ad = varint_char_at_pos(a, i);
        const unsigned char bd = varint_char_at_pos(b, i);
        const bool done_both = (i >= a->size) && (i >= b->size);
        LARGER_THAN_UCHAR res = (LARGER_THAN_UCHAR) ad + (LARGER_THAN_UCHAR) bd + (overflow ? 1 : 0);
        if (res > UCHAR_MAX) {
            res -= UCHAR_BASE;
            overflow = true;
        } else {
            overflow = false;
        }
        assert (res <= UCHAR_MAX);
        if (a->size <= i) {
            RESIZE(a->data, i + 1);
            a->size = i + 1;
        }
        a->data[i] = res;
        if (!overflow && done_both)
            break;
    }
    RESIZE(a->data, a->size);
    varint_trim(a);
}

bool varint_assert_sub(const struct varint_t *const a, const struct varint_t *const b, const struct varint_t *const c) {
    bool ret = true;

    struct varint_t a_m_b = varint_sub(a, b);
    struct varint_t b_m_a = varint_sub(b, a);
    struct varint_t c_p_b = varint_add(c, b);
    struct varint_t n_c = varint_negate(c);

    if (!varint_equals(c, &a_m_b)) { // a - b = c
        ret = false;
    } else if (!varint_equals(&n_c, &b_m_a)) { // b - a = -c
        ret = false;
    } else if (!varint_equals(a, &c_p_b)) {// c + b = a
        ret = false;
    }
    varint_free(&a_m_b);
    varint_free(&b_m_a);
    varint_free(&c_p_b);
    varint_free(&n_c);
    return ret;
}

struct varint_t varint_sub(const struct varint_t *const a, const struct varint_t *const b) {
    ASSERT_NOREC(!varint_needs_trim(a));
    ASSERT_NOREC(!varint_needs_trim(b));
    // a - b = c

    struct varint_t copy = varint_copy(a);
    varint_sub_to(&copy, b);

    ASSERT_NOREC(varint_assert_sub(a, b, &copy));

    ASSERT_NOREC(!varint_needs_trim(&copy));
    return copy;

}
void varint_sub_to(struct varint_t *const a, const struct varint_t *const b) {
    ASSERT_NOREC(!varint_needs_trim(a));
    ASSERT_NOREC(!varint_needs_trim(b));
    if (a->sign) {
        // (-a) - b = -a + -b = - (a + b)
        varint_negate_to(a);
        varint_add_to(a, b);
        varint_negate_to(a);
        return;
    }
    if (b->sign) {
        // a - (-b) = a + b
        struct varint_t neg_b = *b;
        varint_negate_to(&neg_b);
        varint_add_to(a, &neg_b);
        return;
    }
    if (varint_greater_than(b, a)) {
        // a - (b > a) = - (b - a)
        struct varint_t copy = varint_sub(b, a);
        varint_negate_to(&copy);
        varint_copy_to(a, &copy);
        varint_free(&copy);
        return;
    }
    bool underflow = false;
    for (size_t i = 0; ;i++) {
        const unsigned char ad = varint_char_at_pos(a, i);
        const unsigned char bd = varint_char_at_pos(b, i);
        const bool done_both = (i >= a->size) && (i >= b->size);
        LARGER_THAN_CHAR res = (LARGER_THAN_CHAR) ad - (LARGER_THAN_CHAR) bd - (underflow ? 1 : 0);
        if (res < 0) {
            res += UCHAR_BASE;
            underflow = true;
        } else {
            underflow = false;
        }
        assert (res >= 0);
        if (a->size <= i) {
            RESIZE(a->data, i + 1);
            a->size = (i + 1);
        }
        a->data[i] = res;
        if (done_both)
            break;
    }
    assert (!underflow);
    RESIZE(a->data, a->size);
    varint_trim(a);
}

bool varint_assert_mul(const struct varint_t *const a, const struct varint_t *const b, const struct varint_t *const c) {
    bool ret = true;

    struct varint_t a_m_b = varint_mul(a, b);
    struct varint_t b_m_a = varint_mul(b, a);

    if (!varint_equals(c, &a_m_b)) { // a * b = c
        assert(false);
        ret = false;
    } else if (!varint_equals(c, &b_m_a)) { // b * a = c
        assert(false);
        ret = false;
    }
    if (varint_equals_zero(c)) {
        if (!varint_equals_zero(b) && !varint_equals_zero(a)) {
            assert(false);
            ret = false;
        }
    } else if (varint_equals_zero(b) || varint_equals_zero(a)) {
        assert(false);
        ret = false;
    } else {
        struct varint_t c_d_b = varint_div(c, b);
        struct varint_t c_d_a = varint_div(c, a);
        if (!varint_equals(a, &c_d_b)) { //a = c / b
            assert(false);
            ret = false;
        } else if (!varint_equals(b, &c_d_a)) { //b = c / a
            assert(false);
            ret = false;
        } else
        varint_free(&c_d_b);
        varint_free(&c_d_a);
    }
    varint_free(&a_m_b);
    varint_free(&b_m_a);
    return ret;
}

struct varint_t varint_mul(const struct varint_t *const a, const struct varint_t *const b) {
    ASSERT_NOREC(!varint_needs_trim(a));
    ASSERT_NOREC(!varint_needs_trim(b));
    const bool new_sign = a->sign != b->sign;
    struct varint_t res;
    if (a->size + b->size < sizeof(uintmax_t)) {
        const uintmax_t am = varint_as_uintmax(a);
        const uintmax_t bm = varint_as_uintmax(b);
        const uintmax_t res_u = am * bm;
        res = varint_u_new(res_u);
        res.sign = new_sign;
    } else {
        res = varint_new(0);
        size_t num_shift = 0;
        for (size_t ip1 = a->size; ip1 > 0; ip1--) {
            size_t i = ip1 - 1;
            if (a->data[i] == 0) {
                num_shift += 1;
                continue;
            }
            varint_shift_left_word_to(&res, num_shift + 1);
            num_shift = 0;
            if (a->data[i] == 1) {
                varint_add_to(&res, b);
            } else {
                struct varint_t temp = varint_mul_small(b, a->data[i]);
                varint_add_to(&res, &temp);
                varint_free(&temp);
            }
        }
        varint_shift_left_word_to(&res, num_shift);
        res.sign = new_sign;
    }

    ASSERT_NOREC(varint_assert_mul(a, b, &res));
    ASSERT_NOREC(!varint_needs_trim(&res));
    return res;
}
struct varint_t varint_shift_right_word(const struct varint_t *const v, const size_t num) {
    ASSERT_NOREC(!varint_needs_trim(v));
    struct varint_t res = varint_copy(v);
    varint_shift_right_word_to(&res, num);
    ASSERT_NOREC(!varint_needs_trim(&res));
    return res;
}
void varint_shift_right_word_to(struct varint_t *const v, const size_t num) {
    ASSERT_NOREC(!varint_needs_trim(v));
    if (num == 0)
        return;
    size_t vs = v->size;
    if (num >= vs) {
        // Set to 0
        RESIZE(v->data, 0);
        v->size = 0;
        return;
    }
    memmove(v->data, &v->data[num], vs - num);
    RESIZE(v->data, vs - num);
    v->size -= num;
    varint_trim(v);
}

bool varint_assert_signum(const struct varint_t *const a, const int_fast8_t res) {
    bool ret = true;

    struct varint_t neg_a = varint_negate(a);

    if (res != varint_signum(a)) {
        ret = false;
    } else if (res != -varint_signum(&neg_a)) {
        ret = false;
    } else if (res > 1 || res < -1) {
        ret = false;
    }
    varint_free(&neg_a);
    return ret;
}

int_fast8_t varint_signum(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    int_fast8_t ret;
    if (varint_equals_zero(v)) {
        ret = 0;
    } else {
        ret = (v->sign ? -1 : 1);
    }
    ASSERT_NOREC(varint_assert_signum(v, ret));

    return ret;
}

bool varint_assert_shift_left_word(const struct varint_t *const a, const struct varint_t *const b, const size_t num) {
    bool ret = true;

    struct varint_t a_left_num = varint_shift_left_word(a, num);
    struct varint_t b_right_num = varint_shift_right_word(b, num);

    if (!varint_equals(b, &a_left_num)) {
        ret = false;
    } else if (!varint_equals(a, &b_right_num)) {
        ret = false;
    } else if (varint_signum(a) != varint_signum(b)) {
        ret = false;
    }
    varint_free(&a_left_num);
    varint_free(&b_right_num);
    return ret;
}

struct varint_t varint_shift_left_word(const struct varint_t *const v, const size_t num) {
    ASSERT_NOREC(!varint_needs_trim(v));
    struct varint_t res = varint_copy(v);
    varint_shift_left_word_to(&res, num);
    // For varints, (a << b) >> b == a, always.
    ASSERT_NOREC(varint_assert_shift_left_word(v, &res, num));
    ASSERT_NOREC(!varint_needs_trim(&res));
    return res;
}
void varint_shift_left_word_to(struct varint_t *const v, const size_t num) {
    ASSERT_NOREC(!varint_needs_trim(v));
    if (num == 0 || varint_equals_zero(v))
        return;
    const size_t vs = v->size;
    v->size += num;
    RESIZE(v->data, v->size);
    memset(&v->data[vs], 0, num);
    memmove(&v->data[num], v->data, vs);
    memset(v->data, 0, num);
    ASSERT_NOREC(!varint_needs_trim(v));
}

void varint_mul_small_to(struct varint_t *const a, const unsigned char c) {
    ASSERT_NOREC(!varint_needs_trim(a));
    if (c == 0) {
        RESIZE(a->data, 0);
        a->size = 0;
    } else if (c != 1) { // x * 1 = x
        unsigned int overflow = 0;
        for (size_t i = 0; (i < a->size) || (overflow != 0); i++) {
            unsigned char ai = i >= a->size ? 0 : a->data[i];
            LARGER_THAN_UCHAR res = ((LARGER_THAN_UCHAR) ai) * (LARGER_THAN_UCHAR) c + overflow;
            if (a->size <= i) {
                RESIZE(a->data, i + 1);
                a->size = (i + 1);
            }
            a->data[i] = res % UCHAR_BASE;
            overflow = res / UCHAR_BASE;
        }
    }
    ASSERT_NOREC(!varint_needs_trim(a));
}

bool varint_assert_mul_small(const struct varint_t *const a, const unsigned char b, const struct varint_t *const c) {
    struct varint_t b_v = varint_u_new(b);
    bool ret = varint_assert_mul(a, &b_v, c);

    varint_free(&b_v);

    return ret;
}

struct varint_t varint_mul_small(const struct varint_t *const a, const unsigned char c) {
    ASSERT_NOREC(!varint_needs_trim(a));
    struct varint_t res;
    if (c == 0) {
        res = varint_zero();
    } else {
        res = varint_copy(a);
        varint_mul_small_to(&res, c);
    }

    ASSERT_NOREC(varint_assert_mul_small(a, c, &res));
    ASSERT_NOREC(!varint_needs_trim(&res));
    return res;
}

bool varint_can_be_uintmax(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    struct varint_t uintmax_varint = varint_u_new(UINTMAX_MAX);
    bool ret = !varint_greater_than_abs(v, &uintmax_varint);
    varint_free(&uintmax_varint);
    return ret;
}

bool varint_assert_as_uintmax(const struct varint_t *const v, const uintmax_t b) {
    struct varint_t b_v = varint_u_new(b);
    bool ret = varint_equals_abs(v, &b_v);
    varint_free(&b_v);
    return ret;
}

uintmax_t varint_as_uintmax(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    if (!varint_can_be_uintmax(v)) {
        ERROR("Tried to convert large varint to uintmax_t");
    }
    uintmax_t res = 0;
    for (size_t ip1 = v->size; ip1 > 0; ip1--) {
        size_t i = ip1 - 1;
        res *= UCHAR_BASE;
        res += (uintmax_t) v->data[i];
    }

    ASSERT_NOREC(varint_assert_as_uintmax(v, res));

    return res;
}

bool varint_equals_zero(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    for (size_t i = 0; i < v->size; i++) {
        if (v->data[i] != 0)
            return false;
    }
    return true;
}

bool varint_assert_equals_pm_one(const struct varint_t *const v, bool b) {
    struct varint_t b_v = varint_negate(v);
    bool ret = b == varint_equals_pm_one(&b_v);
    varint_free(&b_v);
    return ret;
}

bool varint_equals_pm_one(const struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    bool ret = true;
    if (v->size <= 0) {
        ret = false;
    } else if (v->data[0] != 1) {
        ret = false;
    } else {
        for (size_t i = 1; i < v->size; i++) {
            if (v->data[i] != 0) {
                ret = false;
                break;
            }
        }
    }
    ASSERT_NOREC(varint_assert_equals_pm_one(v, ret));
    return ret;
}

void varint_mul_to(struct varint_t *const a, const struct varint_t *const b) {
    ASSERT_NOREC(!varint_needs_trim(a));
    ASSERT_NOREC(!varint_needs_trim(b));
    bool new_sign = a->sign != b->sign;
    if (varint_equals_zero(a) || varint_equals_pm_one(b)) {
        a->sign = new_sign;
        return;
    }
    if (varint_equals_zero(b) || varint_equals_pm_one(a)) {
        varint_copy_to(a, b);
        a->sign = new_sign;
        return;
    }
    struct varint_t temp = varint_mul(a, b);
    varint_copy_to(a, &temp);
    varint_free(&temp);
    ASSERT_NOREC(!varint_needs_trim(a));
}



struct varint_t varint_div(const struct varint_t *const v, const struct varint_t *const div_by) {
    ASSERT_NOREC(!varint_needs_trim(v));
    ASSERT_NOREC(!varint_needs_trim(div_by));
    struct varint_t div = {};
    struct varint_t mod = {};
    varint_div_mod(&div, &mod, v, div_by);
    varint_free(&mod);
    return div;
}
void varint_div_to(struct varint_t *const v, const struct varint_t *const div_by) {
    ASSERT_NOREC(!varint_needs_trim(v));
    ASSERT_NOREC(!varint_needs_trim(div_by));
    struct varint_t mod = {};
    varint_div_mod(v, &mod, v, div_by);
    varint_free(&mod);
}

struct varint_t varint_mod(const struct varint_t *const v, const struct varint_t *const mod_by) {
    ASSERT_NOREC(!varint_needs_trim(v));
    ASSERT_NOREC(!varint_needs_trim(mod_by));
    struct varint_t div = {};
    struct varint_t mod = {};
    varint_div_mod(&div, &mod, v, mod_by);
    varint_free(&div);
    return mod;
}
void varint_mod_to(struct varint_t *const v, const struct varint_t *const mod_by) {
    ASSERT_NOREC(!varint_needs_trim(v));
    ASSERT_NOREC(!varint_needs_trim(mod_by));
    struct varint_t div = {};
    varint_div_mod(&div, v, v, mod_by);
    varint_free(&div);
}

unsigned char varint_char_at_pos(const struct varint_t *const v, const size_t pos) {
    //ASSERT_NOREC(!varint_needs_trim(v));
    if (pos >= v->size)
        return 0;
    return v->data[pos];
}

void varint_shift_right_one_bit(struct varint_t *const v) {
    ASSERT_NOREC(!varint_needs_trim(v));
    unsigned char carry = 0;
    for (size_t ip1 = v->size; ip1 > 0; ip1--) {
        size_t i = ip1 - 1;
        unsigned char c = (v->data[i] >> 1) | (carry << (CHAR_BIT - 1));
        carry = v->data[i] & 1;
        v->data[i] = c;
    }
    varint_trim(v);
}

bool varint_assert_div_mod(struct varint_t *const div, struct varint_t *const mod, const struct varint_t *const v_in, const struct varint_t *const div_by_in) {

    bool ret = true;

    struct varint_t div_m_div_by_p_mod = varint_mul(div, div_by_in);

    varint_add_to(&div_m_div_by_p_mod, mod);


    if (!varint_greater_than_abs(div_by_in, mod)) {
        ret = false;
    } else if (!varint_equals(v_in, &div_m_div_by_p_mod)) {
        ret = false;
    }
    varint_free(&div_m_div_by_p_mod);
    return ret;
}

void varint_div_mod(struct varint_t *const div, struct varint_t *const mod, const struct varint_t *const v_in, const struct varint_t *const div_by_in) {
    ASSERT_NOREC(!varint_needs_trim(div));
    ASSERT_NOREC(!varint_needs_trim(mod));
    ASSERT_NOREC(!varint_needs_trim(v_in));
    ASSERT_NOREC(!varint_needs_trim(div_by_in));

    struct varint_t v_slot;
    bool v_aliased = v_in == div || v_in == mod || v_in == div_by_in;
    if (v_aliased) {
        v_slot = varint_copy(v_in);
    }
    const struct varint_t *const v = v_aliased ? &v_slot : v_in;


    struct varint_t div_by_slot;
    bool div_by_aliased = div_by_in == div || div_by_in == mod;
    if (div_by_aliased) {
        div_by_slot = varint_copy(div_by_in);
    }
    const struct varint_t *const div_by = div_by_aliased ? &div_by_slot : div_by_in;
    assert (div != mod);
    if (div->data != NULL && div->data == mod->data) {
        *div = varint_copy(div);
    }

    assert(div->data == NULL || div->data != mod->data);
    assert(div->data == NULL || div->data != v->data);
    assert(div->data == NULL || div->data != div_by->data);
    assert(mod->data == NULL || mod->data != v->data);
    assert(mod->data == NULL || mod->data != div_by->data);
    assert(v->data == NULL || v->data != div_by->data);

    if (varint_equals_zero(div_by)) {
        ERROR("Division by 0");
    }
    bool sg = v->sign != div_by->sign;
    if (varint_equals_zero(v)) {
        varint_copy_to(div, v);
        div->sign = sg;
        varint_copy_to(mod, v);
        mod->sign = sg;
    } else if (varint_equals_pm_one(div_by)) {
        struct varint_t zero = varint_zero();
        varint_copy_to(mod, &zero);
        varint_free(&zero);
        varint_copy_to(div, v);
        div->sign = sg;
    } else {
        struct varint_t zero = varint_zero();
        varint_copy_to(div, &zero);
        varint_free(&zero);

        struct varint_t pow_two = varint_u_new(1);

        struct varint_t cur_div = varint_copy(div_by);
        cur_div.sign = false;


        //TODO: This is SLOW.
        size_t ppo = 0 + 1;
        while (true) {
            varint_shift_left_word_to(&cur_div, 1);
            if (varint_greater_than_abs(&cur_div, v)) {
                varint_shift_right_word_to(&cur_div, 1);
                break;
            }
            varint_shift_left_word_to(&pow_two, 1);
            ppo += CHAR_BIT;
        }
        while (true) {
            varint_mul_small_to(&cur_div, 2);
            if (varint_greater_than_abs(&cur_div, v)) {
                varint_shift_right_one_bit(&cur_div);
                break;
            }
            varint_mul_small_to(&pow_two, 2);
            ppo += 1;
        }

        varint_copy_to(mod, v);
        mod->sign = false;

        // while mod >= div_by
        // while ! (div_by > mod)
        while (ppo > 0) {
            varint_sub_to(mod, &cur_div);

            if (varint_signum(mod) < 0) {
                varint_add_to(mod, &cur_div);
            } else {
                varint_add_to(div, &pow_two);
            }

            varint_shift_right_one_bit(&cur_div);
            varint_shift_right_one_bit(&pow_two);
            ppo -= 1;
        }

        div->sign = sg;
        varint_free(&cur_div);
        varint_free(&pow_two);

    }

    ASSERT_NOREC(varint_assert_div_mod(div, mod, v, div_by));

    if (v_aliased) {
        varint_free(&v_slot);
    }
    if (div_by_aliased) {
        varint_free(&div_by_slot);
    }

    ASSERT_NOREC(!varint_needs_trim(div));
    ASSERT_NOREC(!varint_needs_trim(mod));
}

bool varint_equals(const struct varint_t *const a, const struct varint_t *const b) {
    bool ret = (varint_signum(a) == varint_signum(b)) && varint_equals_abs(a, b);

    ASSERT_NOREC(ret == varint_equals(b, a));
    ASSERT_NOREC(!(ret && varint_greater_than(a, b)));
    ASSERT_NOREC(!(ret && varint_greater_than(b, a)));
    ASSERT_NOREC(ret || varint_greater_than(a, b) || varint_greater_than(b, a));

    return ret;
}
bool varint_equals_abs(const struct varint_t *const a, const struct varint_t *const b) {
    bool ret = true;
    size_t num_chars = a->size;
    if (num_chars < b->size)
        num_chars = b->size;
    for (size_t i = 0; i < num_chars; i++) {
        unsigned char ad = varint_char_at_pos(a, i);
        unsigned char bd = varint_char_at_pos(b, i);
        if (ad != bd) {
            ret = false;
            break;
        }
    }
    ASSERT_NOREC(ret == varint_equals_abs(b, a));
    return ret;
}

bool varint_greater_than(const struct varint_t *const a, const struct varint_t *const b) {
    bool ret;
    if (a->sign != b->sign) {
        if (varint_equals_zero(a) && varint_equals_zero(b)) {
            ret = false;
        } else {
            ret = !a->sign;
        }
    } else {
        ret = varint_greater_than_abs(a, b) != a->sign;
    }
    ASSERT_NOREC(!(ret && varint_equals(a, b)));
    ASSERT_NOREC(!(ret && varint_greater_than(b, a)));
    ASSERT_NOREC(ret || varint_greater_than(b, a) || varint_equals(a, b));
    return ret;
}

bool varint_greater_than_abs(const struct varint_t *const a, const struct varint_t *const b) {
    bool ret = false;
    size_t num_chars = a->size;
    if (num_chars < b->size)
        num_chars = b->size;
    for (size_t ipo = num_chars; ipo > 0; ipo--) {
        size_t i = ipo - 1;
        unsigned char ad = varint_char_at_pos(a, i);
        unsigned char bd = varint_char_at_pos(b, i);
        if (ad < bd) {
            ret = false;
            break;
        } else if (ad > bd) {
            ret = true;
            break;
        }
    }
    return ret;
}
#if 0

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    struct varint_t *ts = NULL;
    size_t num_elems = 0;
    char c;
    long l;
    unsigned long lu;
    size_t a;
    size_t b;
    while ((c = getchar()) != EOF) {
        switch (c) {
            case 'a':
                if (scanf(" %ld", &l) <= 0)
                    return 0;
                printf("Adding %ld -> %zu\n", l, num_elems);
                RESIZE(ts, num_elems + 1);
                ts[num_elems] = varint_new(l);
                num_elems += 1;
                break;
            case 'b':
                if (scanf(" %lu", &lu) <= 0)
                    return 0;
                printf("Adding (unsigned) %lu -> %zu\n", lu, num_elems);
                RESIZE(ts, num_elems + 1);
                ts[num_elems] = varint_u_new(lu);
                num_elems += 1;
                break;
            case 'p':
                if (scanf(" %zu %zu", &a, &b) <= 0)
                    return 0;
                if (a >= num_elems || b >= num_elems)
                    return 0;
                printf("%zu + %zu -> %zu\n", a, b, num_elems);
                RESIZE(ts, num_elems + 1);
                printf("(");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") + (");
                varint_print_hex(&ts[b]);
                printf(" = ");
                varint_print_dec(&ts[b]);
                printf(") = (");
                ts[num_elems] = varint_add(&ts[a], &ts[b]);
                varint_print_hex(&ts[num_elems]);
                printf(" = ");
                varint_print_dec(&ts[num_elems]);
                printf(")\n");
                num_elems += 1;
                break;
            case 'q':
                if (scanf(" %zu %zu", &a, &b) <= 0)
                    return 0;
                if (a >= num_elems || b >= num_elems)
                    return 0;
                printf("%zu += %zu\n", a, b);
                printf("(");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") - (");
                varint_print_hex(&ts[b]);
                printf(" = ");
                varint_print_dec(&ts[b]);
                printf(") = (");
                varint_add_to(&ts[a], &ts[b]);
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(")\n");
                break;
            case 's':
                if (scanf(" %zu %zu", &a, &b) <= 0)
                    return 0;
                if (a >= num_elems || b >= num_elems)
                    return 0;
                printf("%zu - %zu -> %zu\n", a, b, num_elems);
                RESIZE(ts, num_elems + 1);
                printf("(");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") - (");
                varint_print_hex(&ts[b]);
                printf(" = ");
                varint_print_dec(&ts[b]);
                printf(") = (");
                ts[num_elems] = varint_sub(&ts[a], &ts[b]);
                varint_print_hex(&ts[num_elems]);
                printf(" = ");
                varint_print_dec(&ts[num_elems]);
                printf(")\n");

                num_elems += 1;
                break;
            case 't':
                if (scanf(" %zu %zu", &a, &b) <= 0)
                    return 0;
                if (a >= num_elems || b >= num_elems)
                    return 0;
                printf("%zu -= %zu\n", a, b);
                printf("(");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") - (");
                varint_print_hex(&ts[b]);
                printf(" = ");
                varint_print_dec(&ts[b]);
                printf(") = (");
                varint_sub_to(&ts[a], &ts[b]);
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf("\n");
                break;
            case 'm':
                if (scanf(" %zu %zu", &a, &b) <= 0)
                    return 0;
                if (a >= num_elems || b >= num_elems)
                    return 0;
                printf("%zu * %zu -> %zu\n", a, b, num_elems);
                RESIZE(ts, num_elems + 1);
                printf("(");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") * (");
                varint_print_hex(&ts[b]);
                printf(" = ");
                varint_print_dec(&ts[b]);
                printf(") = (");
                ts[num_elems] = varint_mul(&ts[a], &ts[b]);
                varint_print_hex(&ts[num_elems]);
                printf(" = ");
                varint_print_dec(&ts[num_elems]);
                printf(")\n");
                num_elems += 1;
                break;
            case 'n':
                if (scanf(" %zu %zu", &a, &b) <= 0)
                    return 0;
                if (a >= num_elems || b >= num_elems)
                    return 0;
                printf("%zu *= %zu\n", a, b);
                printf("(");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") * (");
                varint_print_hex(&ts[b]);
                printf(" = ");
                varint_print_dec(&ts[b]);
                printf(") = (");
                varint_mul_to(&ts[a], &ts[b]);
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(")\n");
                break;
            case 'c':
                if (scanf(" %zu %zu", &a, &b) <= 0)
                    return 0;
                if (a >= num_elems || b >= num_elems || a == b)
                    return 0;
                printf("%zu = %zu\n", a, b);
                printf("(");
                varint_print_hex(&ts[b]);
                printf(" = ");
                varint_print_dec(&ts[b]);
                printf(") -> (");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") = (");
                varint_copy_to(&ts[a], &ts[b]);
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(")\n");
                break;
            case 'd':
                if (scanf(" %zu", &a) <= 0)
                    return 0;
                if (a >= num_elems)
                    return 0;
                printf("%zu = %zu\n", num_elems, b);
                printf("(");
                RESIZE(ts, num_elems + 1);
                ts[num_elems] = varint_copy(&ts[a]);
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(") -> (");
                varint_print_hex(&ts[num_elems]);
                printf(" = ");
                varint_print_dec(&ts[num_elems]);
                printf(")\n");

                num_elems += 1;
                break;
            case '.':
                if (scanf(" %zu", &a) <= 0)
                    return 0;
                if (a >= num_elems)
                    return 0;
                printf("(");
                varint_print_hex(&ts[a]);
                printf(" = ");
                varint_print_dec(&ts[a]);
                printf(")\n");
                break;
            case '\n':
                break;
            default:
                return 0;
        }
    }
}

#endif
